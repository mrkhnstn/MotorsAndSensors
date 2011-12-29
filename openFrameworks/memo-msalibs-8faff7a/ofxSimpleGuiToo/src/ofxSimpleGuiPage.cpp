/***********************************************************************
 
 Copyright (c) 2008, 2009, 2010, Memo Akten, www.memo.tv
 *** The Mega Super Awesome Visuals Company ***
 * All rights reserved.
 
 based on Todd Vanderlin's ofxSimpleGui API
 http://toddvanderlin.com/
 
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of MSA Visuals nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ***********************************************************************/


//mark 12/01/2011: changed ofSetHexColor to ofSetColor in order to make it work with previous oF version

#include "ofxSimpleGuiPage.h"

string ofxSimpleGuiPage::folderPath = "";

ofxSimpleGuiPage::ofxSimpleGuiPage(string name) : ofxSimpleGuiControl(name) {
	disableAllEvents();
	width = ofGetWidth();
	height = ofGetHeight();
	eventStealingControl = NULL;
	setXMLName(name + "_settings.xml");
	activeControlId = 0;
	scroll = 0;
	
}

ofxSimpleGuiPage::~ofxSimpleGuiPage() {
	// delete all controls
}

ofxSimpleGuiPage &ofxSimpleGuiPage::setXMLName(string s) {
	xmlFilename = s;
	return *this;
}

void ofxSimpleGuiPage::loadFromXML() {
	//ofLog(OF_LOG_VERBOSE, "ofxSimpleGuiPage::loadFromXML: " + folderPath + xmlFilename);
	
	if(xmlFilename.compare("") == 0) return;
	
	if(XML.loadFile(folderPath + xmlFilename) == false) {
		ofLog(OF_LOG_ERROR, "Error loading xmlFilename: " + folderPath + xmlFilename);
		return;
	}
	
	XML.pushTag("controls");
	for(int i=0; i < controls.size(); i++) {
		controls[i]->loadFromXML(XML);
	}
	XML.popTag();
}

void ofxSimpleGuiPage::saveToXML() {
	if(controls.size() < 1 || xmlFilename.compare("") == 0) return;	// if it has no controls (title counts as one control)
	
	XML.saveFile(folderPath + xmlFilename + ".bak");
	
	XML.clear();	// clear cause we are building a new xml file
	
	XML.addTag("controls");
	XML.pushTag("controls");
	for(int i=0; i < controls.size(); i++) {
		controls[i]->saveToXML(XML);
	}
	XML.popTag();
	
	XML.saveFile(folderPath + xmlFilename);
	//	if(doSaveBackup) 
	// ofLog(OF_LOG_VERBOSE, "ofxSimpleGuiPage::saveToXML: " + folderPath + xmlFilename + " " + ofToString(controls.size(), 0) + " items");
}

ofxSimpleGuiControl &ofxSimpleGuiPage::addControl(ofxSimpleGuiControl& control) {
	string tempKey = control.key;
	string newKey = createUniqueKey(tempKey,0);
	control.setKey(newKey);
	controls.push_back(&control);
	//width += control.width + config->padding.x;
	return control;
}

ofxSimpleGuiButton &ofxSimpleGuiPage::addButton(string name, bool &value) {
	return (ofxSimpleGuiButton &)addControl(* new ofxSimpleGuiButton(name, value));
}

ofxSimpleGuiContent &ofxSimpleGuiPage::addContent(string name, ofBaseDraws &content, float fixwidth) {
	if(fixwidth == -1) fixwidth = config->gridSize.x - config->padding.x;
	return (ofxSimpleGuiContent &)addControl(* new ofxSimpleGuiContent(name, content, fixwidth));
}

ofxSimpleGuiFPSCounter &ofxSimpleGuiPage::addFPSCounter() {
	return (ofxSimpleGuiFPSCounter &)addControl(* new ofxSimpleGuiFPSCounter());
}

ofxSimpleGuiQuadWarp &ofxSimpleGuiPage::addQuadWarper(string name, ofBaseDraws &baseDraw, ofPoint *pts) {
	return (ofxSimpleGuiQuadWarp &)addControl(* new ofxSimpleGuiQuadWarp(name, baseDraw, pts));
}

ofxSimpleGuiSliderInt &ofxSimpleGuiPage::addSlider(string name, int &value, int min, int max) {
	return (ofxSimpleGuiSliderInt &)addControl(* new ofxSimpleGuiSliderInt(name, value, min, max));
}

ofxSimpleGuiSliderFloat &ofxSimpleGuiPage::addSlider(string name, float &value, float min, float max) {
	return (ofxSimpleGuiSliderFloat &)addControl(* new ofxSimpleGuiSliderFloat(name, value, min, max));
}

ofxSimpleGuiSlider2d &ofxSimpleGuiPage::addSlider2d(string name, ofPoint& value, float xmin, float xmax, float ymin, float ymax) {
	return (ofxSimpleGuiSlider2d &)addControl(* new ofxSimpleGuiSlider2d(name, value, xmin, xmax, ymin, ymax));
}

ofxSimpleGuiTitle &ofxSimpleGuiPage::addTitle(string name, float height) {
	return (ofxSimpleGuiTitle &)addControl(* new ofxSimpleGuiTitle(name, height));
}

ofxSimpleGuiToggle &ofxSimpleGuiPage::addToggle(string name, bool &value) {
	return (ofxSimpleGuiToggle &)addControl(* new ofxSimpleGuiToggle(name, value));
}

ofxSimpleGuiColorPicker2 &ofxSimpleGuiPage::addColorPicker(string name, float *values, bool useFloat, bool displayInteger) {
	return (ofxSimpleGuiColorPicker2 &)addControl(* new ofxSimpleGuiColorPicker2(name, values, useFloat ? 1 : 255, displayInteger));
}

ofxSimpleGuiComboBox &ofxSimpleGuiPage::addComboBox(string name, int &choice_out, int numChoices, string* choiceTitles) {
	return (ofxSimpleGuiComboBox &)addControl(* new ofxSimpleGuiComboBox(name, choice_out, numChoices, this, choiceTitles));
}

ofxSimpleGuiDebugInt &ofxSimpleGuiPage::addDebug(string name, int& value){
	return (ofxSimpleGuiDebugInt &)addControl(* new ofxSimpleGuiDebugInt(name, value));
}

ofxSimpleGuiDebugFloat &ofxSimpleGuiPage::addDebug(string name, float& value){
	return (ofxSimpleGuiDebugFloat &)addControl(* new ofxSimpleGuiDebugFloat(name, value));
}

ofxSimpleGuiDebugBool &ofxSimpleGuiPage::addDebug(string name, bool& value){
	return (ofxSimpleGuiDebugBool &)addControl(* new ofxSimpleGuiDebugBool(name, value));
}

ofxSimpleGuiDebugString &ofxSimpleGuiPage::addDebug(string name, string& value){
	return (ofxSimpleGuiDebugString &)addControl(* new ofxSimpleGuiDebugString(name, value));
}

ofxSimpleGuiPageShortcut &ofxSimpleGuiPage::addPageShortcut(string name, string shortcutPageName) {
	return (ofxSimpleGuiPageShortcut &)addControl(* new ofxSimpleGuiPageShortcut(name, shortcutPageName));
}

ofxSimpleGuiPageShortcut &ofxSimpleGuiPage::addPageShortcut(string shortcutPageName){
	return addPageShortcut(shortcutPageName,shortcutPageName);
}

ofxSimpleGuiPageShortcut &ofxSimpleGuiPage::addPageShortcut(ofxSimpleGuiPage& shortcutPage){
	return addPageShortcut(shortcutPage.name,shortcutPage.name);
}

ofxSimpleGuiTextInput &ofxSimpleGuiPage::addTextInput(string name, string& value){
	return (ofxSimpleGuiTextInput &)addControl(* new ofxSimpleGuiTextInput(name, value));
}

float ofxSimpleGuiPage::getNextY(float y) {
	return y;
	int iy = (int)ceil(y/config->gridSize.y);
	return (iy) * config->gridSize.y;
}

void ofxSimpleGuiPage::draw(float x, float y, float w, float h, bool alignRight) {
	
	//setPos(x += config->offset.x, y += config->offset.y);
	
	setPos(x, y);
	setSize(w, h);
	
	if(alignRight) x = width - x -  config->gridSize.x;
	
	float posX		= 0;
	float posY		= 0;
	float stealingX = 0;
	float stealingY = 0;
	
	ofSetRectMode(OF_RECTMODE_CORNER);
	
	bool titleOpen = true;
	bool isTitle = false;
	
	for(int i=0; i<controls.size(); i++) {
		
		if(alignRight){
			if(posX < 0) break;
		} else {
			if(posX + config->gridSize.x > width) break;
		}
		
		ofxSimpleGuiControl &control = *controls[i];
		
		//isTitle = typeid(control) == typeid(ofxSimpleGuiTitle);
		
		//if(control.getPriority() <= ofxSimpleGuiControl::mainPriority && titleOpen || isTitle){ 
		if(control.enabled && i >= scroll){
			
			if(control.newColumn) {
				if(alignRight) posX -= config->gridSize.x;
				else posX += config->gridSize.x;
				posY = 0;
			}
			
			if(posY + control.height + config->padding.y >= height){// - control.height - config->padding.y) {
				
				if(alignRight) posX -= config->gridSize.x;
				else posX += config->gridSize.x;
				
				posY = 0;
				
			}
			
			if(alignRight){
				if(posX < 0) break;
			} else {
				if(posX + config->gridSize.x > width) break;
			}
			
			float controlX = posX + x;
			float controlY = posY + y;
			
			//we don't draw the event stealing controls until the end because they can expand and overlap with other controls (e.g. combo box)
			if(eventStealingControl == &control) {
				stealingX = controlX;
				stealingY = controlY;
			} else {
				//			printf("drawing control: %s %s\n", control.controlType.c_str(), control.name.c_str());
				control.draw(controlX, controlY);
			}
			
			if(control.hasTitle && false) {
				ofNoFill();
				ofSetColor(config->borderColor);
				glLineWidth(0.5f);
				ofRect(controlX, controlY, control.width, control.height);
			}
			
			//posY = getNextY(posY + control.height + config->padding.y);
			
			posY += control.height + config->padding.y;
			
			
			
		}
		
		/*
		if(isTitle){
			titleOpen = ((ofxSimpleGuiTitle&)control).isOpen;
		}
		*/
		
		//		if(guiFocus == controls[i]->guiID) controls[i]->focused = true;		// MEMO
		//		else							   controls[i]->focused = false;
	}
	//event stealing controls get drawn on top
	if(eventStealingControl) {
		eventStealingControl->draw(stealingX, stealingY);
		if(eventStealingControl->hasTitle) {
			ofNoFill();
			ofSetColor(config->borderColor);
			glLineWidth(0.5f);
			ofRect(stealingX, stealingY, eventStealingControl->width, eventStealingControl->height);
		}
	}
}

void ofxSimpleGuiPage::update(ofEventArgs &e) {
	bool titleOpen = true;
	bool isTitle = false;
	
	for(int i=0; i<controls.size(); i++){
		
		ofxSimpleGuiControl &control = *controls[i];
		
		isTitle = typeid(control) == typeid(ofxSimpleGuiTitle);
		
		if(isTitle){
			control.enabled = control.getPriority() <= ofxSimpleGuiControl::mainPriority;
			titleOpen = ((ofxSimpleGuiTitle&)control).isOpen;
		} else {
			control.enabled = control.getPriority() <= ofxSimpleGuiControl::mainPriority && titleOpen;
		}
		
		//if(control.getPriority() <= ofxSimpleGuiControl::mainPriority && titleOpen || isTitle)
		//	control.update();
			
	}
	
	for(int i=0; i<controls.size(); i++){
		ofxSimpleGuiControl &control = *controls[i];
		if(control.enabled) control.update();
	}
}

void ofxSimpleGuiPage::SetEventStealingControl(ofxSimpleGuiControl &control) {
	eventStealingControl = &control;
}

void ofxSimpleGuiPage::ReleaseEventStealingControl() {
	eventStealingControl = NULL;
}

void ofxSimpleGuiPage::mouseMoved(ofMouseEventArgs &e) {
	if(eventStealingControl)
		eventStealingControl->_mouseMoved(e);
	else
		for(int i=0; i<controls.size(); i++){
			if(controls[i]->enabled){
				controls[i]->_mouseMoved(e);
				if(controls[i]->isMouseOver()){
					setActiveControl(i);
				}
			}
		}
}

void ofxSimpleGuiPage::mousePressed(ofMouseEventArgs &e) {
	if(eventStealingControl)
		eventStealingControl->_mousePressed(e);
	else
		for(int i=0; i<controls.size(); i++){
			if(controls[i]->enabled) 
				controls[i]->_mousePressed(e);
		}
}

void ofxSimpleGuiPage::mouseDragged(ofMouseEventArgs &e) {
	if(eventStealingControl)
		eventStealingControl->_mouseDragged(e);
	else
		for(int i=0; i<controls.size(); i++) 
			if(controls[i]->enabled)
				controls[i]->_mouseDragged(e);
}

void ofxSimpleGuiPage::mouseReleased(ofMouseEventArgs &e) {
	if(eventStealingControl)
		eventStealingControl->_mouseReleased(e);
	else
		for(int i=0; i<controls.size(); i++) 
			if(controls[i]->enabled)
				controls[i]->_mouseReleased(e);
}

bool ofxSimpleGuiPage::focusNextControl(){
	if(controls.size() > 0){
		if(controls[activeControlId]->focusNextControl()) return true; //< internal focus next
		
		int tempIndex = activeControlId + 1;
		if(tempIndex >= controls.size()) tempIndex = 0;
		while (tempIndex != activeControlId && !controls[tempIndex]->enabled){//controls[tempIndex]->getPriority() > ofxSimpleGuiControl::mainPriority) {
			tempIndex++;
			if(tempIndex >= controls.size()) tempIndex = 0;	
		}
		if(tempIndex != activeControlId){
			//setActiveControl(tempIndex);
			controls[activeControlId]->setFocus(false); //unfocus previous active control
			activeControlId = tempIndex;
			controls[activeControlId]->focusNextControl();
			
			return true;
		} else {
			return false;
		}
		/*
		 int tempActive = activeControlId+1;
		 if(tempActive >= controls.size()){
		 tempActive = 0;
		 }
		 setActiveControl(tempActive);
		 */
	} else {
		return false;
	}
}

bool ofxSimpleGuiPage::focusPreviousControl(){
	if(controls.size() > 0){
		
		if(controls[activeControlId]->focusPrevControl()) return true; //< internal focus next
		
		int tempIndex = activeControlId - 1;
		if(tempIndex < 0) tempIndex = controls.size()-1;
		while (tempIndex != activeControlId && !controls[tempIndex]->enabled){// controls[tempIndex]->getPriority() > ofxSimpleGuiControl::mainPriority) {
			tempIndex--;
			if(tempIndex < 0) tempIndex = controls.size()-1;	
		}
		if(tempIndex != activeControlId){
			//setActiveControl(tempIndex);
			controls[activeControlId]->setFocus(false); //unfocus previous active control
			activeControlId = tempIndex;
			controls[activeControlId]->focusPrevControl();
			return true;
		} else {
			return false;
		}
		/*
		 int tempActive = activeControlId-1;
		 if(tempActive < 0){
		 tempActive = controls.size() - 1;
		 }
		 setActiveControl(tempActive);
		 */
	} else {
		return false;
	}
}

void ofxSimpleGuiPage::setActiveControl(int id){
	if(id != activeControlId && controls.size() > 0){
		// disable previous control
		if(activeControlId >= 0 && activeControlId < controls.size()){
			controls[activeControlId]->setFocus(false);
		}
		activeControlId = ofClamp(id,0,controls.size() - 1);
		controls[activeControlId]->setFocus(true);
	}
}

void ofxSimpleGuiPage::keyPressed(ofKeyEventArgs &e) {	
	if(name == "Header") return;
	if(activeControlId < controls.size())
 		controls[activeControlId]->_keyPressed(e);
}

void ofxSimpleGuiPage::keyReleased(ofKeyEventArgs &e) {
	if(name == "Header") return;
	if(activeControlId < controls.size())
 		controls[activeControlId]->_keyReleased(e);
	/*
	for(int i=0; i<controls.size(); i++) if(controls[i]->isMouseOver()) controls[i]->_keyReleased(e);
	*/
}

void ofxSimpleGuiPage::onKeyUp(){
	if(name == "Header") return;
	focusPreviousControl();
}

void ofxSimpleGuiPage::onKeyDown(){
	if(name == "Header") return;
	focusNextControl();
}

void ofxSimpleGuiPage::onKeyLeft(){
	if(name == "Header") return;
	if(activeControlId < controls.size())
		controls[activeControlId]->onKeyLeft();
}

void ofxSimpleGuiPage::onKeyRight(){
	if(name == "Header") return;
	if(activeControlId < controls.size())
		controls[activeControlId]->onKeyRight();
}

void ofxSimpleGuiPage::onKeyEnter(){
	if(name == "Header") return;
	if(activeControlId < controls.size())
		controls[activeControlId]->onKeyEnter();
}

void ofxSimpleGuiPage::onJoystick(float x, float y, int id){
	if(name == "Header") return;
	if(activeControlId < controls.size())
		controls[activeControlId]->onJoystick(x,y,id);
}

vector <ofxSimpleGuiControl*>&	ofxSimpleGuiPage::getControls() {
	return controls;
}

string ofxSimpleGuiPage::createUniqueKey(string key, int i){
	
	string tempKey = i == 0 ? key : key + ofToString(i);
	
	for(vector<ofxSimpleGuiControl*>::iterator it = controls.begin(); it != controls.end(); ++it){
		
		ofxSimpleGuiControl& control = **it;
		if(control.key.compare(tempKey) == 0){
			return createUniqueKey(key,i+1);
		}
		
	}
	
	return i == 0 ? key : tempKey;
}

void ofxSimpleGuiPage::closeAllTitles(){
	for(int i=0; i<controls.size(); i++) {
		ofxSimpleGuiControl &control = *controls[i];
		if(typeid(control) == typeid(ofxSimpleGuiTitle)){ // if is title
			ofxSimpleGuiTitle* title = (ofxSimpleGuiTitle*)&control;
			title->isOpen = false;
		}
	}
}

void ofxSimpleGuiPage::openAllTitles(){
	for(int i=0; i<controls.size(); i++) {
		ofxSimpleGuiControl &control = *controls[i];
		if(typeid(control) == typeid(ofxSimpleGuiTitle)){ // if is title
			ofxSimpleGuiTitle* title = (ofxSimpleGuiTitle*)&control;
			title->isOpen = true;
		}
	}
}

void ofxSimpleGuiPage::decrementScroll(){
	scroll = ofClamp(scroll - 1, 0, controls.size() - 1);
}

void ofxSimpleGuiPage::incrementScroll(){
	scroll = ofClamp(scroll + 1, 0, controls.size() - 1);
}





