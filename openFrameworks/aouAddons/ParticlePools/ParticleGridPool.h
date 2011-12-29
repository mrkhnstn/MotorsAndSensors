#pragma once
/*
 *  ParticleGridPool.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 27/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ParticlePool.h"

/**
 - allows access to particles using column and rows indices
 - originally created for ParticleMeshSystem
 */
class ParticleGridPool  : public ParticlePool {

public:
	
	ParticleGridPool(){
		cols = 1;
		rows = 1;
		particleCount = 1;
	}
	
	void setup(int cols, int rows){
		this->cols = cols;
		this->rows = rows;
		particleCount = cols * rows;
		ParticlePool::setup();
	}
	
	/*
	 - returns particle at grid position defined by col and row
	 - no bounds checking!!!
	 */
	Particle* getParticle(int col, int row){
		return particles[getIndex(col, row)];
	}
	
	/*
	 - returns one dimensional index of grid position defined by col and row
	 - no bounds checking!!!
	 */
	int getIndex(int col, int row){
		return row * cols + col;
	}
	
	/**
	 no dead or alive sorting as in base ParticlePool
	 */
	void update(){
	}
	
	/**
	 alive particles equals all particles
	 */
	int getAliveCount(){
		return particles.size();
	}
		
	/**
	 alive particles equals all particles
	 */
	vector<T*>& getAlive(){
		return particles;
	}
	
	int getCols(){
		return cols;
	}
	
	int getRows(){
		return rows;
	}
	
protected:
	
	int cols;
	int rows;
	
};

