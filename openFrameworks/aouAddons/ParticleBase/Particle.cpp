/*
 *  Particle.cpp
 *  atmosphere
 *
 *  Created by Mark Hauenstein on 06/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Particle.h"

Particle::Particle(){
	setup();
}

void Particle::setup(){
	color.r = 1;
	color.g = 1;
	color.b = 1;
	color.a = 1;
	radius = 1;
	rotation = ofRandomuf() * 2 * PI;
	frame = 0; 
	nProperty = ofRandomuf();
	state = DEAD;
	life = 0;
}

void Particle::update(){
	step();
	life++;
}

void Particle::draw(){
	glColor4f(color.r, color.g, color.b, color.a);
	ofCircle(p.x, p.y, radius);
}

void Particle::setState(int state_){
	if(state != state_){
		state = state_;
		if(state == ALIVE)
			life = 0;
	}
}

int Particle::getState(){
	return state;
}

bool Particle::isDead(){
	return state == DEAD;
}

bool Particle::isAlive(){
	return state == ALIVE;
}