#pragma once
/*
 *  ParticleEmitter.h
 *  atmosphere
 *
 *  Created by Robin Beitra (robin@beitra.net) on 14/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

//TODO: separate and refactor other emitters

#include "ofMain.h"
#include "MSAPerlin.h"
#include "ParticlePool.h"
#include "Particle.h"
#include "ofxVec2f.h" 
#include "Boundary2f.h"
#include "ImageUtil.h"
#include "ParticleEffector.h"
#include "ofxBaseObject.h"

#include "Utils.h"

//template<class T>
typedef Particle T;
class ParticleEmitter : public ofxBaseObject {
public:
	
	float _emitRate;
	float _emitted;
	
	ParticleEmitter(){
		_pool = NULL;
		_emitRate = 1;
		_emitted = 0;
	}
	
	void setRate(float rate){
		_emitRate = rate;
	}
	
	/**
	 returns targeted number of particles to be emitted per frame 
	 */
	virtual float getRate(){
		return _emitRate;
	};
	
	void setPool(ParticlePool& pool){
		_pool = &pool;
	}
	
	void unsetPool(){
		_pool = NULL;
	}
	
	void addEffector(ParticleEffector& effector){
		effectors.push_back(&effector);
	}
		
	/**
	 emit particles with the set _emitRate;
	 */
	virtual int emit(){
		if(_pool == NULL) return 0; //< do nothing if no pool has been set
		
		_emitted += _emitRate;
		
		if (_emitted < 1) return 0;
		
		vector<T*> emitParticles;
		
		vector<T*>& deadParticles = _pool->getDead();
		for (int i = 1; i < _emitted && deadParticles.size() > 0; i++) {
			emitParticles.push_back(deadParticles.back());
			deadParticles.pop_back();
		}
		
		for (vector<ParticleEffector*>::iterator it = effectors.begin(); it != effectors.end(); ++it) {
			(*it)->apply(emitParticles);
		}
		
		_emitted -= emitParticles.size();
			
		//int emitterNo = emitParticles.size();
		//ofLog(OF_LOG_VERBOSE, "emitted: " + ofToString(emitterNo));
		
		return emitParticles.size();
	}
	
	
	/*
	//emit a particular number of particles
	virtual int emitFixed(float count){
		
		if(_pool == NULL) return 0; //< do nothing if no pool has been set
		
		vector<T*> emitParticles;
		
		vector<T*>& deadParticles = _pool->getDead();
		for (int i = 0; i < count && deadParticles.size() > 0; i++) {
			emitParticles.push_back(deadParticles.back());
			deadParticles.pop_back();
		}
		
		for (vector<ParticleEffector*>::iterator it = effectors.begin(); it != effectors.end(); ++it) {
			(*it)->apply(emitParticles);
		}
		
		return emitParticles.size();
	}
	
	//emit a number proportional to the desiredemitcount
	virtual int emitRatio(float ratio){
		if(_pool == NULL) return 0;
		float targetCount = ratio*_emitRate;
		return emitFixed(targetCount);
	}
	*/
	 
	virtual void update(){
	
	}

	virtual void prepareParticle(T* particle){
	
	}
	
	void reset(){
		_emitted = 0;
	}
	
protected:
	

	
	ParticlePool* _pool;
	vector<ParticleEffector*> effectors; //< this effector is applied when a particle is emitted

	ofxVec2f initialForce;
};



class MovingCircleEmitter: public ParticleEmitter{
public:
	ofxVec2f position;
	ofxVec2f shape;
	ofxVec2f theta;
	ofxVec2f thetaSpeed;
	void update(){
		theta += thetaSpeed;
	}
	void prepareParticle(T* particle){
		ofxVec2f p = position;		
		p.x += sin(theta.x)*shape.x;
		p.y += cos(theta.y)*shape.y;
		particle->jump(p);
		ParticleEmitter::prepareParticle(particle);
	}
};

//template<class T>
class PerlinBucketEmitter: public ParticleEmitter{//<T>{
public:
	struct PerlinParameters {
		int octaves;
		float freq;
		float amp;
		int seed;
		float y;
		MSA::Perlin perlin;
		
		PerlinParameters(){
			octaves = 4;
			freq = 2;
			amp = 0.5;
			seed = 1;
			y = 0;
			updatePerlin();
		}
		
		float get(float x){
			return perlin.get(x,y);
		}
		
		/*
		 float get(float x, float y, float z){
		 return perlin.get(x,y,z);
		 }
		 */
		
		void updatePerlin(){
			perlin.setup(octaves, freq, amp, seed);
		}
		
		//void setupGUI(string name_ = ""){
//			gui.addSlider(name_+" octaves", octaves, 1, 10);
//			gui.addSlider(name_+" frequency", freq, 0, 0.1);
//			gui.addSlider(name_+" amplitude", amp, 0, 1);
//			gui.addSlider(name_+" seed", seed, 1, 256);
//			gui.addSlider(name_+" z", z, 0, 1024);
//		}
		
	} perlinX;
	
	void setup(int bucketsH, int bucketsV){
		this->bucketsH = bucketsH;
		this->bucketsV = bucketsV;
		
		maxR = 0;
		minR = 0;
		peakyness = 4;
		bucketTightness = 0.5;
		bucketWidthRandomness = 0.5;
		bucketOffsetRandomness = 0.5;
		
		int max = bucketsH*bucketsV;
		for(int i = 0; i < max; ++i){
			bucketRates.push_back(0);
			bucketWidths.push_back(ofRandomuf());
			bucketOffsets.push_back(ofRandomuf() - 0.5);
		}	
	}
	
	void update(){
		perlinX.y += ySpeed;
		totalRate = 0;
		
		int m = bucketRates.size();
		for(int i = 0; i < m; ++i){
			float r = perlinX.get(i);;//ofRandomuf();
			r = r/perlinX.amp;
			r = r + 0.5;
			r = ofClamp(r, 0, 1);
			//r = r*r;//*r;//make it a bit more exaggerated!
			r = powf(r, peakyness);
			//r = r*0.5 + 0.5;
			//r += perlinX.amp*0.5;
			maxR = max(maxR, r);
			minR = min(minR, r);
			totalRate += r;
			bucketRates[i] = r;
		}
	}
	
	void prepareParticle(T* particle){
		int max = bucketRates.size();
		if(max > 0){
			
			float r = ofRandomuf()*totalRate;
			int index = 0;
			while(index < max){
				r -= bucketRates[index];
				if(r < 0){
					break;
				} else {
					++index;
				}
			}
			
			if(index >= max){
				index = max-1;
			}
			
			//we now have a bucket index!
			
			int bucketX = index%bucketsH;
			int bucketY = index/bucketsH;
			
			float bucketWidth = rect.getWidth()/bucketsH;
			float bucketHeight = rect.getHeight()/bucketsV;
			
			ofxVec2f p = rect.min;// = position;
			
			float ox = bucketWidth*(bucketOffsetRandomness*bucketOffsets[index]);
			float s = bucketWidths[index];
			s = bucketWidthRandomness*s + (1 - bucketWidthRandomness)*(1 - s);
			p.x += bucketWidth*(bucketX + 0.5 + ox + (ofRandomuf()-0.5)*bucketTightness*s);
			p.y += bucketHeight*(bucketY + 0.5 + (ofRandomuf()-0.5)*bucketTightness);
			particle->jump(p);
		}
	}
	//float getDesiredEmitCount(){
//		return _emitRate*totalRate;///((float)bucketRates.size());
//	};
//	
	Boundary2f rect;
	int bucketsH;
	int bucketsV;
	vector<float> bucketRates;
	vector<float> bucketWidths;
	vector<float> bucketOffsets;
	float totalRate;
	
	float maxR, minR;
	
	float ySpeed;
	float peakyness;
	float bucketTightness;
	float bucketWidthRandomness;
	float bucketOffsetRandomness;
};

class ImageEmitter: public ParticleEmitter{
public:
	float ox, oy, sx, sy;
	vector<ofxVec2f> points;
	ImageEmitter(){
		ox = oy = 0;
		sx = sy = 2;
	}
	
	void setup(ofImage& image, int count, int min, int max){
		ofPoint point;
		unsigned char color[4];
		for(int i = 0; i < count; ++i){
			ofPoint point = ImageUtil::getMatchingPoint(image, 
														min, max,
														min, max,
														min, max,
														0, 255,
														32);
			
			ImageUtil::getPixelFromAnyImage(image, point.x, point.y, color);
			if(color[0] >= min && color[0] <= max){
				ofxVec2f p = point;
				points.push_back(p);
			}
		}
		//_T("found points: " + _S((int)points.size()));
	}
	void setup(string path, int count, int min, int max){
		ofImage image;
		image.loadImage(path);
		setup(image, count, min, max);
	}
	void prepareParticle(T* particle){
		int max = points.size();
		if(max > 0){
			int i = ofRandomuf()*max;
			i = i%max;
			ofxVec2f p = points[i];
			particle->jump(p.x*sx + ox, p.y*sy + oy);
			ParticleEmitter::prepareParticle(particle);
		}
	}
};

class LineEmitter: public ParticleEmitter{
public:
	ofxVec2f a, b;
	void prepareParticle(T* particle){
		ofxVec2f p;
		float phase = ofRandomuf();
		particle->jump(a.getInterpolated(b, phase));
		ParticleEmitter::prepareParticle(particle);
	}
};
class CircleEmitter: public ParticleEmitter{
public:
	float radius;
	float radiusRange;
	float radiusRangePow;
	
	ofxVec2f center;
	float minTheta, maxTheta;
	CircleEmitter(){
		radiusRange = 0;
		radiusRangePow = 1;
	}
	void prepareParticle(T* particle){
		float theta = ofRandom(minTheta, maxTheta);
		float r = radius;
		if(radiusRange != 0){
			float p = ofRandomuf();
			p = powf(p, radiusRangePow);
			if(ofRandomuf() < 0.5){
				p = -p;
			}
			r += p*radiusRange;
		}
		particle->jump(center.x + r*cos(theta), center.y + r*sin(theta));
		particle->force(initialForce.getRotatedRad(theta));
	}
};

class GroupEmitter: public ParticleEmitter{
public:
	void update(){
		totalRate = 0;
		
		int m = bucketRates.size();
		for(int i = 0; i < m; ++i){
			float r = bucketRates[i];
			r = powf(r, peakyness);
			totalRate += r;
			peakyRates[i] = r;
		}
	}
	
	
	void prepareParticle(T* particle){
		int max = peakyRates.size();
		if(max > 0){
			float r = ofRandomuf()*totalRate;
			int index = 0;
			while(index < max){
				r -= peakyRates[index];
				if(r < 0){
					break;
				} else {
					++index;
				}
			}
			
			if(index >= max){
				index = max-1;
			}
			
			//we now have a bucket index!
			ParticleEmitter::prepareParticle(particle);//prepare it ourself
			ParticleEmitter* emitter = emitters[index];
			emitter->prepareParticle(particle);
		}
	}
	
	int addEmitter(ParticleEmitter* emitter){
		int index = emitters.size();
		emitters.push_back(emitter);
		bucketRates.push_back(0);
		peakyRates.push_back(0);
		return index;
	}
	
	void setSubRate(int index, float rate){
		if(index >= 0 && index < bucketRates.size()){
			bucketRates[index] = rate;
		}
	}
	float getSubRate(int index){
		if(index >= 0 && index < bucketRates.size()){
			return bucketRates[index];
		} else {
			return 0;
		}
	}
	
	vector<ParticleEmitter* > emitters;
	vector<float> bucketRates;
	vector<float> peakyRates;
	
	float totalRate;
	float peakyness;
	
};