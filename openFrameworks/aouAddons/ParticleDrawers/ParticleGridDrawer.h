#pragma once
/*
 *  ParticleGridDrawer.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 27/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ParticleDrawer.h"

/**
 draws a particle grid pool as a mesh with mapped texture uvs
 
 !!!! this pool needs to be used with a ParticleGridPool to work
*/

#include "ParticleGridPool.h"
//#include "ParticleMeshSystemProperty.h"
#include "BlendSetting.h"
#include "Singleton.h"

class ParticleMeshSystemProperty;

class ParticleGridDrawer : public ParticleDrawer {

public:
	
	//BlendSetting		blendSetting;
	//ofColor				blendColor;
	
	ParticleMeshSystemProperty* meshProperty;
	float				alpha;
	bool				flip; // rotates image by 180 degrees
	
	ParticleGridDrawer();
	
	void setupGUI();
	
	void draw(ParticlePool& pool);
	
	/*
	 disabled
	 **/
	void draw(set<Particle*>& particles);
	
	/**
	 disabled
	 */
	void draw(vector<Particle*>& particles);
	
	void setTexture(ofTexture& texture);
	
protected:
	
	ofTexture* texture;

};
