#pragma once
/*
 *  ParticleSystem.h
 *  atmosphere
 *
 *  Created by Robin Beitra (robin@beitra.net) on 15/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxBaseObject.h"
#include "Particle.h"
#include "ParticlePool.h"
#include "ParticleEmitter.h"
#include "ParticleEffector.h"
#include "ParticleDrawer.h"

typedef Particle T;
class ParticleSystem : public ofxBaseObject {
public:
	
	bool drawEnabled;
	bool updateEnabled;
	bool updateOnce;
	
	bool drawEmittersEnabled;
	bool drawEffectorsEnabled;
	
	list<ParticleDrawer*>		drawers;			//< drawers used to draw particles
	list<ParticleEffector*>		resetEffectors;		//< effectors applied on calling resetParticles()
	list<ParticleEffector*>		updateEffectors;	//< effectors applied in update()
	list<ParticleEmitter*>		emitters;			// < emitters
	list<ParticlePool*>			pools;				// < pools of particles
	
	///////////////////////////////////////////////////////////////////////////////////
	
	ParticleSystem();
	
	// main /////////////////////////////////////////////////////////////////////////////////
	
	virtual void setup();
	
	virtual void setupGUI();
	
	virtual void update();
	
	virtual void draw();
	
	// reset /////////////////////////////////////////////////////////////////////////////////
	
	/**
	 clears all registered containers (drawers, emitter, effectors, resetEffectors)
	 */
	virtual void reset();
	
	void resetParticles(ofEventArgs& e);
	
	virtual void resetParticles();
	
	// update /////////////////////////////////////////////////////////////////////////////////
	
	/**
	 apply reset effectors to all particle of all pools
	 */
	
	virtual void emitParticles();
	
	virtual void applyEffectors();
	
	/**
	 update alive particles of all pools
	 */
	virtual void updateParticles();
	
	// draw /////////////////////////////////////////////////////////////////////////////////
	
	/**
	 draw particles of all pools with all drawers
	 */
	virtual void drawParticles();
	
	/**
	 draw debug helpers of all update effectors
	 */
	virtual void drawEffectors();
	
	/**
	 draw debug helpers of all emitters
	 */
	virtual void drawEmitters();
	
	// pool /////////////////////////////////////////////////////////////////////////////////
	
	void addPool(ParticlePool& pool);
	
	void removePool(ParticlePool& pool);
	
	void clearPools();
	
	// drawer /////////////////////////////////////////////////////////////////////////////////
	
	void addDrawer(ParticleDrawer& drawer);
	
	void removeDrawer(ParticleDrawer& drawer);
	
	void clearDrawers();
	
	// reset effector /////////////////////////////////////////////////////////////////////////////////
	
	void addResetEffector(ParticleEffector& effector);
	
	void removeResetEffector(ParticleEffector& effector);
	
	void clearResetEffectors();
	
	// update effector /////////////////////////////////////////////////////////////////////////////////
	
	void addEffector(ParticleEffector& effector);
	
	void removeEffector(ParticleEffector& effector);
	
	void clearEffectors();
	
	// emitter /////////////////////////////////////////////////////////////////////////////////
	
	void addEmitter(ParticleEmitter& emitter);
	
	void removeEmitter(ParticleEmitter& emitter);
	
	void clearEmitters();
	
	///////////////////////////////////////////////////////////////////////////////////
	
	
};