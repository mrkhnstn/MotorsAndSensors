/*
 *  TestPattern.cpp
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 31/03/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "TestPattern.h"

void TestPattern::setup(){
	doDraw = false;
	doDrawNumbers = true;
	doDrawBlank = false;
	cols = 10;
	cols = 10;
	ellipseCount = 4;
}

void TestPattern::setupGUI(){
	gui.page(1).addPageShortcut(gui.addPage("Test Pattern"));
	gui.addToggle("draw", doDraw);
	gui.addSlider("columns", cols, 1, 256);
	gui.addSlider("rows", rows, 1, 256);
	gui.addSlider("ellipseCount", ellipseCount, 0, 32);
	gui.addSlider("line width", lineWidth, 1, 4);
	gui.addColorPicker("color", &color.r, false, true);
	gui.addSlider("width", boundary.width, 0, 1920 * 2);
	gui.addSlider("height", boundary.height, 0, 1920 * 2);
	gui.addDebug("col size", colSize);
	gui.addDebug("row size", rowSize);
	gui.addToggle("draw numbers", doDrawNumbers);
	gui.addToggle("draw blank", doDrawBlank);
	gui.addColorPicker("blank color", &blankColor.r, false, true);
}

void TestPattern::postGUI(){
	doDraw = false;
}

void TestPattern::update(){
	
}

void TestPattern::draw(){
	if(!doDraw) return;
	
	ofPushStyle();
	
	// draw blank background
	if(doDrawBlank){
		ofEnableAlphaBlending();
		ofSetColor(blankColor.r, blankColor.g, blankColor.b, blankColor.a);
		ofFill();
		ofRect(boundary.x,boundary.y,boundary.width,boundary.height);
	}
	
	ofSetColor(color.r, color.g, color.a);
	ofSetLineWidth(lineWidth);
	ofNoFill();
	
	colSize = boundary.width / cols;
	rowSize = boundary.height / rows;
	
	// draw horizontal
	for (int i=0; i<rows; i++) {
		float y = i * rowSize;
		ofLine(boundary.x, y, boundary.x + boundary.width, y);
	}
	
	// draw vertical
	for (int i=0; i<cols; i++) {
		float x = i * colSize;
		ofLine(x, boundary.y, x, boundary.y + boundary.height);
	}		
	
	// draw ellipse
	if(ellipseCount > 0){
		
		float ellipseWidthSeg = boundary.width * 0.5 / ellipseCount;
		float ellipseHeightSeg = boundary.height * 0.5 / ellipseCount;
		
		for(int i=0; i<ellipseCount; i++){
			ofEllipse(boundary.x + boundary.width * 0.5, boundary.y  + boundary.height * 0.5, ellipseWidthSeg * (i+1) * 2, ellipseHeightSeg * (i+1) * 2);
		}
	}
	
	// draw diagonals
	ofLine(boundary.x, boundary.y, boundary.x+boundary.width, boundary.y+boundary.height);
	ofLine(boundary.x, boundary.y+boundary.height, boundary.x+boundary.width, boundary.y);
	
	// draw numbers
	if(doDrawNumbers)
		for (int col=0; col<cols; col++) {
			for(int row=0; row<rows; row++){
				float y = row * rowSize;
				float x = col * colSize;
				ofDrawBitmapString(ofToString(col)+","+ofToString(row), x+5, y+18);
			}
		}
	
	ofPopStyle();
	
}
