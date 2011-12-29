#pragma once
/*
 *  DefaultParticleSystem.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 23/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ParticleSystem.h"

#include "DefaultResetEffector.h"
#include "ForceEffector.h"
#include "ConstForceField2f.h"
#include "WrapEffector.h"
#include "UniformPropertiesEffector.h"
#include "MappedPropertiesEffector.h"
#include "RectEmitter.h"
#include "RectLiveAreaEffector.h"

class DefaultParticleSystem : public ParticleSystem {

	public:
	
	ParticleDrawer defaultParticleDrawer;
	DefaultResetEffector defaultResetEffector;
	
	ForceEffector forceEffector;
	ConstForceField2f constForceField;
	
	WrapEffector wrapEffector;
	RectLiveAreaEffector rectLiveAreaEffector;
	
	//UniformPropertiesEffector uniformPropertiesEffector; //< sets mass, damping, radius and color of all particles in real time using the gui
	MappedPropertiesEffector mappedPropertiesEffector;
	
	ParticlePool defaultPool;
	
	RectEmitter	rectEmitter;
	
	virtual void setup(){
		
		name = "DefaultParticleSystem";
	
		// set pool
		defaultPool.setup(1024 * 10);
		pools.push_back(&defaultPool);
		
		// add default particle drawer
		drawers.push_back(&defaultParticleDrawer);
		
		// add reset effector
		resetEffectors.push_back(&defaultResetEffector);
		
		// add update effectors
		
		//addEffector(uniformPropertiesEffector);
		updateEffectors.push_back(&mappedPropertiesEffector);
		
		forceEffector.field = &constForceField;
		constForceField.force.x = -1;
		updateEffectors.push_back(&forceEffector);
		
		updateEffectors.push_back(&rectLiveAreaEffector);
		
		// add emitter
		emitters.push_back(&rectEmitter);
		//rectEmitter.addEffector(uniformPropertiesEffector); //< sets up the particle with uniform properties before it is emitted
		rectEmitter.addEffector(mappedPropertiesEffector);
		
		ParticleSystem::setup();

	}
	
	void setupGUI(){
		
		ofxSimpleGuiPage& page = gui.addPage(name);
		
		gui.page(1).addPageShortcut(page);
		gui.addToggle("enabled", updateEnabled);
		gui.addButton("step", updateOnce);
		gui.addButton("reset particles", (ParticleSystem*)this, &ParticleSystem::resetParticles);
		
		gui.addToggle("draw emitters", drawEmittersEnabled);
		gui.addToggle("draw effectors", drawEffectorsEnabled);
		
		gui.addTitle("particle properties");
		/*
		uniformPropertiesEffector.name = "p_"; //< to ensure unique gui control names
		uniformPropertiesEffector.setupGUI();
		*/
		mappedPropertiesEffector.name = "p_";
		mappedPropertiesEffector.setupGUI();
		
		page.addPageShortcut(gui.addPage("live area"));
		rectLiveAreaEffector.setupGUI();
		
		page.addPageShortcut(gui.addPage("rectangular emitter"));
		rectEmitter.setupGUI();
		
		//page.loadFromXML();
		
	}
	
	void postGUI(){
		//uniformPropertiesEffector.enabled = true;
		mappedPropertiesEffector.enabled = true;
	}
	
};