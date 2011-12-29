#pragma once
/*
 *  ParticleBinPool.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 03/02/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ParticlePool.h"

/**
 - same as base particle pool but provides functionality that allows to query particles based on rectangular regions or distance from point
 - the functionality is achieved through a system of bins to which on each frame particles are added based on their position  
 - inspired by an example by Kyle McDonald (TODO: find reference)
*/

class ParticleBinPool : public ParticlePool {

public:
	
	ParticleBinPool() : ParticlePool(){
		width = ofGetWidth();
		height = ofGetHeight();
		k = 2;
	}
	
	void setup(int particleCount, int width, int height, int k){
		this->width = width;
		this->height = height;
		this->k = k;
		this->particleCount = particleCount;
		setup();
	}
	
	void setup(int particleCount){
		setup(particleCount,width,height,k);
	}
	
	void setup(){
		
		ClearPointerVector(particles);
		
		binSize = 1 << k;
		xBins = (int) ceilf((float) width / (float) binSize);
		yBins = (int) ceilf((float) height / (float) binSize);
		bins.resize(xBins * yBins);
		
		particles.clear();
		alive.clear();
		dead.clear();
		
		particles.reserve(particleCount);
		alive.reserve(particleCount);
		dead.reserve(particleCount);
		
		//TODO: reserve space for bin vectors
		
		for(int i = 0; i < particleCount; ++i){
			Particle* particle = new Particle();
			particles.push_back(particle);
		}
		
	}
	
	/**
	 - separate all particles into alive and dead particle lists
	 - add particles to bins based on their position
	 */
	virtual void update(){
		
		alive.clear();
		dead.clear();
		
		// clear all bins
		int n = bins.size();
		for(int i = 0; i < n; i++) {
			bins[i].clear();
		}
		
		unsigned xBin, yBin, bin;
		vector<T*>::iterator it;
		T* particle;
		for(it = particles.begin(); it != particles.end(); ++it){
			particle = *it;	
			
			//TODO: this is probably a good place to do the particle update too
			if(particle->isAlive()){
				alive.push_back(particle);
				
				xBin = ((unsigned) particle->p.x) >> k;
				yBin = ((unsigned) particle->p.y) >> k;
				bin = yBin * xBins + xBin;
				if(xBin >= 0 && xBin < xBins && yBin >= 0 && yBin < yBins){
					bins[bin].push_back(particle);
				} else {
					//TODO: put into left over bin
				}
				
			} else {
				dead.push_back(particle);
			}
		}
		
		
	}
	
	/**
	 returns all particles
	 radius argument is the search radius
	 */
	vector<Particle*> hitTest(float x, float y, float radius){
		vector<Particle*> roughHit = getNeighbors(x,y,radius);
		
		
		vector<Particle*> hit;
		for (vector<Particle*>::iterator it = roughHit.begin(); it != roughHit.end(); ++it) {
			Particle& particle = **it;
			float dx = x - particle.p.x;
			float dy = y - particle.p.y;
			float d2 = dx * dx + dy * dy;
			float r2 = particle.radius * particle.radius;
			if (r2 > d2) { // if distance between particle and x,y smaller then radius
				hit.push_back(&particle);
			}
		}
		
		return hit;
	}
	
	vector<Particle*> getNeighbors(Particle& particle, float radius){
		return getNeighbors(particle.p.x, particle.p.y, radius);
	}

	vector<Particle*> getNeighbors(float x, float y, float radius){
		vector<Particle*> region = getRegion(
											 (int) (x - radius),
											 (int) (y - radius),
											 (int) (x + radius),
											 (int) (y + radius));
		vector<Particle*> neighbors;
		int n = region.size();
		float xd, yd, rsq, maxrsq;
		maxrsq = radius * radius;
		for(int i = 0; i < n; i++) {
			Particle& cur = *region[i];
			xd = cur.p.x - x;
			yd = cur.p.y - y;
			rsq = xd * xd + yd * yd;
			if(rsq < maxrsq)
				neighbors.push_back(region[i]);
		}
		return neighbors;
	}
	
	vector<Particle*> getRegion(unsigned minX, unsigned minY, unsigned maxX, unsigned maxY){
		vector<Particle*> region;
		back_insert_iterator< vector<Particle*> > back = back_inserter(region);
		unsigned minXBin = minX >> k;
		unsigned maxXBin = maxX >> k;
		unsigned minYBin = minY >> k;
		unsigned maxYBin = maxY >> k;
		maxXBin++;
		maxYBin++;
		if(maxXBin > xBins)
			maxXBin = xBins;
		if(maxYBin > yBins)
			maxYBin = yBins;
		for(int y = minYBin; y < maxYBin; y++) {
			for(int x = minXBin; x < maxXBin; x++) {
				vector<Particle*>& cur = bins[y * xBins + x];
				copy(cur.begin(), cur.end(), back);
			}
		}
		return region;
	}
	
protected:
	
	vector< vector<Particle*> > bins;
	int width, height, k, xBins, yBins, binSize; //< parameters to define bins
};