/*
    Copyright (c) 2008 NetAllied Systems GmbH

    This file is part of MayaDataModel.

    Licensed under the MIT Open Source License,
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/
#ifndef __MayaDM_BLENDSHAPE_H__
#define __MayaDM_BLENDSHAPE_H__
#include "MayaDMTypes.h"
#include "MayaDMConnectables.h"
#include "MayaDMGeometryFilter.h"
namespace MayaDM
{
class BlendShape : public GeometryFilter
{
public:
	struct InputTarget{
		struct InputTargetGroup{
			struct InputTargetItem{
				pointArray inputPointsTarget;
				componentList inputComponentsTarget;
				void write(FILE* file) const
				{
					inputPointsTarget.write(file);
					fprintf(file, " ");
					inputComponentsTarget.write(file);
				}
			}* inputTargetItem;
			float* targetWeights;
			void write(FILE* file) const
			{
				size_t size = sizeof(inputTargetItem)/sizeof(InputTargetItem);
				for(size_t i=0; i<size; ++i)
				{
					inputTargetItem[i].write(file);
					fprintf(file, " ");
					if(i+1<size) fprintf(file," ");
				}
				size = sizeof(targetWeights)/sizeof(float);
				for(size_t i=0; i<size; ++i)
				{
					fprintf(file,"%f", targetWeights[i]);
					if(i+1<size) fprintf(file," ");
				}
			}
		}* inputTargetGroup;
		void write(FILE* file) const
		{
			size_t size = sizeof(inputTargetGroup)/sizeof(InputTargetGroup);
			for(size_t i=0; i<size; ++i)
			{
				inputTargetGroup[i].write(file);
				if(i+1<size) fprintf(file," ");
			}
		}
	};
public:
	BlendShape(FILE* file,const std::string& name,const std::string& parent=""):GeometryFilter(file, name, parent, "blendShape"){}
	virtual ~BlendShape(){}
	void setTopologyCheck(bool tc)
	{
		if(tc == true) return;
		fprintf(mFile,"setAttr \".tc\" %i;\n", tc);

	}
	void setWeight(size_t w_i,float w)
	{
		if(w == 0) return;
		fprintf(mFile,"setAttr \".w[%i]\" %f;\n", w_i,w);

	}
	void setIcon(size_t icn_i,const string& icn)
	{
		if(icn == "NULL") return;
		fprintf(mFile,"setAttr \".icn[%i]\" -type \"string\" ",icn_i);
		icn.write(mFile);
		fprintf(mFile,";\n");

	}
	void setInputTarget(size_t it_i,const InputTarget& it)
	{
		fprintf(mFile,"setAttr \".it[%i]\" ",it_i);
		it.write(mFile);
		fprintf(mFile,";\n");

	}
	void setInputTargetGroup(size_t it_i,size_t itg_i,const InputTarget::InputTargetGroup& itg)
	{
		fprintf(mFile,"setAttr \".it[%i].itg[%i]\" ",it_i,itg_i);
		itg.write(mFile);
		fprintf(mFile,";\n");

	}
	void setInputTargetItem(size_t it_i,size_t itg_i,size_t iti_i,const InputTarget::InputTargetGroup::InputTargetItem& iti)
	{
		fprintf(mFile,"setAttr \".it[%i].itg[%i].iti[%i]\" ",it_i,itg_i,iti_i);
		iti.write(mFile);
		fprintf(mFile,";\n");

	}
	void setInputPointsTarget(size_t it_i,size_t itg_i,size_t iti_i,const pointArray& ipt)
	{
		fprintf(mFile,"setAttr \".it[%i].itg[%i].iti[%i].ipt\" -type \"pointArray\" ",it_i,itg_i,iti_i);
		ipt.write(mFile);
		fprintf(mFile,";\n");

	}
	void setInputComponentsTarget(size_t it_i,size_t itg_i,size_t iti_i,const componentList& ict)
	{
		fprintf(mFile,"setAttr \".it[%i].itg[%i].iti[%i].ict\" -type \"componentList\" ",it_i,itg_i,iti_i);
		ict.write(mFile);
		fprintf(mFile,";\n");

	}
	void setTargetWeights(size_t it_i,size_t itg_i,size_t tw_i,float tw)
	{
		if(tw == 1.0) return;
		fprintf(mFile,"setAttr \".it[%i].itg[%i].tw[%i]\" %f;\n", it_i,itg_i,tw_i,tw);

	}
	void setOrigin(unsigned int or)
	{
		if(or == 1) return;
		fprintf(mFile,"setAttr \".or\" %i;\n", or);

	}
	void setBaseOrigin(const double3& bo)
	{
		fprintf(mFile,"setAttr \".bo\" -type \"double3\" ");
		bo.write(mFile);
		fprintf(mFile,";\n");

	}
	void setBaseOriginX(double bx)
	{
		if(bx == 0) return;
		fprintf(mFile,"setAttr \".bo.bx\" %f;\n", bx);

	}
	void setBaseOriginY(double by)
	{
		if(by == 0) return;
		fprintf(mFile,"setAttr \".bo.by\" %f;\n", by);

	}
	void setBaseOriginZ(double bz)
	{
		if(bz == 0) return;
		fprintf(mFile,"setAttr \".bo.bz\" %f;\n", bz);

	}
	void setTargetOrigin(const double3& to)
	{
		fprintf(mFile,"setAttr \".to\" -type \"double3\" ");
		to.write(mFile);
		fprintf(mFile,";\n");

	}
	void setTargetOriginX(double tx)
	{
		if(tx == 0) return;
		fprintf(mFile,"setAttr \".to.tx\" %f;\n", tx);

	}
	void setTargetOriginY(double ty)
	{
		if(ty == 0) return;
		fprintf(mFile,"setAttr \".to.ty\" %f;\n", ty);

	}
	void setTargetOriginZ(double tz)
	{
		if(tz == 0) return;
		fprintf(mFile,"setAttr \".to.tz\" %f;\n", tz);

	}
	void setParallelBlender(bool pb)
	{
		if(pb == 0) return;
		fprintf(mFile,"setAttr \".pb\" %i;\n", pb);

	}
	void getTopologyCheck()
	{
		fprintf(mFile,"\"%s.tc\"",mName.c_str());

	}
	void getWeight(size_t w_i)
	{
		fprintf(mFile,"\"%s.w[%i]\"",mName.c_str(),w_i);

	}
	void getInputTarget(size_t it_i)
	{
		fprintf(mFile,"\"%s.it[%i]\"",mName.c_str(),it_i);

	}
	void getInputTargetGroup(size_t it_i,size_t itg_i)
	{
		fprintf(mFile,"\"%s.it[%i].itg[%i]\"",mName.c_str(),it_i,itg_i);

	}
	void getInputTargetItem(size_t it_i,size_t itg_i,size_t iti_i)
	{
		fprintf(mFile,"\"%s.it[%i].itg[%i].iti[%i]\"",mName.c_str(),it_i,itg_i,iti_i);

	}
	void getInputGeomTarget(size_t it_i,size_t itg_i,size_t iti_i)
	{
		fprintf(mFile,"\"%s.it[%i].itg[%i].iti[%i].igt\"",mName.c_str(),it_i,itg_i,iti_i);

	}
	void getInputPointsTarget(size_t it_i,size_t itg_i,size_t iti_i)
	{
		fprintf(mFile,"\"%s.it[%i].itg[%i].iti[%i].ipt\"",mName.c_str(),it_i,itg_i,iti_i);

	}
	void getInputComponentsTarget(size_t it_i,size_t itg_i,size_t iti_i)
	{
		fprintf(mFile,"\"%s.it[%i].itg[%i].iti[%i].ict\"",mName.c_str(),it_i,itg_i,iti_i);

	}
	void getTargetWeights(size_t it_i,size_t itg_i,size_t tw_i)
	{
		fprintf(mFile,"\"%s.it[%i].itg[%i].tw[%i]\"",mName.c_str(),it_i,itg_i,tw_i);

	}
	void getOrigin()
	{
		fprintf(mFile,"\"%s.or\"",mName.c_str());

	}
	void getBaseOrigin()
	{
		fprintf(mFile,"\"%s.bo\"",mName.c_str());

	}
	void getBaseOriginX()
	{
		fprintf(mFile,"\"%s.bo.bx\"",mName.c_str());

	}
	void getBaseOriginY()
	{
		fprintf(mFile,"\"%s.bo.by\"",mName.c_str());

	}
	void getBaseOriginZ()
	{
		fprintf(mFile,"\"%s.bo.bz\"",mName.c_str());

	}
	void getTargetOrigin()
	{
		fprintf(mFile,"\"%s.to\"",mName.c_str());

	}
	void getTargetOriginX()
	{
		fprintf(mFile,"\"%s.to.tx\"",mName.c_str());

	}
	void getTargetOriginY()
	{
		fprintf(mFile,"\"%s.to.ty\"",mName.c_str());

	}
	void getTargetOriginZ()
	{
		fprintf(mFile,"\"%s.to.tz\"",mName.c_str());

	}
protected:
	BlendShape(FILE* file,const std::string& name,const std::string& parent,const std::string& nodeType):GeometryFilter(file, name, parent, nodeType) {}

};
}//namespace MayaDM
#endif//__MayaDM_BLENDSHAPE_H__
