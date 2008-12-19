/*
    Copyright (c) 2008 NetAllied Systems GmbH

    This file is part of MayaDataModel.

    Licensed under the MIT Open Source License,
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/
#ifndef __MayaDM_POLYSPLITVERT_H__
#define __MayaDM_POLYSPLITVERT_H__
#include "MayaDMTypes.h"
#include "MayaDMConnectables.h"
#include "MayaDMPolyModifierWorld.h"
namespace MayaDM
{
class PolySplitVert : public PolyModifierWorld
{
public:
	PolySplitVert(FILE* file,const std::string& name,const std::string& parent=""):PolyModifierWorld(file, name, parent, "polySplitVert"){}
	virtual ~PolySplitVert(){}
protected:
	PolySplitVert(FILE* file,const std::string& name,const std::string& parent,const std::string& nodeType):PolyModifierWorld(file, name, parent, nodeType) {}

};
}//namespace MayaDM
#endif//__MayaDM_POLYSPLITVERT_H__
