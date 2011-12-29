/*
------------------------------------------------------------------------------------------------
   NuclearResetEffector.h | TestBed_Matt | 11/02/2011
 
   created by MATT JAKOB - www.mattjakob.com
   Copyright 2011. All rights reserved.
------------------------------------------------------------------------------------------------
*/

#include "ParticleEffector.h"

class NuclearResetEffector : public ParticleEffector {
	
public:
	
	NuclearResetEffector(){
		name = "Default Effector";
	}
	
	virtual void applyTo(Particle* particle){
		particle->property.physics.e = particle->property.physics.e_latency = particle->property.physics.e_level = particle->property.physics.t_bond = 0;
		particle->property.physics.release = 0;
		particle->property.physics.life = 0;
		particle->property.physics.burnoutTime = 0;
		particle->property.physics.idleTime = 0;
		particle->property.physics.mode = 0;
		particle->color.a = 1;
		particle->color.r = 1;
		particle->color.g = 1;
		particle->color.b = 1;
		particle->rotation = ofRandom(0, 360);
		
	}
	
};

