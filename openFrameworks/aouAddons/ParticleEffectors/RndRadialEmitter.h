/*
 *  RndRadialEmitter.h
 *  TestBed_Matt
 *
 *  Created by TAQA_CH_2 on 02/03/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include "ParticleEmitter.h"

class RndRadialEmitter : public ParticleEmitter {
	
public:
	
	//int minRad;
	//int maxRad;
	//int time;
	//int duration;
	
	float radius;
	float emitRate;
	float emitCount;
	bool tweak;
	
	Particle* centre;
	ofxVec3f forceStrength;
	
	RndRadialEmitter(){
		tweak = false;
	}
	
	void reset(){
		centre = NULL;
		radius = 100;
		emitRate = 1;
		emitCount = 5;
		_emitted = 0;
	}
	
	int emit(){
		if(_pool == NULL) return 0; //< do nothing if no pool has been set
		if(centre == NULL && !tweak) return 0;
				
		_emitted += emitRate;
		
		if (_emitted < 1) return 0;
		
		vector<T*> emitParticles;
		
		vector<T*>& deadParticles = _pool->getDead();
		
		
		float r = radius; //ofMap(time, 0, duration, minRad, maxRad, true);
		
		ofxVec2f pt, qt, rt;
		float a,b,r1,r2;
		if (tweak) {
			pt.set(1440,384);
		} else {
			pt.set(centre->p.x, centre->p.y);
		}
		
		
		int divs = _emitted;
		
		for (int d = 1; d<divs; d++) {
			
			a = ofRandom(0, TWO_PI);
			r2 = ofRandom(0 , r);
			
			//			qt.set(r2*cos(a), r2*sin(a));
			//			qt.normalize();
			
			b = ofRandom(0, TWO_PI);
			qt.set(cos(b), sin(b));
			
			for(int c=0; (c<emitCount) && deadParticles.size();c++)
			{
				
				r1 = r2 + ofRandom(0, 30);
				b = a + ofRandomuf() * TWO_PI * 0.015;
				
				rt.set(r1*cos(b), r1*sin(b));
				
				Particle *p = deadParticles.back();
				deadParticles.pop_back();
				emitParticles.push_back(p);
				
				(*p).setState(ALIVE);
				(*p).jump(pt + rt);
				(*p).radius = 0;
				
				(*p).property.physics.x = (pt + rt).x;
				(*p).property.physics.y = (pt + rt).y;
				(*p).property.physics.z = 0;
				
				float var = ofRandomuf();
				
				(*p).property.physics.dir_x = forceStrength.x * qt.x * var;
				(*p).property.physics.dir_y = forceStrength.y * qt.y * var;
				(*p).property.physics.dir_z = forceStrength.z * ofRandom(0.7,1.0);
			}
		}
		
		
		for (vector<ParticleEffector*>::iterator it = effectors.begin(); it != effectors.end(); ++it) {
			(*it)->apply(emitParticles);
		}
		
		_emitted -= emitParticles.size();
		
		return emitParticles.size();
	}
	
	void setupGUI(){
		gui.addToggle("tweak", tweak);
		gui.addSlider("emit rate", emitRate, 0, 500);
		gui.addSlider("emit count", emitCount, 0, 20);
		gui.addSlider("radius", radius,0, 300);
		gui.addSlider("force strength x", forceStrength.x, 0, 10);
		gui.addSlider("force strength y", forceStrength.y, 0, 10);
		gui.addSlider("force strength z", forceStrength.z, 0, 100);
		
	}
};