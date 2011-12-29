/*
 *  AtlasQuadParticleDrawer.cpp
 *  TestBed_Mark2
 *
 *  Created by TAQA_CH_0 on 15/02/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "AtlasQuadParticleDrawer.h"

AtlasQuadParticleDrawer::AtlasQuadParticleDrawer() : QuadParticleDrawer(){
	textureAtlas = NULL;
}

void AtlasQuadParticleDrawer::setTextureAtlas(TextureAtlas& textureAtlas){
	this->textureAtlas = &textureAtlas;
}

void AtlasQuadParticleDrawer::draw(vector<Particle*>& particles){
	
	if(!allocated || textureAtlas == NULL) return;
	
	setTexture(textureAtlas->getTexture());
	
	float* colorPointer = colorBuffer;
	float* vertexPointer = vertexBuffer;
	float* texturePointer = texCoordBuffer;
	
	// texture sides
	float tLeft = 0;
	float tRight = 32;
	float tTop = 0;
	float tBottom = 32;
	
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
		
		//top right
		*vertexPointer++ = vRight;
		*vertexPointer++ = vTop;
		
		//bottom right
		*vertexPointer++ = vRight;
		*vertexPointer++ = vBottom;
		
		//bottom left
		*vertexPointer++ = vLeft;
		*vertexPointer++ = vBottom;
		
		//ofLog(OF_LOG_VERBOSE, "=>"+ofToString(particle->frame));
		
		memcpy(texturePointer, textureAtlas->getTextureCoord(particle->frame), 8 * sizeof(float));
		//ofLog(OF_LOG_VERBOSE, "=>"+ofToString(particle->frame)+":"+ofToString(*(texturePointer+0))+","+ofToString(*(texturePointer+1))+":"+ofToString(*(texturePointer+2))+","+ofToString(*(texturePointer+3))+ofToString(*(texturePointer+4))+","+ofToString(*(texturePointer+5))+":"+ofToString(*(texturePointer+6))+","+ofToString(*(texturePointer+7)));
		
		
		texturePointer += 8;
				
		for(int i=0; i<4; i++){
			memcpy(colorPointer, &particle->color.r, 4 * sizeof(float));
			colorPointer += 4;
		}
		
		count++;
	}
	
	QuadParticleDrawer::draw(count);
	

	
}