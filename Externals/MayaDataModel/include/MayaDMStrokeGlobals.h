/*
    Copyright (c) 2008 NetAllied Systems GmbH

    This file is part of MayaDataModel.

    Licensed under the MIT Open Source License,
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/
#ifndef __MayaDM_STROKEGLOBALS_H__
#define __MayaDM_STROKEGLOBALS_H__
#include "MayaDMTypes.h"
#include "MayaDMConnectables.h"
#include "MayaDMDependNode.h"
namespace MayaDM
{
class StrokeGlobals : public DependNode
{
public:
public:
	StrokeGlobals(FILE* file,const std::string& name,const std::string& parent=""):DependNode(file, name, parent, "strokeGlobals"){}
	virtual ~StrokeGlobals(){}
	void setSceneScale(double pss)
	{
		if(pss == 5.0) return;
		fprintf(mFile,"setAttr \".pss\" %f;\n", pss);

	}
	void setCanvasScale(double pcs)
	{
		if(pcs == 1.0) return;
		fprintf(mFile,"setAttr \".pcs\" %f;\n", pcs);

	}
	void setWrapH(bool wrh)
	{
		if(wrh == false) return;
		fprintf(mFile,"setAttr \".wrh\" %i;\n", wrh);

	}
	void setWrapV(bool wrv)
	{
		if(wrv == false) return;
		fprintf(mFile,"setAttr \".wrv\" %i;\n", wrv);

	}
	void setSceneWrapH(bool swh)
	{
		if(swh == false) return;
		fprintf(mFile,"setAttr \".swh\" %i;\n", swh);

	}
	void setSceneWrapV(bool swv)
	{
		if(swv == false) return;
		fprintf(mFile,"setAttr \".swv\" %i;\n", swv);

	}
	void setForceRealLights(bool frl)
	{
		if(frl == true) return;
		fprintf(mFile,"setAttr \".frl\" %i;\n", frl);

	}
	void setForceDepth(bool fdp)
	{
		if(fdp == true) return;
		fprintf(mFile,"setAttr \".fdp\" %i;\n", fdp);

	}
	void setUseCanvasLight(bool ucl)
	{
		if(ucl == true) return;
		fprintf(mFile,"setAttr \".ucl\" %i;\n", ucl);

	}
	void setForceTubeDirAlongPath(bool ftd)
	{
		if(ftd == true) return;
		fprintf(mFile,"setAttr \".ftd\" %i;\n", ftd);

	}
	void setLightDirection(const double3& ldr)
	{
		fprintf(mFile,"setAttr \".ldr\" -type \"double3\" ");
		ldr.write(mFile);
		fprintf(mFile,";\n");

	}
	void setLightDirectionX(double ldx)
	{
		if(ldx == 0.2) return;
		fprintf(mFile,"setAttr \".ldr.ldx\" %f;\n", ldx);

	}
	void setLightDirectionY(double ldy)
	{
		if(ldy == -.9) return;
		fprintf(mFile,"setAttr \".ldr.ldy\" %f;\n", ldy);

	}
	void setLightDirectionZ(double ldz)
	{
		if(ldz == -0.5) return;
		fprintf(mFile,"setAttr \".ldr.ldz\" %f;\n", ldz);

	}
	void getSceneScale()
	{
		fprintf(mFile,"\"%s.pss\"",mName.c_str());

	}
	void getCanvasScale()
	{
		fprintf(mFile,"\"%s.pcs\"",mName.c_str());

	}
	void getWrapH()
	{
		fprintf(mFile,"\"%s.wrh\"",mName.c_str());

	}
	void getWrapV()
	{
		fprintf(mFile,"\"%s.wrv\"",mName.c_str());

	}
	void getSceneWrapH()
	{
		fprintf(mFile,"\"%s.swh\"",mName.c_str());

	}
	void getSceneWrapV()
	{
		fprintf(mFile,"\"%s.swv\"",mName.c_str());

	}
	void getForceRealLights()
	{
		fprintf(mFile,"\"%s.frl\"",mName.c_str());

	}
	void getForceDepth()
	{
		fprintf(mFile,"\"%s.fdp\"",mName.c_str());

	}
	void getUseCanvasLight()
	{
		fprintf(mFile,"\"%s.ucl\"",mName.c_str());

	}
	void getForceTubeDirAlongPath()
	{
		fprintf(mFile,"\"%s.ftd\"",mName.c_str());

	}
	void getLightDirection()
	{
		fprintf(mFile,"\"%s.ldr\"",mName.c_str());

	}
	void getLightDirectionX()
	{
		fprintf(mFile,"\"%s.ldr.ldx\"",mName.c_str());

	}
	void getLightDirectionY()
	{
		fprintf(mFile,"\"%s.ldr.ldy\"",mName.c_str());

	}
	void getLightDirectionZ()
	{
		fprintf(mFile,"\"%s.ldr.ldz\"",mName.c_str());

	}
protected:
	StrokeGlobals(FILE* file,const std::string& name,const std::string& parent,const std::string& nodeType):DependNode(file, name, parent, nodeType) {}

};
}//namespace MayaDM
#endif//__MayaDM_STROKEGLOBALS_H__
