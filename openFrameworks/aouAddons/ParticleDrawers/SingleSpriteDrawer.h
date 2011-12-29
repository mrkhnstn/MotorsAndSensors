/*
 ------------------------------------------------------------------------------------------------
 NuclearParticleDrawer.h | TestBed_Matt | 11/02/2011
 
 created by MATT JAKOB - www.mattjakob.com
 Copyright 2011. All rights reserved.
 ------------------------------------------------------------------------------------------------
 */

#pragma once

#include "ParticleDrawer.h"
#include "ofxSimpleGuiToo.h"
#include "BlendSetting.h"
#include "ofxVec2f.h"
#include "ofNoise.h"

/**
 - draws points using vertex arrays (not vbos)
 - requires setup(pointCount) to be called before using, this will set a maximum amount of points that can be drawn with this drawer
 - other parameters include blend modes
 - point sizes of all particles are the same and set with the pointSize parameter
 - if no texture is set, gl points will be drawn with the set point size, otherwise
 - the texture has to be setup with ofDisableArb() !!!!
 */



class SingleSpriteDrawer : public ParticleDrawer {
	
public:
	
	bool				enablePointSmoothing;
	BlendSetting		blendSetting;
	float				pointSize;
	string				name;
	
	ofTexture			textures[10];
	ofImage				imgLoader;
	
	//GLOW
	float    glow_size;
	float    glow_sizeVar;
	float    glow_sizeFrq;
	float    glow_alphaVar;
	float    glow_alphaFrq;
	ofColor  glow_color;
	bool	 glow_usePColor;
	
	//GAS
	float    gas_size;
	float    gas_sizeVar;
	float	 gas_sizeFrq;
	float    gas_alphaVar;
	float    gas_alphaFrq;
	ofColor  gas_color;
	bool	 gas_usePColor;
	
	//EXPLOSION
	float    exp_size;
	float    exp_sizeVar;
	float    exp_sizeFrq;
	float    exp_alphaVar;
	float    exp_alphaFrq;
	ofColor  exp_color;
	ofColor  crk_color;
	bool	 exp_usePColor;
	
	//burnoutTime
	float    brn_size;
	float    brn_sizeVar;
	float	 brn_sizeFrq;
	float    brn_alphaVar;
	float	 brn_alphaFrq;
	ofColor  brn_color;
	bool	 brn_usePColor;
	
	
	float    gasSize;
	float	 gasFreq;
	float	 gasAmp;
	float	 gasAlpha;
	float	 gasRotate;
	
	
	ofColor  baseColor;
	GLuint	 quad_DL;
	
	
	void render(ofxVec2f pos, float size, ofColor col, float alpha, float rotation)
	{
		glPushMatrix();
		ofTranslate(pos.x, pos.y);
		glRotatef(rotation, 0, 0, 1);
		glScalef(size, size, 1.0);
		glColor4f(col.r, col.g, col.b, alpha);
		glCallList(quad_DL);
		glPopMatrix();
	}
	
	NuclearParticleDrawer(){
		
		ofDisableArbTex();
		quad_DL = glGenLists(1);
		glNewList(quad_DL, GL_COMPILE);
		glBegin(GL_QUADS);
		glVertex2f(-.5, -.5);glTexCoord2f (0, 0);
		glVertex2f(.5,-.5); glTexCoord2f(1,0);	
		glVertex2f(.5,.5); glTexCoord2f(1,1);
		glVertex2f(-.5,.5); glTexCoord2f(0,1);
		glEnd();
		glEndList();
		
		
		
		imgLoader.loadImage("images/nuclear/corona.png");
		textures[0].allocate(imgLoader.getWidth(),imgLoader.getHeight(),GL_RGBA);
		textures[0].loadData(imgLoader.getPixels(),imgLoader.getWidth(),imgLoader.getHeight(),GL_RGBA);
		glEnable(textures[0].getTextureData().textureTarget);
		
		imgLoader.loadImage("images/nuclear/glow.png");
		textures[1].allocate(imgLoader.getWidth(),imgLoader.getHeight(),GL_RGBA);
		textures[1].loadData(imgLoader.getPixels(),imgLoader.getWidth(),imgLoader.getHeight(),GL_RGBA);
		glEnable(textures[1].getTextureData().textureTarget);
		
		//simplex = ofxSimplex();
		
		
		isSetup = false;
		enablePointSmoothing = true;
		pointSize = 1;
		texture = NULL;
		name = "SingleSpriteDrawer";
		
		
		
	}
	
	~NuclearParticleDrawer(){
		if (!isSetup) return;
		
		// clean up dynamically created arrays
		delete[] vertexArray;
		delete[] colorArray;
	}
	
	void setup(int pointCount){
		if(!isSetup){
			maxPoints = pointCount;
			vertexArray = new float[pointCount*2];
			colorArray = new float[pointCount*4];
			isSetup = true;
			
		}
	}
	
	void setupGUI(){
		blendSetting.setupGUI();
	}
	
	virtual void draw(ParticlePool& pool){
		draw(pool.getAlive());
	};
	
	virtual void draw(set<Particle*>& particles){
		
		//	
	}
	
	virtual void draw(vector<Particle*>& particles){
		
		vector<Particle*>::iterator it;
		int count = 0;
		for(it = particles.begin(); it != particles.end() && count < maxPoints; ++it){	
			Particle& p = **it;
			
			// NEW DRAWING
			float a, s, r;
			ofColor c,cc;
			
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			
			c.r = p.property.physics.r/255.0;
			c.g = p.property.physics.g/255.0;
			c.b = p.property.physics.b/255.0;
			
			cc.r = p.property.physics.cr/255.0;
			cc.g = p.property.physics.cg/255.0;
			cc.b = p.property.physics.cb/255.0;
			
			//GLOW
			if(p.property.physics.e > 0)
			{
				//a = (p.life % (int)(glow_alphaFrq * 200))/(glow_alphaFrq * 200);
				
				a = ofSignedNoise((p.life%(int)(glow_alphaFrq * 200 + 1))/(glow_alphaFrq * 200 + 1));
				//a = TWO_PI * a;
				
				a = glow_color.a + glow_color.a * glow_alphaVar * a;
				
				//s = (p.life % (int)(glow_sizeFrq * 200))/(glow_sizeFrq * 200);
				
				s = ofSignedNoise((p.life%(int)(glow_sizeFrq * 200 + 1))/(glow_sizeFrq * 200 + 1));
				//s = TWO_PI * s;
				
				s = glow_size + glow_size * glow_sizeVar * s;
				s = s * p.property.physics.e_latency;
				
				textures[1].bind();
				if (glow_usePColor)
					render(p.p, s, c, a,0);
				else 
					render(p.p, s, glow_color, a,0);
				textures[1].unbind();
				
			}
			
		}
	}
	
	
protected:
	
	bool isSetup;
	int maxPoints;
	float* vertexArray;
	float* colorArray;
	ofTexture* texture;
};


