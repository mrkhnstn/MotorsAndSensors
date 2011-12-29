/*
 *  ParticleGridDrawer.cpp
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 27/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ParticleGridDrawer.h"
#include "ParticleMeshSystem.h"

ParticleGridDrawer::ParticleGridDrawer() : ParticleDrawer() {
	texture = NULL;
	meshProperty = Singleton<ParticleMeshSystemProperty>::instance();
	flip = false;
	alpha = 1;
}

void ParticleGridDrawer::setupGUI(){
	//blendSetting.setupGUI();
	//gui.addColorPicker("blend color", &blendColor.r);
}

void ParticleGridDrawer::draw(ParticlePool& pool){
	if (typeid(pool)!=typeid(ParticleGridPool)) { // if pool is not ParticleGridPool then do nothing and return
		ofLog(OF_LOG_ERROR, "ParticleGridDrawer cannot draw anything other than particleGridPools");
		return;
	}
	
	if(texture == NULL) return;
	
	static int quadX[4] = {0,1,1,0};
	static int quadY[4] = {1,1,0,0};
	
	ParticleGridPool& gridPool = (ParticleGridPool&)pool;
	
	int quadCount = (gridPool.getRows()-1)*(gridPool.getCols()-1); //<TODO: only create / recreate buffers if quadCount changed
	float vertexBuffer[quadCount*4*2];
	float textureBuffer[quadCount*4*2];
	
	float* vertexPt = &vertexBuffer[0]; 
	float* texturePt = &textureBuffer[0];
	
	for(int row = 0; row < gridPool.getRows() - 1; row++){
		for(int col = 0; col < gridPool.getCols() - 1; col++){
			
			for(int i=0; i<4; i++){
				int tempCol = col+quadX[i];
				int tempRow = row+quadY[i];
				
				Particle* p = gridPool.getParticle(tempCol, tempRow);
				float x = p->p.x;
				float y = p->p.y;
				float tx; //< TODO: texture uvs have to be only set if number of grid columns / rows change or if texture changes
				float ty;
				if (flip) {
					tx = ofMap(tempCol, 0.0, gridPool.getCols()-1, texture->getWidth(),	0);
					ty = ofMap(tempRow, 0.0, gridPool.getRows()-1, texture->getHeight(), 0);
				} else {
					tx = ofMap(tempCol, 0.0, gridPool.getCols()-1, 0, texture->getWidth());
					ty = ofMap(tempRow, 0.0, gridPool.getRows()-1, 0, texture->getHeight());
				}
				*vertexPt++ = x;
				*vertexPt++ = y;
				*texturePt++ = tx;
				*texturePt++ = ty;
			}
		}
	}
	
	meshProperty->blendSetting.begin();
	ofColor& blendColor = meshProperty->blendColor;
	glColor4f(blendColor.r, blendColor.g, blendColor.b, blendColor.a * alpha);
	
	texture->bind();
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vertexBuffer);
	glTexCoordPointer(2, GL_FLOAT, 0, textureBuffer);
	glDrawArrays( GL_QUADS, 0, quadCount*4);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	texture->unbind();
	
	meshProperty->blendSetting.end();
	
}

/*
 disabled
 **/
void ParticleGridDrawer::draw(set<Particle*>& particles){
}

/**
 disabled
 */
void ParticleGridDrawer::draw(vector<Particle*>& particles){
}

void ParticleGridDrawer::setTexture(ofTexture& texture){
	this->texture = &texture;
}