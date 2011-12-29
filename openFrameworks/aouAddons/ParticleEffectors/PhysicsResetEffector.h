#pragma once
/*
 *  DefaultResetEffector.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 23/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ParticleEffector.h"

class PhysicsResetEffector : public ParticleEffector {
	
public:
	
	PhysicsResetEffector(){
		name = "DefaultResetEffector";
	}
	
	virtual void applyTo(Particle* particle){
		/*
		 particle->setup();
		 particle->jump(ofRandomuf()*ofGetWidth(),ofRandomuf()*ofGetHeight());
		 particle->setState(ALIVE);
		 */
		particle->setState(DEAD);
		particle->property.base.maxLife = 600;
		particle->property.physics.id = 0;
		particle->radius =0; //< target radius usually
		particle->property.physics.r = 0 ; //< target colour usually
		particle->property.physics.g = 0;
		particle->property.physics.b = 0;
		particle->property.physics.a = 0;
		particle->property.physics.maxLife = 999999;
		particle->property.physics.e = 0;
		particle->property.physics.e_latency = 0;
		particle->property.physics.e_level = 0;
		particle->property.physics.level = 0;
		particle->property.physics.life = 0;
		particle->property.physics.release = 0;
		particle->property.physics.parent_x = 0;
		particle->property.physics.parent_y = 0;
		particle->property.physics.t_bond = 0;
		particle->property.physics.burnoutTime = 0;
		particle->property.physics.idleTime = 0;
		particle->property.physics.t_burnoutTime = 0;
		particle->property.physics.mode = 0;
		particle->property.physics.cr = 0;
		particle->property.physics.cg = 0;
		particle->property.physics.cb = 0;
		particle->property.physics.dir_x = 0;
		particle->property.physics.dir_y = 0;
		particle->property.physics.dir_z = 0;
		particle->property.physics.x = 0;
		particle->property.physics.y = 0;
		particle->property.physics.z = 0;
		particle->life = 0;
	}
	
};

