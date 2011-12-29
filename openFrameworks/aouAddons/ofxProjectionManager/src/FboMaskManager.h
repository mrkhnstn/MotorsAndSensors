#pragma once
/*
 *  FboMaskManager.h
 *  warp
 *
 *  Created by Robin Beitra (robin@beitra.net) on 26/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#include "ofMain.h"
#include "ofxSimpleGuiToo.h"
#include "ofxFBOTexture.h"
#include "ofxVec2f.h"

#include "ofxXmlSettings.h"

#include "FboMaskSelection.h"
#include "FboMaskShape.h"
#include "FboMask.h"
#include "FboUtils.h"
#include "ofxShader.h"
#include "ImageUtil.h"

class FboMaskManager{
public:
	
	FboMaskManager();
	
	void setup(ofxFBOTexture& worldFbo, string filepath = "FboMaskShape.xml");
	
	void setupGUI(string name="");
	
	void update();
	
	void draw();
	
	void drawEditShapes();
	
	void drawToWorld();
		
	void drawToFboBlending();
	
	float getZoom();
	
	ofxVec2f mouseVec(float mouseX, float mouseY);
	
	void addListeners();
	
	void removeListeners();
	
	void mouseMoved(ofMouseEventArgs &e);
	
	void mousePressed(ofMouseEventArgs &e);	
		
	void mouseReleased(ofMouseEventArgs &e);
	
	void keyPressed(ofKeyEventArgs &e);
	
	void shiftSelection(float dx, float dy);
	
	void keyReleased(ofKeyEventArgs &e);
	
	void mouseMove(float mouseX, float mouseY);
	
	void mouseDown(float mouseX, float mouseY);
	
	void mouseUp(float mouseX, float mouseY);
	
	FboMask mask;
	ofxFBOTexture* worldFbo;
	ofxFBOTexture maskFbo;
	
	ofImage maskImage;
	
	ofxVec2f displayCoords;
	ofxVec2f lastMouse;
	float displayZoom;
	
	bool drawing;
	bool editing;
	bool _editing;
	bool drawEditOverlay;
	bool hideMouseWhileEditing;
	bool dragging;
	FboMaskSelection dragPoint;
	
	bool drawToOutputs;
	bool blankOutput;
	
	bool save;
	bool load;
	bool autoload;
	bool hasAutoloaded;
	string loadPath;
	
	int maskImageTime;
	
	float clickDistance;
	bool invert;
	
	// blur related
	ofxShader shaderH;
	ofxShader shaderV;
	
	ofxFBOTexture fbo2;
	
	float blurDistance;
	int noPasses;
	bool blurEnabled;
};