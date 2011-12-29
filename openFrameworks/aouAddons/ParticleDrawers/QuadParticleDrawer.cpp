/*
 *  QuadParticleDrawer.cpp
 *  TestBed_Mark2
 *
 *  Created by TAQA_CH_0 on 14/02/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "QuadParticleDrawer.h"

bool QuadParticleDrawer::rotatedVertexInitialized = false;
float QuadParticleDrawer::rotatedVertex[ROTATION_COUNT][8];

QuadParticleDrawer::QuadParticleDrawer(){
	allocated = false;	
	texture = NULL;
	quadCount = 0;
	vertexBuffer = NULL;
	texCoordBuffer = NULL;
	colorBuffer = NULL;
	
	if(!rotatedVertexInitialized){
		ofxVec2f a, b, c, d;
		a.set(-1,-1);
		b.set(1,-1);
		c.set(1,1);
		d.set(-1,1);

		
		
		ofxVec2f a2,b2,c2;
		for (int i=0; i<ROTATION_COUNT;i++) {
			float rotateAngle = ofMap(i, 0, ROTATION_COUNT-1, 0, 360, true);
			ofxVec2f a2 = a.getRotated(rotateAngle);
			ofxVec2f b2 = b.getRotated(rotateAngle);
			ofxVec2f c2 = c.getRotated(rotateAngle);
			ofxVec2f d2 = c.getRotated(rotateAngle);
			
			rotatedVertex[i][0] = a2.x;
			rotatedVertex[i][1] = a2.y;
			rotatedVertex[i][2] = b2.x;
			rotatedVertex[i][3] = b2.y;
			rotatedVertex[i][4] = c2.x;
			rotatedVertex[i][5] = c2.y;
			rotatedVertex[i][6] = d2.x;
			rotatedVertex[i][7] = d2.y;
		}
		
		rotatedVertexInitialized = true;
	}
}

QuadParticleDrawer::~QuadParticleDrawer(){
	reset();
}

void QuadParticleDrawer::setup(int quadCount){
	
	reset();
	
	this->quadCount	= quadCount;
	
	// generate vbos
	vertexBuffer = new float[4*2*quadCount];
	texCoordBuffer = new float[4*2*quadCount];
	colorBuffer = new float[4*4*quadCount];
	
	glGenBuffersARB(3, &vbo[0]);
	
	// initialize vbo for vertices
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[0]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, 4*2*quadCount*sizeof(float), vertexBuffer, GL_STREAM_DRAW_ARB);
	
	// initialize vbo for textureCoords
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[1]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, 4*2*quadCount*sizeof(float), texCoordBuffer, GL_STREAM_DRAW_ARB);
	
	// initialize vbo for colour
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[2]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, 4*4*quadCount*sizeof(float), colorBuffer, GL_STREAM_DRAW_ARB);
	
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	
	allocated = true;
}

void QuadParticleDrawer::setupGUI(){
	blendSetting.setupGUI();
	gui.addToggle("debug draw",doDebugDraw);
}

void QuadParticleDrawer::draw(vector<Particle*>& particles){
	
	if(!allocated) return;
	
	float* colorPointer = colorBuffer;
	float* vertexPointer = vertexBuffer;
	float* texturePointer = texCoordBuffer;
	
	// texture sides	h
	float tLeft = 0;
	float tRight = 0;
	float tTop = 0;
	float tBottom = 0;
	
	if (texture != NULL) {
		tRight = texture->getWidth();
		tBottom = texture->getHeight();
	}
	
	vector<Particle*>::iterator it;
	int count = 0;
	for(it = particles.begin(); it != particles.end() && count < quadCount; ++it){
		Particle* particle = *it;
		
		// vertex sides
		float vLeft = particle->p.x - particle->radius;
		float vRight = particle->p.x + particle->radius;
		float vTop = particle->p.y - particle->radius;
		float vBottom = particle->p.y + particle->radius;
		
		//top left
		*vertexPointer++ = vLeft;
		*vertexPointer++ = vTop;
		*texturePointer++ = tLeft;
		*texturePointer++ = tTop;
		
		//top right
		*vertexPointer++ = vRight;
		*vertexPointer++ = vTop;
		*texturePointer++ = tRight;
		*texturePointer++ = tTop;
		
		//bottom right
		*vertexPointer++ = vRight;
		*vertexPointer++ = vBottom;
		*texturePointer++ = tRight;
		*texturePointer++ = tBottom;
		
		//bottom left
		*vertexPointer++ = vLeft;
		*vertexPointer++ = vBottom;
		*texturePointer++ = tLeft;
		*texturePointer++ = tBottom;
		
		
		for(int i=0; i<4; i++){
			*colorPointer++ = particle->color.r;
			*colorPointer++ = particle->color.g;
			*colorPointer++ = particle->color.b;
			*colorPointer++ = particle->color.a;
		}
		
		count++;
	}
	
	draw(count);
	
}

void QuadParticleDrawer::setTexture(ofTexture& texture){
	this->texture = &texture;
}

void QuadParticleDrawer::draw(int count){
	
	blendSetting.begin();
	
	if(texture != NULL){
		texture->bind();
	}
	
	int count_ = CLAMP(count,0,quadCount); // safe guard to not go beyond allocated arrays
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY); 
	glEnableClientState(GL_COLOR_ARRAY);
	
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[0]);
	glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, 4*2*count_ * sizeof(float), vertexBuffer);
	glVertexPointer(2, GL_FLOAT, 0, 0);
	
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[1]);
	glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, 4*2*count_ * sizeof(float), texCoordBuffer);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);
	
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[2]);
	glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, 4*4*count_ * sizeof(float), colorBuffer);
	glColorPointer(4, GL_FLOAT, 0, 0);
	
	glDrawArrays(GL_QUADS, 0, 4*count_);
	
	glDisableClientState(GL_VERTEX_ARRAY); 
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	
	if(texture != NULL)
		texture->unbind();
	
	blendSetting.end();
}

void QuadParticleDrawer::reset(){
	if(allocated){
		allocated = false;
		glDeleteBuffersARB(3, &vbo[0]);
		delete[] vertexBuffer;
		delete[] texCoordBuffer;
		delete[] colorBuffer;
	}
}