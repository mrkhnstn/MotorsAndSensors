/*
 *  ParticleSystem.cpp
 *  atmosphere
 *
 *  Created by Robin Beitra (robin@beitra.net) on 15/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ParticleSystem.h"

///////////////////////////////////////////////////////////////////////////////////

ParticleSystem::ParticleSystem(){
	
	drawEnabled = true;
	updateEnabled = true;
	updateOnce = false;
	drawEmittersEnabled = false;
	drawEffectorsEnabled = false;
	
}

// main /////////////////////////////////////////////////////////////////////////////////

void ParticleSystem::setup(){
	resetParticles();
}

void ParticleSystem::setupGUI(){
	
	//gui.page(1).addPageShortcut(gui.addPage(name));
	
	gui.addToggle("enabled", updateEnabled);
	gui.addButton("step", updateOnce);
	gui.addButton("reset particles", (ParticleSystem*)this, &ParticleSystem::resetParticles);
	
	gui.addToggle("draw emitters", drawEmittersEnabled);
	gui.addToggle("draw effectors", drawEffectorsEnabled);
}

void ParticleSystem::update(){
	
	if(!enabled) return;
	
	if(updateEnabled || updateOnce) { //< updateOnce is used for stepping through frames when update is disabled 
		updateOnce = false;
		
		// update all pools
		for (list<ParticlePool*>::iterator poolIt = pools.begin(); poolIt != pools.end(); ++poolIt) {
			ParticlePool* pool = *poolIt;
			pool->update(); //< separate into alive and dead particles
		}
		
		emitParticles();
		applyEffectors();
		updateParticles();
		
	}
}

void ParticleSystem::draw(){
	
	if(!enabled || !drawEnabled) return;
	
	// draw particles
	drawParticles();
	
	// draw effectors
	if (drawEffectorsEnabled)
		drawEffectors();
	
	// draw emitters
	if (drawEmittersEnabled) {
		drawEmitters();
	}
}

// reset /////////////////////////////////////////////////////////////////////////////////

/**
 clears all registered containers (drawers, emitter, effectors, resetEffectors)
 */
void ParticleSystem::reset(){
	pools.clear();
	resetEffectors.clear();
	updateEffectors.clear();
	drawers.clear();
	emitters.clear();
}

void ParticleSystem::resetParticles(ofEventArgs& e){
	resetParticles();
}

void ParticleSystem::resetParticles(){
	
	for (list<ParticlePool*>::iterator poolIt = pools.begin(); poolIt != pools.end(); ++poolIt) {
		
		ParticlePool* pool = *poolIt;
		
		for (list<ParticleEffector*>::iterator it = resetEffectors.begin(); it != resetEffectors.end(); ++it) {
			(*it)->apply(pool->getParticles());
		}
		
		pool->update();
		
	}
}

// update /////////////////////////////////////////////////////////////////////////////////

/**
 apply reset effectors to all particle of all pools
 */

void ParticleSystem::emitParticles(){
	
	for (list<ParticlePool*>::iterator poolIt = pools.begin(); poolIt != pools.end(); ++poolIt) {
		
		ParticlePool* pool = *poolIt;
		for (list<ParticleEmitter*>::iterator it = emitters.begin(); it != emitters.end(); ++it) {
			ParticleEmitter& emitter = **it;
			emitter.setPool(*pool);
			emitter.emit();
		}
		
	}
}

void ParticleSystem::applyEffectors(){
	
	for (list<ParticlePool*>::iterator poolIt = pools.begin(); poolIt != pools.end(); ++poolIt) {
		
		ParticlePool* pool = *poolIt;
		
		for (list<ParticleEffector*>::iterator it = updateEffectors.begin(); it != updateEffectors.end(); ++it) {
			ParticleEffector& effector = **it;
			if(effector.enabled){
				effector.update();
				effector.apply(*pool);
			}
		}
		
	}
}

/**
 update alive particles of all pools
 */
void ParticleSystem::updateParticles(){
	
	for (list<ParticlePool*>::iterator poolIt = pools.begin(); poolIt != pools.end(); ++poolIt) {
		
		ParticlePool* pool = *poolIt;
		vector<Particle*>& particles = pool->getAlive();
		for (vector<Particle*>::iterator it = particles.begin(); it != particles.end(); ++it) {
			(*it)->update();
		}
		
	}
}

// draw /////////////////////////////////////////////////////////////////////////////////

/**
 draw particles of all pools with all drawers
 */
void ParticleSystem::drawParticles(){
	
	for (list<ParticlePool*>::iterator poolIt = pools.begin(); poolIt != pools.end(); ++poolIt) {
		ParticlePool* pool = *poolIt;
		for (list<ParticleDrawer*>::iterator it = drawers.begin(); it != drawers.end(); ++it) {
			(*it)->draw(*pool);
		}
	}
}

/**
 draw debug helpers of all update effectors
 */
void ParticleSystem::drawEffectors(){
	for (list<ParticleEffector*>::iterator it = updateEffectors.begin(); it != updateEffectors.end(); ++it) {
		(*it)->draw();
	}
}

/**
 draw debug helpers of all emitters
 */
void ParticleSystem::drawEmitters(){
	for (list<ParticleEmitter*>::iterator it = emitters.begin(); it != emitters.end(); ++it) {
		(*it)->draw();
	}
}

// pool /////////////////////////////////////////////////////////////////////////////////

void ParticleSystem::addPool(ParticlePool& pool){
	pools.push_back(&pool);
}

void ParticleSystem::removePool(ParticlePool& pool){
	pools.remove(&pool);
}

void ParticleSystem::clearPools(){
	
}

// drawer /////////////////////////////////////////////////////////////////////////////////

void ParticleSystem::addDrawer(ParticleDrawer& drawer){
	drawers.push_back(&drawer);
}

void ParticleSystem::removeDrawer(ParticleDrawer& drawer){
	drawers.remove(&drawer);
}

void ParticleSystem::clearDrawers(){
	drawers.clear();
}

// reset effector /////////////////////////////////////////////////////////////////////////////////

void ParticleSystem::addResetEffector(ParticleEffector& effector){
	resetEffectors.push_back(&effector);
}

void ParticleSystem::removeResetEffector(ParticleEffector& effector){
	resetEffectors.remove(&effector);
}

void ParticleSystem::clearResetEffectors(){
	resetEffectors.clear();
}

// update effector /////////////////////////////////////////////////////////////////////////////////

void ParticleSystem::addEffector(ParticleEffector& effector){
	updateEffectors.push_back(&effector);
}

void ParticleSystem::removeEffector(ParticleEffector& effector){
	updateEffectors.remove(&effector);
}

void ParticleSystem::clearEffectors(){
	updateEffectors.clear();
}

// emitter /////////////////////////////////////////////////////////////////////////////////

void ParticleSystem::addEmitter(ParticleEmitter& emitter){
	emitters.push_back(&emitter);
}

void ParticleSystem::removeEmitter(ParticleEmitter& emitter){
	emitters.remove(&emitter);
}

void ParticleSystem::clearEmitters(){
	emitters.clear();
}

///////////////////////////////////////////////////////////////////////////////////