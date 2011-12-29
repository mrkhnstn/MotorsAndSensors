/*
 *  AtlasBillboardDrawer.cpp
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 15/03/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "AtlasBillboardDrawer.h"

ofxShader AtlasBillboardDrawer::shader;
bool AtlasBillboardDrawer::shaderInited = false;

AtlasBillboardDrawer::AtlasBillboardDrawer(){
	allocated = false;	
	quadCount = 0;
	vertexBuffer = NULL;
	texCoordBuffer = NULL;
	colorBuffer = NULL;
	textureAtlas = NULL;
	
	if (!shaderInited) {
		//shader.loadShaderGS("shaders/AtlasBillboardDrawer.fs", "shaders/AtlasBillboardDrawer.vs", "shaders/AtlasBillboardDrawer.gs");
		loadShaders(); 
		shaderInited = true;
	}
	
}

void AtlasBillboardDrawer::loadShaders(){
	shader.loadShaderGS("shaders/AtlasGSDrawer.fs", "shaders/AtlasGSDrawer.vs", "shaders/AtlasBillboardDrawer.gs");
}

void AtlasBillboardDrawer::setTextureAtlas(TextureAtlas& textureAtlas){
	this->textureAtlas = &textureAtlas;
}

AtlasBillboardDrawer::~AtlasBillboardDrawer(){
	reset();
}

void AtlasBillboardDrawer::setup(int quadCount){
	
	reset();
	
	this->quadCount	= quadCount;
	
	// generate vbos
	vertexBuffer = new float[2*quadCount];
	texCoordBuffer = new float[4*quadCount];
	colorBuffer = new float[4*quadCount];
	
	glGenBuffersARB(3, &vbo[0]);
	
	// initialize vbo for vertices
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[0]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, 2*quadCount*sizeof(float), vertexBuffer, GL_STREAM_DRAW_ARB);
	
	// initialize vbo for textureCoords
	// use frame, radius, rotation, spare
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[1]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, 4*quadCount*sizeof(float), texCoordBuffer, GL_STREAM_DRAW_ARB);
	
	// initialize vbo for colour
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[2]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, 4*quadCount*sizeof(float), colorBuffer, GL_STREAM_DRAW_ARB);
	
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	
	allocated = true;
}

void AtlasBillboardDrawer::setupGUI(){
	blendSetting.setupGUI();
	gui.addToggle("debug draw",doDebugDraw);
}

void AtlasBillboardDrawer::draw(vector<Particle*>& particles){
	
	if(!allocated || textureAtlas == NULL) return;
	
	float* colorPointer = colorBuffer;
	float* vertexPointer = vertexBuffer;
	float* texturePointer = texCoordBuffer;
	
	vector<Particle*>::iterator it;
	
	int count = 0;
	for(it = particles.begin(); it != particles.end() && count < quadCount; ++it){
		Particle* particle = *it;
		
		*vertexPointer++ = particle->p.x;
		*vertexPointer++ = particle->p.y;
		
		float* tex = textureAtlas->getTextureCoord(particle->frame);
		*texturePointer++ = *tex++; // tex u
		*texturePointer++ = *tex; // tex v
		*texturePointer++ = particle->radius; // radius
		*texturePointer++ = particle->rotation; // rotation in degrees
		
		*colorPointer++ = particle->color.r;
		*colorPointer++ = particle->color.g;
		*colorPointer++ = particle->color.b;
		*colorPointer++ = particle->color.a;
		
		count++;
	}
	
	draw(count);
	
}

void AtlasBillboardDrawer::draw(int count){
	
	blendSetting.beginNoShader();
	
	textureAtlas->getTexture().bind();
	shader.setShaderActive(true);
	shader.setUniformVariable2f("cellDim", textureAtlas->colSpan, textureAtlas->rowSpan);
	shader.setUniformVariable1i("preMultiply", blendSetting.preMultiplySetting);
	int count_ = CLAMP(count,0,quadCount); // safe guard to not go beyond allocated arrays
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY); 
	glEnableClientState(GL_COLOR_ARRAY);
	
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[0]);
	glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, 2*count_ * sizeof(float), vertexBuffer);
	glVertexPointer(2, GL_FLOAT, 0, 0);
	
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[1]);
	glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, 4*count_ * sizeof(float), texCoordBuffer);
	glTexCoordPointer(4, GL_FLOAT, 0, 0);
	
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[2]);
	glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, 4*count_ * sizeof(float), colorBuffer);
	glColorPointer(4, GL_FLOAT, 0, 0);
	
	glDrawArrays(GL_POINTS, 0, count_);
	
	glDisableClientState(GL_VERTEX_ARRAY); 
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	
	shader.setShaderActive(false);
	textureAtlas->getTexture().unbind();
	
	blendSetting.endNoShader();
}

void AtlasBillboardDrawer::reset(){
	if(allocated){
		allocated = false;
		glDeleteBuffersARB(3, &vbo[0]);
		delete[] vertexBuffer;
		delete[] texCoordBuffer;
		delete[] colorBuffer;
	}
}