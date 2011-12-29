#pragma once
/*
 *  SizedPointSpriteParticleDrawer.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 26/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ParticleDrawer.h"
#include "ofxSimpleGuiToo.h"
#include "BlendSetting.h"
#include "ofxShader.h"

/**
- THIS DOES NOT WORK ON ATI HARDWARE!!!!
- TODO: non texture mode is not working at the moment
- same as PointSpriteParticleDrawer but with varying point sizes set by particle radiuses
- requires "PointSpriteParticleDrawer" shaders in bin/data/shaders
*/
class SizedPointSpriteParticleDrawer : public ParticleDrawer {
	
public:
	
	string				name;
	bool				enablePointSmoothing;
	BlendSetting		blendSetting;
	float				pointSize;
	ofColor				blendColor; //< each particles colour is multiplied by blendColor in the shader
	
	SizedPointSpriteParticleDrawer(){
		isSetup = false;
		enablePointSmoothing = true;
		pointSize = 1;
		texture = NULL;
		blendColor.r = 1;
		blendColor.g = 1;
		blendColor.b = 1;
		blendColor.a = 1;
		name = "";
	}
	
	~SizedPointSpriteParticleDrawer(){
		if (!isSetup) return;
		
		// clean up dynamically created arrays
		delete[] vertexArray;
		delete[] colorArray;
		delete[] pointSizeArray;
		shader.unload();
	}
	
	void setup(int pointCount){
		if(!isSetup){
			maxPoints = pointCount;
			vertexArray = new float[pointCount*2];
			colorArray = new float[pointCount*4];
			pointSizeArray = new float[pointCount];
			isSetup = true;
			shader.loadShader("shaders/SizedPointSpriteParticleDrawer.frag","shaders/SizedPointSpriteParticleDrawer.vert","pointSize");
		}
	}
	
	void setupGUI(){
		blendSetting.setupGUI();
		gui.addToggle(name+"point smoothing enabled", enablePointSmoothing);
		gui.addColorPicker(name+"point blend colour", &blendColor.r);
	}
	
	void draw(set<Particle*>& particles){
		
		if(!isSetup) return;
		
		float* colorArrayPointer = &colorArray[0];
		float* vertexArrayPointer = &vertexArray[0];
		float* pointSizeArrayPointer = &pointSizeArray[0];
		
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
			
			*pointSizeArrayPointer++ = particle->radius;
			
			count++;
		}
		
		drawPointArray(count);
		
	}
	
	virtual void draw(vector<Particle*>& particles){
		
		if(!isSetup) return;
		
		float* colorArrayPointer = &colorArray[0];
		float* vertexArrayPointer = &vertexArray[0];
		float* pointSizeArrayPointer = &pointSizeArray[0];
		
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
			
			*pointSizeArrayPointer++ = particle->radius;
			
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
			
			if(texture != NULL){
				texture->bind();
				glEnable(GL_POINT_SPRITE);
				glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
			} else {
				glDisable(GL_POINT_SPRITE);
			}
			
			/*
			// setup shader
			shader.setShaderActive(true);
			glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
			GLuint attribId = glGetAttribLocationARB(shader.shader, "pointSize");
			shader.setUniformVariable4fv("blend", 1, &blendColor.r);
			
			// draw vertex array
			glEnableVertexAttribArray(attribId);
			glEnableClientState( GL_VERTEX_ARRAY);	
			glEnableClientState( GL_COLOR_ARRAY );
			
			glVertexAttribPointer(0,1,GL_FLOAT,GL_FALSE,0,pointSizeArray);
			glColorPointer(4, GL_FLOAT, 0, colorArray );
			glVertexPointer(2, GL_FLOAT, 0, vertexArray );
			glDrawArrays( GL_POINTS, 0, count );
			
			glDisableClientState( GL_COLOR_ARRAY );
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableVertexAttribArray(attribId);
			
			glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
			shader.setShaderActive(false);
			*/
			
			
			
			// setup shader
			shader.setShaderActive(true);
			
			GLuint attribId = glGetAttribLocationARB(shader.shader, "pointSize");
			//shader.setUniformVariable4fv("blend", 1, &blendColor.r);
			
			// draw vertex array
			glEnableVertexAttribArray(attribId);
			//glPointSize(10);
			//glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
			glEnableClientState( GL_VERTEX_ARRAY);	
			glEnableClientState( GL_COLOR_ARRAY );
			
			glVertexAttribPointer(0,1,GL_FLOAT,GL_FALSE,0,pointSizeArray);
			glColorPointer(4, GL_FLOAT, 0, colorArray );
			glVertexPointer(2, GL_FLOAT, 0, vertexArray );
			glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
			glDrawArrays( GL_POINTS, 0, count );
			
			glDisableClientState( GL_COLOR_ARRAY );
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableVertexAttribArray(attribId);
			
			//glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
			shader.setShaderActive(false);
			
			
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
	
	bool		isSetup;
	int			maxPoints;
	float*		vertexArray;
	float*		colorArray;
	float*		pointSizeArray;
	ofTexture*	texture;
	ofxShader	shader;
};
