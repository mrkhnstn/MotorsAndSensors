/*
 *  ofxProjectionManager.cpp
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 06/02/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxProjectionManager.h"

#define SCALE_MODE_NONE				0
#define SCALE_MODE_STRETCH			1
#define SCALE_MODE_FIT_HORIZONTAL	2
#define SCALE_MODE_FIT_VERTICAL		3
#define SCALE_MODE_OUTPUTS			4
#define SCALE_MODE_COUNT			5

ofxShader ofxProjectionManager::shader;
bool ofxProjectionManager::shaderIsSetup = false;

ofTrueTypeFont ofxProjectionManager::hudFont;

ofxProjectionManager::ofxProjectionManager(){
	
	name = "Projection Manager";
	scaleMode = SCALE_MODE_NONE;
	backgroundImageSetTime = 0;
	drawerRecursionPrecision = 1;
	drawerMaxRecursion = 4;
	
	editByKeyScaleNormal = 0.1;
	editByKeyScaleShift = 1;
	
	mode = 0;//EDIT_MODE_DEST_TRANSFORM;
	activeModifier = &dstTransformModifier;
	outputIndex = 0;
	doEditByKey = false;
	doDrawInfoOverlay = false;
	
	hudScale = 1;
	hudPos.x = 10;
	hudPos.y = 10;
	
	hudTextModifier.projectionManager = this;
	
	drawHUDInsideFbo = true;
	
	//modifiers.reserve(EDIT_MODE_COUNT);
	modifiers.push_back(&dstTransformModifier);
	modifiers.push_back(&dstPointModifier);
	modifiers.push_back(&dstColModifier);
	modifiers.push_back(&dstRowModifier);
	modifiers.push_back(&srcTransformModifier);
	modifiers.push_back(&drawModifier);
	modifiers.push_back(&hudTextModifier);
	modifiers.push_back(&edgeBlendModifier);
}

ofxProjectionManager::~ofxProjectionManager(){
	for (int i=0; i<outputs.size(); i++) {
		delete outputs[i];
	}
	outputs.clear();
	shader.unload();
}

void ofxProjectionManager::setup(int width, int height){
	
	this->width = width;
	this->height = height;
	drawWidth = width;
	drawWidth = height;
	
	if (!shaderIsSetup) {
		shader.loadShader("ofxProjectionManager/rgbMultiplyMask");
		shaderIsSetup = true;
	}
	
	if (!hudFont.bLoadedOk) {
		hudFont.loadFont("ofxProjectionManager/FUTURAB.TTF", 12, true, false, true);
	}
	
	drawer.setup(width, height, shader);
	
	ofAddListener(ofEvents.keyPressed, this, &ofxProjectionManager::keyPressed);
	
}

void ofxProjectionManager::setupGUI(){
	gui.page(1).addPageShortcut(gui.addPage(name));
	
	// add scale mode control
	string choiceNames[SCALE_MODE_COUNT];
	choiceNames[SCALE_MODE_NONE] = "none";
	choiceNames[SCALE_MODE_STRETCH] = "stretch";
	choiceNames[SCALE_MODE_FIT_HORIZONTAL] = "fit horizontal";
	choiceNames[SCALE_MODE_FIT_VERTICAL] = "fit vertical";
	choiceNames[SCALE_MODE_OUTPUTS] = "outputs";
	gui.addComboBox("Scale Mode",scaleMode,SCALE_MODE_COUNT,choiceNames);
	gui.addColorPicker("Blend Color", &blendColor.r,true,true);
	
	gui.addTitle("Drawing");
	gui.addSlider("recursion precision", drawerRecursionPrecision, 0.1, 4);
	gui.addSlider("max recursion", drawerMaxRecursion, 0, 6);
	gui.addSlider("draw layer", drawLayer, 0, 10);
	
	gui.addTitle("Key Editing");
	gui.addToggle("edit by key", doEditByKey);
	gui.addSlider("key value normal", editByKeyScaleNormal, 0.01,1);
	gui.addSlider("key value shift", editByKeyScaleShift, 0.01,10);

	gui.addToggle("show info", doDrawInfoOverlay);
	gui.addSlider("hud text scale", hudScale, 1, 5);
	gui.addSlider("hud text x", hudPos.x, 0, 1920);
	gui.addSlider("hud text y", hudPos.y, 0, 1200);
	gui.addToggle("hud inside fbo", drawHUDInsideFbo);
	
	
	gui.addTitle("Outputs");
	for(int i = 0; i < outputs.size(); ++i){
		FboWarpOutput* output = outputs[i];
		output->parentName = name;
		output->drawer->parentName = name;
		output->blending.parentName = name;
		output->setupGUI();
	}
	
		
}

void ofxProjectionManager::postGUI(){
	doEditByKey = false;
	doDrawInfoOverlay = false;


		for(int i = 0; i < outputs.size(); ++i){
			FboWarpOutput* output = outputs[i];
			output->editingGrid = false;
			output->keyboardEditingEnabled = false;
		}
	
	drawer.maxRecursion = drawerMaxRecursion;
	drawer.perspectiveCorrectionPrecision = drawerRecursionPrecision;
	redraw();
}

void ofxProjectionManager::draw(){
	
	// setup initial for SCALE_MODE_NONE
	
	switch (scaleMode) {
		case SCALE_MODE_NONE:	
			drawWidth= width;
			drawHeight = height;
			x = 0;
			y = 0;			
			drawFbo(x, y, drawWidth, drawHeight);
			break;
		case SCALE_MODE_STRETCH:
			x = 0;
			y = 0;
			drawWidth = ofGetWidth();
			drawHeight = ofGetHeight();
			drawFbo(x, y, drawWidth, drawHeight);
			break;
		case SCALE_MODE_FIT_HORIZONTAL:
			drawWidth = ofGetWidth();
			drawHeight = ofGetWidth() / width * height;
			x = 0;
			y = (ofGetHeight() - drawHeight) * 0.5;		
			drawFbo(x, y, drawWidth, drawHeight);
			break;
		case SCALE_MODE_FIT_VERTICAL:
			drawWidth = ofGetHeight() / height * width;
			drawHeight = ofGetHeight();
			x = (ofGetWidth() - drawWidth) * 0.5;
			y = 0;	
			drawFbo(x, y, drawWidth, drawHeight);
			break;
		case SCALE_MODE_OUTPUTS:
			drawOutputs();
			break;
		default:
			break;
	}
	
		
}

void ofxProjectionManager::update(){
	for(int i = 0; i < outputs.size(); ++i){
		FboWarpOutput* output = outputs[i];
		output->update();
	}
	
	if(drawerMaxRecursion != drawer.maxRecursion || drawerRecursionPrecision != drawer.perspectiveCorrectionPrecision){
		drawer.maxRecursion = drawerMaxRecursion;
		drawer.perspectiveCorrectionPrecision = drawerRecursionPrecision;
		redraw();
	}
}

void ofxProjectionManager::begin(){
	drawer.swapIn(true);
}

void ofxProjectionManager::end(){
	drawer.swapOut();
}

// Drawing and output ////////////////////////////

void ofxProjectionManager::addOutput(string suffix = "A", string xmlFolderpath = ""){
	FboWarpOutput* output = new FboWarpOutput(xmlFolderpath);
	output->id = outputs.size();
	outputs.add(suffix, output);
	output->setup(&drawer, suffix);
	redraw();//make sure we rebuild the grid using this new output
}

void ofxProjectionManager::setOutputSourceRect(string suffix, float x, float y, float width, float height){
	if(outputs.hasKey(suffix)){
		FboWarpOutput* output = outputs[suffix];
		output->src.x = x;
		output->src.y = y;
		output->src.width = width;
		output->src.height = height;
	}
}

void ofxProjectionManager::registerBackgroundImage(ofImage* background){
	for(int i = 0; i < outputs.size(); ++i){
		FboWarpOutput* output = outputs[i];
		output->blending.setBackgroundImage(background);
	}
}

void ofxProjectionManager::updateBackgroundImage(){
	backgroundImageSetTime = ofGetElapsedTimeMillis();
	for(int i = 0; i < outputs.size(); ++i){
		FboWarpOutput* output = outputs[i];
		output->blending.backgroundUpdated();
	}
}

void ofxProjectionManager::redraw(){
	drawer.buildGrids();
	// update the current output
	output = outputs[outputIndex];
	updateMode();
	updateOutputs();
}

void ofxProjectionManager::drawFbo(float x, float y, float width, float height){
	// draw fbo to main screen without warping
	glDisable(GL_BLEND);
	glColor4f(blendColor.r, blendColor.g, blendColor.b, 1);
	getTexture().draw(x, y,width,height);
}

void ofxProjectionManager::drawOutputHUD(FboWarpOutput* output ){
	
	output->drawer->swapIn(false);
	
	ofPushStyle();
	ofPushMatrix();
	
	ofTranslate(output->src.x+10, output->src.y+10, 0);
	ofSetColor(255, 255, 255);

	string debugString = "Output: " + output->name + "\n";
	if (activeModifier != NULL) {
		activeModifier->output = output;
	}
	debugString = activeModifier->getHUDString();
	ofPushMatrix();
	
	ofTranslate(hudPos.x, hudPos.y, 0);
	ofScale(hudScale, hudScale, 0);
	hudFont.drawStringAsShapes(debugString, 10, 20);
	//hudFont.drawString(debugString, 10, 20);
	//ofDrawBitmapString(debugString, 10, 20);
	ofPopMatrix();
	
	ofPopMatrix();
	ofPopStyle();
	
		// draw selection inline
		ofPushStyle();
		ofNoFill();
		ofSetLineWidth(3); //< TODO: gui parameter
		ofEnableAlphaBlending();
		ofSetColor(255, 255, 255,128); //< TODO: gui parameter
		ofRect(output->src.x+10, output->src.y+10, output->src.width-20, output->src.height-20);
		ofPopStyle();
		
		output->drawer->swapOut();
	
}

void ofxProjectionManager::drawOutputHUDOutside(FboWarpOutput* output ){
	
	ofPushStyle();
	ofPushMatrix();
	
	//ofTranslate(gui.displayRect.x, gui.displayRect.y, 0);
	ofTranslate(hudPos.x, hudPos.y, 0);
	//ofTranslate(output->src.x+10, output->src.y+10, 0);
	ofSetColor(255, 255, 255);
	
	string debugString = "Output: " + output->name + "\n";
	if (activeModifier != NULL) {
		activeModifier->output = output;
	}
	debugString = activeModifier->getHUDString();
	ofPushMatrix();
	
	
	
	
	ofScale(hudScale, hudScale, 0);
	hudFont.drawStringAsShapes(debugString, 10, 20);
	//hudFont.drawString(debugString, 10, 20);
	//ofDrawBitmapString(debugString, 10, 20);
	ofPopMatrix();
	
	ofPopMatrix();
	ofPopStyle();
	
}

void ofxProjectionManager::drawOutputs(){
	
	for(int i = 0; i < outputs.size(); ++i){
		FboWarpOutput* output = outputs[i];
		output->draw(); // draw internal grids / highlights
		
		if (outputIndex == i && doDrawInfoOverlay && drawHUDInsideFbo) {
			drawOutputHUD(output); //< draws the hud when using non gui control
		}
	}	
	
	glColor4f(blendColor.r, blendColor.g, blendColor.b, 1);
	for(int i = 0; i < outputs.size(); ++i){
		FboWarpOutput* output = outputs[i];
		if(output->drawOutput)
			drawer.drawSub(i,output); //< this draws the actual warped fbo
	}
	
	for(int i = 0; i < outputs.size(); ++i){
		FboWarpOutput* output = outputs[i];
		output->drawHUD();
		//if (outputIndex == i && doDrawInfoOverlay && !drawHUDInsideFbo) {
			//drawOutputHUDOutside(output); //< draws the hud when using non gui control
		//}
	}

}

ofxFBOTexture& ofxProjectionManager::getTexture(){
	return drawer.getTexture();
}

void ofxProjectionManager::enableEditing(bool enabled){
	doEditByKey = enabled;
	doDrawInfoOverlay = enabled;
	output->editingGrid = enabled;
	output->keyboardEditingEnabled = enabled;
	if (!enabled) {
		for(int i = 0; i < outputs.size(); ++i){
			FboWarpOutput* output = outputs[i];
			output->doSaveGrid();
		}
	}
}

// Input ///////////////////////////////////////////////////////////////////////

void ofxProjectionManager::keyPressed(ofKeyEventArgs& e){
	if(doEditByKey) keyPressed(e.key);
}

void ofxProjectionManager::keyPressed(int key){
	
	float amount = (checkShiftKey()) ? editByKeyScaleShift : editByKeyScaleNormal;
	
	switch(key) {
			
			// Change edit mode
		case 'z':
			previousMode();
			break;
		case 'x':
			nextMode();
			break;
		case 'c':
		    nextOutput();
			break;
		case 'v':
			prevOutput();
			break;
			// Left Arrows
		case 'w':
			leftUp(amount);
			break;
		case 's':
			leftDown(amount);
			break;
		case 'a':
			leftLeft(amount);
			break;
		case 'd':
			leftRight(amount);
			break;
			
			// Right Arrows
		case OF_KEY_UP:
			rightUp(amount);
			break;	
		case OF_KEY_DOWN:
			rightDown(amount);
			break;
		case OF_KEY_LEFT:
			rightLeft(amount);
			break;
		case OF_KEY_RIGHT:
			rightRight(amount);
			break;
	}
}

void ofxProjectionManager::nextMode(){
	mode++;
	//if (mode >= EDIT_MODE_COUNT) mode = 0;
	if (mode >= modifiers.size()) mode = 0;
	updateMode();
}

void ofxProjectionManager::previousMode(){
	mode--;
	if (mode < 0) mode = modifiers.size() - 1;
	updateMode();
}

void ofxProjectionManager::updateMode(){
	
	if(doEditByKey) {
		
		activeModifier = modifiers[mode];
		
		/*
		switch (mode) {
			case EDIT_MODE_DEST_TRANSFORM:
				activeModifier = &dstTransformModifier;
				break;
			case EDIT_MODE_SRC_TRANSFORM:
				activeModifier = &srcTransformModifier;
				break;
			case EDIT_MODE_WARP_COLUMN:
				activeModifier = &dstColModifier;
				output->editMode = WARP_EDIT_COLUMN;
				break;
			case EDIT_MODE_WARP_ROW:
				activeModifier = &dstRowModifier;
				output->editMode = WARP_EDIT_ROW;
				break;
			case EDIT_MODE_WARP_POINT:
				activeModifier = &dstPointModifier;
				output->editMode = WARP_EDIT_POINT;
				break;
			case EDIT_MODE_DRAW:
				activeModifier = &drawModifier;
				//output->editMode = WARP_EDIT_POINT;
				break;
			case EDIT_MODE_HUD_TEXT:
				activeModifier = &hudTextModifier;
				break;
			/*
			case EDIT_MODE_SCALE_COLUMN:
				activeModifier = &dstPointScaleModifier;
				output->editMode = WARP_EDIT_COLUMN;
				break;
			case EDIT_MODE_SCALE_ROW:
				activeModifier = &dstPointScaleModifier;
				output->editMode = WARP_EDIT_ROW;
				break;
			*/	
		/*
			default:
				activeModifier = NULL;
				break;
		}
		*/
	}
}

void ofxProjectionManager::updateOutputs(){
	if(doEditByKey) {
		for(int i = 0; i < outputs.size(); ++i){
			FboWarpOutput* tOutput = outputs[i];
			tOutput->editingGrid = false;
			tOutput->keyboardEditingEnabled = false;
		}
		output->editingGrid = true;
		output->keyboardEditingEnabled = true;
	}
}

// Key/Button action functions ////////////////////////////////////////////

void ofxProjectionManager::leftUp(float scale, bool analog){
	if (activeModifier != NULL) {
		activeModifier->output = output;
		activeModifier->leftUp(scale,analog);
	}
}

void ofxProjectionManager::leftDown(float scale, bool analog){
	if (activeModifier != NULL) {
		activeModifier->output = output;
		activeModifier->leftDown(scale,analog);
	}
}

void ofxProjectionManager::leftLeft(float scale, bool analog){
	if (activeModifier != NULL) { 
		activeModifier->output = output;
		activeModifier->leftLeft(scale,analog);
	}
}

void ofxProjectionManager::leftRight(float scale, bool analog){
	if (activeModifier != NULL) {
		activeModifier->output = output;
		activeModifier->leftRight(scale,analog);
	}
}

void ofxProjectionManager::rightUp(float scale, bool analog){
	if (activeModifier != NULL) {
		activeModifier->output = output;
		activeModifier->rightUp(scale,analog);
	}
}

void ofxProjectionManager::rightDown(float scale, bool analog){
	if (activeModifier != NULL) {
		activeModifier->output = output;
		activeModifier->rightDown(scale,analog);
	}
}

void ofxProjectionManager::rightLeft(float scale, bool analog){
	if (activeModifier != NULL) {
		activeModifier->output = output;
		activeModifier->rightLeft(scale,analog);
	}
}

void ofxProjectionManager::rightRight(float scale, bool analog){
	if (activeModifier != NULL) {
		activeModifier->output = output;
		activeModifier->rightRight(scale,analog);
	}
}

void ofxProjectionManager::setOutput(int i){
	outputIndex = ofClamp(i, 0, outputs.size()-1);
	output = outputs[outputIndex];
	updateOutputs();
}

void ofxProjectionManager::nextOutput(){
	// target next warp output
	outputIndex++;
	if (outputIndex >= outputs.size()) outputIndex = 0;
	output = outputs[outputIndex];
	updateOutputs();
}

void ofxProjectionManager::prevOutput(){
	// target previous warp output
	outputIndex--;
	if (outputIndex < 0) outputIndex = outputs.size()-1;
	output = outputs[outputIndex];
	updateOutputs();
}
