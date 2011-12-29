#pragma once
/*
 *  PointSpriteParticleDrawer.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 25/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ParticleDrawer.h"
#include "ofxSimpleGuiToo.h"
#include "BlendSetting.h"

/**
 - draws points using vertex arrays (not vbos)
 - requires setup(pointCount) to be called before using, this will set a maximum amount of points that can be drawn with this drawer
 - other parameters include blend modes
 - point sizes of all particles are the same and set with the pointSize parameter
 - if no texture is set, gl points will be drawn with the set point size, otherwise
 - the texture has to be setup with ofDisableArb() !!!!
 */
class PointSpriteParticleDrawer : public ParticleDrawer {
	
public:
	
	bool				enablePointSmoothing;
	BlendSetting		blendSetting;
	float				pointSize;
	string				name;
	
	PointSpriteParticleDrawer(){
		isSetup = false;
		enablePointSmoothing = true;
		pointSize = 1;
		texture = NULL;
		name = "PointSpriteParticleDrawer";
	}
	
	~PointSpriteParticleDrawer(){
		if (!isSetup) return;
		
		// clean up dynamically created arrays
		delete[] vertexArray;
		delete[] colorArray;
	}
	
	void setup(int pointCount){
		if(!isSetup){
			maxPoints = pointCount;
			vertexArray = new float[pointCount*2];
			colorArray = new float[pointCount*4];
			isSetup = true;
		}
	}
	
	void setupGUI(){
		blendSetting.setupGUI();
		gui.addToggle("point smoothing enabled", enablePointSmoothing);
		gui.addSlider("pointSize", pointSize, 0, 100);
	}
	
	virtual void draw(ParticlePool& pool){
		draw(pool.getAlive());
	};
	
	virtual void draw(set<Particle*>& particles){
		
		if(!isSetup) return;
		
		float* colorArrayPointer = &colorArray[0];
		float* vertexArrayPointer = &vertexArray[0];
		
		set<Particle*>::iterator it;
		int count = 0;
		for(it = particles.begin(); it != particles.end() && count < maxPoints; ++it){
			Particle* particle = *it;
			
			*vertexArrayPointer++ = particle->p.x;
			*vertexArrayPointer++ = particle->p.y;
			
			*colorArrayPointer++ = particle->color.r;
			*colorArrayPointer++ = particle->color.g;
			*colorArrayPointer++ = particle->color.b;
			*colorArrayPointer++ = particle->color.a;
			
			count++;
		}
		
		drawPointArray(count);
		
	}
	
	virtual void draw(vector<Particle*>& particles){
		
		if(!isSetup) return;
		
		float* colorArrayPointer = &colorArray[0];
		float* vertexArrayPointer = &vertexArray[0];
		
		vector<Particle*>::iterator it;
		int count = 0;
		for(it = particles.begin(); it != particles.end() && count < maxPoints; ++it){
			Particle* particle = *it;
			
			*vertexArrayPointer++ = particle->p.x;
			*vertexArrayPointer++ = particle->p.y;
			
			*colorArrayPointer++ = particle->color.r;
			*colorArrayPointer++ = particle->color.g;
			*colorArrayPointer++ = particle->color.b;
			*colorArrayPointer++ = particle->color.a;
			
			count++;
		}
		
		drawPointArray(count);
	}
	
	void drawPointArray(int count){
		
		if(count > 0){
			ofPushStyle();
			
			blendSetting.begin();
			
			if(enablePointSmoothing) {
				glEnable(GL_POINT_SMOOTH);
			} else {
				glDisable(GL_POINT_SMOOTH);
			}
			
			glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
			
			glPointSize(pointSize);
			
			if(texture != NULL){
				texture->bind();
				glEnable(GL_POINT_SPRITE);
				glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
			} else {
				glDisable(GL_POINT_SPRITE);
			}
			
			// draw vertex array
			glEnableClientState( GL_VERTEX_ARRAY);	
			glEnableClientState( GL_COLOR_ARRAY );
			
			glColorPointer(4, GL_FLOAT, 0, colorArray );
			glVertexPointer(2, GL_FLOAT, 0, vertexArray );
			glDrawArrays( GL_POINTS, 0, count );
			
			glDisableClientState( GL_COLOR_ARRAY );
			glDisableClientState(GL_VERTEX_ARRAY);
			
			if(texture != NULL)
				texture->unbind();
			
			blendSetting.end();
			
			ofPopStyle();
		}
	}
	
	void setTexture(ofTexture& texture){
		this->texture = &texture;
	}
	
protected:
	
	bool isSetup;
	int maxPoints;
	float* vertexArray;
	float* colorArray;
	ofTexture* texture;
};