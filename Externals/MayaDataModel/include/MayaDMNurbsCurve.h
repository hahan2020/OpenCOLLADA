/*
    Copyright (c) 2008 NetAllied Systems GmbH

    This file is part of MayaDataModel.

    Licensed under the MIT Open Source License,
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/
#ifndef __MayaDM_NURBSCURVE_H__
#define __MayaDM_NURBSCURVE_H__
#include "MayaDMTypes.h"
#include "MayaDMConnectables.h"
#include "MayaDMCurveShape.h"
namespace MayaDM
{
class NurbsCurve : public CurveShape
{
public:
public:
	NurbsCurve(FILE* file,const std::string& name,const std::string& parent=""):CurveShape(file, name, parent, "nurbsCurve"){}
	virtual ~NurbsCurve(){}
	void setCached(const nurbsCurve& cc)
	{
		fprintf(mFile,"setAttr \".cc\" -type \"nurbsCurve\" ");
		cc.write(mFile);
		fprintf(mFile,";\n");

	}
	void setInPlace(bool ipo)
	{
		if(ipo == false) return;
		fprintf(mFile,"setAttr \".ipo\" %i;\n", ipo);

	}
	void setDispCV(bool dcv)
	{
		if(dcv == false) return;
		fprintf(mFile,"setAttr \".dcv\" %i;\n", dcv);

	}
	void setDispEP(bool dep)
	{
		if(dep == false) return;
		fprintf(mFile,"setAttr \".dep\" %i;\n", dep);

	}
	void setDispHull(bool dh)
	{
		if(dh == false) return;
		fprintf(mFile,"setAttr \".dh\" %i;\n", dh);

	}
	void setDispGeometry(bool dg)
	{
		if(dg == true) return;
		fprintf(mFile,"setAttr \".dg\" %i;\n", dg);

	}
	void getCreate()
	{
		fprintf(mFile,"\"%s.cr\"",mName.c_str());

	}
	void getLocal()
	{
		fprintf(mFile,"\"%s.l\"",mName.c_str());

	}
	void getWorldSpace(size_t ws_i)
	{
		fprintf(mFile,"\"%s.ws[%i]\"",mName.c_str(),ws_i);

	}
	void getWorldNormal(size_t wn_i)
	{
		fprintf(mFile,"\"%s.wn[%i]\"",mName.c_str(),wn_i);

	}
	void getWorldNormalX(size_t wn_i)
	{
		fprintf(mFile,"\"%s.wn[%i].wnx\"",mName.c_str(),wn_i);

	}
	void getWorldNormalY(size_t wn_i)
	{
		fprintf(mFile,"\"%s.wn[%i].wny\"",mName.c_str(),wn_i);

	}
	void getWorldNormalZ(size_t wn_i)
	{
		fprintf(mFile,"\"%s.wn[%i].wnz\"",mName.c_str(),wn_i);

	}
	void getForm()
	{
		fprintf(mFile,"\"%s.f\"",mName.c_str());

	}
	void getDegree()
	{
		fprintf(mFile,"\"%s.d\"",mName.c_str());

	}
	void getSpans()
	{
		fprintf(mFile,"\"%s.s\"",mName.c_str());

	}
	void getEditPoints(size_t eps_i)
	{
		fprintf(mFile,"\"%s.eps[%i]\"",mName.c_str(),eps_i);

	}
	void getXValueEp(size_t eps_i)
	{
		fprintf(mFile,"\"%s.eps[%i].xve\"",mName.c_str(),eps_i);

	}
	void getYValueEp(size_t eps_i)
	{
		fprintf(mFile,"\"%s.eps[%i].yve\"",mName.c_str(),eps_i);

	}
	void getZValueEp(size_t eps_i)
	{
		fprintf(mFile,"\"%s.eps[%i].zve\"",mName.c_str(),eps_i);

	}
	void getCached()
	{
		fprintf(mFile,"\"%s.cc\"",mName.c_str());

	}
	void getInPlace()
	{
		fprintf(mFile,"\"%s.ipo\"",mName.c_str());

	}
	void getDispCV()
	{
		fprintf(mFile,"\"%s.dcv\"",mName.c_str());

	}
	void getDispEP()
	{
		fprintf(mFile,"\"%s.dep\"",mName.c_str());

	}
	void getDispHull()
	{
		fprintf(mFile,"\"%s.dh\"",mName.c_str());

	}
	void getDispCurveEndPoints()
	{
		fprintf(mFile,"\"%s.dce\"",mName.c_str());

	}
	void getDispGeometry()
	{
		fprintf(mFile,"\"%s.dg\"",mName.c_str());

	}
	void getTweakSize()
	{
		fprintf(mFile,"\"%s.ts\"",mName.c_str());

	}
	void getMinMaxValue()
	{
		fprintf(mFile,"\"%s.mmv\"",mName.c_str());

	}
	void getMinValue()
	{
		fprintf(mFile,"\"%s.mmv.min\"",mName.c_str());

	}
	void getMaxValue()
	{
		fprintf(mFile,"\"%s.mmv.max\"",mName.c_str());

	}
protected:
	NurbsCurve(FILE* file,const std::string& name,const std::string& parent,const std::string& nodeType):CurveShape(file, name, parent, nodeType) {}

};
}//namespace MayaDM
#endif//__MayaDM_NURBSCURVE_H__
