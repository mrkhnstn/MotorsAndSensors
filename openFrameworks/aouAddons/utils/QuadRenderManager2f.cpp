/*
 *  QuadDrawer2f.cpp
 *  openFrameworks
 *
 *  Created by Mark Hauenstein on 11/12/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "QuadRenderManager2f.h"

QuadRenderManager2f::QuadRenderManager2f(){
	allocated = false;	
}

QuadRenderManager2f::QuadRenderManager2f(int quadCount){
	allocated = false;
	setup(quadCount);
}

QuadRenderManager2f::~QuadRenderManager2f(){
	reset();
}

void QuadRenderManager2f::setup(int quadCount){
	
	reset();
	
	this->quadCount	= quadCount;
	
	//this->tex = tex;
	
	// generate vbos
	vertexBuffer = new float[4*2*quadCount];
	texCoordBuffer = new int[4*2*quadCount];
	colorBuffer = new float[4*4*quadCount];
	
	
	glGenBuffersARB(3, &vbo[0]);
	
	
	// initialize vbo for vertices
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[0]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, 4*2*quadCount*sizeof(float), vertexBuffer, GL_STREAM_DRAW_ARB);
	
	// initialize vbo for textureCoords
	

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[1]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, 4*2*quadCount*sizeof(int), texCoordBuffer, GL_STREAM_DRAW_ARB);
	
	// initialize vbo for colour
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[2]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, 4*4*quadCount*sizeof(float), colorBuffer, GL_STREAM_DRAW_ARB);
	
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	
	allocated = true;
}

void QuadRenderManager2f::reset(){
	
	if(allocated){
		allocated = false;
		
		glDeleteBuffersARB(3, &vbo[0]);
		
		delete[] vertexBuffer;
		delete[] texCoordBuffer;
		delete[] colorBuffer;
		
		
	}
}

void QuadRenderManager2f::update(){}

void QuadRenderManager2f::draw(int count, bool useTexCoords, bool useColor){
	
	int count_ = CLAMP(count,0,quadCount); // safe guard to not go beyond allocated arrays
	
	// enable alpha blending using the alpha channel of texture too
	/*
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	tex->bind();		// ofTexture needs to be hacked to only use TEXTURE2D
	*/
	
	glEnableClientState(GL_VERTEX_ARRAY);
	
	if (useTexCoords)	glEnableClientState(GL_TEXTURE_COORD_ARRAY); 
	if (useColor)		glEnableClientState(GL_COLOR_ARRAY);
	
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[0]);
	glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, 4*2*count_ * sizeof(float), vertexBuffer);
	glVertexPointer(2, GL_FLOAT, 0, 0);
	
	if (useTexCoords) {
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[1]);
		glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, 4*2*count_ * sizeof(int), texCoordBuffer);
		glTexCoordPointer(2, GL_INT, 0, 0);
	}
	
	if (useColor) {
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[2]);
		glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, 4*4*count_ * sizeof(float), colorBuffer);
		glColorPointer(4, GL_FLOAT, 0, 0);
	}

	glDrawArrays(GL_QUADS, 0, 4*count_);
	
	glDisableClientState(GL_VERTEX_ARRAY); 
	if (useTexCoords)	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	if (useColor)		glDisableClientState(GL_COLOR_ARRAY);
	
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	
	/*
	tex->unbind();
	glDisable(GL_BLEND);
	*/
}

void QuadRenderManager2f::draw(bool useTexCoords, bool useColor){
	draw(quadCount, useTexCoords, useColor);	
}