/*
    Copyright (c) 2008 NetAllied Systems GmbH

    This file is part of MayaDataModel.

    Licensed under the MIT Open Source License,
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/
#ifndef __MayaDM_RENDERGLOBALS_H__
#define __MayaDM_RENDERGLOBALS_H__
#include "MayaDMTypes.h"
#include "MayaDMConnectables.h"
#include "MayaDMDependNode.h"
namespace MayaDM
{
class RenderGlobals : public DependNode
{
public:
public:
	RenderGlobals(FILE* file,const std::string& name,const std::string& parent=""):DependNode(file, name, parent, "renderGlobals"){}
	virtual ~RenderGlobals(){}
	void setMacCodec(int macc)
	{
		if(macc == 1919706400) return;
		fprintf(mFile,"setAttr \".macc\" %i;\n", macc);

	}
	void setMacDepth(int macd)
	{
		if(macd == 32) return;
		fprintf(mFile,"setAttr \".macd\" %i;\n", macd);

	}
	void setMacQual(int macq)
	{
		if(macq == 1024) return;
		fprintf(mFile,"setAttr \".macq\" %i;\n", macq);

	}
	void setComFrrt(int mcfr)
	{
		if(mcfr == 24) return;
		fprintf(mFile,"setAttr \".mcfr\" %i;\n", mcfr);

	}
	void setIgnoreFilmGate(bool ifg)
	{
		if(ifg == true) return;
		fprintf(mFile,"setAttr \".ifg\" %i;\n", ifg);

	}
	void setClipFinalShadedColor(bool clip)
	{
		if(clip == true) return;
		fprintf(mFile,"setAttr \".clip\" %i;\n", clip);

	}
	void setEnableDepthMaps(bool edm)
	{
		if(edm == true) return;
		fprintf(mFile,"setAttr \".edm\" %i;\n", edm);

	}
	void setEnableDefaultLight(bool edl)
	{
		if(edl == true) return;
		fprintf(mFile,"setAttr \".edl\" %i;\n", edl);

	}
	void setCurrentRenderer(const string& ren)
	{
		if(ren == "mayaSoftware") return;
		fprintf(mFile,"setAttr \".ren\" -type \"string\" ");
		ren.write(mFile);
		fprintf(mFile,";\n");

	}
	void setEnableStrokeRender(bool esr)
	{
		if(esr == true) return;
		fprintf(mFile,"setAttr \".esr\" %i;\n", esr);

	}
	void setOnlyRenderStrokes(bool ors)
	{
		if(ors == false) return;
		fprintf(mFile,"setAttr \".ors\" %i;\n", ors);

	}
	void setStrokesDepthFile(const string& sdf)
	{
		if(sdf == "NULL") return;
		fprintf(mFile,"setAttr \".sdf\" -type \"string\" ");
		sdf.write(mFile);
		fprintf(mFile,";\n");

	}
	void setImageFormat(unsigned int outf)
	{
		if(outf == 7) return;
		fprintf(mFile,"setAttr \".outf\" %i;\n", outf);

	}
	void setImfPluginKey(const string& imfkey)
	{
		if(imfkey == "") return;
		fprintf(mFile,"setAttr \".imfkey\" -type \"string\" ");
		imfkey.write(mFile);
		fprintf(mFile,";\n");

	}
	void setGammaCorrection(float gama)
	{
		if(gama == 1.0) return;
		fprintf(mFile,"setAttr \".gama\" %f;\n", gama);

	}
	void setAnimation(bool an)
	{
		if(an == false) return;
		fprintf(mFile,"setAttr \".an\" %i;\n", an);

	}
	void setAnimationRange(unsigned int ar)
	{
		if(ar == 1) return;
		fprintf(mFile,"setAttr \".ar\" %i;\n", ar);

	}
	void setByFrameStep(float bfs)
	{
		if(bfs == 1.0) return;
		fprintf(mFile,"setAttr \".bfs\" %f;\n", bfs);

	}
	void setModifyExtension(bool me)
	{
		if(me == false) return;
		fprintf(mFile,"setAttr \".me\" %i;\n", me);

	}
	void setStartExtension(float se)
	{
		if(se == 1.0) return;
		fprintf(mFile,"setAttr \".se\" %f;\n", se);

	}
	void setByExtension(float be)
	{
		if(be == 1.0) return;
		fprintf(mFile,"setAttr \".be\" %f;\n", be);

	}
	void setExtensionPadding(int ep)
	{
		if(ep == 1) return;
		fprintf(mFile,"setAttr \".ep\" %i;\n", ep);

	}
	void setFieldExtControl(unsigned int fec)
	{
		if(fec == 0) return;
		fprintf(mFile,"setAttr \".fec\" %i;\n", fec);

	}
	void setOutFormatControl(unsigned int ofc)
	{
		if(ofc == 0) return;
		fprintf(mFile,"setAttr \".ofc\" %i;\n", ofc);

	}
	void setOddFieldExt(const string& ofe)
	{
		if(ofe == "NULL") return;
		fprintf(mFile,"setAttr \".ofe\" -type \"string\" ");
		ofe.write(mFile);
		fprintf(mFile,";\n");

	}
	void setEvenFieldExt(const string& efe)
	{
		if(efe == "NULL") return;
		fprintf(mFile,"setAttr \".efe\" -type \"string\" ");
		efe.write(mFile);
		fprintf(mFile,";\n");

	}
	void setOutFormatExt(const string& oft)
	{
		if(oft == "NULL") return;
		fprintf(mFile,"setAttr \".oft\" -type \"string\" ");
		oft.write(mFile);
		fprintf(mFile,";\n");

	}
	void setUseMayaFileName(bool umfn)
	{
		if(umfn == true) return;
		fprintf(mFile,"setAttr \".umfn\" %i;\n", umfn);

	}
	void setUseFrameExt(bool ufe)
	{
		if(ufe == false) return;
		fprintf(mFile,"setAttr \".ufe\" %i;\n", ufe);

	}
	void setPutFrameBeforeExt(bool pff)
	{
		if(pff == false) return;
		fprintf(mFile,"setAttr \".pff\" %i;\n", pff);

	}
	void setPeriodInExt(unsigned int peie)
	{
		if(peie == 1) return;
		fprintf(mFile,"setAttr \".peie\" %i;\n", peie);

	}
	void setImageFilePrefix(const string& ifp)
	{
		if(ifp == "NULL") return;
		fprintf(mFile,"setAttr \".ifp\" -type \"string\" ");
		ifp.write(mFile);
		fprintf(mFile,";\n");

	}
	void setRenderVersion(const string& rv)
	{
		if(rv == "NULL") return;
		fprintf(mFile,"setAttr \".rv\" -type \"string\" ");
		rv.write(mFile);
		fprintf(mFile,";\n");

	}
	void setComposite(bool comp)
	{
		if(comp == false) return;
		fprintf(mFile,"setAttr \".comp\" %i;\n", comp);

	}
	void setCompositeThreshold(float cth)
	{
		if(cth == 0.0) return;
		fprintf(mFile,"setAttr \".cth\" %f;\n", cth);

	}
	void setShadowsObeyLightLinking(bool soll)
	{
		if(soll == true) return;
		fprintf(mFile,"setAttr \".soll\" %i;\n", soll);

	}
	void setShadowsObeyShadowLinking(bool sosl)
	{
		if(sosl == false) return;
		fprintf(mFile,"setAttr \".sosl\" %i;\n", sosl);

	}
	void setRecursionDepth(int rd)
	{
		if(rd == 2) return;
		fprintf(mFile,"setAttr \".rd\" %i;\n", rd);

	}
	void setLeafPrimitives(int lp)
	{
		if(lp == 200) return;
		fprintf(mFile,"setAttr \".lp\" %i;\n", lp);

	}
	void setSubdivisionPower(float sp)
	{
		if(sp == 0.25) return;
		fprintf(mFile,"setAttr \".sp\" %f;\n", sp);

	}
	void setSubdivisionHashSize(int shs)
	{
		if(shs == 5) return;
		fprintf(mFile,"setAttr \".shs\" %i;\n", shs);

	}
	void setLogRenderPerformance(bool lpr)
	{
		if(lpr == false) return;
		fprintf(mFile,"setAttr \".lpr\" %i;\n", lpr);

	}
	void setGeometryVector(int gv)
	{
		if(gv == 20) return;
		fprintf(mFile,"setAttr \".gv\" %i;\n", gv);

	}
	void setShadingVector(int sv)
	{
		if(sv == 60) return;
		fprintf(mFile,"setAttr \".sv\" %i;\n", sv);

	}
	void setMaximumMemory(int mm)
	{
		if(mm == 48) return;
		fprintf(mFile,"setAttr \".mm\" %i;\n", mm);

	}
	void setNumCpusToUse(int npu)
	{
		if(npu == 0) return;
		fprintf(mFile,"setAttr \".npu\" %i;\n", npu);

	}
	void setInterruptFrequency(int itf)
	{
		if(itf == 1) return;
		fprintf(mFile,"setAttr \".itf\" %i;\n", itf);

	}
	void setShadowPass(bool shp)
	{
		if(shp == false) return;
		fprintf(mFile,"setAttr \".shp\" %i;\n", shp);

	}
	void setIprShadowPass(bool isp)
	{
		if(isp == false) return;
		fprintf(mFile,"setAttr \".isp\" %i;\n", isp);

	}
	void setUseFileCache(bool uf)
	{
		if(uf == true) return;
		fprintf(mFile,"setAttr \".uf\" %i;\n", uf);

	}
	void setOptimizeInstances(bool oi)
	{
		if(oi == true) return;
		fprintf(mFile,"setAttr \".oi\" %i;\n", oi);

	}
	void setReuseTessellations(bool rut)
	{
		if(rut == true) return;
		fprintf(mFile,"setAttr \".rut\" %i;\n", rut);

	}
	void setMatteOpacityUsesTransparency(bool mot)
	{
		if(mot == true) return;
		fprintf(mFile,"setAttr \".mot\" %i;\n", mot);

	}
	void setMotionBlur(bool mb)
	{
		if(mb == false) return;
		fprintf(mFile,"setAttr \".mb\" %i;\n", mb);

	}
	void setMotionBlurByFrame(float mbf)
	{
		if(mbf == 1.0) return;
		fprintf(mFile,"setAttr \".mbf\" %f;\n", mbf);

	}
	void setApplyFogInPost(bool afp)
	{
		if(afp == false) return;
		fprintf(mFile,"setAttr \".afp\" %i;\n", afp);

	}
	void setPostFogBlur(int pfb)
	{
		if(pfb == 1) return;
		fprintf(mFile,"setAttr \".pfb\" %i;\n", pfb);

	}
	void setPreMel(const string& pram)
	{
		if(pram == "NULL") return;
		fprintf(mFile,"setAttr \".pram\" -type \"string\" ");
		pram.write(mFile);
		fprintf(mFile,";\n");

	}
	void setPostMel(const string& poam)
	{
		if(poam == "NULL") return;
		fprintf(mFile,"setAttr \".poam\" -type \"string\" ");
		poam.write(mFile);
		fprintf(mFile,";\n");

	}
	void setPreRenderLayerMel(const string& prlm)
	{
		if(prlm == "NULL") return;
		fprintf(mFile,"setAttr \".prlm\" -type \"string\" ");
		prlm.write(mFile);
		fprintf(mFile,";\n");

	}
	void setPostRenderLayerMel(const string& polm)
	{
		if(polm == "NULL") return;
		fprintf(mFile,"setAttr \".polm\" -type \"string\" ");
		polm.write(mFile);
		fprintf(mFile,";\n");

	}
	void setPreRenderMel(const string& prm)
	{
		if(prm == "NULL") return;
		fprintf(mFile,"setAttr \".prm\" -type \"string\" ");
		prm.write(mFile);
		fprintf(mFile,";\n");

	}
	void setPostRenderMel(const string& pom)
	{
		if(pom == "NULL") return;
		fprintf(mFile,"setAttr \".pom\" -type \"string\" ");
		pom.write(mFile);
		fprintf(mFile,";\n");

	}
	void setPreFurRenderMel(const string& pfrm)
	{
		if(pfrm == "NULL") return;
		fprintf(mFile,"setAttr \".pfrm\" -type \"string\" ");
		pfrm.write(mFile);
		fprintf(mFile,";\n");

	}
	void setPostFurRenderMel(const string& pfom)
	{
		if(pfom == "NULL") return;
		fprintf(mFile,"setAttr \".pfom\" -type \"string\" ");
		pfom.write(mFile);
		fprintf(mFile,";\n");

	}
	void setBlurLength(float bll)
	{
		if(bll == 1.0) return;
		fprintf(mFile,"setAttr \".bll\" %f;\n", bll);

	}
	void setBlurSharpness(float bls)
	{
		if(bls == 1.0) return;
		fprintf(mFile,"setAttr \".bls\" %f;\n", bls);

	}
	void setSmoothValue(int smv)
	{
		if(smv == 2) return;
		fprintf(mFile,"setAttr \".smv\" %i;\n", smv);

	}
	void setUseBlur2DMemoryCap(bool ubc)
	{
		if(ubc == true) return;
		fprintf(mFile,"setAttr \".ubc\" %i;\n", ubc);

	}
	void setBlur2DMemoryCap(float mbc)
	{
		if(mbc == 200.0) return;
		fprintf(mFile,"setAttr \".mbc\" %f;\n", mbc);

	}
	void setMotionBlurType(unsigned int mbt)
	{
		if(mbt == 1) return;
		fprintf(mFile,"setAttr \".mbt\" %i;\n", mbt);

	}
	void setUseDisplacementBoundingBox(bool udbx)
	{
		if(udbx == true) return;
		fprintf(mFile,"setAttr \".udbx\" %i;\n", udbx);

	}
	void setSmoothColor(bool smc)
	{
		if(smc == false) return;
		fprintf(mFile,"setAttr \".smc\" %i;\n", smc);

	}
	void setKeepMotionVector(bool kmv)
	{
		if(kmv == false) return;
		fprintf(mFile,"setAttr \".kmv\" %i;\n", kmv);

	}
	void setIprRenderShading(bool isl)
	{
		if(isl == true) return;
		fprintf(mFile,"setAttr \".isl\" %i;\n", isl);

	}
	void setIprRenderShadowMaps(bool ism)
	{
		if(ism == true) return;
		fprintf(mFile,"setAttr \".ism\" %i;\n", ism);

	}
	void setIprRenderMotionBlur(bool imb)
	{
		if(imb == true) return;
		fprintf(mFile,"setAttr \".imb\" %i;\n", imb);

	}
	void setRenderLayerEnable(bool rlen)
	{
		if(rlen == false) return;
		fprintf(mFile,"setAttr \".rlen\" %i;\n", rlen);

	}
	void setForceTileSize(bool frts)
	{
		if(frts == false) return;
		fprintf(mFile,"setAttr \".frts\" %i;\n", frts);

	}
	void setTileWidth(short tlwd)
	{
		if(tlwd == 64) return;
		fprintf(mFile,"setAttr \".tlwd\" %i;\n", tlwd);

	}
	void setTileHeight(short tlht)
	{
		if(tlht == 64) return;
		fprintf(mFile,"setAttr \".tlht\" %i;\n", tlht);

	}
	void setJitterFinalColor(bool jfc)
	{
		if(jfc == true) return;
		fprintf(mFile,"setAttr \".jfc\" %i;\n", jfc);

	}
	void setRaysSeeBackground(bool rsb)
	{
		if(rsb == true) return;
		fprintf(mFile,"setAttr \".rsb\" %i;\n", rsb);

	}
	void setOversamplePaintEffects(bool ope)
	{
		if(ope == false) return;
		fprintf(mFile,"setAttr \".ope\" %i;\n", ope);

	}
	void setOversamplePfxPostFilter(bool oppf)
	{
		if(oppf == false) return;
		fprintf(mFile,"setAttr \".oppf\" %i;\n", oppf);

	}
	void setHyperShadeBinList(const string& hbl)
	{
		if(hbl == "NULL") return;
		fprintf(mFile,"setAttr \".hbl\" -type \"string\" ");
		hbl.write(mFile);
		fprintf(mFile,";\n");

	}
	void getMacCodec()
	{
		fprintf(mFile,"\"%s.macc\"",mName.c_str());

	}
	void getMacDepth()
	{
		fprintf(mFile,"\"%s.macd\"",mName.c_str());

	}
	void getMacQual()
	{
		fprintf(mFile,"\"%s.macq\"",mName.c_str());

	}
	void getComFrrt()
	{
		fprintf(mFile,"\"%s.mcfr\"",mName.c_str());

	}
	void getRenderAll()
	{
		fprintf(mFile,"\"%s.ra\"",mName.c_str());

	}
	void getQuality()
	{
		fprintf(mFile,"\"%s.qual\"",mName.c_str());

	}
	void getResolution()
	{
		fprintf(mFile,"\"%s.res\"",mName.c_str());

	}
	void getClipFinalShadedColor()
	{
		fprintf(mFile,"\"%s.clip\"",mName.c_str());

	}
	void getEnableDepthMaps()
	{
		fprintf(mFile,"\"%s.edm\"",mName.c_str());

	}
	void getEnableDefaultLight()
	{
		fprintf(mFile,"\"%s.edl\"",mName.c_str());

	}
	void getEnableStrokeRender()
	{
		fprintf(mFile,"\"%s.esr\"",mName.c_str());

	}
	void getOnlyRenderStrokes()
	{
		fprintf(mFile,"\"%s.ors\"",mName.c_str());

	}
	void getStrokesDepthFile()
	{
		fprintf(mFile,"\"%s.sdf\"",mName.c_str());

	}
	void getImageFormat()
	{
		fprintf(mFile,"\"%s.outf\"",mName.c_str());

	}
	void getImfPluginKey()
	{
		fprintf(mFile,"\"%s.imfkey\"",mName.c_str());

	}
	void getGammaCorrection()
	{
		fprintf(mFile,"\"%s.gama\"",mName.c_str());

	}
	void getTopRegion()
	{
		fprintf(mFile,"\"%s.top\"",mName.c_str());

	}
	void getLeftRegion()
	{
		fprintf(mFile,"\"%s.left\"",mName.c_str());

	}
	void getBottomRegion()
	{
		fprintf(mFile,"\"%s.bot\"",mName.c_str());

	}
	void getRightRegion()
	{
		fprintf(mFile,"\"%s.rght\"",mName.c_str());

	}
	void getUseRenderRegion()
	{
		fprintf(mFile,"\"%s.urr\"",mName.c_str());

	}
	void getByFrame()
	{
		fprintf(mFile,"\"%s.bf\"",mName.c_str());

	}
	void getByFrameStep()
	{
		fprintf(mFile,"\"%s.bfs\"",mName.c_str());

	}
	void getByExtension()
	{
		fprintf(mFile,"\"%s.be\"",mName.c_str());

	}
	void getFieldExtControl()
	{
		fprintf(mFile,"\"%s.fec\"",mName.c_str());

	}
	void getOutFormatControl()
	{
		fprintf(mFile,"\"%s.ofc\"",mName.c_str());

	}
	void getOddFieldExt()
	{
		fprintf(mFile,"\"%s.ofe\"",mName.c_str());

	}
	void getEvenFieldExt()
	{
		fprintf(mFile,"\"%s.efe\"",mName.c_str());

	}
	void getOutFormatExt()
	{
		fprintf(mFile,"\"%s.oft\"",mName.c_str());

	}
	void getComposite()
	{
		fprintf(mFile,"\"%s.comp\"",mName.c_str());

	}
	void getCompositeThreshold()
	{
		fprintf(mFile,"\"%s.cth\"",mName.c_str());

	}
	void getShadowsObeyLightLinking()
	{
		fprintf(mFile,"\"%s.soll\"",mName.c_str());

	}
	void getShadowsObeyShadowLinking()
	{
		fprintf(mFile,"\"%s.sosl\"",mName.c_str());

	}
	void getRecursionDepth()
	{
		fprintf(mFile,"\"%s.rd\"",mName.c_str());

	}
	void getLeafPrimitives()
	{
		fprintf(mFile,"\"%s.lp\"",mName.c_str());

	}
	void getSubdivisionPower()
	{
		fprintf(mFile,"\"%s.sp\"",mName.c_str());

	}
	void getSubdivisionHashSize()
	{
		fprintf(mFile,"\"%s.shs\"",mName.c_str());

	}
	void getLogRenderPerformance()
	{
		fprintf(mFile,"\"%s.lpr\"",mName.c_str());

	}
	void getGeometryVector()
	{
		fprintf(mFile,"\"%s.gv\"",mName.c_str());

	}
	void getShadingVector()
	{
		fprintf(mFile,"\"%s.sv\"",mName.c_str());

	}
	void getMaximumMemory()
	{
		fprintf(mFile,"\"%s.mm\"",mName.c_str());

	}
	void getNumCpusToUse()
	{
		fprintf(mFile,"\"%s.npu\"",mName.c_str());

	}
	void getInterruptFrequency()
	{
		fprintf(mFile,"\"%s.itf\"",mName.c_str());

	}
	void getShadowPass()
	{
		fprintf(mFile,"\"%s.shp\"",mName.c_str());

	}
	void getIprShadowPass()
	{
		fprintf(mFile,"\"%s.isp\"",mName.c_str());

	}
	void getUseFileCache()
	{
		fprintf(mFile,"\"%s.uf\"",mName.c_str());

	}
	void getOptimizeInstances()
	{
		fprintf(mFile,"\"%s.oi\"",mName.c_str());

	}
	void getReuseTessellations()
	{
		fprintf(mFile,"\"%s.rut\"",mName.c_str());

	}
	void getMatteOpacityUsesTransparency()
	{
		fprintf(mFile,"\"%s.mot\"",mName.c_str());

	}
	void getMotionBlurByFrame()
	{
		fprintf(mFile,"\"%s.mbf\"",mName.c_str());

	}
	void getFogGeometry()
	{
		fprintf(mFile,"\"%s.fg\"",mName.c_str());

	}
	void getApplyFogInPost()
	{
		fprintf(mFile,"\"%s.afp\"",mName.c_str());

	}
	void getPostFogBlur()
	{
		fprintf(mFile,"\"%s.pfb\"",mName.c_str());

	}
	void getPreMel()
	{
		fprintf(mFile,"\"%s.pram\"",mName.c_str());

	}
	void getPostMel()
	{
		fprintf(mFile,"\"%s.poam\"",mName.c_str());

	}
	void getPreRenderLayerMel()
	{
		fprintf(mFile,"\"%s.prlm\"",mName.c_str());

	}
	void getPostRenderLayerMel()
	{
		fprintf(mFile,"\"%s.polm\"",mName.c_str());

	}
	void getPreRenderMel()
	{
		fprintf(mFile,"\"%s.prm\"",mName.c_str());

	}
	void getPostRenderMel()
	{
		fprintf(mFile,"\"%s.pom\"",mName.c_str());

	}
	void getPreFurRenderMel()
	{
		fprintf(mFile,"\"%s.pfrm\"",mName.c_str());

	}
	void getPostFurRenderMel()
	{
		fprintf(mFile,"\"%s.pfom\"",mName.c_str());

	}
	void getCreateIprFile()
	{
		fprintf(mFile,"\"%s.cif\"",mName.c_str());

	}
	void getBlurLength()
	{
		fprintf(mFile,"\"%s.bll\"",mName.c_str());

	}
	void getBlurSharpness()
	{
		fprintf(mFile,"\"%s.bls\"",mName.c_str());

	}
	void getSmoothValue()
	{
		fprintf(mFile,"\"%s.smv\"",mName.c_str());

	}
	void getUseBlur2DMemoryCap()
	{
		fprintf(mFile,"\"%s.ubc\"",mName.c_str());

	}
	void getBlur2DMemoryCap()
	{
		fprintf(mFile,"\"%s.mbc\"",mName.c_str());

	}
	void getUseDisplacementBoundingBox()
	{
		fprintf(mFile,"\"%s.udbx\"",mName.c_str());

	}
	void getSmoothColor()
	{
		fprintf(mFile,"\"%s.smc\"",mName.c_str());

	}
	void getKeepMotionVector()
	{
		fprintf(mFile,"\"%s.kmv\"",mName.c_str());

	}
	void getIprRenderShading()
	{
		fprintf(mFile,"\"%s.isl\"",mName.c_str());

	}
	void getIprRenderShadowMaps()
	{
		fprintf(mFile,"\"%s.ism\"",mName.c_str());

	}
	void getIprRenderMotionBlur()
	{
		fprintf(mFile,"\"%s.imb\"",mName.c_str());

	}
	void getRendercallback()
	{
		fprintf(mFile,"\"%s.rcb\"",mName.c_str());

	}
	void getRenderLayerEnable()
	{
		fprintf(mFile,"\"%s.rlen\"",mName.c_str());

	}
	void getForceTileSize()
	{
		fprintf(mFile,"\"%s.frts\"",mName.c_str());

	}
	void getTileWidth()
	{
		fprintf(mFile,"\"%s.tlwd\"",mName.c_str());

	}
	void getTileHeight()
	{
		fprintf(mFile,"\"%s.tlht\"",mName.c_str());

	}
	void getJitterFinalColor()
	{
		fprintf(mFile,"\"%s.jfc\"",mName.c_str());

	}
	void getRaysSeeBackground()
	{
		fprintf(mFile,"\"%s.rsb\"",mName.c_str());

	}
	void getOversamplePaintEffects()
	{
		fprintf(mFile,"\"%s.ope\"",mName.c_str());

	}
	void getOversamplePfxPostFilter()
	{
		fprintf(mFile,"\"%s.oppf\"",mName.c_str());

	}
protected:
	RenderGlobals(FILE* file,const std::string& name,const std::string& parent,const std::string& nodeType):DependNode(file, name, parent, nodeType) {}

};
}//namespace MayaDM
#endif//__MayaDM_RENDERGLOBALS_H__
