/*
 ------------------------------------------------------------------------------------------------
 NuclearParticleDrawer.h | TestBed_Matt | 11/02/2011
 
 created by MATT JAKOB - www.mattjakob.com
 Copyright 2011. All rights reserved.
 ------------------------------------------------------------------------------------------------
 */

#pragma once

#include "GlobalConfig.h"
#include "AtlasGSDrawer.h"
#include "AtlasQuadParticleDrawer.h"
#include "ofxSimpleGuiToo.h"
//#include "BlendSetting.h"
//#include "ofxVec2f.h"
#include "ofNoise.h"
#include "Boundary1f.h"

/**
 - draws points using vertex arrays (not vbos)
 - requires setup(pointCount) to be called before using, this will set a maximum amount of points that can be drawn with this drawer
 - other parameters include blend modes
 - point sizes of all particles are the same and set with the pointSize parameter
 - if no texture is set, gl points will be drawn with the set point size, otherwise
 - the texture has to be setup with ofDisableArb() !!!!
 */


#ifdef ATI
class NuclearParticleDrawer : public AtlasQuadParticleDrawer {
#else
class NuclearParticleDrawer : public AtlasGSDrawer {
#endif
	
public:
		
	ofTexture			textures[10];
	ofImage				imgLoader;
	ofImage				atomImage;
	
	//GLOW
	float    glow_size;
	float    glow_sizeVar;
	float    glow_sizeFrq;
	float    glow_alphaVar;
	float    glow_alphaFrq;
	ofColor  glow_color;
	bool	 glow_usePColor;
	
	//GAS
	float    gas_size;
	float    gas_sizeVar;
	float	 gas_sizeFrq;
	float    gas_alphaVar;
	float    gas_alphaFrq;
	ofColor  gas_color;
	bool	 gas_usePColor;
	
	//EXPLOSION
	float    exp_size;
	float    exp_sizeVar;
	float    exp_sizeFrq;
	float    exp_alphaVar;
	float    exp_alphaFrq;
	ofColor  exp_color;
	ofColor  crk_color;
	bool	 exp_usePColor;
	
	//burnoutTime
	float    brn_size;
	float    brn_sizeVar;
	float	 brn_sizeFrq;
	float    brn_alphaVar;
	float	 brn_alphaFrq;
	ofColor  brn_color;
	bool	 brn_usePColor;
		
	float    gasSize;
	float	 gasFreq;
	float	 gasAmp;
	float	 gasAlpha;
	float	 gasRotate;

	ofColor  baseColor;
	GLuint	 quad_DL;
	
	int		drawCount;
	float* colorPointer;
	float* vertexPointer;
	float* texturePointer;
	TextureAtlas myTextureAtlas;
	
	// atomic core
	Boundary1f atomRadiusRange;
	Boundary1f atomAlphaRange;
	float radiusPow;
	
	
	void resetPointers();
	
	void render(ofxVec2f pos, float size, ofColor col, float alpha, float rotation, int frame = 0);
	
	NuclearParticleDrawer();
	
	~NuclearParticleDrawer();
	
	
	/*
	ofxVec2f pointOnLine(int x1, int y1, int x2, int y2, float t)
	{
		ofxVec2f p;
		p.x = x1 + (x2 - x1) * (1-t);
		p.y = y1 + (y2 - y1) * (1-t);
		return p;
	}
	*/
	
	void setup(int pointCount);
	
	void setupGUI();
	
	void draw(vector<Particle*>& particles);
	
	
protected:
	
	bool isSetup;
	int maxPoints;

};


