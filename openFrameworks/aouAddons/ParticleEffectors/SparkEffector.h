/*
 *  SparkEffector.h
 *  TestBed_Matt
 *
 *  Created by TAQA_CH_2 on 22/02/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include "ParticleEffector.h"
#include "ParticleBinPool.h"
#include "GlobalConfig.h"
#include "Singleton.h"

class SparkEffector : public ParticleEffector{
	
public:
	Kacare::GlobalConfig*	globalConfig;
	
	int targetCount;
	
	ParticlePool* users;		//User
	ParticlePool* exit;			//Egg
	ParticlePool* pool;			//Sparks
	ParticleBinPool* cold;
	ParticleBinPool* ambient;
	
	ofxVec3f forceStrength;
	float perspRatio;
	float sparkRepulsionF;
	
	float rndEmitRate;
	
	float userEmitRate;
	float eggEmitRate;
	float viewEmitRate;
	
	float userEmitLen;
	float eggEmitLen;
	float viewEmitLen;
	
	int   userEmitAmount;
	int	  eggEmitAmount;
	int   viewEmitAmount;
	
	float rndEmitCount;
	float userEmitCount;
	float eggEmitCount;
	float viewEmitCount;
	
	int userEmitRad;
	int eggEmitRad;
	int viewEmitRad;
	
	float userEmitRadV[2];
	float eggEmitRadV[2];
	float viewEmitRadV[2];	
	
	float userLife;
	float userLifeV;
	float eggLife;
	float eggLifeV;
	float viewLife;
	float viewLifeV;
	float emitUserAmbient;
	
	Particle* triggerParticle;
	
	SparkEffector(){
		userEmitCount = eggEmitCount = viewEmitCount = 0;
		globalConfig = Singleton<Kacare::GlobalConfig>::instance();
	}
	
	void setupGUI ()
	{
		gui.addTitle("EMITTER SETTINGS");
		gui.addSlider("Target count", targetCount,0,10240);
		gui.addSlider("Force strength x", forceStrength.x, 0, 10);
		gui.addSlider("Force strength y", forceStrength.y, 0, 10);
		gui.addSlider("Force strength z", forceStrength.z, 0, 100);
		gui.addSlider("Perspective ratio", perspRatio, 0, 100);
		gui.addSlider("Repulsion at ground", sparkRepulsionF, 0, 1.0);
		
		gui.addTitle("SCATTER SETTINGS");
		gui.addSlider("Rnd emitter rate", rndEmitRate, 0, 10);
		
		gui.addTitle("USER SETTINGS");
		gui.addSlider("Emit on lava", emitUserAmbient,0,30);
		gui.addSlider("User emitter rate",	 userEmitRate,0,3);
		gui.addSlider("User emitter length", userEmitLen, 0, 1.0);
		gui.addSlider("User emitter amount", userEmitAmount,0, 30);
		
		gui.addSlider("User rad",			userEmitRad,0,100);		
		gui.addSlider("User rad var in",	userEmitRadV[0],0,1);
		gui.addSlider("User rad var out",	userEmitRadV[1],0,1);
		
		gui.addSlider("User life",			userLife, 0, 300);
		gui.addSlider("User life var",		userLifeV, 0, 1);
		
		gui.addTitle("EGG SETTINGS");
		gui.addSlider("Egg emitter rate",	eggEmitRate,0,3);
		gui.addSlider("Egg emitter length", eggEmitLen, 0, 1.0);
		gui.addSlider("Egg emitter amount", eggEmitAmount,0, 30);
		
		gui.addSlider("Egg rad",			eggEmitRad,0,100);
		gui.addSlider("Egg rad var in",     eggEmitRadV[0],0,1);
		gui.addSlider("Egg rad var out",    eggEmitRadV[1],0,1);
		
		gui.addSlider("Egg life",			eggLife, 0, 300);
		gui.addSlider("Egg life var",		eggLifeV, 0, 1);
		
		gui.addTitle("VIEW SETTINGS");
		gui.addSlider("View emitter rate",	 viewEmitRate,0,3);
		gui.addSlider("View emitter length", viewEmitLen, 0, 1.0);
		gui.addSlider("View emitter amount", viewEmitAmount,0, 30);
		
		gui.addSlider("View rad",			viewEmitRad,0,100);
		gui.addSlider("View rad var in",    viewEmitRadV[0],0,1);
		gui.addSlider("View rad var out",   viewEmitRadV[1],0,1);
		
		gui.addSlider("View life",			viewLife, 0, 300);
		gui.addSlider("View life var",		viewLifeV, 0, 1);
	}
	
	void integrate(Particle* particle)
	{
		ofxVec3f f,p,g;
		g.set(0,0,-1.2);
		f.set(particle->property.physics.dir_x,particle->property.physics.dir_y,particle->property.physics.dir_z);
		p.set(particle->property.physics.x, particle->property.physics.y, particle->property.physics.z);
		
		f = f+g;
		f.x *= 0.95;
		f.y *= 0.95;
		p = p+f;
		if(p.z < 0)
		{
			f.set(0,0,0);
			p.z = 0;
		}
		particle->property.physics.dir_x = f.x;
		particle->property.physics.dir_y = f.y;
		particle->property.physics.dir_z = f.z;
		particle->property.physics.x = p.x;
		particle->property.physics.y = p.y;
		particle->property.physics.z = p.z;
		
		particle->jump(p.x,p.y);
		particle->radius = 4 + p.z/perspRatio;
		
	}
	
	vector<Particle*> getClosest(Particle* particle, float rad = 30)
	{
		float tmp1 = 0;
		float tmp2 = 0;
		
		Particle* closest;
		
		vector<Particle*> neighborParticles = cold->getNeighbors(*particle, rad);
		
		//SORTING
		Particle tmp;
		int min;
		for(long i=0;i<neighborParticles.size();i++)
		{
			min = i;
			for(long x=i; x<neighborParticles.size(); x++)
			{
				Particle& p = *neighborParticles[x];
				Particle& q = *neighborParticles[min];
				if((p.p.squareDistance(particle->p) < q.p.squareDistance(particle->p)))
				{
					min = x;
				}
			}
			tmp = *neighborParticles[i];
			neighborParticles[i] = neighborParticles[min];
			neighborParticles[min] = &tmp;
		}
		///////
		return neighborParticles;
	}
	
	vector<Particle*> getClosest(int x, int y, float rad = 30)
	{
		Particle p;
		p.p.set(x,y);
		p.radius = 20;
		return getClosest(&p, rad);
	}
	
	void apply(ParticlePool& _pool){
		
		
		vector<T*> emitParticles;						//Particles available for emission
		vector<T*>& deadParticles = _pool.getDead();	//Dead pool
		
		int aliveCount = _pool.getAlive().size();		//Currently alive count
		
		for (int i = 0; (deadParticles.size() > 0) && (aliveCount < targetCount); i++) {
			emitParticles.push_back(deadParticles.back());
			deadParticles.pop_back();
			aliveCount++;
		}
		
		vector<Particle*>::iterator it;
		vector<T*> particles;
		
		int i, available, count;
		float x,y;
		float rad, a, b;
		
		i = 0;														//Position in the emitParticles array
		available = emitParticles.size();							//How many we can emit
		
		userEmitCount += userEmitRate;
		eggEmitCount  += eggEmitRate;
		viewEmitCount += viewEmitRate;
		rndEmitCount += rndEmitRate;
		
		//USER
		particles = (vector<Particle*>) users->getAlive();
		for(it = particles.begin(); it != particles.end(); ++it)	//Go through each user particle
		{
			Particle* p1 = *it;

			for(int d=1; d<=userEmitCount; d++)						//Emitter rate
			{
				
				a = ofRandom(0, TWO_PI);							//Section angle
																	//Section radius
				rad = userEmitRad + ofRandomuf() * userEmitRad * userEmitRadV[1] - ofRandomuf() * userEmitRad * userEmitRadV[0];
				
				for(int ap=0; ap<userEmitAmount; ap++)				//Number of particles for each section
				{
					if(available > 0)
					{
						b = a + ofRandomf() * userEmitLen * TWO_PI * 0.5f;	//Angle variation
						
						x = p1->p.x + 1.6 * rad * cos(b);
						y = p1->p.y + 1.6 * rad * sin(b);
						
						vector<Particle*> neighborParticles = getClosest(x,y);	
						
						if(neighborParticles.size() > 0)			//Check if there are cold particles around the user
						{
							x = p1->p.x + rad * cos(b);
							y = p1->p.y + rad * sin(b);
							
							
							
							Particle* p2 = emitParticles[i];
							p2->setState(ALIVE);
							p2->life = 0;
							available --;
							i++;
							
							p2->jump(x, y);
							p2->property.physics.x = x;
							p2->property.physics.y = y;
							p2->property.physics.z = 0;
							float var = ofRandom(0.5,1.0);
							p2->property.physics.dir_x = forceStrength.x * cos(b) * var;
							p2->property.physics.dir_y = forceStrength.y * sin(b) * var;
							p2->property.physics.dir_z = forceStrength.z * ofRandom(0.2,1.0);
							p2->property.physics.life  = userLife + ofRandomuf() * userLife * userLifeV;
							
							p2->radius = 3;
				
						}
						
					}
					else break;
				}
			}
		}
		
		// RANDOM
		for(int d=1; d<=ofRandom(0, rndEmitCount); d++)
			{
				
				a = ofRandom(0, TWO_PI);
				rad = userEmitRad + ofRandomuf() * userEmitRad * userEmitRadV[1] - ofRandomuf() * userEmitRad * userEmitRadV[0];
				float x1 = ofRandom(0,globalConfig->boundary.width);
				float y1 = ofRandom(0,globalConfig->boundary.height);
				
				for(int ap=0; ap<userEmitAmount; ap++)
				{
					if(available > 0)
					{
						b = a + ofRandomf() * userEmitLen * TWO_PI * 0.5f;
						
						x = x1 + 1.6 * rad * cos(b);
						y = y1 + 1.6 * rad * sin(b);
						
							x = x1 + rad * cos(b);
							y = y1 + rad * sin(b);
							
							Particle* p2 = emitParticles[i];
							p2->setState(ALIVE);
							p2->life = 0;
							available --;
							i++;
							
							p2->jump(x, y);
							p2->property.physics.x = x;
							p2->property.physics.y = y;
							p2->property.physics.z = 0;
							float var = ofRandom(0.5,1.0);
							p2->property.physics.dir_x = forceStrength.x * cos(b) * var;
							p2->property.physics.dir_y = forceStrength.y * sin(b) * var;
							p2->property.physics.dir_z = forceStrength.z * ofRandom(0.2,1.0);
							
							p2->radius = 3;
							
						
					}
					else break;
				}
			}

		if(userEmitCount >= 1)
			userEmitCount = 0;
		if(rndEmitCount >= 1)
			rndEmitCount = 0;
		if(eggEmitCount >= 1)
			eggEmitCount = 0;
		if(viewEmitCount >= 1)
			viewEmitCount = 0;
		
		pool = &_pool;
		apply(pool->getAlive());
	}
	
	virtual void apply(set<Particle*>& particles){
	}
	
	virtual void apply(vector<Particle*>& particles){
		float t_life;
		float x,y, deleteRate;
		// particle to particle iteration
		vector<Particle*>::iterator it;
		vector<Particle*>::iterator it2;
		Particle* a;
		ofxVec2f d,c;
		
		for(it = particles.begin(); it != particles.end(); ++it){
			Particle* p1 = *it;
			
			integrate(p1);
			
			if ((p1->life > 0 )&& (p1->property.physics.z == 0))
			{
				p1->setState(DEAD);
				p1->life = 0;
			}
			
			if(p1->property.physics.z == 0)
			{
				vector<Particle*> neighborParticles = getClosest(p1,40);
				
				for(int i=0; i<neighborParticles.size(); i++)
				{
					if(p1->state == DEAD){ 
						(deleteRate = 0.4);
					} else { 
						(deleteRate = 0.2);
					}
					if (ofRandomuf() <deleteRate) 
					{
						//c = (neighborParticles[i]->p - p1->p);
						//d = 50 * sparkRepulsionF * c.getNormalized();
						//neighborParticles[i]->force(d);
						//neighborParticles[i]->radius *= 0.2;
						neighborParticles[i]->property.base.maxLife = neighborParticles[i]->life;
						//neighborParticles[i]->color.a = 1.0;
						//neighborParticles[i]->setState(DEAD);
					}
				}

			}
			 
			
		}			
	}
	
	// disable
	void applyTo(Particle* particle){}
};