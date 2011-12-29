/*
 ------------------------------------------------------------------------------------------------
 NuclearParticleDrawer.cpp | TestBed_Matt | 11/02/2011
 
 created by MATT JAKOB - www.mattjakob.com
 Copyright 2011. All rights reserved.
 ------------------------------------------------------------------------------------------------
 */


#include "NuclearParticleDrawer.h"

void NuclearParticleDrawer::render(ofxVec2f pos, float size, ofColor col, float alpha, float rotation, int frame)
{
	/*
	 glPushMatrix();
	 ofTranslate(pos.x, pos.y);
	 glRotatef(rotation, 0, 0, 1);
	 glScalef(size, size, 1.0);
	 glColor4f(col.r, col.g, col.b, alpha);
	 glCallList(quad_DL);
	 glPopMatrix();
	 */
	
#ifdef ATI

	//*vertexPointer++ = pos.x;
	//*vertexPointer++ = pos.y;
	
	//float* tex = textureAtlas->getTextureCoord(frame);
	//*texturePointer++ = *tex++; // tex u
	//*texturePointer++ = *tex; // tex v
	//*texturePointer++ = size; // radius
	//*texturePointer++ = rotation; // rotation in degrees
	
	int rotationId = ofRandomuf() * (ROTATION_COUNT-1);
	
	int k=0;
	for(int i=0; i<4; i++){
		*vertexPointer++ = rotatedVertex[rotationId][k++] * size + pos.x;
		*vertexPointer++ = rotatedVertex[rotationId][k++] * size + pos.y;
	}
	/*
	// vertex sides
	float vLeft = pos.x - size;// * 0.5f;
	float vRight = pos.x + size;// * 0.5f;
	float vTop = pos.y - size;// * 0.5f;
	float vBottom = pos.y + size;// * 0.5f;
	
	//top left
	*vertexPointer++ = vLeft;
	*vertexPointer++ = vTop;
	
	//top right
	*vertexPointer++ = vRight;
	*vertexPointer++ = vTop;
	
	//bottom right
	*vertexPointer++ = vRight;
	*vertexPointer++ = vBottom;
	
	//bottom left
	*vertexPointer++ = vLeft;
	*vertexPointer++ = vBottom;
	*/
	
	   
	//ofLog(OF_LOG_VERBOSE, "frame:"+ofToString(frame));
	
	memcpy(texturePointer, textureAtlas->getTextureCoord(frame), 8 * sizeof(float));
	//ofLog(OF_LOG_VERBOSE, "=>"+ofToString(frame)+":"+ofToString(*(texturePointer+0))+","+ofToString(*(texturePointer+1))+":"+ofToString(*(texturePointer+2))+","+ofToString(*(texturePointer+3))+ofToString(*(texturePointer+4))+","+ofToString(*(texturePointer+5))+":"+ofToString(*(texturePointer+6))+","+ofToString(*(texturePointer+7)));

	texturePointer += 8;

	
	for(int i=0; i<4; i++){
		*colorPointer++ = col.r;
		*colorPointer++ = col.g;
		*colorPointer++ = col.b;
		*colorPointer++ = alpha;
	}
	
	drawCount++;
	
#else


	
	*vertexPointer++ = pos.x;
	*vertexPointer++ = pos.y;
	
	float* tex = textureAtlas->getTextureCoord(frame);
	*texturePointer++ = *tex++; // tex u
	*texturePointer++ = *tex; // tex v
	*texturePointer++ = size; // radius
	*texturePointer++ = rotation; // rotation in degrees
	
	*colorPointer++ = col.r;
	*colorPointer++ = col.g;
	*colorPointer++ = col.b;
	*colorPointer++ = alpha;
	
	
	
	drawCount++;
	
	#endif
}

void NuclearParticleDrawer::resetPointers(){
	drawCount = 0;
	colorPointer = colorBuffer;
	vertexPointer = vertexBuffer;
	texturePointer = texCoordBuffer;
}

#ifdef ATI
	NuclearParticleDrawer::NuclearParticleDrawer() :  AtlasQuadParticleDrawer::AtlasQuadParticleDrawer(){
#else
	NuclearParticleDrawer::NuclearParticleDrawer() :  AtlasGSDrawer::AtlasGSDrawer(){
#endif
	
	drawCount = 0;
	myTextureAtlas.setup(256,256, 2, 2);
	myTextureAtlas.addFolder("animations/nuclear");
	textureAtlas = &myTextureAtlas;
	radiusPow = 10;
	isSetup = false;
		//setTexture(myTextureAtlas.getTexture());
}

NuclearParticleDrawer::~NuclearParticleDrawer(){
	if (!isSetup) return;
}


/*
 ofxVec2f pointOnLine(int x1, int y1, int x2, int y2, float t)
 {
 ofxVec2f p;
 p.x = x1 + (x2 - x1) * (1-t);
 p.y = y1 + (y2 - y1) * (1-t);
 return p;
 }
 */

void NuclearParticleDrawer::setup(int pointCount){

#ifdef ATI
	AtlasQuadParticleDrawer::setup(pointCount * 5);
#else
	AtlasGSDrawer::setup(pointCount * 5);
#endif

	if(!isSetup){	
		maxPoints = pointCount;
		isSetup = true;			
	}
}

void NuclearParticleDrawer::setupGUI(){
	blendSetting.setupGUI();
	
	
	gui.addTitle("GLOW");//.setNewColumn(true);
	gui.addSlider("glow size", glow_size, 0 , 512);
	gui.addSlider("glow size var", glow_sizeVar, 0 , 1.0);
	gui.addSlider("glow size frq", glow_sizeFrq, 0 , 1.0);
	gui.addSlider("glow alpha var", glow_alphaVar, 0 , 1.0);
	gui.addSlider("glow alpha frq", glow_alphaFrq, 0 , 1.0);
	gui.addToggle("glow dyn color", glow_usePColor);
	gui.addColorPicker("g color", &glow_color.r);
	
	gui.addTitle("GAS").setNewColumn(true);
	gui.addSlider("gas size", gas_size, 0 , 512);
	gui.addSlider("gas size var", gas_sizeVar, 0 , 1.0);
	gui.addSlider("gas size frq", gas_sizeFrq, 0 , 1.0);
	gui.addSlider("gas alpha var", gas_alphaVar, 0 , 1.0);
	gui.addSlider("gas alpha frq", gas_alphaFrq, 0 , 1.0);
	gui.addToggle("gas dyn color", gas_usePColor);
	gui.addColorPicker("gas color", &gas_color.r);
	
	gui.addTitle("EXPLOSION").setNewColumn(true);
	gui.addSlider("explosion size", exp_size, 0 , 512);
	gui.addSlider("explosion size var", exp_sizeVar, 0 , 1.0);
	gui.addSlider("explosion size frq", exp_sizeFrq, 0 , 1.0);
	gui.addSlider("explosion alpha var", exp_alphaVar, 0 , 1.0);
	gui.addSlider("explosion alpha frq", exp_alphaFrq, 0 , 1.0);
	gui.addToggle("explosion dyn color", exp_usePColor);
	gui.addColorPicker("explosion color", &exp_color.r);
	gui.addColorPicker("cracks color", &crk_color.r);
	
	gui.addTitle("BURNOUT").setNewColumn(true);
	gui.addSlider("brn size", brn_size, 0 , 512);
	gui.addSlider("brn size var", brn_sizeVar, 0 , 1.0);
	gui.addSlider("brn size frq", brn_sizeFrq, 0 , 1.0);
	gui.addSlider("brn alpha var", brn_alphaVar, 0 , 1.0);
	gui.addSlider("brn alpha frq", brn_alphaFrq, 0 , 1.0);
	gui.addToggle("brn dyn color", brn_usePColor);
	gui.addColorPicker("brn color", &brn_color.r);
	
	gui.addTitle("ATOM CORE");
	atomRadiusRange.addToGUI("radius", 0, 32);
	gui.addSlider("radius pow", radiusPow, 1, 32);
	atomAlphaRange.addToGUI("alpha", 0, 1);
	
}

void NuclearParticleDrawer::draw(vector<Particle*>& particles){
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	
	float atomWidth = 16;
	float atomHeight = 16;
	
	resetPointers();
	
	setTexture(textureAtlas->getTexture());
	
	vector<Particle*>::iterator it;
	int count = 0;
	for(it = particles.begin(); it != particles.end() && count < maxPoints; ++it){	
		Particle& p = **it;
		
		// NEW DRAWING
		float a, s, r;
		ofColor c;
		
		c.r = p.property.physics.r/255.0;
		c.g = p.property.physics.g/255.0;
		c.b = p.property.physics.b/255.0;
		
		//glColor4f(c.r,c.g,c.b,1);
		//glVertex3f(p.p.x, p.p.y, 0);
		//atomImage.draw(-0.5*atomWidth+p.p.x, -0.5*atomHeight+p.p.y,atomWidth,atomHeight);
		
		//atomImage.getTextureReference().bind();
		//render(p.p, 16, c, 1, 0);
		//atomImage.getTextureReference().unbind();
		
		
		
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
			
			//textures[1].bind();
			if (glow_usePColor)
				render(p.p, s, c, a,0,1);
			else 
				render(p.p, s, glow_color, a,0,1);
			//textures[1].unbind();
			
			
			
		}
		
		// render atom

		render(p.p, atomRadiusRange.getMapped(powf(p.nProperty, radiusPow), 0, 1), glow_color, atomAlphaRange.getRandomValue(),p.rotation + p.life,2);
		
		//GAS
		if(p.property.physics.e > 0)
		{
			//a = (p.life % (int)(gas_alphaFrq * 200))/(gas_alphaFrq * 200);
			
			a = ofSignedNoise((p.life%(int)(gas_alphaFrq * 200 + 1))/(gas_alphaFrq * 200 + 1));
			//a = TWO_PI * a;
			
			a = gas_color.a	+ gas_color.a * gas_alphaVar * a;
			
			//s = (p.life % (int)(gas_sizeFrq * 200))/(gas_sizeFrq * 200);
			
			s = ofSignedNoise((p.life%(int)(gas_sizeFrq * 200 + 1))/(gas_sizeFrq * 200 + 1));
			//s = TWO_PI * s;
			
			s = gas_size + gas_size * gas_sizeVar * s;
			s = s * p.property.physics.e_level;
			
			//textures[0].bind();
			if (gas_usePColor)
				render(p.p, s, c, a,ofRandom(0, 360),0);
			else 
				render(p.p, s, gas_color, a, ofRandom(0, 360),0);
			//textures[0].unbind();
		}
		
		
		
		//BOND	
		if(p.property.physics.parent_x !=0 &&  p.property.physics.parent_y != 0)
		{	
			//a = (p.life % (int)(exp_alphaFrq * 200))/(exp_alphaFrq * 200);
			
			a = ofSignedNoise((p.life%(int)(exp_alphaFrq * 200 + 1))/(exp_alphaFrq * 200 + 1));
			//a = TWO_PI * a;
			
			a = exp_color.a + (1.0 - exp_color.a) * exp_alphaVar * a;
			
			//s = (p.life % (int)(exp_sizeFrq * 200))/(exp_sizeFrq * 200);
			
			s = ofSignedNoise((p.life%(int)(exp_sizeFrq * 200 + 1))/(exp_sizeFrq * 200 + 1));
			//s = TWO_PI * s;
			
			s = exp_size + exp_size * exp_sizeVar * s;
			s = s * p.property.physics.t_bond;
			
			//textures[0].bind();
			if (exp_usePColor)
				render(p.p, s, c, a,ofRandom(0, 360),0);
			else 
				render(p.p, s, exp_color, a, ofRandom(0, 360),0);
			//textures[0].unbind();
			
			
			if (exp_usePColor)
				ofSetColor(c.r*255,c.g*255,c.b*255,crk_color.a*255);
			else
				ofSetColor(crk_color.r*255,crk_color.g*255,crk_color.b*255,crk_color.a*255);
			ofLine(p.p.x, p.p.y, p.property.physics.parent_x, p.property.physics.parent_y);
			
		}
		
		
		//BURNOUT
		if(p.property.physics.t_burnoutTime > 0)
		{
			
			//a = (p.life % (int)(brn_alphaFrq * 200))/(brn_alphaFrq * 200);
			
			a = ofSignedNoise((p.life%(int)(brn_alphaFrq * 200 + 1))/(brn_alphaFrq * 200 + 1));
			//a = TWO_PI * a; 
			
			a = brn_color.a + brn_color.a * brn_alphaVar * a;
			
			s = (p.life % (int)(brn_sizeFrq * 200))/(brn_sizeFrq * 200);
			
			s = ofSignedNoise((p.life%(int)(brn_sizeFrq * 200 + 1))/(brn_sizeFrq * 200 + 1));
			//s = TWO_PI * s;
			
			s = brn_size + brn_size * brn_sizeVar * s;
			s = s * p.property.physics.t_burnoutTime;
			
			//textures[1].bind();
			if (brn_usePColor)
				render(p.p, s, c, a,1);
			else 
				render(p.p, s, brn_color, a,1);
			//textures[1].unbind();
			
			//a = (p.life % (int)(exp_alphaFrq * 200))/(exp_alphaFrq * 200);
			
			a = ofSignedNoise((p.life%(int)(exp_alphaFrq * 200 + 1))/(exp_alphaFrq * 200 + 1));
			//a = TWO_PI * a;
			
			a = exp_color.a + (1.0 - exp_color.a) * exp_alphaVar * a;
			
			//s = (p.life % (int)(exp_sizeFrq * 200))/(exp_sizeFrq * 200);
			
			s = ofSignedNoise((p.life%(int)(exp_sizeFrq * 200 + 1))/(exp_sizeFrq * 200 + 1));
			//s = TWO_PI * s;
			
			s = exp_size + exp_size * exp_sizeVar * s;
			s = s * p.property.physics.t_bond;
			
			//textures[0].bind();
			if (exp_usePColor)
				render(p.p, s, c, a,ofRandom(0, 360),0);
			else 
				render(p.p, s, exp_color, a, ofRandom(0, 360),0);
			//textures[0].unbind();
			
		}
		
		//ofLog(OF_LOG_VERBOSE, ofToString(drawCount));
		
		
	}
	
	
#ifdef ATI
	QuadParticleDrawer::draw(drawCount);
#else
	AtlasGSDrawer::draw(drawCount);
#endif
	
	//AtlasGSDrawer::draw(drawCount);
}
