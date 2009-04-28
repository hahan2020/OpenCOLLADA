/*
    Copyright (c) 2008 NetAllied Systems GmbH

    This file is part of COLLADAMaya.

    Portions of the code are:
    Copyright (c) 2005-2007 Feeling Software Inc.
    Copyright (c) 2005-2007 Sony Computer Entertainment America
    Copyright (c) 2004-2005 Alias Systems Corp.

    Licensed under the MIT Open Source License, 
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/

#include "COLLADAMayaStableHeaders.h"
#include "COLLADAMayaVisualSceneImporter.h"
#include "COLLADAMayaDocumentImporter.h"
#include "COLLADAMayaDagHelper.h"
#include "COLLADAMayaSyntax.h"
#include "COLLADAMayaMaterialImporter.h"
#include "COLLADAMayaConversion.h"

#include "MayaDMJoint.h"
#include "MayaDMDependNode.h"
#include "MayaDMCommands.h"

#include "COLLADAFWRotate.h"
#include "COLLADAFWScale.h"
#include "COLLADAFWTranslate.h"
#include "COLLADAFWLookat.h"

#include "Math/COLLADABUMathMatrix4.h"

#include <maya/MFnDagNode.h>
#include <maya/MFnTransform.h>
#include <maya/MVector.h>

#include <vector>


namespace COLLADAMaya
{
    
    const String VisualSceneImporter::TRANSFORM_NODE_NAME = "Transform";


    // -----------------------------------
    VisualSceneImporter::VisualSceneImporter ( DocumentImporter* documentImporter ) 
    : BaseImporter ( documentImporter )
    {}
    
    // -----------------------------------
    VisualSceneImporter::~VisualSceneImporter ()
    {
        UniqueIdMayaNodesMap::iterator it = mMayaTransformNodesMap.begin ();
        while ( it != mMayaTransformNodesMap.end () )
        {
            MayaNodesList mayaNodes = it->second;
            for ( size_t i=0; i<mayaNodes.size (); ++i )
            {
                MayaNode* mayaNode = mayaNodes[i];
                delete mayaNode;
            }
            mayaNodes.clear ();
            ++it;
        }
        mMayaTransformNodesMap.clear ();
    }

    // -----------------------------------
    void VisualSceneImporter::importVisualScene ( const COLLADAFW::VisualScene* visualScene )
    {
        // Iterate over the root nodes of the current visual scene.
        const COLLADAFW::NodeArray& rootNodes = visualScene->getRootNodes ();
        size_t count = rootNodes.getCount ();
        for ( size_t i=0; i<count; ++i )
        {
            COLLADAFW::Node* rootNode = rootNodes [i];
            importNode ( rootNode );
        }
    }

    // -----------------------------------
    void VisualSceneImporter::importLibraryNodes ( const COLLADAFW::LibraryNodes* libraryNodes )
    {
        // A library node is always instanciated from the visual scene.
        // So we can't create the nodes on root.
        const COLLADAFW::NodePointerArray& nodes = libraryNodes->getNodes ();
        size_t numNodes = nodes.getCount ();
        for ( size_t i=0; i<numNodes; ++i )
        {
            COLLADAFW::Node* node = nodes [i];
            importNode ( node, 0, false );
        }
    }

    // -----------------------------------
    void VisualSceneImporter::importNode ( 
        const COLLADAFW::Node* node, 
        MayaNode* parentMayaNode /*= NULL*/, 
        const bool createNode /*= true*/ )
    {
        // Check if the current node is already imported.
        const COLLADAFW::UniqueId& transformNodeId = node->getUniqueId ();
        if ( findMayaTransformNode ( transformNodeId ) != 0 ) return;

        // Check for a parent node name
        String parentNodeName = ""; 
        if ( parentMayaNode != 0 )
            parentNodeName = parentMayaNode->getName ();
        
        // Get the unique node name
        String nodeName = node->getName ();
        if ( COLLADABU::Utils::equals ( nodeName, "" ) )
            nodeName = TRANSFORM_NODE_NAME;
        nodeName = DocumentImporter::frameworkNameToMayaName ( nodeName );
        nodeName = mTransformNodeIdList.addId ( nodeName );

        // Create a maya node object of the current node and push it into the map.
        MayaNode* mayaNode = new MayaNode ( transformNodeId, nodeName, parentMayaNode, createNode );
        mMayaTransformNodesMap [ transformNodeId ].push_back ( mayaNode );

        // Create the node object (joint or node)
        MayaDM::Transform* transformNode = createMayaNode ( node, nodeName, parentNodeName );

        // Get the current maya ascii file to write the data.
        FILE* file = getDocumentImporter ()->getFile ();

        // Add the original id attribute.
        String colladaId = node->getOriginalId ();
        if ( !COLLADABU::Utils::equals ( colladaId, "" ) )
        {
            MayaDM::addAttr ( file, COLLADA_ID_ATTRIBUTE_NAME, "", "string" );
            MayaDM::setAttr ( file, COLLADA_ID_ATTRIBUTE_NAME, "", "string", colladaId );
        }

        // Import the transformations.
        importTransformations ( node, transformNode );

        // TODO We need the maya transform node for connect it with the animations.
        mMayaDMTransformMap [ transformNodeId ] = *transformNode;

        // Destroy the node object.
        delete transformNode;
        
        // Import the instances.
        readNodeInstances ( node );
        readGeometryInstances ( node );
        readCameraInstances ( node );
        readLightInstances ( node );

        // Recursive call for all child elements.
        const COLLADAFW::NodeArray& childNodes = node->getChildNodes ();
        size_t numChildNodes = childNodes.getCount ();
        for ( size_t i=0; i<numChildNodes; ++i )
        {
            COLLADAFW::Node* childNode = childNodes [i];
            importNode ( childNode, mayaNode, createNode );
        }
    }

    // -----------------------------------
    bool VisualSceneImporter::importTransformations ( 
        const COLLADAFW::Node* rootNode, 
        MayaDM::Transform* transformNode )
    {
        // This is the order of the transforms:
        //
        // matrix = [SP-1 * S * SH * SP * ST] * [RP-1 * RA * R * JO * RP * RT] * T
        //          [        scale          ] * [          rotation          ] * translation
        //
        // Where SP is scale pivot translation, S is scale, SH is shear, ST is scale pivot translation
        // RP is rotation pivot, RA is rotation axis, R is rotation, RP is rotation pivot,
        // RT is rotation pivot translation, T is translation, JO is joint orientation
        //
        // references: Maya documentation - transform node, Maya documentation - joint node
        // NOTE: Left multiplying, column-order matrices
        //

        // Go through the transforms and try to fill the maya transformation. 
        // This just works, if the framework has the transformations in any order of the style
        // T* R* T* S* T*, if the order differs from, we have to transform with a matrix (but 
        // with matrix transformation is no animation possible).
        MayaTransformation mayaTransform;
        std::vector<TransformAnimation> transformAnimations;

        bool hasRotatePivot = false;
        bool hasScalePivot = false;
        bool isLookatTransform = false;

        bool validMayaTransform = 
            readMayaTransformations ( rootNode, mayaTransform, transformNode, transformAnimations, 
                                        hasRotatePivot, hasScalePivot, isLookatTransform );
        if ( !isLookatTransform )
        {
            if ( validMayaTransform )
            {
                // Set the transform animation information.
                size_t numTransformAnimations = transformAnimations.size ();
                for ( size_t i=0; i<numTransformAnimations; ++i )
                {
                    const TransformAnimation& transformAnim = transformAnimations [i];
                    const COLLADAFW::UniqueId& animationListId = transformAnim.getAnimationListId ();
                    mTransformAnimationMap [animationListId] = transformAnim;
                }

                // Set the transform values.
                importDecomposedTransform ( mayaTransform, transformNode, hasRotatePivot, hasScalePivot );
            }
            else
            {
                // Set the transform matrix to the transform object
                importMatrixTransform ( rootNode, transformNode );
            }
        }
        return true;
    }

    // -----------------------------------
    void VisualSceneImporter::convertMatrix4ToTransposedDouble4x4 ( 
        const COLLADABU::Math::Matrix4& inputMatrix, 
        double outputMatrix[][4] )
    {
        if (COLLADABU::Math::Utils::equalsZero(inputMatrix[0][0])) outputMatrix[0][0] = 0.0;
        else outputMatrix[0][0] = inputMatrix[0][0];
        if (COLLADABU::Math::Utils::equalsZero(inputMatrix[0][1])) outputMatrix[1][0] = 0.0;
        else outputMatrix[1][0] = inputMatrix[0][1];
        if (COLLADABU::Math::Utils::equalsZero(inputMatrix[0][2])) outputMatrix[2][0] = 0.0;
        else outputMatrix[2][0] = inputMatrix[0][2];
        if (COLLADABU::Math::Utils::equalsZero(inputMatrix[0][3])) outputMatrix[3][0] = 0.0;
        else outputMatrix[3][0] = inputMatrix[0][3];
//        outputMatrix[3][0] = 0;

        if (COLLADABU::Math::Utils::equalsZero(inputMatrix[1][0])) outputMatrix[0][1] = 0.0;
        else outputMatrix[0][1] = inputMatrix[1][0];
        if (COLLADABU::Math::Utils::equalsZero(inputMatrix[1][1])) outputMatrix[1][1] = 0.0;
        else outputMatrix[1][1] = inputMatrix[1][1];
        if (COLLADABU::Math::Utils::equalsZero(inputMatrix[1][2])) outputMatrix[2][1] = 0.0;
        else outputMatrix[2][1] = inputMatrix[1][2];
        if (COLLADABU::Math::Utils::equalsZero(inputMatrix[1][3])) outputMatrix[3][1] = 0.0;
        else outputMatrix[3][1] = inputMatrix[1][3];
//        outputMatrix[3][1] = 0;

        if (COLLADABU::Math::Utils::equalsZero(inputMatrix[2][0])) outputMatrix[0][2] = 0.0;
        else outputMatrix[0][2] = inputMatrix[2][0];
        if (COLLADABU::Math::Utils::equalsZero(inputMatrix[2][1])) outputMatrix[1][2] = 0.0;
        else outputMatrix[1][2] = inputMatrix[2][1];
        if (COLLADABU::Math::Utils::equalsZero(inputMatrix[2][2])) outputMatrix[2][2] = 0.0;
        else outputMatrix[2][2] = inputMatrix[2][2];
        if (COLLADABU::Math::Utils::equalsZero(inputMatrix[2][3])) outputMatrix[3][2] = 0.0;
        else outputMatrix[3][2] = inputMatrix[2][3];
//        outputMatrix[3][2] = 0;

        if (COLLADABU::Math::Utils::equalsZero(inputMatrix[3][0])) outputMatrix[0][3] = 0.0;
        else outputMatrix[0][3] = inputMatrix[3][0];
        if (COLLADABU::Math::Utils::equalsZero(inputMatrix[3][1])) outputMatrix[1][3] = 0.0;
        else outputMatrix[1][3] = inputMatrix[3][1];
        if (COLLADABU::Math::Utils::equalsZero(inputMatrix[3][2])) outputMatrix[2][3] = 0.0;
        else outputMatrix[2][3] = inputMatrix[3][2];
        if (COLLADABU::Math::Utils::equalsZero(inputMatrix[3][3])) outputMatrix[3][3] = 0.0;
        else outputMatrix[3][3] = inputMatrix[3][3];
//        outputMatrix[3][3] = 1;
    }

    // -----------------------------------
    void VisualSceneImporter::importMatrixTransform ( 
        const COLLADAFW::Node* rootNode, 
        MayaDM::Transform* transformNode )
    {
        COLLADABU::Math::Matrix4 transformMatrix;
        rootNode->getTransformationMatrix ( transformMatrix );

        // Convert the matrix to a double[4][4]
        double mtx[4][4];
        convertMatrix4ToTransposedDouble4x4 ( transformMatrix, mtx );

        // Convert the matrix to a maya matrix.
        MMatrix matrix ( mtx );
        MTransformationMatrix tm ( matrix );
        
        MStatus status;
        MVector transVec = tm.getTranslation ( MSpace::kTransform, &status );
        transformNode->setTranslate ( toLinearUnit ( MayaDM::double3 ( transVec.x, transVec.y, transVec.z ) ) );

        double rotation[3];
        MTransformationMatrix::RotationOrder order;
        tm.getRotation ( rotation, order, MSpace::kTransform );
        if ( ! ( MVector (0,0,0) == MVector ( rotation ) ) )
            transformNode->setRotate ( toAngularUnit ( MayaDM::double3 ( rotation[0], rotation[1], rotation[2] ) ) );

        double scale[3];
        tm.getScale ( scale, MSpace::kTransform );
        if ( ! ( MVector (1,1,1) == MVector ( scale ) ) )
            transformNode->setScale ( ( MayaDM::double3 ( scale[0], scale[1], scale[2] ) ) );
            //transformNode->setScale ( toUpAxisTypeFactor ( MayaDM::double3 ( scale[0], scale[1], scale[2] ) ) );

        double shear[3];
        tm.getShear ( shear, MSpace::kTransform );
        if ( ! ( MVector (0,0,0) == MVector ( shear ) ) )
            transformNode->setShear ( ( MayaDM::double3 ( shear[0], shear[1], shear[2] ) ) );
            //transformNode->setShear ( toUpAxisTypeAxis ( MayaDM::double3 ( shear[0], shear[1], shear[2] ) ) );

    }

    // -----------------------------------
    MayaDM::Transform* VisualSceneImporter::createMayaNode ( 
        const COLLADAFW::Node* node, 
        const String& nodeName, 
        const String& parentNodeName )
    {
        // Get the current maya ascii file to write the data.
        FILE* file = getDocumentImporter ()->getFile ();

        // Create a transform node of the specific type.
        MayaDM::Transform* transformNode;

        COLLADAFW::Node::NodeType nodeType = node->getType ();
        switch ( nodeType )
        {
        case COLLADAFW::Node::JOINT:
            {
                transformNode = new MayaDM::Joint ( file, nodeName.c_str (), parentNodeName );
                String message = "VisualSceneImporter::importVisualScene :: Transform type JOINT not implemented!";
                std::cerr << message << std::endl;
                MGlobal::displayError ( message.c_str () );
                assert ( "Transform type JOINT not implemented!" );
                break;
            }
        case COLLADAFW::Node::NODE:
            {
                // Create the transform node of the current root node.
                transformNode = new MayaDM::Transform ( file, nodeName.c_str (), parentNodeName );
                break;
            }
        default:
            std::cerr << "Not a valid node type!" << std::endl;
            MGlobal::displayError ("Not a valid node type!" );
            assert ( "Not a valid node type!" );
        }

        return transformNode;
    }

    // -----------------------------------
    bool VisualSceneImporter::readMayaTransformations ( 
        const COLLADAFW::Node* rootNode, 
        MayaTransformation& mayaTransform, 
        MayaDM::Transform* transformNode, 
        std::vector<TransformAnimation>& transformAnimations,
        bool& hasRotatePivot,
        bool& hasScalePivot,
        bool& isLookatTransform )
    {
        bool validMayaTransform = true;

        const COLLADAFW::TransformationArray& transforms = rootNode->getTransformations ();
        size_t numTransforms = transforms.getCount ();
        for ( size_t i=0; i<numTransforms && validMayaTransform; ++i )
        {
            const COLLADAFW::Transformation* transformation = transforms [i];

            COLLADAFW::Transformation::TransformationType transformType; 
            transformType = transformation->getTransformationType ();

            // TODO Get the id of animation list of the current transformation and store 
            // the transform node id, the mayaTransform node and the transformation type.
            const COLLADAFW::UniqueId& animationListId = transformation->getAnimationList ();
            if ( animationListId.isValid () )
            {
                // Create a TransformAnimation objekt and push it in the list.
                TransformAnimation transformAnim;
                transformAnim.setAnimationListId ( animationListId );
                const COLLADAFW::UniqueId& transformNodeId = rootNode->getUniqueId ();
                transformAnim.setTransformNodeId ( transformNodeId );
                transformAnim.setTransformation ( transformation );
                transformAnimations.push_back ( transformAnim );
            }

            // Set the transformation information in depend of the transform type.
            switch ( transformType )
            {
            case COLLADAFW::Transformation::LOOKAT:
                {
                    // Import the camera's lookat transform matrix.
                    importLookatTransform ( rootNode, transformation, transformNode );

                    isLookatTransform = true; 
                    validMayaTransform = false;
                    return validMayaTransform;

                    break;
                }
            case COLLADAFW::Transformation::MATRIX:
                // Nothing to do, the matrix will be read automatically.
                validMayaTransform = false;
                return validMayaTransform;
                break;
            case COLLADAFW::Transformation::ROTATE:
                {
                    // Rotation is maya conform, if there is not more than one rotation per axis
                    // (except the axis rotations are direct successive).

                    // On the first rotation set the actual phase to the rotate phase 1.
                    if ( mayaTransform.phase < MayaTransformation::PHASE_ROTATE1 )
                    {
                        mayaTransform.phase = MayaTransformation::PHASE_ROTATE1;

                        COLLADAFW::Rotate* rotation = ( COLLADAFW::Rotate* )transformation;
                        double angle = rotation->getRotationAngle ();
                        COLLADABU::Math::Vector3& axis = rotation->getRotationAxis ();

                        mayaTransform.axisPhaseRotate1 = axis;
                    }

                    // Handle the rotations.
                    if ( mayaTransform.phase < MayaTransformation::PHASE_ROTATE3 )
                    {
                        COLLADAFW::Rotate* rotation = ( COLLADAFW::Rotate* )transformation;
                        double angle = rotation->getRotationAngle ();
                        COLLADABU::Math::Vector3& axis = rotation->getRotationAxis ();
                        
                        // Check if the axis has changed.
                        switch ( mayaTransform.phase )
                        {
                        case MayaTransformation::PHASE_ROTATE1:
                            {
                                // Change the phase set the axis if necessary.
                                if ( mayaTransform.axisPhaseRotate1 != axis )
                                {
                                    mayaTransform.phase = MayaTransformation::PHASE_ROTATE2;
                                    mayaTransform.axisPhaseRotate2 = axis;
                                }
                            }
                            break;

                        case MayaTransformation::PHASE_ROTATE2:
                            {
                                // Check, if the axis is not already used.
                                if ( axis == mayaTransform.axisPhaseRotate1 )
                                {
                                    validMayaTransform = false;
                                    return validMayaTransform;
                                }

                                // Change the phase and set the axis if necessary.
                                if ( mayaTransform.axisPhaseRotate2 != axis )
                                {
                                    mayaTransform.phase = MayaTransformation::PHASE_ROTATE3;
                                    mayaTransform.axisPhaseRotate3 = axis;
                                }
                            }
                            break;

                        case MayaTransformation::PHASE_ROTATE3:
                            {
                                // Check, if the axis is not already used.
                                if ( axis == mayaTransform.axisPhaseRotate1 || 
                                     axis == mayaTransform.axisPhaseRotate2 )
                                {
                                    validMayaTransform = false;
                                    return validMayaTransform;
                                }
                            }
                            break;

                        default:
                            MGlobal::displayError ( "No valid transformation phase!" );
                            break;
                        }

                        // Add the rotation angle.
                        if ( axis == COLLADABU::Math::Vector3::UNIT_X )
                        {
                            MVector rotation = mayaTransform.eulerRotation.asVector ();
                            rotation.x += angle;
                            mayaTransform.eulerRotation.setValue ( rotation );
                        }
                        else if ( axis == COLLADABU::Math::Vector3::UNIT_Y )
                        {
                            MVector rotation = mayaTransform.eulerRotation.asVector ();
                            rotation.y += angle;
                            mayaTransform.eulerRotation.setValue ( rotation );
                        }
                        else if ( axis == COLLADABU::Math::Vector3::UNIT_Z )
                        {
                            MVector rotation = mayaTransform.eulerRotation.asVector ();
                            rotation.z += angle;
                            mayaTransform.eulerRotation.setValue ( rotation );
                        }

                    }
                }
                break;
            case COLLADAFW::Transformation::SCALE:
                if ( mayaTransform.phase <= MayaTransformation::PHASE_SCALE )
                {
                    // Set the actual phase to a scale phase.
                    mayaTransform.phase = MayaTransformation::PHASE_SCALE;

                    COLLADAFW::Scale* scale = ( COLLADAFW::Scale* )transformation;
                    COLLADABU::Math::Vector3& scaleVec = scale->getScale ();
                    for ( unsigned int k=0; k<3; ++k )
                        mayaTransform.scale [k] = scaleVec [k];
                }
                else 
                {
                    validMayaTransform = false;
                    return validMayaTransform;
                }
                break;
            case COLLADAFW::Transformation::SKEW:
                {
                    COLLADAFW::Skew* skew = ( COLLADAFW::Skew* )transformation;

                    MMatrix matrix;
                    skewValuesToMayaMatrix ( skew, matrix );
                    MTransformationMatrix tm ( matrix );

                    double shear[3];
                    tm.getShear ( shear, MSpace::kTransform );

                    for ( unsigned int k=0; k<3; ++k )
                        mayaTransform.skew [k] = shear [k];
                    break;
                }
            case COLLADAFW::Transformation::TRANSLATE:
                {
                    // Set the actual phase to a scale phase.
                    if ( ( mayaTransform.phase != MayaTransformation::PHASE_TRANS1 ) && 
                        ( mayaTransform.phase != MayaTransformation::PHASE_TRANS2 ) && 
                        ( mayaTransform.phase != MayaTransformation::PHASE_TRANS3 ) )
                    {
                        mayaTransform.phase += 1;
                    }

                    COLLADAFW::Translate* translate = ( COLLADAFW::Translate* )transformation;
                    COLLADABU::Math::Vector3 translation = translate->getTranslation ();
                    if ( mayaTransform.phase == MayaTransformation::PHASE_TRANS1 )
                    {
                        mayaTransform.translate1Vec.push_back ( MVector (translation[0],translation[1],translation[2] ) );
                        ++mayaTransform.numTranslate1;
                        for ( unsigned int j=0; j<3; ++j )
                            mayaTransform.translate1[j] += translation [j];
                    }
                    else if ( mayaTransform.phase == MayaTransformation::PHASE_TRANS2 )
                    {
                        mayaTransform.translate2Vec.push_back ( MVector (translation[0],translation[1],translation[2] ) );
                        ++mayaTransform.numTranslate2;
                        for ( unsigned int j=0; j<3; ++j )
                            mayaTransform.translate2[j] += translation [j];
                    }
                    else if ( mayaTransform.phase == MayaTransformation::PHASE_TRANS3 )
                    {
                        mayaTransform.translate3Vec.push_back ( MVector (translation[0],translation[1],translation[2] ) );
                        ++mayaTransform.numTranslate3;
                        for ( unsigned int j=0; j<3; ++j )
                            mayaTransform.translate3[j] += translation [j];
                    }
                }
                break;
            default:
                std::cerr << "Unknown transformation type!" << endl;
                MGlobal::displayError ( "Unknown transformation type!" );
                assert ( "Unknown transformation type!" );
                break;
            }
        }

        if ( validMayaTransform )
        {
            // Get the number of vectors
            size_t numTranslate1Vec = mayaTransform.translate1Vec.size ();
            size_t numTranslate2Vec = mayaTransform.translate2Vec.size ();
            size_t numTranslate3Vec = mayaTransform.translate3Vec.size ();

            // Just one translate3Vec is allowed.
            if (  numTranslate3Vec > 1 )
            {
                validMayaTransform = false;
                return validMayaTransform;
            }

            // If we have one translate3, which is the scalePivotInverse, 
            // the last Vector of the translate2 has to be the scalePivot.
            if ( numTranslate3Vec == 1 )
            {
                // The first translate3 vector has to be the scalePivotInverse.
                MVector scalePivotInverse = mayaTransform.translate3Vec[0];

                // If we have a scalePivotInverse, we need minimum one translate2 vector. 
                // The last translate2 vector has to be the scalePivot. 
                MVector scalePivot (0,0,0);
                if ( numTranslate2Vec > 0 )
                {
                    // The last translate2 vector has to be the scalePivot. 
                    scalePivot = mayaTransform.translate2Vec[numTranslate2Vec-1];
                }
                else
                {
                    // Except we don't have a rotation... 
                    // Then the scalePivot is the last translate1 vector.
                    // The last translate2 vector has to be the scalePivot. 
                    scalePivot = mayaTransform.translate1Vec[numTranslate1Vec-1];
                }

                // Check, if the vectors are inverse.
                if ( ( scalePivot * (-1) ) != scalePivotInverse )
                {
                    validMayaTransform = false;
                    return validMayaTransform;
                }

                // We have a valid scalePivot.
                hasScalePivot = true;
            }


            // Without a translate1 vector, we can't have a rotatePivot
            if ( numTranslate1Vec > 0 )
            {
                // We also need minimum one translate2 vector, 
                // respectively minimum two translate2 vectors, if we have a scalePivot.
                if ( numTranslate2Vec > 0 || ( hasScalePivot && numTranslate2Vec > 1 ) )
                {
                    // Check if we have a rotatePivotInverse at the beginning of the translate2 vectors
                    // and the rotatePivot at the end of the translate1 vectors.

                    // The first translate2 vector has to be the rotatePivotInverse.
                    MVector rotatePivotInverse = mayaTransform.translate2Vec[0];

                    // The last translate1 vector has to be the rotatePivot. 
                    MVector rotatePivot = mayaTransform.translate1Vec[numTranslate1Vec-1];

                    // Check, if the vectors are inverse.
                    if ( ( rotatePivot * (-1) ) != rotatePivotInverse )
                    {
                        validMayaTransform = false;
                        return validMayaTransform;
                    }
                    hasRotatePivot = true;
                }
            }
        }

        return validMayaTransform;
    }

    // -----------------------------------
    void VisualSceneImporter::importDecomposedTransform ( 
        const MayaTransformation &mayaTransform, 
        MayaDM::Transform* transformNode,
        const bool hasRotatePivot,
        const bool hasScalePivot )
    {
        // This is the order of the transforms:
        //
        // matrix = [SP-1 * S * SH * SP * ST] * [RP-1 * RA * R * JO * RP * RT] * T
        //          [        scale          ] * [          rotation          ] * translation
        //
        // Where SP is scale pivot translation, S is scale, SH is shear, ST is scale pivot translation
        // RP is rotation pivot, RA is rotation axis, R is rotation, RP is rotation pivot,
        // RT is rotation pivot translation, T is translation, JO is joint orientation

        // T1 = T + RP + RPT
        // R  = R
        // T2 = RP-1 + SPT + SP
        // S  = S
        // T3 = SP-1
        // ==> T = T1 + T2 + T3 - SPT - RPT

        // In the following calculation the RPT and SPT values are absent, 
        // so the calculation is wrong!
        {
//             MVector translate3 = mayaTransform.translate3;
//             MVector inverseScalePivot ( translate3 [0], translate3 [1], translate3 [2] );
//             MVector scalePivot = inverseScalePivot * (-1);
// 
//             MVector translate2 = mayaTransform.translate2;
//             MVector inverseRotatePivot = scalePivot - translate2;
//             MVector rotatePivot = inverseRotatePivot * (-1);
// 
//             MVector translate1 = mayaTransform.translate1;
//             MVector translate = translate1 - rotatePivot;
//             MVector tester = translate1 + translate2 + translate3;
// 
//             MQuaternion mayaRotate = mayaTransform.rotation;
//             MEulerRotation eulerRotation = mayaRotate.asEulerRotation ();
//             MEulerRotation::RotationOrder order = eulerRotation.order;
//             MVector rotation = eulerRotation.asVector ();
        }

        // Get the number of vectors
        size_t numTranslate1Vec = mayaTransform.translate1Vec.size ();
        size_t numTranslate2Vec = mayaTransform.translate2Vec.size ();
        size_t numTranslate3Vec = mayaTransform.translate3Vec.size ();

        MVector scalePivot (0,0,0);
        MVector scalePivotTranslate (0,0,0);

        MVector rotatePivot (0,0,0);
        MVector rotatePivotTranslate (0,0,0);

        bool hasScalePivotTranslate = false;

        if ( hasScalePivot )
        {
            // Get the scalePivot
            scalePivot = mayaTransform.translate3Vec[0] * (-1);

            // Check for a scalePivotTranslate
            if ( hasRotatePivot )
            {
                if ( numTranslate2Vec == 3 )
                {
                    scalePivotTranslate = mayaTransform.translate2Vec[numTranslate2Vec-2];
                    hasScalePivotTranslate = true;
                }
            }
            else
            {
                if ( numTranslate2Vec == 2 )
                {
                    scalePivotTranslate = mayaTransform.translate2Vec[0];
                    hasScalePivotTranslate = true;
                }
            }
        }

        if ( hasRotatePivot )
        {
            // The last translate1 vector has to be the rotatePivot. 
            rotatePivot = mayaTransform.translate1Vec[numTranslate1Vec-1];

            if ( hasScalePivot )
            {
                if ( numTranslate2Vec == 3 )
                    rotatePivotTranslate = mayaTransform.translate1Vec[1];
            }
        }

        // T = T1 + T2 + T3 - SPT - RPT
        MVector translate1 = mayaTransform.translate1;
        MVector translate2 = mayaTransform.translate2;
        MVector translate3 = mayaTransform.translate3;
        MVector translate = translate1 + translate2 + translate3 - scalePivotTranslate - rotatePivotTranslate;

        // Get the euler rotation and detect the rotation order.
        MEulerRotation eulerRotation = mayaTransform.eulerRotation;
        MEulerRotation::RotationOrder order = getRotationOrder ( mayaTransform );
        MVector rotation = eulerRotation.asVector ();

        MVector scale = mayaTransform.scale;
        MVector skew = mayaTransform.skew;

        // Write the transformations directly into the maya file.
        if ( translate != MVector (0, 0, 0) )
            transformNode->setTranslate ( toLinearUnit ( MayaDM::double3 ( translate.x , translate.y, translate.z ) ) );
        if ( rotation != MVector (0, 0, 0) )
            transformNode->setRotate ( MayaDM::double3 ( rotation.x, rotation.y, rotation.z ) );
        if ( scale != MVector (1, 1, 1) )
            transformNode->setScale ( MayaDM::double3 ( scale[0], scale[1], scale[2] ) );
            //transformNode->setScale ( toUpAxisTypeFactor ( MayaDM::double3 ( scale[0], scale[1], scale[2] ) ) );

        if ( skew != MVector (0, 0, 0))
            transformNode->setShear ( MayaDM::double3 ( skew.x, skew.y, skew.z ) );
            //transformNode->setShear ( toUpAxisTypeAxis ( MayaDM::double3 ( skew.x, skew.y, skew.z ) ) );

        if ( rotatePivot != MVector (0, 0, 0) )
            transformNode->setRotatePivot ( toLinearUnit ( MayaDM::double3 ( rotatePivot.x, rotatePivot.y, rotatePivot.z ) ) );
        if ( rotatePivotTranslate != MVector (0, 0, 0) )
            transformNode->setRotatePivotTranslate ( toLinearUnit ( MayaDM::double3 ( rotatePivotTranslate.x, rotatePivotTranslate.y, rotatePivotTranslate.z ) ) );

        if ( scalePivot != MVector (0, 0, 0) )
            transformNode->setScalePivot ( toLinearUnit ( MayaDM::double3 ( scalePivot.x, scalePivot.y, scalePivot.z ) ) );
        if ( scalePivotTranslate != MVector (0, 0, 0) )
            transformNode->setScalePivotTranslate ( toLinearUnit ( MayaDM::double3 ( scalePivotTranslate.x, scalePivotTranslate.y, scalePivotTranslate.z ) ) );
        
        if ( order != MEulerRotation::kXYZ )
            transformNode->setRotateOrder ( order );
    }

    // -----------------------------------
    void VisualSceneImporter::skewValuesToMayaMatrix ( 
        const COLLADAFW::Skew* skew, MMatrix& matrix ) 
    {
        float s = tanf ( COLLADABU::Math::Utils::degToRadF ( skew->getAngle () ) );

        // Unit conversion.
        const COLLADABU::Math::Vector3& rotateAxis = skew->getRotateAxis();
        //MayaDM::double3 rotate ( toUpAxisTypeAxis ( MayaDM::double3 ( rotateAxis.x, rotateAxis.y, rotateAxis.z ) ) );

        const COLLADABU::Math::Vector3& translateAxis = skew->getTranslateAxis();
        //MayaDM::double3 translate ( toUpAxisTypeAxis ( MayaDM::double3 ( translateAxis.x, translateAxis.y, translateAxis.z ) ) );

        for ( int row = 0; row < 3; ++row )
        {
            for ( int col = 0; col < 3; ++col )
            {
                matrix[col][row] = ((row == col) ? 1.0f : 0.0f) + s * (float)rotateAxis [col] * (float)translateAxis [row];
                //matrix[col][row] = ((row == col) ? 1.0f : 0.0f) + s * (float)rotate [col] * (float)translate [row];
            }
        }

        matrix[0][3] = matrix[1][3] = matrix[2][3] = 0.0f;
        matrix[3][0] = matrix[3][1] = matrix[3][2] = 0.0f;
        matrix[3][3] = 1.0f;
    }

    // -----------------------------------
    const BaseImporter::MayaNodesList* VisualSceneImporter::findMayaTransformNode ( 
        const COLLADAFW::UniqueId& transformId ) const
    {
        UniqueIdMayaNodesMap::const_iterator it = mMayaTransformNodesMap.find ( transformId );
        if ( it != mMayaTransformNodesMap.end () )
            return &(it->second);

        return 0;
    }

    // -----------------------------------
    BaseImporter::MayaNodesList* VisualSceneImporter::findMayaTransformNode ( 
        const COLLADAFW::UniqueId& transformId )
    {
        UniqueIdMayaNodesMap::iterator it = mMayaTransformNodesMap.find ( transformId );
        if ( it != mMayaTransformNodesMap.end () )
            return &(it->second);

        return 0;
    }

    // -----------------------------------
    const BaseImporter::UniqueIdVec* VisualSceneImporter::findTransformInstances ( 
        const COLLADAFW::UniqueId& transformId ) const
    {
        UniqueIdUniqueIdsMap::const_iterator it = mTransformInstancesMap.find ( transformId );

        if ( it != mTransformInstancesMap.end () )
            return &(*it).second;

        return 0;
    }

    // -----------------------------------
    const BaseImporter::UniqueIdVec* VisualSceneImporter::findGeometryTransformIds ( 
        const COLLADAFW::UniqueId& geometryId ) const
    {
        UniqueIdUniqueIdsMap::const_iterator it = mGeometryTransformIdsMap.find ( geometryId );

        if ( it != mGeometryTransformIdsMap.end () )
            return &(*it).second;

        return 0;
    }

    // -----------------------------------
    const BaseImporter::UniqueIdVec* VisualSceneImporter::findCameraTransformIds ( 
        const COLLADAFW::UniqueId& cameraId ) const
    {
        UniqueIdUniqueIdsMap::const_iterator it = mCameraTransformIdsMap.find ( cameraId );

        if ( it != mCameraTransformIdsMap.end () )
            return &(*it).second;

        return 0;
    }

    // -----------------------------------
    const BaseImporter::UniqueIdVec* VisualSceneImporter::findLightTransformIds ( 
        const COLLADAFW::UniqueId& lightId ) const
    {
        UniqueIdUniqueIdsMap::const_iterator it = mLightTransformIdsMap.find ( lightId );

        if ( it != mLightTransformIdsMap.end () )
            return &(*it).second;

        return 0;
    }

    // -----------------------------------
    bool VisualSceneImporter::readNodeInstances ( const COLLADAFW::Node* parentNode )
    {
        const COLLADAFW::UniqueId& parentNodeId = parentNode->getUniqueId ();

        const COLLADAFW::InstanceNodeArray& nodeInstances = parentNode->getInstanceNodes ();
        size_t numInstances = nodeInstances.getCount ();
        for ( size_t i=0; i<numInstances; ++i )
        {
            // Hold the information to create this instance.
            const COLLADAFW::InstanceNode* instanceNode = nodeInstances [i];
            const COLLADAFW::UniqueId& instanceNodeId = instanceNode->getInstanciatedObjectId ();

            // Push the parent node of the instanciated node in the list of instance node parents.
            // The map holds for every transform node a list of all existing parent transform nodes
            // (this are the nodes, which hold an instance of the current transform node).
            mTransformInstancesMap [ instanceNodeId ].push_back ( parentNodeId );
        }

        return true;
    }

    // -----------------------------------
    bool VisualSceneImporter::readGeometryInstances ( const COLLADAFW::Node* node )
    {
        // Get the unique id of the current node.
        const COLLADAFW::UniqueId& transformNodeId = node->getUniqueId ();

        // Go through the geometry instances and save the geometry ids to the current node.
        const COLLADAFW::InstanceGeometryArray& geometryInstances = node->getInstanceGeometries ();
        size_t numInstances = geometryInstances.getCount ();
        for ( size_t i=0; i<numInstances; ++i )
        {
            const COLLADAFW::InstanceGeometry* instanceGeometry = geometryInstances [i];
            const COLLADAFW::UniqueId& geometryId = instanceGeometry->getInstanciatedObjectId ();

            // Save for every geometry a list of transform nodes, which refer to it.
            mGeometryTransformIdsMap [ geometryId ].push_back ( transformNodeId );

            // Read the shading engines.
            readMaterialInstances ( transformNodeId, instanceGeometry );
        }

        return true;
    }

    // -----------------------------------
    void VisualSceneImporter::readMaterialInstances ( 
        const COLLADAFW::UniqueId& transformNodeId, 
        const COLLADAFW::InstanceGeometry* instanceGeometry )
    {
        // Write the shader data.
        MaterialImporter* materialImporter = getDocumentImporter ()->getMaterialImporter ();
        materialImporter->writeShaderData ( transformNodeId, instanceGeometry );
    }

    // -----------------------------------
    bool VisualSceneImporter::readCameraInstances ( const COLLADAFW::Node* node )
    {
        // Get the unique id of the current node.
        const COLLADAFW::UniqueId& transformNodeId = node->getUniqueId ();

        // Go through the camera instances and save the geometry ids to the current node.
        const COLLADAFW::InstanceCameraArray& cameraInstances = node->getInstanceCameras ();
        size_t numInstances = cameraInstances.getCount ();
        for ( size_t i=0; i<numInstances; ++i )
        {
            const COLLADAFW::InstanceCamera* instanceCamera = cameraInstances [i];
            const COLLADAFW::UniqueId& cameraId = instanceCamera->getInstanciatedObjectId ();

            // Save for every geometry a list of transform nodes, which refer to it.
            mCameraTransformIdsMap [ cameraId ].push_back ( transformNodeId );
        }

        return true;
    }

    // -----------------------------------
    bool VisualSceneImporter::readLightInstances ( const COLLADAFW::Node* node )
    {
        // Get the unique id of the current node.
        const COLLADAFW::UniqueId& transformNodeId = node->getUniqueId ();

        // Go through the camera instances and save the geometry ids to the current node.
        const COLLADAFW::InstanceLightArray& lightInstances = node->getInstanceLights ();
        size_t numInstances = lightInstances.getCount ();
        for ( size_t i=0; i<numInstances; ++i )
        {
            const COLLADAFW::InstanceLight* instanceLight = lightInstances [i];
            const COLLADAFW::UniqueId& lightId = instanceLight->getInstanciatedObjectId ();

            // Save for every geometry a list of transform nodes, which refer to it.
            mLightTransformIdsMap [ lightId ].push_back ( transformNodeId );
        }

        return true;
    }

    // -----------------------------------
    void VisualSceneImporter::writeNodeInstances ()
    {
        // A pointer to the current maya ascii file.
        FILE* file = getDocumentImporter ()->getFile ();

        // Go through the transform instances and copy / move the instantiated nodes.
        UniqueIdUniqueIdsMap::iterator it = mTransformInstancesMap.begin ();
        while ( it != mTransformInstancesMap.end() )
        {
            const COLLADAFW::UniqueId& instanceNodeId = it->first;
            BaseImporter::UniqueIdVec& parentNodes = it->second;

            // Get the maya child node and read the path.
            MayaNodesList* childTransformNodes = findMayaTransformNode ( instanceNodeId );
            if ( childTransformNodes == 0 || childTransformNodes->size () == 0 )
            {
                MGlobal::displayError ( "The referenced transform node doesn't exist!" );
                std::cerr << "The referenced transform node doesn't exist!" << endl;
                return;
            }
            // The first node is always the reference node for the other instances.
            MayaNode* mayaChildNode = (*childTransformNodes) [0];
            const COLLADAFW::UniqueId& childTransformId = mayaChildNode->getUniqueId ();
            String childNodeName = mayaChildNode->getName ();
            String childNodePath = mayaChildNode->getNodePath ();

            // Go through the instances.
            size_t numInstances = parentNodes.size ();
            for ( size_t i=0; i<numInstances; ++i )
            {
                const COLLADAFW::UniqueId& parentTransformId = parentNodes [i];
                
                // Get the maya parent nodes and read the path.
                MayaNodesList* parentTransformNodes = findMayaTransformNode ( parentTransformId );
                if ( parentTransformNodes == 0 )
                {
                    MGlobal::displayError ( "The referenced transform node doesn't exist!" );
                    std::cerr << "The referenced transform node doesn't exist!" << endl;
                    return;
                }
                // Multiple parent instances...
                MayaNode* mayaParentNode = (*parentTransformNodes) [0];
                String parentNodeName = mayaParentNode->getName ();
                String parentNodePath = mayaParentNode->getNodePath ();

                // This flag is set, if the node is not from the visual scene, 
                // but from the library nodes. We don't have to create a new instance, 
                // instead of this we have to move the node!
                bool isCorrectPositioned = mayaChildNode->getIsCorrectPositioned ();
                if ( !isCorrectPositioned )
                {
                    // parent -shape -noConnections -relative "|node1|node2" "|rootNode";
                    MayaDM::parent ( file, childNodePath, parentNodePath, false, false, true, true  );
                    mayaChildNode->setIsCorrectPositioned ( true );
                    mayaChildNode->setParent ( mayaParentNode );
                    childNodePath = mayaChildNode->getNodePath ();
                }
                else
                {
                    // parent -shape -noConnections -relative -addObject "|node1|node2" "|rootNode";
                    MayaDM::parent ( file, childNodePath, parentNodePath, false, true, true, true  );

                    // Create the maya transform nodes for the internal transform graph of this
                    // node instance and all child nodes.
                    MayaNode* mayaInstanceNode = new MayaNode ( instanceNodeId, childNodeName, mayaParentNode );
                    mMayaTransformNodesMap [ instanceNodeId ].push_back ( mayaInstanceNode );
                }
            }

            ++it;
        }
    }

    // --------------------------------------------
    size_t VisualSceneImporter::getNumTransformInstances ( 
        const COLLADAFW::UniqueId& transformId, 
        const bool recursive /*= false*/ )
    {
        // There is always one instance!
        size_t numNodeInstances = 0;
        if ( !recursive ) ++numNodeInstances;

        MayaNodesList* nodeInstances = findMayaTransformNode ( transformId );
        if ( nodeInstances != 0 )
        {
            size_t nodeCounter = nodeInstances->size ();
            numNodeInstances += nodeCounter - 1;
            for ( size_t i=0; i<nodeCounter; ++i )
            {
                MayaNode* mayaNodeInstance = (*nodeInstances) [i];
                String name = mayaNodeInstance->getName ();
                String path = mayaNodeInstance->getNodePath ();

                MayaNode* parentMayaNode = mayaNodeInstance->getParent ();
                if ( parentMayaNode != 0 )
                {
                    const COLLADAFW::UniqueId& parentTransformNodeId = parentMayaNode->getUniqueId ();
                    numNodeInstances += getNumTransformInstances ( parentTransformNodeId, true );
                }
            }
        }

        return numNodeInstances;
    }

    // --------------------------------------------
    void VisualSceneImporter::getTransformPathes ( 
        std::vector<String>& transformPathes, 
        const COLLADAFW::UniqueId& transformId, 
        const String childSubPath /*= ""*/ )
    {
        MayaNodesList* transformInstances = findMayaTransformNode ( transformId );
        if ( transformInstances != 0 )
        {
            size_t nodeCounter = transformInstances->size ();
            for ( size_t i=0; i<nodeCounter; ++i )
            {
                MayaNode* mayaTransformInstanceNode = (*transformInstances) [i];
                String transformInstancePath = mayaTransformInstanceNode->getNodePath ();

                // The first instance in the list is always the original. 
                // The path of the first instance has to be stored only once!
                if ( i > 0 || COLLADABU::Utils::equals (childSubPath,"") )
                {
                    // Push the path of the instance in the list of node pathes.
                    String transformPath = transformInstancePath + childSubPath;
                    transformPathes.push_back ( transformPath );
                }

                // Get the child path, to append on the parent instance path.
                MayaNode* parentMayaNode = mayaTransformInstanceNode->getParent ();
                if ( parentMayaNode != 0 )
                {
                    String parentChildSubPath =  "|" + mayaTransformInstanceNode->getName () + childSubPath;
                    const COLLADAFW::UniqueId& parentTransformId = parentMayaNode->getUniqueId ();
                    getTransformPathes ( transformPathes, parentTransformId, parentChildSubPath );
                }
            }
        }
    }

    // --------------------------------------------
    void VisualSceneImporter::importLookatTransform ( 
        const COLLADAFW::Node* rootNode,
        const COLLADAFW::Transformation* transformation,
        MayaDM::Transform* transformNode )
    {
        /**
        * Positioning and orienting a camera or object in the scene is often 
        * complicated when using a matrix. A lookat transform is an intuitive 
        * way to specify an eye position, interest point, and orientation.
        */
        COLLADAFW::Lookat* lookat = ( COLLADAFW::Lookat* ) transformation;

        /** The position of the object. */
        COLLADABU::Math::Vector3& eyePosition = lookat->getEyePosition (); 

        /** The position of the interest point. */
        COLLADABU::Math::Vector3& interestPosition = lookat->getInterestPointPosition(); 

        /** The direction that points up. */
        COLLADABU::Math::Vector3& upPosition = lookat->getUpAxisDirection();

        // Create a transform matrix from the lookat elements.
        COLLADABU::Math::Vector3 forward = interestPosition - eyePosition;
        forward.normalise ();
        COLLADABU::Math::Vector3 sideways, upward;
        if ( forward != upPosition && forward != -upPosition)
        {
            sideways = forward.crossProduct ( upPosition );
            sideways.normalise ();
        }
        else if ( upPosition != COLLADABU::Math::Vector3::UNIT_X )
        {
            sideways = COLLADABU::Math::Vector3::UNIT_X;
        }
        else
        {
            sideways = COLLADABU::Math::Vector3::UNIT_Z;
        }
        upward = sideways.crossProduct ( forward );

        // Create the maya transform matrix.
        MMatrix mx;
        mx[0][0] = sideways.x;      mx[0][1] = sideways.y;      mx[0][2] = sideways.z; 
        mx[1][0] = upward.x;        mx[1][1] = upward.y;        mx[1][2] = upward.z; 
        mx[2][0] = -forward.x;	    mx[2][1] = -forward.y;	    mx[2][2] = -forward.z; 
        mx[3][0] = eyePosition.x;	mx[3][1] = eyePosition.y;	mx[3][2] = eyePosition.z;

        mx[0][3] = mx[1][3] = mx[2][3] = 0.0f;
        mx[3][3] = 1.0f;

        // Create the transform matrix.
        MTransformationMatrix transformMatrix ( mx );

        // Get the translation and write it directly into the maya file.
        MVector translation = transformMatrix.getTranslation ( MSpace::kTransform );
        transformNode->setTranslate ( toLinearUnit ( MayaDM::double3 ( translation.x , translation.y, translation.z ) ) );

        // Get the rotation and write it directly into the maya file.
        double rotation[3]; 
        MTransformationMatrix::RotationOrder order;
        transformMatrix.getRotation ( rotation, order, MSpace::kTransform );
        transformNode->setRotate ( toAngularUnit ( MayaDM::double3 ( rotation[0], rotation[1], rotation[2] ) ) );

        // Compute the center of interest distance. We want store it for creating the camera.
        double centerOfInterestDistance = ( interestPosition - eyePosition ).length ();
        const COLLADAFW::UniqueId& transformNodeId = rootNode->getUniqueId ();

        // Set the center of interest distance value in a map to the current transform node.
        mCenterOfInterestDistances [ transformNodeId ] = centerOfInterestDistance;
    }

    // --------------------------------------------
    bool VisualSceneImporter::findCenterOfInterestDistance ( 
        const COLLADAFW::UniqueId& transformId, 
        double& centerOfInterestDistance )
    {
        std::map<COLLADAFW::UniqueId, double>::const_iterator it = mCenterOfInterestDistances.find ( transformId );
        if ( it != mCenterOfInterestDistances.end () )
        {
            centerOfInterestDistance = it->second;
            return true;
        }
        return false;
    }

    // --------------------------------------------
    const TransformAnimation* VisualSceneImporter::findTransformAnimation ( 
        const COLLADAFW::UniqueId& animationListId )
    {
        std::map <COLLADAFW::UniqueId, TransformAnimation>::const_iterator it = mTransformAnimationMap.find ( animationListId );
        if ( it != mTransformAnimationMap.end () )
        {
            return &(it->second);
        }
        return 0;
    }

    // --------------------------------------------
    const MayaDM::Transform* VisualSceneImporter::findMayaDMTransform ( const COLLADAFW::UniqueId& transformId )
    {
        std::map<COLLADAFW::UniqueId, MayaDM::Transform>::const_iterator it = mMayaDMTransformMap.find ( transformId );
        if ( it != mMayaDMTransformMap.end () )
        {
            return &(it->second);
        }
        return 0;
    }

    // --------------------------------------------
    MEulerRotation::RotationOrder VisualSceneImporter::getRotationOrder ( 
        const MayaTransformation &mayaTransform )
    {
        // Attention: the order is from behind!
        MEulerRotation::RotationOrder order = MEulerRotation::kXYZ;

        // X..
        if ( mayaTransform.axisPhaseRotate1 == COLLADABU::Math::Vector3::UNIT_X ) 
        {
            if ( mayaTransform.axisPhaseRotate2 == COLLADABU::Math::Vector3::UNIT_Y ) 
            {
                if ( mayaTransform.axisPhaseRotate3 == COLLADABU::Math::Vector3::UNIT_Z ) 
                {
                    order = MEulerRotation::kZYX; // xyz
                }
                else if ( mayaTransform.axisPhaseRotate3 == COLLADABU::Math::Vector3 (0,0,0) )
                {
                    order = MEulerRotation::kZYX; // xy
                }
                else MGlobal::displayError ( "No valid euler rotation order!" );
            }
            else if ( mayaTransform.axisPhaseRotate2 == COLLADABU::Math::Vector3::UNIT_Z ) 
            {
                if ( mayaTransform.axisPhaseRotate3 == COLLADABU::Math::Vector3::UNIT_Y )
                {
                    order = MEulerRotation::kYZX; // xzy
                }
                else if ( mayaTransform.axisPhaseRotate3 == COLLADABU::Math::Vector3 (0,0,0) )
                {
                    order = MEulerRotation::kZYX; // xz
                }
                else MGlobal::displayError ( "No valid euler rotation order!" );
            }
            else if ( mayaTransform.axisPhaseRotate2 == COLLADABU::Math::Vector3 (0,0,0) )
            {
                order = MEulerRotation::kXYZ; // x
            }
            else MGlobal::displayError ( "No valid euler rotation order!" );
        }

        // Y..
        else if ( mayaTransform.axisPhaseRotate1 == COLLADABU::Math::Vector3::UNIT_Y ) 
        {
            if ( mayaTransform.axisPhaseRotate2 == COLLADABU::Math::Vector3::UNIT_X ) 
            {
                if ( mayaTransform.axisPhaseRotate3 == COLLADABU::Math::Vector3::UNIT_Z ) 
                {
                    order = MEulerRotation::kZXY; // yxz
                }
                else if ( mayaTransform.axisPhaseRotate3 == COLLADABU::Math::Vector3 (0,0,0) )
                {
                    order = MEulerRotation::kXYZ; // yx
                }
                else MGlobal::displayError ( "No valid euler rotation order!" );
            }
            else if ( mayaTransform.axisPhaseRotate2 == COLLADABU::Math::Vector3::UNIT_Z ) 
            {
                if ( mayaTransform.axisPhaseRotate3 == COLLADABU::Math::Vector3::UNIT_X ) 
                {
                    order = MEulerRotation::kXZY; // yzx
                }
                else if ( mayaTransform.axisPhaseRotate3 == COLLADABU::Math::Vector3 (0,0,0) )
                {
                    order = MEulerRotation::kZYX; // yz
                }
                else MGlobal::displayError ( "No valid euler rotation order!" );
            }
            else if ( mayaTransform.axisPhaseRotate2 == COLLADABU::Math::Vector3 (0,0,0) )
            {
                order = MEulerRotation::kXYZ; // y
            }
            else MGlobal::displayError ( "No valid euler rotation order!" );
        }

        // Z..
        else if ( mayaTransform.axisPhaseRotate1 == COLLADABU::Math::Vector3::UNIT_Z ) 
        {
            if ( mayaTransform.axisPhaseRotate2 == COLLADABU::Math::Vector3::UNIT_X ) 
            {
                if ( mayaTransform.axisPhaseRotate3 == COLLADABU::Math::Vector3::UNIT_Y ) 
                {
                    order = MEulerRotation::kYXZ; // zxy
                }
                else if ( mayaTransform.axisPhaseRotate3 == COLLADABU::Math::Vector3 (0,0,0) )
                {
                    order = MEulerRotation::kXYZ; // zx
                }
                else MGlobal::displayError ( "No valid euler rotation order!" );
            }
            else if ( mayaTransform.axisPhaseRotate2 == COLLADABU::Math::Vector3::UNIT_Y ) 
            {
                if ( mayaTransform.axisPhaseRotate3 == COLLADABU::Math::Vector3::UNIT_X ) 
                {
                    order = MEulerRotation::kXYZ; // zyx
                }
                else if ( mayaTransform.axisPhaseRotate3 == COLLADABU::Math::Vector3 (0,0,0) )
                {
                    order = MEulerRotation::kXYZ; // zy
                }
                else MGlobal::displayError ( "No valid euler rotation order!" );
            }
            else if ( mayaTransform.axisPhaseRotate2 == COLLADABU::Math::Vector3 (0,0,0) )
            {
                order = MEulerRotation::kXYZ; // z
            }
            else MGlobal::displayError ( "No valid euler rotation order!" );
        }	
    
        return order;
    }

}