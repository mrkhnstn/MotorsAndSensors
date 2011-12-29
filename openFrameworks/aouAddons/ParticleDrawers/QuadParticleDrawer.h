#pragma once
/*
 *  QuadParticleDrawer.h
 *  TestBed_Mark2
 *
 *  Created by TAQA_CH_0 on 14/02/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ParticleDrawer.h"
#include "ofxSimpleGuiToo.h"
#include "BlendSetting.h"

#define ROTATION_COUNT 1000

/**
uses gl quads to draw particles with colors and texture coordinates
 */
class QuadParticleDrawer : public ParticleDrawer {

public:
	
	BlendSetting		blendSetting;
	//ofColor				blendColor; //< each particles colour is multiplied by blendColor in the shader
	//TODO: implement blendColor
	
	QuadParticleDrawer();
	~QuadParticleDrawer();	
	void setup(int quadCount);	
	void setupGUI();	
	virtual void draw(vector<Particle*>& particles);
	void setTexture(ofTexture& texture);
	
protected:
	
	static bool rotatedVertexInitialized;
	static float rotatedVertex[ROTATION_COUNT][8];
	
	ofTexture* texture;
	bool	allocated;
	int		quadCount;
	float*	vertexBuffer;
	float*	texCoordBuffer;
	float* colorBuffer;
	GLuint	vbo[3];
	
	void draw(int count);
	void reset();
};