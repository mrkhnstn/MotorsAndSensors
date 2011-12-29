#pragma once
/*
 *  AtlasQuadParticleDrawer.h
 *  TestBed_Mark2
 *
 *  Created by TAQA_CH_0 on 15/02/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */


#include "QuadParticleDrawer.h"
#include "TextureAtlas.h"

////////////////////////////////////////////////////////////////

/**
 - same as QuadParticleDrawer but uses a texture atlas to set the texture uvs for each particle
 */
class AtlasQuadParticleDrawer : public QuadParticleDrawer {

public:
	
	TextureAtlas* textureAtlas;
	
	AtlasQuadParticleDrawer();
	
	void setTextureAtlas(TextureAtlas& textureAtlas);
	
	virtual void draw(vector<Particle*>& particles);
	
};
