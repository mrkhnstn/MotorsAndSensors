/*
 *  QuadDrawer2f.h
 *  openFrameworks
 *
 *  Created by Mark Hauenstein on 11/12/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

//#ifndef QUAD_RENDER_MANAGER
//#define QUAD_RENDER_MANAGER
//#include "MSATexture.h"
#pragma once
#include "ofMain.h"

class QuadRenderManager2f{
	
public: 
	
	QuadRenderManager2f();
	QuadRenderManager2f(int quadCount);
	
	~QuadRenderManager2f();
	
	int quadCount;
	float * vertexBuffer;
	int * texCoordBuffer;
	float * colorBuffer;
	
	//MSATexture * tex;
	//ofTexture * tex;
	
	virtual void setup(int quadCount);
	
	virtual void reset();
	
	virtual void update();
	
	virtual void draw(bool useTexCoords = true, bool useColor = true);
	
	virtual void draw(int count, bool useTexCoords = true, bool useColor = true);
	
	bool allocated;

private:
	GLuint		vbo[3];
};
