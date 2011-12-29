#pragma once

/*
 *  AtlasDisplayListDrawer.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 14/03/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

// THIS ONE DOES NOT WORK !!!!!!!!

#include "ParticleDrawer.h"
#include "TextureAtlas.h"
#include "ofxSimpleGuiToo.h"
#include "BlendSetting.h"

class AtlasDisplayListDrawer : public ParticleDrawer {

public:
	
	TextureAtlas*	textureAtlas;
	BlendSetting	blendSetting;
	int testFrame;
	
	AtlasDisplayListDrawer() : ParticleDrawer(){
		textureAtlas = NULL;
		testFrame = 0;
	}
	
	void setupGUI(){
		blendSetting.setupGUI();
		gui.addToggle("debug draw",doDebugDraw);
		gui.addSlider("testFrame", testFrame, 0, 27);
	}
	
	void draw(vector<Particle*>& particles){
		if (textureAtlas == NULL) return;
		
		// create display lists if not done before
		if (!textureAtlas->displayListsCreated) { 
			textureAtlas->createDisplayLists();
			
			// if failed to create display lists
			if (!textureAtlas->displayListsCreated) { 
				return;
			}
		}
		
		blendSetting.begin();
		textureAtlas->getTexture().bind();
		
		GLuint t = 0;
		
		vector<Particle*>::iterator it;
		for(it = particles.begin(); it != particles.end(); ++it){
			Particle* particle = *it;
			glPushMatrix();
			ofTranslate(particle->p.x, particle->p.y);
			glRotatef(particle->rotation, 0, 0, 1);
			glScalef(particle->radius, particle->radius, 1.0);
			glColor4f(particle->color.r, particle->color.g, particle->color.b, particle->color.a);
			textureAtlas->drawDisplayList(20);
			t++;
			if (t >23) {
				t = 0;
			}
			
			//if (particle->frame >= 28) {
				ofLog(OF_LOG_VERBOSE, "frame: " + ofToString(particle->frame));
			//}
			glPopMatrix();
		}
		textureAtlas->getTexture().unbind();
		
		blendSetting.end();
	}
	
	void setTextureAtlas(TextureAtlas& textureAtlas){
		this->textureAtlas = &textureAtlas;
	}
};