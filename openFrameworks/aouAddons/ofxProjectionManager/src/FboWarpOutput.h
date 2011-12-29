#pragma once
/*
 *  FboWarpOutput.h
 *  warp
 *
 *  Created by Robin Beitra (robin@beitra.net) on 24/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"
#include "ofxVec2f.h"

#include "FboWarpDrawer.h"
#include "FboWarpGrid.h"
#include "FboBlending.h"
#include "FboUtils.h"

#define WARP_EDIT_POINT		0
#define WARP_EDIT_COLUMN	1
#define WARP_EDIT_ROW		2
#define WARP_EDIT_COUNT		3

struct Levels {
	float minInput, maxInput, gamma, minOutput, maxOutput;
	ofColor minInputRGB, maxInputRGB, gammaRGB, minOutputRGB, maxOutputRGB;
	bool useRGB;
};

class FboWarpOutput {
public:
	
	FboWarpOutput(string xmlFolderpath);
	void setup(FboWarpDrawer* _drawer, string _name);
	void reset();
	bool isEditing();
	void draw();
	void drawHUD();
	void update();
	void updateGUIFromGrid();
	void doLoadGrid();
	void doSaveGrid();
	void gridModified();
	void redraw();
	void redrawBlending();
	void keyPressed(ofKeyEventArgs& e);
	void keyPressed(int key);
	void scaleSelection(float incrementPx);
	void shiftSelection(float dx, float dy);
	void changeSelection(int dx, int dy);
	void setupGUI();
	
	
	// For external inputs ////////////////////////////////////////////////////////////////
	
	void increaseDstX(float scale);
	void decreaseDstX(float scale);
	void increaseDstY(float scale);
	void decreaseDstY(float scale);
	void increaseDstWidth(float scale);
	void decreaseDstWidth(float scale);
	void increaseDstHeight(float scale);
	void decreaseDstHeight(float scale);
	
	void increaseSrcX(float scale);
	void increaseSrcY(float scale);
	void increaseSrcWidth(float scale);
	void increaseSrcHeight(float scale);
	void decreaseSrcX(float scale);
	void decreaseSrcY(float scale);
	void decreaseSrcWidth(float scale);
	void decreaseSrcHeight(float scale);
	
	
	// Fields /////////////////////////////////////////////////////////////////////////////
	
	FboWarpGrid		grid;//active grid for editing.
	FboWarpDrawer*	drawer;
	ofRectangle		window;
	int				id;
	string			name;
	string			parentName;
	
	//display
	bool		drawOutput;
	//bool		drawOuterGridAsPoints;
	int			drawOuterGrid;
	bool		drawInnerGrid;
	bool		drawBlank; // draws a black rectangle on top of rendered elements before drawing outer and inner grid
	bool		drawHudInsideFbo; ///< if true then draw grid and highlights inside fbo
	
	ofColor outerGridColor;
	ofColor innerGridColor;
	ofColor blankColor;
	//ofColor highlightColor;
	ofColor edgeColor;
	
	ofColor highlightColor;
	float	highlightPointSize;
	bool	highlightCrossHair;
	float	highlightRadius;
	bool	highlightCircle;
	
	//editing
	bool		editingGrid;
	bool		keyboardEditingEnabled;
	int			editMode; // Determines the current keyboard edit mode: point/column/row
	int			gridCols;
	int			gridRows;
	float		cellWidth;
	float		cellHeight;
	ofRectangle	src, dst;
	ofxVec2f	shift;
	ofxVec2f	stretchTL, stretchTR, stretchBR, stretchBL;
	float		radialOffsetX;
	float		radialOffsetY;
	float		radialAmount;
	float		radialInverseAmount;
	float		radialScale;
	float		radialBias;
	bool		clearGrid;
	bool		fullReset;
	int			editingPoint;
	vector<int> editingPoints; // grouped points when editing a column or row
	
	//saving
	string		xmlFolderpath;
	bool		autoLoadGrid;
	bool		saveGrid;
	bool		loadGrid;
	FboBlending	blending;
	
	//levels controll
	//float levelsMinInput0,levelsMaxInput0, levelsMinOutput0, levelsMaxOutput0, levelsGamma0;
	
	Levels levels0, levels2;
	
protected:
	
	bool		autoLoadedGrid;
	FboWarpGrid _grid;//internal grid to check for changes.
};
