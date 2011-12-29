#pragma once
/*
 *  ofxProjectionManager.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 06/02/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxBaseObject.h"
#include "ofxFBOTexture.h"
#include "ofxShader.h"
#include "Utils.h"
#include "VectorMap.h"
#include "QuadRenderManager2f.h"

#include "FboBlending.h"
#include "FboWarpGrid.h"
#include "FboWarpDrawer.h"
#include "FboWarpOutput.h"

#include "FboModifier.h"
#include "FboDstTransformModifier.h"
#include "FboSrcTransformModifier.h"
#include "FboDstPointModifier.h"
#include "FboDstPointScaleModifier.h"

/*
// Modes
//#define EDIT_MODE_OUTPUT_SELECT 0
#define EDIT_MODE_DEST_TRANSFORM 0
#define EDIT_MODE_SRC_TRANSFORM 1
#define EDIT_MODE_WARP_COLUMN 2
#define EDIT_MODE_WARP_ROW 3
#define EDIT_MODE_WARP_POINT 4
#define EDIT_MODE_DRAW 5
#define EDIT_MODE_HUD_TEXT 6
//#define EDIT_MODE_SCALE_COLUMN 5
//#define EDIT_MODE_SCALE_ROW 6
#define EDIT_MODE_COUNT 7
*/

class ofxProjectionManager : public ofxBaseObject {
public:
	
	// Properties
	ofColor	blendColor;
	float	drawWidth; //< used for non warp output draw modes
	float	drawHeight; //< used for non warp output draw modes
	
	// States
	int		mode;
	int		outputIndex;
	int		scaleMode;
	bool	doEditByKey;
	float	editByKeyScaleNormal; ///< value used for keyboard based editing when a,s,d,w,left,right,up,down is pressed
	float	editByKeyScaleShift; ///< value used for keyboard based editing when shift key is pressed
	bool	doDrawInfoOverlay;
	int		drawLayer;///< used by camera stitcher to order drawing
	
	// Fbo mode dependant modifiers
	FboModifier*				activeModifier;
	vector<FboModifier*>		modifiers;
	
	// destination modifiers
	FboDstTransformModifier		dstTransformModifier;
	FboDstPointModifier			dstPointModifier;
	FboDstColModifier			dstColModifier;
	FboDstRowModifier			dstRowModifier;
	//FboDstPointScaleModifier	dstPointScaleModifier;
	// source modifiers
	FboSrcTransformModifier		srcTransformModifier;
	FboDrawModifier				drawModifier;
	HudTextModifier				hudTextModifier;
	EdgeBlendModifier			edgeBlendModifier;
	
	// Drawing and output
	FboWarpDrawer						drawer;
	VectorMap<string, FboWarpOutput*>	outputs;
	FboWarpOutput*						output;
	static ofxShader					shader;
	static bool							shaderIsSetup;
	float								drawerRecursionPrecision;
	int									drawerMaxRecursion;
	int									backgroundImageSetTime;
	
	static ofTrueTypeFont	hudFont;
	ofPoint					hudPos;
	float					hudScale;
	
	bool drawHUDInsideFbo;
	
	// Con/Destructor ////////////////////////////////////
	
	ofxProjectionManager();
	
	~ofxProjectionManager();
	
	// Methods ///////////////////////////////////////////
	
	/**
	 - width and height determine the internal draw area or the dimensions of the fbo 
	 - slices determine the number of slices to draw on to the main stage
	 */
	void setup(int width, int height);
	
	void setupGUI();
	
	void postGUI();
	
	void draw();
	
	void update();
	
	/**
	 call this function to start drawing into the internal fbo
	 */
	void begin();
	
	/**
	 call this function to stop drawing into the internal fbo
	 */
	void end();
	
	
	// Drawing and output ////////////////////////////
	
	void addOutput(string suffix, string xmlFolderpath);
	
	void setOutputSourceRect(string suffix, float x, float y, float width, float height);
	
	void registerBackgroundImage(ofImage* background);
	
	void updateBackgroundImage();
	
	void redraw();
	
	void drawFbo(float x, float y, float width, float height);
	
	void drawOutputs();
	
	void drawOutputHUD(FboWarpOutput* output );
	
	void drawOutputHUDOutside(FboWarpOutput* output );
	
	ofxFBOTexture& getTexture();
	
	
	// Key Listeners //////////////////////////////////
	
	void keyPressed(ofKeyEventArgs& e);
	void keyPressed(int key);
	
	// Mode Navigation ////////////////////////////////
	
	void nextMode();
	void previousMode();
	void updateMode();
	void updateOutputs();
	void enableEditing(bool enabled);
	
	
	// Input ///////////////////////////////////////////
	
	void leftUp(float scale, bool analog=false);
	void leftDown(float scale, bool analog=false);
	void leftLeft(float scale, bool analog=false);
	void leftRight(float scale, bool analog=false);
	
	void rightUp(float scale, bool analog=false);
	void rightDown(float scale, bool analog=false);
	void rightLeft(float scale, bool analog=false);
	void rightRight(float scale, bool analog=false);
	
	void nextOutput();	
	void prevOutput();
	void setOutput(int i);
		
};