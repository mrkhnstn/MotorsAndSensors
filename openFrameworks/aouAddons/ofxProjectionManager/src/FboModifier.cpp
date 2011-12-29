/*
 *  FboModifier.cpp
 *  TestBed_Chris
 *
 *  Created by TAQA_CH_1 on 10/03/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "FboModifier.h"
#include "ofxProjectionManager.h"
#include "FboBlending.h"

FboModifier::FboModifier(){
	output = NULL;
}

string FboModifier::getHUDString(){
	return "-";
}

// FboDrawModifier ///////////////////////////////////////

void FboDrawModifier::leftUp(float scale, bool analog){
	output->blending.top.enabled = !output->blending.top.enabled;
}

void FboDrawModifier::leftDown(float scale, bool analog){
	output->blending.bottom.enabled = !output->blending.bottom.enabled;
}

void FboDrawModifier::leftLeft(float scale, bool analog){
	output->blending.left.enabled = !output->blending.left.enabled;
}

void FboDrawModifier::leftRight(float scale, bool analog){
	output->blending.right.enabled = !output->blending.right.enabled;
}

void FboDrawModifier::rightUp(float scale, bool analog){
}

void FboDrawModifier::rightDown(float scale, bool analog){
	output->drawInnerGrid = !output->drawInnerGrid;
}

void FboDrawModifier::rightLeft(float scale, bool analog){
	output->drawBlank = !output->drawBlank;
}

void FboDrawModifier::rightRight(float scale, bool analog){
	if (output->drawOuterGrid == 2) {
		output->drawOuterGrid = 0;
	} else {
		output->drawOuterGrid++;
	}
}

string FboDrawModifier::getHUDString(){
	string debugString = "Draw";
	debugString += "\nD-PAD : toggle edges";
	debugString += "\nB     : toggle grid";
	debugString += "\nX     : toggle blank color";
	return debugString;
}

// HudTextModifier /////////////////////////////////////////

void HudTextModifier::leftUp(float scale, bool analog){
	projectionManager->hudPos.y -= scale * 10;
}

void HudTextModifier::leftDown(float scale, bool analog){
	projectionManager->hudPos.y += scale * 10;
}

void HudTextModifier::leftLeft(float scale, bool analog){
	projectionManager->hudPos.x -= scale * 10;
}

void HudTextModifier::leftRight(float scale, bool analog){
	projectionManager->hudPos.x += scale * 10;
}

void HudTextModifier::rightUp(float scale, bool analog){
}

void HudTextModifier::rightDown(float scale, bool analog){
}

void HudTextModifier::rightLeft(float scale, bool analog){
	projectionManager->hudScale -= scale;
	if (projectionManager->hudScale < 0.2) {
		projectionManager->hudScale = 0.2;
	}
}

void HudTextModifier::rightRight(float scale, bool analog){
	projectionManager->hudScale += scale;
}

string HudTextModifier::getHUDString(){
	return "Text";
}

// EdgeBlendModifier /////////////////////////////////////////

EdgeBlendModifier::EdgeBlendModifier() : FboModifier(){
	
	edgeId = 0;
	functionId = 0;
	blendPointScale = 0.05;
	shapeScale = 0.25;
	
	edgeNames[0] = "TOP";
	edgeNames[1] = "BOTTOM";
	edgeNames[2] = "LEFT";
	edgeNames[3] = "RIGHT";
	
	functionNames[0] = "width";
	functionNames[1] = "blend point";
	functionNames[2] = "shape";
	functionNames[3] = "enabled";
}

void EdgeBlendModifier::updateEdgeSelection(){
	
	switch (edgeId) {
		case 0:
			edge = &output->blending.top;
			break;
		case 1:
			edge = &output->blending.bottom;
			break;
		case 2:
			edge = &output->blending.left;
			break;
		case 3:
			edge = &output->blending.right;
			break;
		default:
			break;
	}
	
}

void EdgeBlendModifier::leftUp(float scale, bool analog){
	edgeId = 0;
}

void EdgeBlendModifier::leftDown(float scale, bool analog){
	edgeId = 1;
}

void EdgeBlendModifier::leftLeft(float scale, bool analog){
	edgeId = 2;
}

void EdgeBlendModifier::leftRight(float scale, bool analog){
	edgeId = 3;
}

void EdgeBlendModifier::rightUp(float scale, bool analog){
	functionId--;
	if (functionId < 0) {
		functionId = 3;
	}
}

void EdgeBlendModifier::rightDown(float scale, bool analog){
	functionId++;
	if (functionId == 4) {
		functionId = 0;
	}
}

void EdgeBlendModifier::rightLeft(float scale, bool analog){
	/*
	functionNames[0] = "width";
	functionNames[1] = "blend point";
	functionNames[2] = "shape";
	functionNames[3] = "enable/disable";
	*/
	
	updateEdgeSelection();
	
	switch (functionId) {
		case 0:
			edge->width -= scale;
			if (edge->width < 0) {
				edge->width = 0;
			}
			break;
		case 1:
			edge->blendPoint = ofClamp(edge->blendPoint - scale * blendPointScale, 0, 1);
			break;
		case 2:
			edge->shape = ofClamp(edge->shape - scale * shapeScale, 1, 10);
			break;
		case 3: // enabled
			edge->enabled = !edge->enabled;
			break;
		default:
			break;
	}
}

void EdgeBlendModifier::rightRight(float scale, bool analog){
	updateEdgeSelection();
	switch (functionId) {
		case 0:
			edge->width += scale;
			if (edge->width > output->src.width) {
				edge->width = output->src.width;
			}
			break;
		case 1:
			edge->blendPoint = ofClamp(edge->blendPoint + scale * blendPointScale, 0, 1);			
			break;
		case 2:
			edge->shape = ofClamp(edge->shape + scale * shapeScale, 1, 10);
			break;
		case 3:
			edge->enabled = !edge->enabled;
			break;
		default:
			break;
	}
}

string EdgeBlendModifier::getHUDString(){
	updateEdgeSelection();
	string debugString = "Edge: " + edgeNames[edgeId];
	debugString += "\n" + functionNames[functionId] + ": ";
	switch (functionId) {
		case 0:
			debugString += ofToString(edge->width,2);
			break;
		case 1:
			debugString += ofToString(edge->blendPoint,2);
			break;
		case 2:
			debugString += ofToString(edge->shape,2);
			break;
		case 3:
			debugString += edge->enabled ? "true" : "false";
			break;
		default:
			break;
	}
	debugString += "\n\nD-Pad: select edge\nY/A: select parameter\nX/B: adjust parameter";
	return debugString;
}