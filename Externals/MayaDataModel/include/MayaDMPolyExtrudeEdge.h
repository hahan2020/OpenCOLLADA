/*
    Copyright (c) 2008 NetAllied Systems GmbH

    This file is part of MayaDataModel.

    Licensed under the MIT Open Source License,
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/
#ifndef __MayaDM_POLYEXTRUDEEDGE_H__
#define __MayaDM_POLYEXTRUDEEDGE_H__
#include "MayaDMTypes.h"
#include "MayaDMConnectables.h"
#include "MayaDMPolyMoveEdge.h"
namespace MayaDM
{
class PolyExtrudeEdge : public PolyMoveEdge
{
public:
	struct TaperCurve{
		float taperCurve_Position;
		float taperCurve_FloatValue;
		unsigned int taperCurve_Interp;
		void write(FILE* file) const
		{
			fprintf(file,"%f ", taperCurve_Position);
			fprintf(file,"%f ", taperCurve_FloatValue);
			fprintf(file,"%i", taperCurve_Interp);
		}
	};
public:
	PolyExtrudeEdge(FILE* file,const std::string& name,const std::string& parent=""):PolyMoveEdge(file, name, parent, "polyExtrudeEdge"){}
	virtual ~PolyExtrudeEdge(){}
	void setKeepFacesTogether(bool kft)
	{
		if(kft == true) return;
		fprintf(mFile,"setAttr \".kft\" %i;\n", kft);

	}
	void setDivisions(int d)
	{
		if(d == 1) return;
		fprintf(mFile,"setAttr \".d\" %i;\n", d);

	}
	void setTwist(double twt)
	{
		if(twt == 0) return;
		fprintf(mFile,"setAttr \".twt\" %f;\n", twt);

	}
	void setTaper(double tp)
	{
		if(tp == 1.0) return;
		fprintf(mFile,"setAttr \".tp\" %f;\n", tp);

	}
	void setTaperCurve(size_t c_i,const TaperCurve& c)
	{
		fprintf(mFile,"setAttr \".c[%i]\" ",c_i);
		c.write(mFile);
		fprintf(mFile,";\n");

	}
	void setTaperCurve_Position(size_t c_i,float cp)
	{
		if(cp == 0.0) return;
		fprintf(mFile,"setAttr \".c[%i].cp\" %f;\n", c_i,cp);

	}
	void setTaperCurve_FloatValue(size_t c_i,float cfv)
	{
		if(cfv == 0.0) return;
		fprintf(mFile,"setAttr \".c[%i].cfv\" %f;\n", c_i,cfv);

	}
	void setTaperCurve_Interp(size_t c_i,unsigned int ci)
	{
		if(ci == 0) return;
		fprintf(mFile,"setAttr \".c[%i].ci\" %i;\n", c_i,ci);

	}
	void setSmoothingAngle(double sma)
	{
		if(sma == 3) return;
		fprintf(mFile,"setAttr \".sma\" %f;\n", sma);

	}
	void getKeepFacesTogether()
	{
		fprintf(mFile,"\"%s.kft\"",mName.c_str());

	}
	void getDivisions()
	{
		fprintf(mFile,"\"%s.d\"",mName.c_str());

	}
	void getInputProfile()
	{
		fprintf(mFile,"\"%s.ipc\"",mName.c_str());

	}
	void getTwist()
	{
		fprintf(mFile,"\"%s.twt\"",mName.c_str());

	}
	void getTaper()
	{
		fprintf(mFile,"\"%s.tp\"",mName.c_str());

	}
	void getTaperCurve(size_t c_i)
	{
		fprintf(mFile,"\"%s.c[%i]\"",mName.c_str(),c_i);

	}
	void getTaperCurve_Position(size_t c_i)
	{
		fprintf(mFile,"\"%s.c[%i].cp\"",mName.c_str(),c_i);

	}
	void getTaperCurve_FloatValue(size_t c_i)
	{
		fprintf(mFile,"\"%s.c[%i].cfv\"",mName.c_str(),c_i);

	}
	void getTaperCurve_Interp(size_t c_i)
	{
		fprintf(mFile,"\"%s.c[%i].ci\"",mName.c_str(),c_i);

	}
	void getSmoothingAngle()
	{
		fprintf(mFile,"\"%s.sma\"",mName.c_str());

	}
protected:
	PolyExtrudeEdge(FILE* file,const std::string& name,const std::string& parent,const std::string& nodeType):PolyMoveEdge(file, name, parent, nodeType) {}

};
}//namespace MayaDM
#endif//__MayaDM_POLYEXTRUDEEDGE_H__
