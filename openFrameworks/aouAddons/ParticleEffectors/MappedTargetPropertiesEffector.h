#pragma once

/*
 *  MappedTargetPropertiesEffector.h
 *  TestBed_Mark2
 *
 *  Created by TAQA_CH_0 on 18/02/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "MappedPropertiesEffector.h"

/**
 - same as MappedPropertiesEffector but sets color and radius of particle.property.base
 - mainly to use the base property color and radius as targets for tweens
 */
class MappedTargetPropertiesEffector : public MappedPropertiesEffector {

	virtual void applyTo(Particle* particle){
		particle->setMass(mass.getMapped(particle->nProperty, 0, 1));
		particle->setDamping(damping.getMapped(particle->nProperty, 0, 1));
		particle->property.base.r = ofMap(particle->nProperty, 0, 1, color1.r, color2.r);
		particle->property.base.g = ofMap(particle->nProperty, 0, 1, color1.g, color2.g);
		particle->property.base.b = ofMap(particle->nProperty, 0, 1, color1.b, color2.b);
		particle->property.base.a = ofMap(particle->nProperty, 0, 1, color1.a, color2.a);
		particle->property.base.radius = radius.getMapped(particle->nProperty, 0, 1);
	}
	
};