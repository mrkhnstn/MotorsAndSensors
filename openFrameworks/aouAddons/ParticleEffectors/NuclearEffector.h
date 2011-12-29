/*
------------------------------------------------------------------------------------------------
   NuclearEffector.h | TestBed_Matt | 10/02/2011
 
   created by MATT JAKOB - www.mattjakob.com
   Copyright 2011. All rights reserved.
------------------------------------------------------------------------------------------------
*/

#pragma once
#include "ParticleGravityEffector.h"
#include "ParticleBinPool.h"
#include "ParticleEffector.h"
#include "GlobalConfig.h"
#include "Singleton.h"

#define NUCLEAR_USR 0
#define NUCLEAR_EGG 1
#define NUCLEAR_VVW 2
#define NUCLEAR_EXT 3

class ParticleNuclearEffector : public ParticleEffector {
	
public:
	
	ofColor rampLookup[512];
	
	int accumulateSpeed[4];
	int releaseSpeed[4];
	int levelCapacity;
	int	  reactions;
	int	  levelsTreshold;
	int	  electronsTreshold;
	int   bonds;
	int   searchRadius;
	int	  reactionSpeed[4];
	int   burnoutTime[4];
	int   idleTime[4];
	int	  randomChargeCount;
	bool  triggerFinal;
	
	ParticlePool* users;		//User
	ParticlePool* eggs;			//Egg
	//ParticlePool* views;		//View
	ParticleBinPool binPool;	//Ambient
	// singleton access
	Kacare::GlobalConfig*	globalConfig;
	Particle* triggerParticle;
	
	ParticleNuclearEffector(){
		users = NULL;
		globalConfig = Singleton<Kacare::GlobalConfig>::instance();
		triggerFinal = false;
		triggerParticle = NULL;
		
		ofImage ramp;
		if(ramp.loadImage("images/nuclear/ramp2.png"))
		{
			unsigned char *pixels;
			pixels = ramp.getPixels();
			
			for (int i=0; i < 512; i++) {
				rampLookup[i].r = pixels[i*3];
				rampLookup[i].g = pixels[i*3 + 1];
				rampLookup[i].b = pixels[i*3 + 2];
			}
		}
		
	}
	
	
	void setupGUI(){
		gui.addTitle("PHYSICS PROPERTIES");
		gui.addTitle("Charging");
		gui.addSlider("Random accumulation", randomChargeCount, 0, 500);
		gui.addSlider("1 Accumulate speed", accumulateSpeed[NUCLEAR_USR], 0, 1000);
		gui.addSlider("1 Release speed", releaseSpeed[NUCLEAR_USR], 0, 10);
		gui.addTitle("Explosion trigger");
		gui.addSlider("Single level capacity", levelCapacity,0,200);
		gui.addDebug("Treshold levels", levelsTreshold);
		gui.addDebug("Treshold electrons", electronsTreshold);
		gui.addTitle("Reaction");
		gui.addSlider("Spread size", reactions,0,20);
		gui.addSlider("Bonds", bonds,1,10);
		gui.addSlider("Max bond length", searchRadius,1,200);
		gui.addSlider("1 Reaction speed", reactionSpeed[NUCLEAR_USR], 0, 20);
		gui.addSlider("1 Burnout time",burnoutTime[NUCLEAR_USR],0,600);
		gui.addSlider("1 Idle time",idleTime[NUCLEAR_USR],0,600);
		gui.addTitle("EGG SETTINGS");
		gui.addSlider("2 Accumulate speed", accumulateSpeed[NUCLEAR_EGG], 0, 1000);
		gui.addSlider("2 Release speed", releaseSpeed[NUCLEAR_EGG], 0, 10);
		gui.addSlider("2 Reaction speed", reactionSpeed[NUCLEAR_EGG], 0, 20);
		gui.addSlider("2 Burnout time",burnoutTime[NUCLEAR_EGG],0,600);
		gui.addSlider("2 Idel time",idleTime[NUCLEAR_EGG],0,600);
		gui.addTitle("VIEW SETTINGS");
		gui.addSlider("3 Accumulate speed", accumulateSpeed[NUCLEAR_VVW], 0, 100000);
		gui.addSlider("3 Release speed", releaseSpeed[NUCLEAR_VVW], 0, 10);
		gui.addSlider("3 Reaction speed", reactionSpeed[NUCLEAR_VVW], 0, 20);
		gui.addSlider("3 Burnout time",burnoutTime[NUCLEAR_VVW],0,600);
		gui.addSlider("3 Idle time",idleTime[NUCLEAR_VVW],0,600);
		gui.addTitle("EXIT SETTINGS");
		gui.addSlider("4 Accumulate speed", accumulateSpeed[NUCLEAR_EXT], 0, 10000000);
		gui.addSlider("4 Release speed", releaseSpeed[NUCLEAR_EXT], 0, 10);
		gui.addSlider("4 Reaction speed", reactionSpeed[NUCLEAR_EXT], 0, 20);
		gui.addSlider("4 Burnout time",burnoutTime[NUCLEAR_EXT],0,600);
		gui.addSlider("4 Idle time",idleTime[NUCLEAR_EXT],0,600);
		gui.addTitle("FINAL ANIMATION");
		gui.addToggle("Start", triggerFinal);
		
		
		
	}
		
	
	void apply(ParticlePool& _pool){
		
		levelsTreshold = pow((float)bonds, (float)reactions);
		electronsTreshold = levelsTreshold * levelCapacity;
		
		if (typeid(_pool) != typeid(ParticleBinPool)) return;
		
		binPool = (ParticleBinPool&) _pool;
		
		
		//USER particles accumulation
		vector<Particle*> userParticles = users->getAlive();
		for(vector<Particle*>::iterator it = userParticles.begin(); it != userParticles.end(); ++it){	
			Particle& p = **it;
			vector<Particle*> neighborParticles = getClosest(&p);	
				if(neighborParticles.size() > 0)
					accumulate(neighborParticles[0], NUCLEAR_USR);			
		}
		
		//EGG particles accumulation
		vector<Particle*> eggParticles = eggs->getAlive();
		for(vector<Particle*>::iterator it = eggParticles.begin(); it != eggParticles.end(); ++it){	
			Particle& p = **it;
			if(!p.property.egg.isOpen){
			//p.radius = 30;
				/*
				vector<Particle*> neighborParticles = getClosest(&p);	
				if(neighborParticles.size() > 0)
				{
					//int id = ofRandom(0,neighborParticles.size());
					accumulate(neighborParticles[0], NUCLEAR_EGG);
				}
				 */
			} else if (p.property.egg.isOpen == 2) {
				vector<Particle*> neighborParticles = getClosest(&p);	
				if(neighborParticles.size() > 0)
					accumulate(neighborParticles[0], NUCLEAR_VVW);	
			}

		}
		
		//VIEW particles accumulation
		/*
		vector<Particle*> viewParticles = views->getAlive();
		for(vector<Particle*>::iterator it = viewParticles.begin(); it != viewParticles.end(); ++it){	
			Particle& p = **it;
			p.radius = 30;
			if(p.life == 1)	//triggered only on opening
			{
				vector<Particle*> neighborParticles = getClosest(&p);	
				if(neighborParticles.size() > 0)
					accumulate(neighborParticles[0], NUCLEAR_VVW);	
			}
		}
		*/
		
		//RANDOM PARTICLES ACCUMULATION
		for (int i=0; i<ofRandomuf() * randomChargeCount; i++) {
			
            Particle p;
            p.p.x = ofRandomuf() * globalConfig->boundary.width;
            p.p.y = ofRandomuf() * globalConfig->boundary.height;
            p.radius = 20;
            vector<Particle*> neighborParticles = getClosest(&p);
            if(neighborParticles.size() > 0)
                accumulate(neighborParticles[0], NUCLEAR_USR);
        }
		
		//FINAL ANIM
		if(triggerFinal) // force animation in centre via toggled variable
		{
			triggerFinal = false;
			Particle p;
			p.p.x = globalConfig->boundary.width /2;
			p.p.y = globalConfig->boundary.height /2;
			p.radius = 30;
			
			vector<Particle*> neighborParticles = getClosest(&p);	
				
			if(neighborParticles.size() > 0)
					accumulate(neighborParticles[0], NUCLEAR_EXT);	
		}
		
		
		if(triggerParticle != NULL){
			
			vector<Particle*> tempParticles = binPool.getAlive();
			for(vector<Particle*>::iterator it = tempParticles.begin(); it != tempParticles.end(); ++it){	
				Particle& p = **it;
				p.property.physics.idleTime = 0;
			}
			
			
			Particle p;
			p.p.x = triggerParticle->p.x;
			p.p.y = triggerParticle->p.y;
			p.radius = 30;
			
			vector<Particle*> neighborParticles = getClosest(&p);	
			
			if(neighborParticles.size() > 0)
				accumulate(neighborParticles[0], NUCLEAR_EXT);	
			triggerParticle = NULL;
		}
		
		
		//Updates the ambient particles with the reaction model
		apply(binPool.getAlive());	
	}
	
	void apply(set<Particle*>& particles){
		if (users == NULL) return; //< prevent usage if no pool is set
		ParticleEffector::apply(particles);
	}
	
	void apply(vector<Particle*>& particles){
		if (users == NULL) return; //< prevent usage if no pool is set
		ParticleEffector::apply(particles);
	}
	
	void applyTo(Particle* particle){
		
		
		
		int mode = particle->property.physics.mode;
		
		if(particle->property.physics.e < levelCapacity)														//If charge is less than 1 level then particle is not doing a reaction
		{
			particle->property.physics.release  = 0;
			particle->property.physics.parent_x = 0;
			particle->property.physics.parent_y = 0;
		}
		
		if(particle->property.physics.burnoutTime > 0)															//Updates the burnout timer
			particle->property.physics.burnoutTime--;
		
		if(particle->property.physics.idleTime > 0)															//Updates the burnout timer
			particle->property.physics.idleTime--;
		
		if((particle->property.physics.e >= electronsTreshold) && (particle->property.physics.release == 0))	//If particle is charged above the threshold but is not reacting set it to react
			particle->property.physics.release = 1;
		
		if(particle->property.physics.release >= reactionSpeed[mode])											//Triggers the explosion
		{
			
			vector<Particle*> neighborParticles = getClosest(particle);
			
			int max_bonds = (bonds < neighborParticles.size()) ? bonds : neighborParticles.size();
			
			int energy_to_transmit = (particle->property.physics.e)/max_bonds - releaseSpeed[mode]*max_bonds;
			
			//if (energy_to_transmit < levelCapacity)
			//	energy_to_transmit = levelCapacity;
			
			for(int i = 0; i < max_bonds; i++ )
			{
				neighborParticles[i]->property.physics.e        += energy_to_transmit;
				
				//if(neighborParticles[i]->property.physics.e > levelsTreshold*levelCapacity)
				//	neighborParticles[i]->property.physics.e = levelsTreshold*levelCapacity;
				
				neighborParticles[i]->property.physics.parent_x = particle->p.x;
				neighborParticles[i]->property.physics.parent_y = particle->p.y;
				neighborParticles[i]->property.physics.mode	= particle->property.physics.mode;
				neighborParticles[i]->property.physics.release  = 1;											//Set the receiving particle to react
				
			}
			
			//if(max_bonds > 0)
			{
				particle->property.physics.e = 0;
				particle->property.physics.burnoutTime = burnoutTime[mode];
				particle->property.physics.idleTime = burnoutTime[mode] + idleTime[mode];
			}
			
		}
		
		if(particle->property.physics.release > 0)																//Updates the reaction timer
			particle->property.physics.release++;

		release(particle);																						//Energy dissipation
		updateParticlePhysics(particle);																		//Updates relative values
	}
	
	void accumulate(Particle* particle, int mode)
	{
		if(particle->property.physics.e < electronsTreshold)
		{
			particle->property.physics.mode = mode;
			particle->property.physics.e += accumulateSpeed[mode];
		}
		//else	
		//	particle->property.physics.e = levelsTreshold*levelCapacity;
		
		if (particle->property.physics.e >= electronsTreshold)			//Trigger explosion
			particle->property.physics.release = reactionSpeed[mode] + 1;
	}
	
	void release(Particle* particle)
	{
		int mode = particle->property.physics.mode;
		
		if(particle->property.physics.e > 0)
			particle->property.physics.e -= releaseSpeed[mode];
		else
		{
			particle->property.physics.e = 0;
			particle->property.physics.parent_x = particle->property.physics.parent_y = 0;
		}
			
	}
	
	void updateParticlePhysics(Particle* particle)
	{
		
		if(!particle->property.physics.e)
		{
			/*
			float H = (particle->life % 5000)/5000.0f;
			ofColor c = getColor(H, 0.5, 0.5);
			particle->property.physics.r = c.r;
			particle->property.physics.g = c.g;
			particle->property.physics.b = c.b;
			
			H = ((particle->life + 5000/11) % 5000)/5000.0f;
			c = getColor(H, 0.5, 0.5);
			particle->property.physics.cr = c.r;
			particle->property.physics.cg = c.g;
			particle->property.physics.cb = c.b;
			*/
			int h = ((particle->life % 4000) / 4000.0f) * 512;
			particle->property.physics.r = rampLookup[h].r;
			particle->property.physics.g = rampLookup[h].g;
			particle->property.physics.b = rampLookup[h].b;
			
			
			h = (((particle->life + 4000/9) % 4000) / 4000.0f) * 512;
			particle->property.physics.cr = rampLookup[h].r;
			particle->property.physics.cg = rampLookup[h].g;
			particle->property.physics.cb = rampLookup[h].b;
			
			 
			 
		}
		
		int mode = particle->property.physics.mode;
		
		particle->property.physics.e_level = ofMap(particle->property.physics.e, 0, levelCapacity, 0, 1.0);
		if(particle->property.physics.e_level > 1.0)
			particle->property.physics.e_level = 1.0;
		
		particle->property.physics.e_latency = ofMap(particle->property.physics.e, 0, levelsTreshold*levelCapacity, 0, 1.0);
		if(particle->property.physics.e_latency > 1.0)
			particle->property.physics.e_latency = 1.0;
		
		particle->property.physics.t_bond = ofMap(particle->property.physics.release, 0, reactionSpeed[mode], 0, 1.0);
		if(particle->property.physics.t_bond > 1.0)
			particle->property.physics.t_bond = 1.0;
		
		particle->property.physics.level = particle->property.physics.e / levelCapacity;
		particle->property.physics.life++;
		
		particle->property.physics.t_burnoutTime = (float)particle->property.physics.burnoutTime/burnoutTime[mode];
	}
	
	vector<Particle*> getClosest(Particle* particle)
	{
		float tmp1 = 0;
		float tmp2 = 0;
		
		Particle* closest;

		vector<Particle*> neighborParticles = binPool.getNeighbors(*particle, searchRadius);
		
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
					if((p.p.squareDistance(particle->p) < q.p.squareDistance(particle->p)) && (p.p.squareDistance(particle->p) != 0) && (p.property.physics.idleTime == 0))
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
	
	ofColor getColor(double h, double sl, double l)
	{
		double v;
		double r,g,b;
		
		r = l;   // default to gray
		g = l;
		b = l;
		v = (l <= 0.5) ? (l * (1.0 + sl)) : (l + sl - l * sl);
		if (v > 0)
		{
			double m;
			double sv;
			int sextant;
			double fract, vsf, mid1, mid2;
			
			m = l + l - v;
			sv = (v - m ) / v;
			h *= 6.0;
			sextant = (int)h;
			fract = h - sextant;
			vsf = v * sv * fract;
			mid1 = m + vsf;
			mid2 = v - vsf;
			switch (sextant)
			{
				case 0:
					r = v;
					g = mid1;
					b = m;
					break;
				case 1:
					r = mid2;
					g = v;
					b = m;
					break;
				case 2:
					r = m;
					g = v;
					b = mid1;
					break;
				case 3:
					r = m;
					g = mid2;
					b = v;
					break;
				case 4:
					r = mid1;
					g = m;
					b = v;
					break;
				case 5:
					r = v;
					g = m;
					b = mid2;
					break;
			}
		}
		
		ofColor rgb;
		rgb.r = (r * 255.0f);
		rgb.g = (g * 255.0f);
		rgb.b = (b * 255.0f);
		rgb.a = 255;
		return rgb;
	}
	
};
