/*
    Copyright (c) 2008 NetAllied Systems GmbH

    This file is part of MayaDataModel.

    Licensed under the MIT Open Source License,
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/
#ifndef __MayaDM_PFXTOON_H__
#define __MayaDM_PFXTOON_H__
#include "MayaDMTypes.h"
#include "MayaDMConnectables.h"
#include "MayaDMPfxGeometry.h"
namespace MayaDM
{
class PfxToon : public PfxGeometry
{
public:
	struct InputSurface{
		void write(FILE* file) const
		{
		}
	};
	struct CurvatureWidth{
		float curvatureWidth_Position;
		float curvatureWidth_FloatValue;
		unsigned int curvatureWidth_Interp;
		void write(FILE* file) const
		{
			fprintf(file,"%f ", curvatureWidth_Position);
			fprintf(file,"%f ", curvatureWidth_FloatValue);
			fprintf(file,"%i", curvatureWidth_Interp);
		}
	};
public:
	PfxToon(FILE* file,const std::string& name,const std::string& parent=""):PfxGeometry(file, name, parent, "pfxToon"){}
	virtual ~PfxToon(){}
	void setDisplayInViewport(bool div)
	{
		if(div == true) return;
		fprintf(mFile,"setAttr \".div\" %i;\n", div);

	}
	void setProfileLines(unsigned int pln)
	{
		if(pln == 1) return;
		fprintf(mFile,"setAttr \".pln\" %i;\n", pln);

	}
	void setCreaseLines(bool cln)
	{
		if(cln == true) return;
		fprintf(mFile,"setAttr \".cln\" %i;\n", cln);

	}
	void setBorderLines(unsigned int bln)
	{
		if(bln == 1) return;
		fprintf(mFile,"setAttr \".bln\" %i;\n", bln);

	}
	void setIntersectionLines(bool iln)
	{
		if(iln == false) return;
		fprintf(mFile,"setAttr \".iln\" %i;\n", iln);

	}
	void setSelfIntersect(bool sei)
	{
		if(sei == false) return;
		fprintf(mFile,"setAttr \".sei\" %i;\n", sei);

	}
	void setLineWidth(double lwd)
	{
		if(lwd == 0.1) return;
		fprintf(mFile,"setAttr \".lwd\" %f;\n", lwd);

	}
	void setLineWidthMap(double lwm)
	{
		if(lwm == 0.5) return;
		fprintf(mFile,"setAttr \".lwm\" %f;\n", lwm);

	}
	void setLineOpacity(double lop)
	{
		if(lop == 1.0) return;
		fprintf(mFile,"setAttr \".lop\" %f;\n", lop);

	}
	void setLineOpacityMap(double lpm)
	{
		if(lpm == 1.0) return;
		fprintf(mFile,"setAttr \".lpm\" %f;\n", lpm);

	}
	void setLocalOcclusion(unsigned int lcl)
	{
		if(lcl == 0) return;
		fprintf(mFile,"setAttr \".lcl\" %i;\n", lcl);

	}
	void setOcclusionTolerance(double otl)
	{
		if(otl == 0.01) return;
		fprintf(mFile,"setAttr \".otl\" %f;\n", otl);

	}
	void setDepthBias(double dbs)
	{
		if(dbs == 0.0) return;
		fprintf(mFile,"setAttr \".dbs\" %f;\n", dbs);

	}
	void setProfileLineWidth(double plw)
	{
		if(plw == 1.0) return;
		fprintf(mFile,"setAttr \".plw\" %f;\n", plw);

	}
	void setCreaseLineWidth(double clw)
	{
		if(clw == 1.0) return;
		fprintf(mFile,"setAttr \".clw\" %f;\n", clw);

	}
	void setBorderLineWidth(double blw)
	{
		if(blw == 1.0) return;
		fprintf(mFile,"setAttr \".blw\" %f;\n", blw);

	}
	void setIntersectionLineWidth(double ilw)
	{
		if(ilw == 1.0) return;
		fprintf(mFile,"setAttr \".ilw\" %f;\n", ilw);

	}
	void setLineOffset(double lof)
	{
		if(lof == 0.0) return;
		fprintf(mFile,"setAttr \".lof\" %f;\n", lof);

	}
	void setLineOffsetMap(double lom)
	{
		if(lom == 0.0) return;
		fprintf(mFile,"setAttr \".lom\" %f;\n", lom);

	}
	void setLightingBasedWidth(double lbw)
	{
		if(lbw == 0.0) return;
		fprintf(mFile,"setAttr \".lbw\" %f;\n", lbw);

	}
	void setOcclusionWidthScale(bool ows)
	{
		if(ows == true) return;
		fprintf(mFile,"setAttr \".ows\" %i;\n", ows);

	}
	void setDepthOffset(double dff)
	{
		if(dff == 0.0) return;
		fprintf(mFile,"setAttr \".dff\" %f;\n", dff);

	}
	void setCreaseAngleMin(double amn)
	{
		if(amn == 20.0) return;
		fprintf(mFile,"setAttr \".amn\" %f;\n", amn);

	}
	void setCreaseAngleMax(double amx)
	{
		if(amx == 90.0) return;
		fprintf(mFile,"setAttr \".amx\" %f;\n", amx);

	}
	void setHardCreasesOnly(bool hco)
	{
		if(hco == true) return;
		fprintf(mFile,"setAttr \".hco\" %i;\n", hco);

	}
	void setBackfacingCreases(bool bfc)
	{
		if(bfc == true) return;
		fprintf(mFile,"setAttr \".bfc\" %i;\n", bfc);

	}
	void setIntersectionAngleMin(double imn)
	{
		if(imn == 1.0) return;
		fprintf(mFile,"setAttr \".imn\" %f;\n", imn);

	}
	void setIntersectionAngleMax(double imx)
	{
		if(imx == 1.0) return;
		fprintf(mFile,"setAttr \".imx\" %f;\n", imx);

	}
	void setSmoothProfile(bool spf)
	{
		if(spf == true) return;
		fprintf(mFile,"setAttr \".spf\" %i;\n", spf);

	}
	void setTighterProfile(bool tpf)
	{
		if(tpf == false) return;
		fprintf(mFile,"setAttr \".tpf\" %i;\n", tpf);

	}
	void setCurvatureModulation(bool cmo)
	{
		if(cmo == false) return;
		fprintf(mFile,"setAttr \".cmo\" %i;\n", cmo);

	}
	void setCurvatureWidth(size_t cwd_i,const CurvatureWidth& cwd)
	{
		fprintf(mFile,"setAttr \".cwd[%i]\" ",cwd_i);
		cwd.write(mFile);
		fprintf(mFile,";\n");

	}
	void setCurvatureWidth_Position(size_t cwd_i,float cwdp)
	{
		if(cwdp == 0.0) return;
		fprintf(mFile,"setAttr \".cwd[%i].cwdp\" %f;\n", cwd_i,cwdp);

	}
	void setCurvatureWidth_FloatValue(size_t cwd_i,float cwdfv)
	{
		if(cwdfv == 0.0) return;
		fprintf(mFile,"setAttr \".cwd[%i].cwdfv\" %f;\n", cwd_i,cwdfv);

	}
	void setCurvatureWidth_Interp(size_t cwd_i,unsigned int cwdi)
	{
		if(cwdi == 0) return;
		fprintf(mFile,"setAttr \".cwd[%i].cwdi\" %i;\n", cwd_i,cwdi);

	}
	void setProfileWidthModulation(double pwm)
	{
		if(pwm == 0.0) return;
		fprintf(mFile,"setAttr \".pwm\" %f;\n", pwm);

	}
	void setCreaseWidthModulation(double cwm)
	{
		if(cwm == 0.0) return;
		fprintf(mFile,"setAttr \".cwm\" %f;\n", cwm);

	}
	void setBorderWidthModulation(double bwm)
	{
		if(bwm == 0.5) return;
		fprintf(mFile,"setAttr \".bwm\" %f;\n", bwm);

	}
	void setIntersectionWidthModulation(double imd)
	{
		if(imd == 0.0) return;
		fprintf(mFile,"setAttr \".imd\" %f;\n", imd);

	}
	void setProfileBreakAngle(double pba)
	{
		if(pba == 180.0) return;
		fprintf(mFile,"setAttr \".pba\" %f;\n", pba);

	}
	void setCreaseBreakAngle(double cba)
	{
		if(cba == 80) return;
		fprintf(mFile,"setAttr \".cba\" %f;\n", cba);

	}
	void setBorderBreakAngle(double bba)
	{
		if(bba == 80.0) return;
		fprintf(mFile,"setAttr \".bba\" %f;\n", bba);

	}
	void setIntersectionBreakAngle(double iba)
	{
		if(iba == 180.0) return;
		fprintf(mFile,"setAttr \".iba\" %f;\n", iba);

	}
	void setRemoveFlushBorders(bool rfb)
	{
		if(rfb == false) return;
		fprintf(mFile,"setAttr \".rfb\" %i;\n", rfb);

	}
	void setFlushTolerance(double tfl)
	{
		if(tfl == 0.01) return;
		fprintf(mFile,"setAttr \".tfl\" %f;\n", tfl);

	}
	void setFlushAngleMax(double fmx)
	{
		if(fmx == 4.0) return;
		fprintf(mFile,"setAttr \".fmx\" %f;\n", fmx);

	}
	void setLineEndThinning(double let)
	{
		if(let == 0.0) return;
		fprintf(mFile,"setAttr \".let\" %f;\n", let);

	}
	void setLineExtend(double lex)
	{
		if(lex == 0.0) return;
		fprintf(mFile,"setAttr \".lex\" %f;\n", lex);

	}
	void setResampleProfile(bool rpf)
	{
		if(rpf == false) return;
		fprintf(mFile,"setAttr \".rpf\" %i;\n", rpf);

	}
	void setResampleCrease(bool rcr)
	{
		if(rcr == false) return;
		fprintf(mFile,"setAttr \".rcr\" %i;\n", rcr);

	}
	void setResampleBorder(bool rbd)
	{
		if(rbd == false) return;
		fprintf(mFile,"setAttr \".rbd\" %i;\n", rbd);

	}
	void setResampleIntersection(bool rin)
	{
		if(rin == false) return;
		fprintf(mFile,"setAttr \".rin\" %i;\n", rin);

	}
	void setMaxSegmentLength(double msl)
	{
		if(msl == 0.5) return;
		fprintf(mFile,"setAttr \".msl\" %f;\n", msl);

	}
	void setMinSegmentLength(double mns)
	{
		if(mns == 0.0) return;
		fprintf(mFile,"setAttr \".mns\" %f;\n", mns);

	}
	void setScreenSpaceResampling(double ssr)
	{
		if(ssr == 0.0) return;
		fprintf(mFile,"setAttr \".ssr\" %f;\n", ssr);

	}
	void setPfxRandomize(bool prz)
	{
		if(prz == false) return;
		fprintf(mFile,"setAttr \".prz\" %i;\n", prz);

	}
	void setScreenspaceWidth(bool spw)
	{
		if(spw == false) return;
		fprintf(mFile,"setAttr \".spw\" %i;\n", spw);

	}
	void setDistanceScaling(double dsl)
	{
		if(dsl == 0.0) return;
		fprintf(mFile,"setAttr \".dsl\" %f;\n", dsl);

	}
	void setMinPixelWidth(double mpw)
	{
		if(mpw == 0.0) return;
		fprintf(mFile,"setAttr \".mpw\" %f;\n", mpw);

	}
	void setMaxPixelWidth(double mxp)
	{
		if(mxp == 1000.0) return;
		fprintf(mFile,"setAttr \".mxp\" %f;\n", mxp);

	}
	void setProfileColor(const float3& pcl)
	{
		fprintf(mFile,"setAttr \".pcl\" -type \"float3\" ");
		pcl.write(mFile);
		fprintf(mFile,";\n");

	}
	void setProfileColorR(float pcr)
	{
		if(pcr == 0.0) return;
		fprintf(mFile,"setAttr \".pcl.pcr\" %f;\n", pcr);

	}
	void setProfileColorG(float pcg)
	{
		if(pcg == 0.0) return;
		fprintf(mFile,"setAttr \".pcl.pcg\" %f;\n", pcg);

	}
	void setProfileColorB(float pcb)
	{
		if(pcb == 0.0) return;
		fprintf(mFile,"setAttr \".pcl.pcb\" %f;\n", pcb);

	}
	void setCreaseColor(const float3& ccl)
	{
		fprintf(mFile,"setAttr \".ccl\" -type \"float3\" ");
		ccl.write(mFile);
		fprintf(mFile,";\n");

	}
	void setCreaseColorR(float ccr)
	{
		if(ccr == 0.0) return;
		fprintf(mFile,"setAttr \".ccl.ccr\" %f;\n", ccr);

	}
	void setCreaseColorG(float ccg)
	{
		if(ccg == 0.0) return;
		fprintf(mFile,"setAttr \".ccl.ccg\" %f;\n", ccg);

	}
	void setCreaseColorB(float ccb)
	{
		if(ccb == 0.0) return;
		fprintf(mFile,"setAttr \".ccl.ccb\" %f;\n", ccb);

	}
	void setBorderColor(const float3& bcl)
	{
		fprintf(mFile,"setAttr \".bcl\" -type \"float3\" ");
		bcl.write(mFile);
		fprintf(mFile,";\n");

	}
	void setBorderColorR(float bcr)
	{
		if(bcr == 0.0) return;
		fprintf(mFile,"setAttr \".bcl.bcr\" %f;\n", bcr);

	}
	void setBorderColorG(float bcg)
	{
		if(bcg == 0.0) return;
		fprintf(mFile,"setAttr \".bcl.bcg\" %f;\n", bcg);

	}
	void setBorderColorB(float bcb)
	{
		if(bcb == 0.0) return;
		fprintf(mFile,"setAttr \".bcl.bcb\" %f;\n", bcb);

	}
	void setIntersectionColor(const float3& icl)
	{
		fprintf(mFile,"setAttr \".icl\" -type \"float3\" ");
		icl.write(mFile);
		fprintf(mFile,";\n");

	}
	void setIntersectionColorR(float icr)
	{
		if(icr == 0.0) return;
		fprintf(mFile,"setAttr \".icl.icr\" %f;\n", icr);

	}
	void setIntersectionColorG(float icg)
	{
		if(icg == 0.0) return;
		fprintf(mFile,"setAttr \".icl.icg\" %f;\n", icg);

	}
	void setIntersectionColorB(float icb)
	{
		if(icb == 0.0) return;
		fprintf(mFile,"setAttr \".icl.icb\" %f;\n", icb);

	}
	void getInputSurface(size_t ins_i)
	{
		fprintf(mFile,"\"%s.ins[%i]\"",mName.c_str(),ins_i);

	}
	void getSurface(size_t ins_i)
	{
		fprintf(mFile,"\"%s.ins[%i].srf\"",mName.c_str(),ins_i);

	}
	void getInputWorldMatrix(size_t ins_i)
	{
		fprintf(mFile,"\"%s.ins[%i].iwm\"",mName.c_str(),ins_i);

	}
	void getDisplayInViewport()
	{
		fprintf(mFile,"\"%s.div\"",mName.c_str());

	}
	void getProfileLines()
	{
		fprintf(mFile,"\"%s.pln\"",mName.c_str());

	}
	void getCreaseLines()
	{
		fprintf(mFile,"\"%s.cln\"",mName.c_str());

	}
	void getBorderLines()
	{
		fprintf(mFile,"\"%s.bln\"",mName.c_str());

	}
	void getIntersectionLines()
	{
		fprintf(mFile,"\"%s.iln\"",mName.c_str());

	}
	void getSelfIntersect()
	{
		fprintf(mFile,"\"%s.sei\"",mName.c_str());

	}
	void getLineWidth()
	{
		fprintf(mFile,"\"%s.lwd\"",mName.c_str());

	}
	void getLineWidthMap()
	{
		fprintf(mFile,"\"%s.lwm\"",mName.c_str());

	}
	void getLineOpacity()
	{
		fprintf(mFile,"\"%s.lop\"",mName.c_str());

	}
	void getLineOpacityMap()
	{
		fprintf(mFile,"\"%s.lpm\"",mName.c_str());

	}
	void getLocalOcclusion()
	{
		fprintf(mFile,"\"%s.lcl\"",mName.c_str());

	}
	void getOcclusionTolerance()
	{
		fprintf(mFile,"\"%s.otl\"",mName.c_str());

	}
	void getDepthBias()
	{
		fprintf(mFile,"\"%s.dbs\"",mName.c_str());

	}
	void getProfileLineWidth()
	{
		fprintf(mFile,"\"%s.plw\"",mName.c_str());

	}
	void getCreaseLineWidth()
	{
		fprintf(mFile,"\"%s.clw\"",mName.c_str());

	}
	void getBorderLineWidth()
	{
		fprintf(mFile,"\"%s.blw\"",mName.c_str());

	}
	void getIntersectionLineWidth()
	{
		fprintf(mFile,"\"%s.ilw\"",mName.c_str());

	}
	void getLineOffset()
	{
		fprintf(mFile,"\"%s.lof\"",mName.c_str());

	}
	void getLineOffsetMap()
	{
		fprintf(mFile,"\"%s.lom\"",mName.c_str());

	}
	void getLightingBasedWidth()
	{
		fprintf(mFile,"\"%s.lbw\"",mName.c_str());

	}
	void getOcclusionWidthScale()
	{
		fprintf(mFile,"\"%s.ows\"",mName.c_str());

	}
	void getDepthOffset()
	{
		fprintf(mFile,"\"%s.dff\"",mName.c_str());

	}
	void getCreaseAngleMin()
	{
		fprintf(mFile,"\"%s.amn\"",mName.c_str());

	}
	void getCreaseAngleMax()
	{
		fprintf(mFile,"\"%s.amx\"",mName.c_str());

	}
	void getHardCreasesOnly()
	{
		fprintf(mFile,"\"%s.hco\"",mName.c_str());

	}
	void getBackfacingCreases()
	{
		fprintf(mFile,"\"%s.bfc\"",mName.c_str());

	}
	void getIntersectionAngleMin()
	{
		fprintf(mFile,"\"%s.imn\"",mName.c_str());

	}
	void getIntersectionAngleMax()
	{
		fprintf(mFile,"\"%s.imx\"",mName.c_str());

	}
	void getSmoothProfile()
	{
		fprintf(mFile,"\"%s.spf\"",mName.c_str());

	}
	void getTighterProfile()
	{
		fprintf(mFile,"\"%s.tpf\"",mName.c_str());

	}
	void getCurvatureModulation()
	{
		fprintf(mFile,"\"%s.cmo\"",mName.c_str());

	}
	void getCurvatureWidth(size_t cwd_i)
	{
		fprintf(mFile,"\"%s.cwd[%i]\"",mName.c_str(),cwd_i);

	}
	void getCurvatureWidth_Position(size_t cwd_i)
	{
		fprintf(mFile,"\"%s.cwd[%i].cwdp\"",mName.c_str(),cwd_i);

	}
	void getCurvatureWidth_FloatValue(size_t cwd_i)
	{
		fprintf(mFile,"\"%s.cwd[%i].cwdfv\"",mName.c_str(),cwd_i);

	}
	void getCurvatureWidth_Interp(size_t cwd_i)
	{
		fprintf(mFile,"\"%s.cwd[%i].cwdi\"",mName.c_str(),cwd_i);

	}
	void getProfileWidthModulation()
	{
		fprintf(mFile,"\"%s.pwm\"",mName.c_str());

	}
	void getCreaseWidthModulation()
	{
		fprintf(mFile,"\"%s.cwm\"",mName.c_str());

	}
	void getBorderWidthModulation()
	{
		fprintf(mFile,"\"%s.bwm\"",mName.c_str());

	}
	void getIntersectionWidthModulation()
	{
		fprintf(mFile,"\"%s.imd\"",mName.c_str());

	}
	void getProfileBreakAngle()
	{
		fprintf(mFile,"\"%s.pba\"",mName.c_str());

	}
	void getCreaseBreakAngle()
	{
		fprintf(mFile,"\"%s.cba\"",mName.c_str());

	}
	void getBorderBreakAngle()
	{
		fprintf(mFile,"\"%s.bba\"",mName.c_str());

	}
	void getIntersectionBreakAngle()
	{
		fprintf(mFile,"\"%s.iba\"",mName.c_str());

	}
	void getRemoveFlushBorders()
	{
		fprintf(mFile,"\"%s.rfb\"",mName.c_str());

	}
	void getFlushTolerance()
	{
		fprintf(mFile,"\"%s.tfl\"",mName.c_str());

	}
	void getFlushAngleMax()
	{
		fprintf(mFile,"\"%s.fmx\"",mName.c_str());

	}
	void getLineEndThinning()
	{
		fprintf(mFile,"\"%s.let\"",mName.c_str());

	}
	void getLineExtend()
	{
		fprintf(mFile,"\"%s.lex\"",mName.c_str());

	}
	void getResampleProfile()
	{
		fprintf(mFile,"\"%s.rpf\"",mName.c_str());

	}
	void getResampleCrease()
	{
		fprintf(mFile,"\"%s.rcr\"",mName.c_str());

	}
	void getResampleBorder()
	{
		fprintf(mFile,"\"%s.rbd\"",mName.c_str());

	}
	void getResampleIntersection()
	{
		fprintf(mFile,"\"%s.rin\"",mName.c_str());

	}
	void getMaxSegmentLength()
	{
		fprintf(mFile,"\"%s.msl\"",mName.c_str());

	}
	void getMinSegmentLength()
	{
		fprintf(mFile,"\"%s.mns\"",mName.c_str());

	}
	void getScreenSpaceResampling()
	{
		fprintf(mFile,"\"%s.ssr\"",mName.c_str());

	}
	void getPfxRandomize()
	{
		fprintf(mFile,"\"%s.prz\"",mName.c_str());

	}
	void getScreenspaceWidth()
	{
		fprintf(mFile,"\"%s.spw\"",mName.c_str());

	}
	void getDistanceScaling()
	{
		fprintf(mFile,"\"%s.dsl\"",mName.c_str());

	}
	void getMinPixelWidth()
	{
		fprintf(mFile,"\"%s.mpw\"",mName.c_str());

	}
	void getMaxPixelWidth()
	{
		fprintf(mFile,"\"%s.mxp\"",mName.c_str());

	}
	void getProfileColor()
	{
		fprintf(mFile,"\"%s.pcl\"",mName.c_str());

	}
	void getProfileColorR()
	{
		fprintf(mFile,"\"%s.pcl.pcr\"",mName.c_str());

	}
	void getProfileColorG()
	{
		fprintf(mFile,"\"%s.pcl.pcg\"",mName.c_str());

	}
	void getProfileColorB()
	{
		fprintf(mFile,"\"%s.pcl.pcb\"",mName.c_str());

	}
	void getCreaseColor()
	{
		fprintf(mFile,"\"%s.ccl\"",mName.c_str());

	}
	void getCreaseColorR()
	{
		fprintf(mFile,"\"%s.ccl.ccr\"",mName.c_str());

	}
	void getCreaseColorG()
	{
		fprintf(mFile,"\"%s.ccl.ccg\"",mName.c_str());

	}
	void getCreaseColorB()
	{
		fprintf(mFile,"\"%s.ccl.ccb\"",mName.c_str());

	}
	void getBorderColor()
	{
		fprintf(mFile,"\"%s.bcl\"",mName.c_str());

	}
	void getBorderColorR()
	{
		fprintf(mFile,"\"%s.bcl.bcr\"",mName.c_str());

	}
	void getBorderColorG()
	{
		fprintf(mFile,"\"%s.bcl.bcg\"",mName.c_str());

	}
	void getBorderColorB()
	{
		fprintf(mFile,"\"%s.bcl.bcb\"",mName.c_str());

	}
	void getIntersectionColor()
	{
		fprintf(mFile,"\"%s.icl\"",mName.c_str());

	}
	void getIntersectionColorR()
	{
		fprintf(mFile,"\"%s.icl.icr\"",mName.c_str());

	}
	void getIntersectionColorG()
	{
		fprintf(mFile,"\"%s.icl.icg\"",mName.c_str());

	}
	void getIntersectionColorB()
	{
		fprintf(mFile,"\"%s.icl.icb\"",mName.c_str());

	}
	void getOutColor()
	{
		fprintf(mFile,"\"%s.ocl\"",mName.c_str());

	}
	void getOutColorR()
	{
		fprintf(mFile,"\"%s.ocl.ocr\"",mName.c_str());

	}
	void getOutColorG()
	{
		fprintf(mFile,"\"%s.ocl.ocg\"",mName.c_str());

	}
	void getOutColorB()
	{
		fprintf(mFile,"\"%s.ocl.ocb\"",mName.c_str());

	}
	void getOutProfileMesh(size_t opm_i)
	{
		fprintf(mFile,"\"%s.opm[%i]\"",mName.c_str(),opm_i);

	}
protected:
	PfxToon(FILE* file,const std::string& name,const std::string& parent,const std::string& nodeType):PfxGeometry(file, name, parent, nodeType) {}

};
}//namespace MayaDM
#endif//__MayaDM_PFXTOON_H__
