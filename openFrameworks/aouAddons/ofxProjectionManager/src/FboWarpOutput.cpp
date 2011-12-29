/*
 *  FboWarpOutput.cpp
 *  warp
 *
 *  Created by Robin Beitra (robin@beitra.net) on 24/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "FboWarpOutput.h"

FboWarpOutput::FboWarpOutput(string xmlFolderpath = ""){
	drawHudInsideFbo = false;
	drawOuterGrid = false;
	drawInnerGrid = false;
	drawOutput = true;
	drawBlank = false;
	editingGrid = false;
	editingPoint = 0;
	editMode = WARP_EDIT_POINT;
	
	src.x = 0;
	src.y = 0;
	src.width = 1024;
	src.height = 768;
	dst.x = 0;
	dst.y = 0;
	dst.width = 1024;
	dst.height = 768;
	gridCols = 1;
	gridRows = 1;
	cellWidth = src.width / gridCols;
	cellHeight = src.height / gridCols;
	radialAmount = 0;
	radialInverseAmount = 0;
	radialOffsetX = 0;
	radialOffsetY = 0;
	radialScale = 1;
	radialBias = 0;
	clearGrid = false;
	fullReset = false;
	
	autoLoadGrid = false;
	saveGrid = false;
	loadGrid = false;
	keyboardEditingEnabled = false;
	autoLoadedGrid = false;
	this->xmlFolderpath = xmlFolderpath;
	
	highlightRadius = 10;
	highlightPointSize = 1;
	highlightCrossHair = true;
	highlightCircle = true;
		
	parentName = "Projection Manager";
}

void FboWarpOutput::setup(FboWarpDrawer* _drawer, string _name){
	name = _name;
	drawer = _drawer;
	
	window.x = 0;
	window.y = 0;
	window.width = drawer->fboWidth;
	window.height = drawer->fboHeight;
	
	src.width = drawer->fboWidth;
	src.height = drawer->fboHeight;
	dst.width = drawer->fboWidth;
	dst.height = drawer->fboHeight;
	
	drawer->registerGrid(_grid, &blending.blendImage);
	
	grid.setup(window, src, dst, gridCols, gridRows);
	
	blending.setup(src, src.width*0.125, src.height*0.125);
	
	gridModified();
}

void FboWarpOutput::reset(){
	window.x = 0;
	window.y = 0;
	window.width = drawer->fboWidth;
	window.height = drawer->fboHeight;
	src = window;
	dst = window;
	grid.setup(window, src, dst, gridCols, gridRows);
	blending.setup(src, src.width*0.125, src.height*0.125);
	gridModified();
}

bool FboWarpOutput::isEditing(){
	return drawOuterGrid || drawInnerGrid || editingGrid || drawBlank;
}

void FboWarpOutput::draw(){

	if(drawOuterGrid || drawInnerGrid || editingGrid || drawBlank){
		
		drawer->swapIn(false);
		
		drawer->drawGrid(this, _grid, id, drawOuterGrid && drawHudInsideFbo, drawInnerGrid,drawBlank,drawOuterGrid == 2);
		
		if (drawHudInsideFbo) {
			if(editingGrid){
				if (editMode == WARP_EDIT_COLUMN || editMode == WARP_EDIT_ROW) {
					for (uint i = 0; i < editingPoints.size(); i++) {
						drawer->drawHighlight(this, _grid, editingPoints[i]);
					}
				} else {
					drawer->drawHighlight(this, _grid, editingPoint);
				}
			}
		}
		drawer->swapOut();
	}
	
	if(blending.doDraw){
		drawer->swapIn(false);
		blending.draw(this);
		drawer->swapOut();
	}
}

void FboWarpOutput::drawHUD(){
	if(!drawHudInsideFbo){
		if(drawOuterGrid){
			drawer->drawOuterGrid(this, _grid,drawOuterGrid == 2);
		}
		if(editingGrid){
			if (editMode == WARP_EDIT_COLUMN || editMode == WARP_EDIT_ROW) {
				for (uint i = 0; i < editingPoints.size(); i++) {
					drawer->drawHighlight(this, _grid, editingPoints[i], false);
				}
			} else {
				drawer->drawHighlight(this, _grid, editingPoint, false);
			}
		}
	}
}

void FboWarpOutput::update(){
	cellWidth = src.width / gridCols;
	cellHeight = src.height / gridRows;
	
	if(autoLoadGrid && !autoLoadedGrid){
		doLoadGrid();
		editingPoint = 0;
		autoLoadedGrid = true;
		loadGrid = false;
	}
	
	if(saveGrid){
		doSaveGrid();
		saveGrid = false;
	}
	
	if(loadGrid){
		doLoadGrid();
		editingPoint = 0;
		loadGrid = false;
	}
	
	if(editingGrid){
		
		if(clearGrid){
			grid.setup(window, src, dst, gridCols, gridRows);
			updateGUIFromGrid();
			grid.buildGrids(true, true, true);
			gridModified();
		}
		if(fullReset){
			reset();
			updateGUIFromGrid();
		}
		if(grid.rows != gridRows || grid.cols != gridCols){
			// ||!FboWarpGrid::compareRect(dst, grid.dst) || !FboWarpGrid::compareRect(src, grid.src)
			//grid.setup(window, src, dst, gridCols, gridRows);
			
			grid.cols = gridCols;
			grid.rows = gridRows;
			grid.buildGrids(true, true, true);
			
			gridModified();
			editingPoint = 0;
			updateGUIFromGrid();
		}
		
		if(!compareRect(src, grid.src)){
			grid.src = src;
			grid.buildGrids(true, false, false);
			redrawBlending();
			gridModified();
		}
		if(!compareRect(dst, grid.dst)){
			grid.dst = dst;
			grid.buildGrids(false, true, false);
			gridModified();
		}
		
		if(shift != grid.shift ||
		   stretchTL != grid.stretchTL ||
		   stretchTR != grid.stretchTR ||
		   stretchBL != grid.stretchBL ||
		   stretchBR != grid.stretchBR){
			grid.shift = shift;
			grid.stretchTL = stretchTL;
			grid.stretchTR = stretchTR;
			grid.stretchBL = stretchBL;
			grid.stretchBR = stretchBR;
			gridModified();
		}
		
		if(radialAmount != grid.radialAmount || radialInverseAmount != grid.radialInverseAmount ||
		   radialScale != grid.radialScale || radialBias != grid.radialBias ||
		   radialOffsetX != grid.radialCenterOffsetX || radialOffsetY != grid.radialCenterOffsetY){
			
			grid.radialAmount = radialAmount;
			grid.radialInverseAmount = radialInverseAmount;
			grid.radialScale = radialScale;
			grid.radialBias = radialBias;
			grid.radialCenterOffsetX = radialOffsetX;
			grid.radialCenterOffsetY = radialOffsetY;
			
			gridModified();
		}
		
		fullReset = false;
		clearGrid = false;
		
	} else {
		updateGUIFromGrid();
		fullReset = false;
		clearGrid = false;
	}
	
	blending.update();
	
	
	
}

void FboWarpOutput::updateGUIFromGrid(){
	gridRows = grid.rows;
	gridCols = grid.cols;
	src.x = grid.src.x;
	src.y = grid.src.y;
	src.width = grid.src.width;
	src.height = grid.src.height;
	dst.x = grid.dst.x;
	dst.y = grid.dst.y;
	dst.width = grid.dst.width;
	dst.height = grid.dst.height;
	radialAmount = grid.radialAmount;
	radialInverseAmount = grid.radialInverseAmount;
	radialScale = grid.radialScale;
	radialBias = grid.radialBias;
	radialOffsetX = grid.radialCenterOffsetX;
	radialOffsetY = grid.radialCenterOffsetY;
	shift = grid.shift;
	stretchTL = grid.stretchTL;
	stretchTR = grid.stretchTR;
	stretchBL = grid.stretchBL;
	stretchBR = grid.stretchBR;
}

void FboWarpOutput::doLoadGrid(){
	editingGrid = false;
	grid.loadXML(xmlFolderpath+this->name + "_grid.xml");
	updateGUIFromGrid();
	redrawBlending();
	gridModified();
}

void FboWarpOutput::doSaveGrid(){
	//ofLog(OF_LOG_VERBOSE, "FboWarpOutput::doSaveGrid");
	grid.saveXML(xmlFolderpath+this->name + "_grid.xml");
}

void FboWarpOutput::gridModified(){
	_grid.copyFrom(grid);
	redraw();
}

void FboWarpOutput::redraw(){
	drawer->buildGrids();
}

void FboWarpOutput::redrawBlending(){
	ofRectangle blendRect = src;
	blending.redraw(blendRect);
}

void FboWarpOutput::keyPressed(ofKeyEventArgs& e){
	keyPressed(e.key);
}

void FboWarpOutput::keyPressed(int key){
	
	if(editingGrid && keyboardEditingEnabled){
		switch(key) {
				
				// Move
			case 'w':
				changeSelection(0, -1);
				break;
			case 's':
				changeSelection(0, 1);
				break;
			case 'a':
				changeSelection(-1, 0);
				break;
			case 'd':
				changeSelection(1, 0);
				break;
				
				// Scale
			case 'p':
				scaleSelection(0.1);
				break;
			case 'l':
				scaleSelection(-0.1);
				break;
			case 'P':
				scaleSelection(1);
				break;
			case 'L':
				scaleSelection(-1);
				break;
				
				// Shift
			case OF_KEY_UP:
				shiftSelection(0, -1);
				break;	
			case OF_KEY_DOWN:
				shiftSelection(0, 1);
				break;
			case OF_KEY_LEFT:
				shiftSelection(-1, 0);
				break;
			case OF_KEY_RIGHT:
				shiftSelection(1, 0);
				break;
		}
	}
}

void FboWarpOutput::scaleSelection(float incrementPx){
	
	if (editMode == WARP_EDIT_COLUMN) {
		
		if (editingPoints.size() > 1) {
			
			float centre = (editingPoints.size() - 1) * 0.5;
			for (int i=0; i<editingPoints.size(); i++) {
				if(grid.isValidPoint(editingPoints[i])){
					ofxVec2f& point = grid.warpGrid[editingPoints[i]];
					int indexFromCentre = i - centre;
					point.y += indexFromCentre * incrementPx;
				}
			}
			gridModified();
			
		}
		
	} else if(editMode == WARP_EDIT_ROW){
		
		if (editingPoints.size() > 1) {
			
			float centre = (editingPoints.size() - 1) * 0.5;
			for (int i=0; i<editingPoints.size(); i++) {
				if(grid.isValidPoint(editingPoints[i])){
					ofxVec2f& point = grid.warpGrid[editingPoints[i]];
					int indexFromCentre = i - centre;
					point.x += indexFromCentre * incrementPx;
				}
			}
			gridModified();
			
		}
		
	}
	
}

void FboWarpOutput::shiftSelection(float dx, float dy){
	if (editMode == WARP_EDIT_COLUMN || editMode == WARP_EDIT_ROW) {
		// Shift all points in column/row
		for (uint i = 0; i < editingPoints.size(); i++) {
			if(grid.isValidPoint(editingPoints[i])){
				ofxVec2f& point = grid.warpGrid[editingPoints[i]];
				point.x += dx;
				point.y += dy;
			}
		}
		gridModified();
	} else {
		if(grid.isValidPoint(editingPoint)){
			ofxVec2f& point = grid.warpGrid[editingPoint];
			point.x += dx;
			point.y += dy;
			gridModified();
		}
	}
}

void FboWarpOutput::changeSelection(int dx, int dy){
	int newPoint;
	editingPoints.clear();
	// switch on editMode
	switch (editMode) {
		case WARP_EDIT_COLUMN:
			// select all points in column
			newPoint = editingPoint + dx + (_grid.cols+1);
			newPoint = newPoint % (_grid.cols+1);
			for (uint i = newPoint; i < _grid.warpGrid.size(); i += _grid.cols+1) {
				editingPoints.push_back(i);
			}
			break;
		case WARP_EDIT_ROW:
			// select all points in row
			newPoint = editingPoint + (_grid.cols+1)*dy + _grid.warpGrid.size();
			newPoint = newPoint % _grid.warpGrid.size();
			newPoint = floor( (newPoint / _grid.cols) ) * (_grid.cols+1);
			for (uint i = newPoint; i < newPoint + _grid.cols + 1; i++) {
				editingPoints.push_back(i);
			}
			break;
		default:
			// Set editing point
			newPoint = editingPoint + dx + (_grid.cols+1)*dy + (grid.warpGrid.size());
			newPoint = newPoint % _grid.warpGrid.size();
			break;
	}
	editingPoint = newPoint;
	
}

void FboWarpOutput::setupGUI(){
	gui.page(parentName).addPageShortcut(gui.addPage(name));
	
	gui.addTitle("Presets");
	gui.addToggle("autoload", autoLoadGrid);
	gui.addToggle("save", saveGrid);
	gui.addToggle("load", loadGrid);
	gui.addDebug("xml", xmlFolderpath);
	
	gui.addTitle("Drawing");
	gui.addToggle("draw output", drawOutput);
	//gui.addToggle("draw outer grid", drawOuterGrid);
	string gridChoices[3] = {"disabled","lines","points"};
	gui.addComboBox("draw outer grid", drawOuterGrid, 3, gridChoices);
	gui.addToggle("draw inner grid", drawInnerGrid);
	gui.addToggle("draw grid inside fbo", drawHudInsideFbo);
	gui.addToggle("draw blank", drawBlank);
	
	
	gui.addTitle("Editing");
	gui.addToggle("editing enabled", editingGrid);
	gui.addToggle("keyboard enabled", keyboardEditingEnabled);
	gui.addToggle("full reset", fullReset);
	
	string editModeNames[WARP_EDIT_COUNT];
	editModeNames[WARP_EDIT_POINT] = "point";
	editModeNames[WARP_EDIT_COLUMN] = "column";
	editModeNames[WARP_EDIT_ROW] = "row";
	gui.addComboBox("keyboard editing mode", editMode, WARP_EDIT_COUNT, editModeNames);
	
	gui.addTitle("grid");
	gui.addSlider("columns", gridCols, 1, 128);
	gui.addSlider("rows", gridRows, 1, 128);
	gui.addDebug("cell width", cellWidth);
	gui.addDebug("cell height", cellHeight);
	gui.addToggle("clear grid", clearGrid);
	
	gui.addTitle("source rectangle");
	gui.addSlider("source x", src.x, 0, drawer->fboWidth);
	gui.addSlider("source y", src.y, 0, drawer->fboHeight);
	gui.addSlider("source width", src.width, 0, drawer->fboWidth);
	gui.addSlider("source height", src.height, 0, drawer->fboHeight);
	
	//gui.addSlider("shiftX", shift.x, -drawer->fboWidth*0.125, drawer->fboWidth*0.125);
	//gui.addSlider("shiftY", shift.y, -drawer->fboHeight*0.125, drawer->fboHeight*0.125);
	
	gui.addTitle("destination rectangle");
	gui.addSlider("destination x", dst.x, -3000, 3000);
	gui.addSlider("destination y", dst.y, -1200, 1200);
	gui.addSlider("destination width", dst.width, 0, drawer->fboWidth*2);
	gui.addSlider("destination height", dst.height, 0, drawer->fboHeight*2);
	
	gui.addTitle("destination corner offset");
	float stretchRangeX = drawer->fboWidth*0.125;
	float stretchRangeY = drawer->fboHeight*0.125;
	gui.addSlider("top left x", stretchTL.x, -stretchRangeX, stretchRangeX);
	gui.addSlider("top left y", stretchTL.y, -stretchRangeY, stretchRangeY);
	gui.addSlider("top right x", stretchTR.x, -stretchRangeX, stretchRangeX);
	gui.addSlider("top right y", stretchTR.y, -stretchRangeY, stretchRangeY);
	gui.addSlider("bottom left x", stretchBR.x, -stretchRangeX, stretchRangeX);
	gui.addSlider("bottom left y", stretchBR.y, -stretchRangeY, stretchRangeY);
	gui.addSlider("bottom right x", stretchBL.x, -stretchRangeX, stretchRangeX);
	gui.addSlider("bottom right y", stretchBL.y, -stretchRangeY, stretchRangeY);
	
	gui.addTitle("radial distortion");
	gui.addSlider("radial offset x", radialOffsetX, -0.5, 0.5);
	gui.addSlider("radial offset y", radialOffsetY, -0.5, 0.5);
	gui.addSlider("radial amount", radialAmount, -1, 1);
	gui.addSlider("radial inverse amount", radialInverseAmount, -1, 1);
	gui.addSlider("radial scale", radialScale, 0, 2);
	gui.addSlider("radial bias", radialBias, -1, 1);
	
	
	
	gui.addTitle("colors");
	gui.addColorPicker("outer grid",&outerGridColor.r,true,true);
	gui.addColorPicker("inner grid",&innerGridColor.r,true,true);
	gui.addColorPicker("blank",&blankColor.r,true,true);
	
	gui.addTitle("highlights");
	gui.addColorPicker("highlight color",&highlightColor.r,true,true);
	gui.addSlider("highlight point size", highlightPointSize,1,4);
	gui.addSlider("highlight radius", highlightRadius,1,64);
	gui.addToggle("highlight circle", highlightCircle);
	gui.addToggle("highlight  ", highlightCrossHair);
	drawer->setupGUI(name);
	
	gui.addTitle("levels 1");
	gui.addSlider("min input 1", levels0.minInput, 0, 1);
	gui.addSlider("max input 1", levels0.maxInput, 0, 1);
	gui.addSlider("min output 1", levels0.minOutput, 0, 1);
	gui.addSlider("max output 1", levels0.maxOutput, 0, 1);
	gui.addSlider("gamma 1", levels0.gamma, 0, 1);
	gui.addToggle("use RGB 1", levels0.useRGB);
	gui.addColorPicker("gamma RGB 1", &levels0.gammaRGB.r, true, false);
	
	gui.addTitle("levels 2");
	gui.addSlider("min input 2", levels2.minInput, 0, 1);
	gui.addSlider("max input 2", levels2.maxInput, 0, 1);
	gui.addSlider("min output 2", levels2.minOutput, 0, 1);
	gui.addSlider("max output 2", levels2.maxOutput, 0, 1);
	gui.addSlider("gamma 2", levels2.gamma, 0, 1);
	gui.addToggle("use RGB 2", levels2.useRGB);
	gui.addColorPicker("gamma RGB 2", &levels2.gammaRGB.r, true, false);
	
	
	blending.setupGUI(name);
	
	gui.addColorPicker("edge color", &edgeColor.r, false, true);
	
}

// For external inputs ////////////////////////////////////////////////////////////////

void FboWarpOutput::increaseDstX(float scale = 1){
	dst.x += scale;
}
void FboWarpOutput::decreaseDstX(float scale = 1){
	dst.x -= scale;
}
void FboWarpOutput::increaseDstY(float scale = 1){
	dst.y += scale;
}
void FboWarpOutput::decreaseDstY(float scale = 1){
	dst.y -= scale;
}

void FboWarpOutput::increaseDstWidth(float scale = 1){
	dst.width += scale;
}
void FboWarpOutput::decreaseDstWidth(float scale = 1){
	dst.width -= scale;
}
void FboWarpOutput::increaseDstHeight(float scale = 1){
	dst.height += scale;
}
void FboWarpOutput::decreaseDstHeight(float scale = 1){
	dst.height -= scale;
}

void FboWarpOutput::increaseSrcX(float scale = 1){
	src.x += scale;
}
void FboWarpOutput::increaseSrcY(float scale = 1){
	src.y += scale;
}
void FboWarpOutput::increaseSrcWidth(float scale = 1){
	src.width += scale;
}
void FboWarpOutput::increaseSrcHeight(float scale = 1){
	src.height += scale;
}

void FboWarpOutput::decreaseSrcX(float scale = 1){
	src.x -= scale;
}
void FboWarpOutput::decreaseSrcY(float scale = 1){
	src.y -= scale;
}
void FboWarpOutput::decreaseSrcWidth(float scale = 1){
	src.width -= scale;
}
void FboWarpOutput::decreaseSrcHeight(float scale = 1){
	src.height -= scale;
}