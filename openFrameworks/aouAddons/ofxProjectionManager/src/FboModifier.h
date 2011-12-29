/*
 *  FboModifier.h
 *  TestBed_Chris
 *
 *  Created by TAQA_CH_1 on 10/03/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "FboWarpOutput.h"

class ofxProjectionManager;
class FboBlendEdge;

class FboModifier {
public:
	
	FboWarpOutput* output;
	
	FboModifier();
	
	virtual void leftUp(float scale, bool analog=false) = 0;
	virtual void leftDown(float scale, bool analog=false) = 0;
	virtual void leftLeft(float scale, bool analog=false) = 0;
	virtual void leftRight(float scale, bool analog=false) = 0;
	
	virtual void rightUp(float scale, bool analog=false) = 0;
	virtual void rightDown(float scale, bool analog=false) = 0;
	virtual void rightLeft(float scale, bool analog=false) = 0;
	virtual void rightRight(float scale, bool analog=false) = 0;
	
	virtual string getHUDString();
};

// FboDrawModifier ///////////////////////////////////////

class FboDrawModifier : public FboModifier{

public:
	
	void leftUp(float scale, bool analog=false);
	void leftDown(float scale, bool analog=false);
	void leftLeft(float scale, bool analog=false);
	void leftRight(float scale, bool analog=false);
	
	void rightUp(float scale, bool analog=false);
	void rightDown(float scale, bool analog=false);
	void rightLeft(float scale, bool analog=false);
	void rightRight(float scale, bool analog=false);
	
	string getHUDString();
	
};

// FboHudTextModifier ///////////////////////////////////////

/**
 modifies positioning and scaling of projection manager hud text
 */
class HudTextModifier : public FboModifier {

public:
	
	ofxProjectionManager* projectionManager;
	
	void leftUp(float scale, bool analog=false);
	void leftDown(float scale, bool analog=false);
	void leftLeft(float scale, bool analog=false);
	void leftRight(float scale, bool analog=false);
	
	void rightUp(float scale, bool analog=false);
	void rightDown(float scale, bool analog=false);
	void rightLeft(float scale, bool analog=false);
	void rightRight(float scale, bool analog=false);
	
	string getHUDString();
};

// EdgeBlendModifier ///////////////////////////////////////

class EdgeBlendModifier : public FboModifier {
	
public:
	
	FboBlendEdge* edge;
	int		edgeId;
	string	edgeNames[4];
	int		functionId;
	string	functionNames[4];
	int		functionCount;
	float blendPointScale;
	float shapeScale;
	
	EdgeBlendModifier();
	
	void leftUp(float scale, bool analog=false);
	void leftDown(float scale, bool analog=false);
	void leftLeft(float scale, bool analog=false);
	void leftRight(float scale, bool analog=false);
	
	void rightUp(float scale, bool analog=false);
	void rightDown(float scale, bool analog=false);
	void rightLeft(float scale, bool analog=false);
	void rightRight(float scale, bool analog=false);
	
	string getHUDString();
	
	void updateEdgeSelection();
	
};