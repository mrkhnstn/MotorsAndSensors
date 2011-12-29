#pragma once
/*
 *  FboWarpDrawer.h
 *  warp
 *
 *  Created by Robin Beitra (robin@beitra.net) on 24/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"
#include "ofxShader.h"
#include "ofxFBOTexture.h"
#include "ofxSimpleGuiToo.h"
#include "QuadRenderManager2f.h"
#include "FboWarpGrid.h"

class FboWarpOutput;

class FboWarpDrawer{
public:
	
	/*
	ofColor outerGridColor;
	ofColor innerGridColor;
	ofColor blankColor;
	ofColor edgeColor;
	*/
	
	string	parentName;
	
	/*
	ofColor highlightColor;
	float	highlightPointSize;
	bool	highlightCrossHair;
	float	highlightRadius;
	bool	highlightCircle;
	*/
	
	FboWarpDrawer();
	
	void setup(int fboWidth, int fboHeight, ofxShader& shader_);
	
	void setupGUI(string name);
	
	void swapIn(bool clear = true);
	
	void swapOut();
	
	//virtual void draw();
	
	void drawSub(int i, FboWarpOutput* output, bool blending = true, ofTexture* texture = NULL);
	
	void registerGrid(FboWarpGrid& _grid, ofImage* mask);
	
	void buildGrids();
	
	void buildGrid(FboWarpGrid& _grid, int g);
	
	void drawHighlight(FboWarpOutput* output, FboWarpGrid& grid, int index, bool inside = true);
	
	void drawGrid(FboWarpOutput* output, FboWarpGrid& grid, int g, bool outer = false, bool inner = false, bool blank = false, bool drawAsPoints = false);
	
	void drawOuterGrid(FboWarpOutput* output, FboWarpGrid& grid, bool drawAsPoints = true);
	
	void drawQuad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4,
				  float a1, float b1, float a2, float b2, float a3, float b3, float a4, float b4,
				  int depth, float hmid, float vmid
				  );
	
	void reset();
	
	void setColor(ofColor& c);
	
	ofxFBOTexture& getTexture();
	
	ofxFBOTexture fboTexture;
	
	int fboWidth, fboHeight;
	ofColor bgColor;
	
	vector<GLfloat> texCoordVector;
	vector<GLfloat> vertexVector;
	vector<QuadRenderManager2f*> quadRenderManagers;
	vector<ofImage*> masks;
	
	int drawCount;
	
	float perspectiveCorrectionPrecision;
	int maxRecursion;
	
	vector<FboWarpGrid*> grids;

	ofxShader* shader;

	
};