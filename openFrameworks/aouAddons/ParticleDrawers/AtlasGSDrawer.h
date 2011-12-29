#pragma once
/*
 *  AtlasGSDrawer.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 15/03/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ParticleDrawer.h"
#include "ofxSimpleGuiToo.h"
#include "BlendSetting.h"
#include "ofxShader.h"
#include "TextureAtlas.h"

/**
 - runs slower than quad drawer at the moment!!
 - uses geometry shaders and vbos to draw particles
 - makes use of radius, rotation and atlas
 
 */
class AtlasGSDrawer : public ParticleDrawer {

public:
	
	BlendSetting blendSetting;
	TextureAtlas* textureAtlas;
	
	AtlasGSDrawer();
	~AtlasGSDrawer();
	
	void setTextureAtlas(TextureAtlas& textureAtlas);
	
	virtual void setup(int quadCount);	
	virtual void setupGUI();	
	virtual void draw(vector<Particle*>& particles);
	
protected:
	
	static ofxShader shader;
	static bool shaderInited;
	//ofTexture* texture;
	
	bool	allocated;
	int		quadCount;
	float*	vertexBuffer;
	float*	texCoordBuffer;
	float*	colorBuffer;
	GLuint	vbo[3];
	
	void draw(int count);
	void reset();
	
	virtual void loadShaders();
	
	
};