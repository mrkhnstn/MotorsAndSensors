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

#include "ofxSimpleGuiToo.h"

ofxSimpleGuiToo gui;


//------------------------------------------------------------------------------ constrcutor
ofxSimpleGuiToo::ofxSimpleGuiToo() {
	config = NULL;
	doDefaultKeys = false;
}

void ofxSimpleGuiToo::setup() {
	config			= &defaultSimpleGuiConfig;
	
	doSave			= false;
	changePage		= false;
	titleButton		= NULL;
	keyboardShortcutsEnabled = true;
	doDrawPageBoundary = false;
	
	headerPage		= &addPage("Header");
	headerPage->height = config->buttonHeight * 2;
	headerPage->width = 0;
	titleButton = &headerPage->addButton("title", changePage);
	headerPage->addToggle("Auto Save", doAutoSave);
	headerPage->addButton("Save Settings", doSave);
	headerPage->addFPSCounter();
	
	cols = ceilf(ofGetWidth() / config->gridSize.x);
	rows = ceilf(ofGetHeight() / config->gridSize.y);
	
	addPage(); // default settings page
	addFPSCounter();
	
	setAutoSave(true);
	setAlignRight(false);
	setDraw(false);
	
	
	//addToggle("", <#bool value#>)
	
	//addControl(*titleButton);
	
	page(1).addPageShortcut(addPage("Keyboard Shortcuts"));
	page(1).addPageShortcut(addPage("GUI"));
	//setPage("GUI");
	addButton("save settings", doSave);
	addToggle("auto save", doAutoSave);
	//gui.addTitle("gui display");

	gui.addTitle("controls");
	string choices[] = {"simple","advanced","all"};
	gui.addComboBox("display priority", ofxSimpleGuiPage::mainPriority, 3, choices);
	gui.addButton("collapse all", 't', this, &ofxSimpleGuiToo::closeAllTitles);
	gui.addButton("open all", 'T', this, &ofxSimpleGuiToo::openAllTitles);
	
	gui.addTitle("display mode");
	string displayModeChoices[GUI_MODE_COUNT] = {"fixed","floating"};
	addComboBox("display mode", displayMode, GUI_MODE_COUNT, displayModeChoices);
	gui.addTitle("floating parameters");
	gui.addSlider("x", displayRect.x, 0, ofGetWidth());
	gui.addSlider("y", displayRect.y, 0, ofGetHeight());
	gui.addSlider("columns", cols, 1, 10);
	gui.addSlider("rows", rows, 5, 30);
	gui.addButton("reset", this, &ofxSimpleGuiToo::setDefaultDimensions);
	gui.addToggle("draw boundaries", doDrawPageBoundary);
	//gui.addSlider("gui width", displayRect.width, 256, ofGetWidth());
	//gui.addSlider("gui height", displayRect.height, 256, ofGetHeight());

	setPage(1);
	
	//addSlider("advanced gui", ofxSimpleGuiControl::mainPriority, 0, 2);
	ofAddListener(ofEvents.keyPressed, this, &ofxSimpleGuiToo::keyPressed);
}

void ofxSimpleGuiToo::addListeners() {
	//	ofAddListener(ofEvents.setup, this, &ofxSimpleGuiToo::setup);
	ofAddListener(ofEvents.update, this, &ofxSimpleGuiToo::update);
	//	ofAddListener(ofEvents.draw, this, &ofxSimpleGuiToo::draw);
	//	ofAddListener(ofEvents.exit, this, &ofxSimpleGuiToo::exit);
	
	ofAddListener(ofEvents.mousePressed, this, &ofxSimpleGuiToo::mousePressed);
	ofAddListener(ofEvents.mouseMoved, this, &ofxSimpleGuiToo::mouseMoved);
	ofAddListener(ofEvents.mouseDragged, this, &ofxSimpleGuiToo::mouseDragged);
	ofAddListener(ofEvents.mouseReleased, this, &ofxSimpleGuiToo::mouseReleased);
	
	//	ofAddListener(ofEvents.keyPressed, this, &ofxSimpleGuiToo::keyPressed);
	ofAddListener(ofEvents.keyReleased, this, &ofxSimpleGuiToo::keyReleased);
}

void ofxSimpleGuiToo::removeListeners() {
	//	ofRemoveListener(ofEvents.setup, this, &ofxSimpleGuiToo::setup);
	ofRemoveListener(ofEvents.update, this, &ofxSimpleGuiToo::update);
	//	ofRemoveListener(ofEvents.draw, this, &ofxSimpleGuiToo::draw);
	//	ofRemoveListener(ofEvents.exit, this, &ofxSimpleGuiToo::exit);
	
	ofRemoveListener(ofEvents.mousePressed, this, &ofxSimpleGuiToo::mousePressed);
	ofRemoveListener(ofEvents.mouseMoved, this, &ofxSimpleGuiToo::mouseMoved);
	ofRemoveListener(ofEvents.mouseDragged, this, &ofxSimpleGuiToo::mouseDragged);
	ofRemoveListener(ofEvents.mouseReleased, this, &ofxSimpleGuiToo::mouseReleased);
	
	//	ofRemoveListener(ofEvents.keyPressed, this, &ofxSimpleGuiToo::keyPressed);
	ofRemoveListener(ofEvents.keyReleased, this, &ofxSimpleGuiToo::keyReleased);
}

void ofxSimpleGuiToo::setDraw(bool b) {
	if(doDraw != b) {
		doDraw = b;
		if(doDraw) addListeners();
		else removeListeners();
		if(doAutoSave) saveToXML();
	}
}

void ofxSimpleGuiToo::show() {
	setDraw(true);
}

void ofxSimpleGuiToo::hide() {
	setDraw(false);
}

void ofxSimpleGuiToo::toggleDraw() {
	setDraw(!doDraw);
}

bool ofxSimpleGuiToo::isOn() {
	return doDraw;
}

void ofxSimpleGuiToo::setAutoSave(bool b) {
	doAutoSave = b;
}

void ofxSimpleGuiToo::loadFromXML() {
	ofLog(OF_LOG_VERBOSE, "ofxSimpleGuiToo::loadFromXML");// + file);
	
	for(int i=1; i < pages.size(); i++) {
		pages[i]->loadFromXML();
	}
	
	setPage(1);
	setDefaultDimensions();
}

void ofxSimpleGuiToo::saveToXML() {
	doSave = false;
	
	for(int i=1; i < pages.size(); i++) {
		pages[i]->saveToXML();
	}
	
	//ofLog(OF_LOG_VERBOSE, "ofxSimpleGuiToo::saveToXML");
}

void ofxSimpleGuiToo::setFolderPath(string folderPath){
	ofxSimpleGuiPage::folderPath = folderPath;
}

string ofxSimpleGuiToo::getFolderPath(){
	return ofxSimpleGuiPage::folderPath;
}

void ofxSimpleGuiToo::setAlignRight(bool b) {
	alignRight = b;
}

bool ofxSimpleGuiToo::getAlignRight(){
	return alignRight;
}

void ofxSimpleGuiToo::setDefaultKeys(bool b) {
	doDefaultKeys = b;
}

void ofxSimpleGuiToo::drawFocus(float x, float y) {
	glPushMatrix();
	glTranslatef(x, y, 0);
	ofFill();
	//	ofSetColor(config->focusColor.r, config->focusColor.g, config->focusColor.b, 200);
	ofRect(0, 0, 10, 10);
	glPopMatrix();
}

void ofxSimpleGuiToo::draw() {
	if(!doDraw) return;
	
	switch (displayMode) {
			
		case GUI_MODE_CLASSIC:
			
			ofPushStyle();
			glDisable(GL_DEPTH_TEST);
			ofSetLineWidth(3);
			glDisableClientState(GL_COLOR_ARRAY);
			
			headerPage->draw(0, 0, ofGetWidth(), headerPage->height, alignRight);		// this is the header
			
			// draw 
			ofSetColor(config->borderColor);
			if(alignRight) ofLine(ofGetWidth() - headerPage->width, headerPage->height, headerPage->width, headerPage->height);
			else ofLine(0, headerPage->height, headerPage->width, headerPage->height);
			
			pages[currentPageIndex]->draw(0.0f, headerPage->height, ofGetWidth(), ofGetHeight()-headerPage->height, alignRight);
			
			ofPopStyle();
			
			break;
			
		case GUI_MODE_COLUMN:	
			
			
			ofPushStyle();
			glDisable(GL_DEPTH_TEST);
			ofSetLineWidth(3);
			glDisableClientState(GL_COLOR_ARRAY);
			
			displayRect.width = cols * config->gridSize.x;
			displayRect.height = rows * config->gridSize.y;
			
			//headerPage->draw(displayRect.x, displayRect.y, ofGetWidth(), headerPage->height, alignRight);		// this is the header
			titleButton->draw(displayRect.x + config->padding.x, displayRect.y + config->padding.y);
			pages[currentPageIndex]->draw(displayRect.x + config->padding.x, displayRect.y + 2 * config->padding.y + titleButton->height, displayRect.width, displayRect.height, alignRight);
			
			ofPopStyle();
			
			if(doDrawPageBoundary){
			// draw page boundary
			ofPushStyle();
			ofNoFill();
			ofSetColor(config->borderColor);
			glLineWidth(0.5f);
			ofRect(displayRect.x, displayRect.y,displayRect.width + config->padding.x, displayRect.height + 2 * config->padding.y + titleButton->height);
			ofPopStyle();
			}
			
		default:
			break;
	}
	
}

void ofxSimpleGuiToo::nextPage() {
	int tempIndex = currentPageIndex + 1;
	if(tempIndex >= pages.size()) tempIndex = 1;
	while (tempIndex != currentPageIndex && page(tempIndex).getPriority() > ofxSimpleGuiControl::mainPriority) {
		tempIndex++;
		if(tempIndex >= pages.size()) tempIndex = 1;	
	}
	if(tempIndex != currentPageIndex)
		setPage(tempIndex);
}

void ofxSimpleGuiToo::prevPage() {
	int tempIndex = currentPageIndex - 1;
	if(tempIndex < 0) tempIndex = pages.size()-1;
	while (tempIndex != currentPageIndex && page(tempIndex).getPriority() > ofxSimpleGuiControl::mainPriority) {
		tempIndex--;
		if(tempIndex < 0) tempIndex = pages.size()-1;	
	}
	if(tempIndex != currentPageIndex)
		setPage(tempIndex);
}

void ofxSimpleGuiToo::nextPageWithBlank() {
	if(doDraw) {
		setPage(currentPageIndex + 1);
		if(currentPageIndex == 1) setDraw(false);
	} else {
		setDraw(true);
		setPage(1);
	}
}

void ofxSimpleGuiToo::setPage(int i) {
	currentPageIndex = i;
	if(currentPageIndex >= pages.size()) currentPageIndex = 1;
	else if(currentPageIndex < 1) currentPageIndex = pages.size()-1;
	
	if(titleButton) titleButton->setName(ofToString(currentPageIndex) + ": " + pages[currentPageIndex]->name);
}


void ofxSimpleGuiToo::setPage(string name) {
	//	ofxSimpleGuiPage *page;
	for(int i=1; i < pages.size(); i++) {
		if(name.compare(pages[i]->name) == 0) {
			setPage(i);
			break;
		}
	}
}


ofxSimpleGuiPage& ofxSimpleGuiToo::page(int i) {
	return *pages.at(i);
}

ofxSimpleGuiPage& ofxSimpleGuiToo::page(string name) {
	if(!config) setup();
	for(int i=1; i<pages.size(); i++) if(name.compare(pages[i]->name) == 0) return *pages[i];
}


ofxSimpleGuiPage& ofxSimpleGuiToo::currentPage() {
	return page(currentPageIndex);
}

vector <ofxSimpleGuiPage*>&	ofxSimpleGuiToo::getPages() {
	return pages;
}

void ofxSimpleGuiToo::focusNext(){
	currentPage().focusNextControl();
}

void ofxSimpleGuiToo::focusPrev(){
	currentPage().focusPreviousControl();
}

ofxSimpleGuiPage &ofxSimpleGuiToo::addPage(string name) {
	if(!config) setup();
	
	ofxSimpleGuiPage *newPage = new ofxSimpleGuiPage(name);//ofToString(pages.size(), 0) + ": " + name);
	pages.push_back(newPage);
	if(name == "") newPage->setName("SETTINGS");
	static bool b;
	//	if(pages.size() > 1) headerPage->addTitle(newPage->name);		// if this isn't the first page, add to header
	//	if(pages.size() > 1) newPage->addTitle(newPage->name);		// if this isn't the first page, add to header
	setPage(pages.size() - 1);
	return *newPage;
}

ofxSimpleGuiControl &ofxSimpleGuiToo::control(string name) {
	for(int i = 0; i < pages.size(); i++) {
		for(int j = 0; j < pages[i]->getControls().size(); j++) {
			if(name==pages[i]->getControls()[j]->name) {
				return *pages[i]->getControls()[j];
			}
		}
	}
	//	return NULL;
}


ofxSimpleGuiControl &ofxSimpleGuiToo::addControl(ofxSimpleGuiControl& control) {
	if(!config) setup();
	return pages[currentPageIndex]->addControl(control);
}

ofxSimpleGuiButton &ofxSimpleGuiToo::addButton(string name, bool &value) {
	if(!config) setup();
	return pages[currentPageIndex]->addButton(name, value);
}

//ofxSimpleGuiButton &ofxSimpleGuiToo::addButton(string name, ListenerClass  * listener, void (ListenerClass::*method)(ofEventArgs&))

ofxSimpleGuiContent &ofxSimpleGuiToo::addContent(string name, ofBaseDraws &content, float fixwidth) {
	if(!config) setup();
	return pages[currentPageIndex]->addContent(name, content, fixwidth);
}

ofxSimpleGuiFPSCounter &ofxSimpleGuiToo::addFPSCounter() {
	if(!config) setup();
	return pages[currentPageIndex]->addFPSCounter();
}

ofxSimpleGuiQuadWarp &ofxSimpleGuiToo::addQuadWarper(string name, ofBaseDraws &baseDraw, ofPoint *pts) {
	return pages[currentPageIndex]->addQuadWarper(name, baseDraw, pts);
}
//
//ofxSimpleGuiMovieSlider &ofxSimpleGuiToo::addMovieSlider(string name, ofVideoPlayer& input) {
//	return pages[currentPageIndex]->addMovieSlider(name, input);
//}

ofxSimpleGuiSliderInt &ofxSimpleGuiToo::addSlider(string name, int &value, int min, int max) {
	if(!config) setup();
	return pages[currentPageIndex]->addSlider(name, value, min, max);
}

ofxSimpleGuiSliderFloat &ofxSimpleGuiToo::addSlider(string name, float &value, float min, float max) {
	if(!config) setup();
	return pages[currentPageIndex]->addSlider(name, value, min, max);
}

ofxSimpleGuiSlider2d &ofxSimpleGuiToo::addSlider2d(string name, ofPoint& value, float xmin, float xmax, float ymin, float ymax) {
	if(!config) setup();
	return pages[currentPageIndex]->addSlider2d(name, value, xmin, xmax, ymin, ymax);
}

ofxSimpleGuiTitle &ofxSimpleGuiToo::addTitle(string name, float height) {
	if(!config) setup();
	return pages[currentPageIndex]->addTitle(name, height);
}

ofxSimpleGuiToggle &ofxSimpleGuiToo::addToggle(string name, bool &value) {
	if(!config) setup();
	return pages[currentPageIndex]->addToggle(name, value);
}


ofxSimpleGuiColorPicker2 &ofxSimpleGuiToo::addColorPicker(string name, float *values, bool useFloat, bool displayInteger) {
	if(!config) setup();
	return pages[currentPageIndex]->addColorPicker(name, values, useFloat, displayInteger);
}

void ofxSimpleGuiToo::addColorPicker2(string name, float *values, bool useFloat, bool displayInteger){
	if(!config) setup();
	float max = useFloat ? 1 : 255;
	addSlider(name+" r",*values,0,max);
	addSlider(name+" g",*(values+1),0,max);
	addSlider(name+" b",*(values+2),0,max);
	addSlider(name+" a",*(values+3),0,max);
}

ofxSimpleGuiComboBox &ofxSimpleGuiToo::addComboBox(string name, int &value, int numChoices, string* choiceTitles)  {
	if(!config) setup();
	return pages[currentPageIndex]->addComboBox(name, value, numChoices, choiceTitles);
}

ofxSimpleGuiDebugInt &ofxSimpleGuiToo::addDebug(string name, int& value){
	if(!config) setup();
	return pages[currentPageIndex]->addDebug(name, value);
}

ofxSimpleGuiDebugBool &ofxSimpleGuiToo::addDebug(string name, bool& value){
	if(!config) setup();
	return pages[currentPageIndex]->addDebug(name, value);
}

ofxSimpleGuiDebugFloat &ofxSimpleGuiToo::addDebug(string name, float& value){
	if(!config) setup();
	return pages[currentPageIndex]->addDebug(name, value);
}

ofxSimpleGuiDebugString	&ofxSimpleGuiToo::addDebug(string name, string& value){
	if(!config) setup();
	return pages[currentPageIndex]->addDebug(name, value);
}

ofxSimpleGuiPageShortcut &ofxSimpleGuiToo::addPageShortcut(string name, string shortcutPageName){
	if(!config) setup();
	return pages[currentPageIndex]->addPageShortcut(name, shortcutPageName);
}

ofxSimpleGuiPageShortcut &ofxSimpleGuiToo::addPageShortcut(string name, ofxSimpleGuiPage& page){
	return addPageShortcut(name, page.name);
}

ofxSimpleGuiPageShortcut &ofxSimpleGuiToo::addPageShortcut(string pageShortcutName){
	return addPageShortcut(pageShortcutName, pageShortcutName);
}

ofxSimpleGuiPageShortcut &ofxSimpleGuiToo::addPageShortcut(ofxSimpleGuiPage& page){
	return addPageShortcut(page.name, page);
}

ofxSimpleGuiTextInput &ofxSimpleGuiToo::addTextInput(string name, string &value){
	if(!config) setup();
	return pages[currentPageIndex]->addTextInput(name, value);
}


//void ofxSimpleGuiToo::setup(ofEventArgs &e) {
void ofxSimpleGuiToo::update(ofEventArgs &e) {
	if(changePage) {
		nextPage();
		changePage = false;
	}
	
	headerPage->update(e);
	pages[currentPageIndex]->height = ofGetHeight();
	pages[currentPageIndex]->update(e);
	
	
	//	if(doSaveBackup) doSave = true;
	
	if(doSave) saveToXML();
}
//void ofxSimpleGuiToo::draw(ofEventArgs &e) {
//void ofxSimpleGuiToo::exit(ofEventArgs &e) {

void ofxSimpleGuiToo::mouseMoved(ofMouseEventArgs &e) {
	headerPage->mouseMoved(e);
	pages[currentPageIndex]->mouseMoved(e);
}

void ofxSimpleGuiToo::mousePressed(ofMouseEventArgs &e) {
	headerPage->mousePressed(e);
	pages[currentPageIndex]->mousePressed(e);
}

void ofxSimpleGuiToo::mouseDragged(ofMouseEventArgs &e) {
	headerPage->mouseDragged(e);
	pages[currentPageIndex]->mouseDragged(e);
}

void ofxSimpleGuiToo::mouseReleased(ofMouseEventArgs &e) {
	headerPage->mouseReleased(e);
	pages[currentPageIndex]->mouseReleased(e);
	//	if(doAutoSave) doSave = true;
	if(doAutoSave) saveToXML();
}

void ofxSimpleGuiToo::keyPressed(ofKeyEventArgs &e) {
	//ofLog(OF_LOG_VERBOSE, ofToString(e.key));
	
	if(doDefaultKeys && keyboardShortcutsEnabled) {
		if(e.key == ' ') {
			toggleDraw();
			/* disabled for keyboard editing
			 } else if(e.key>='0' && e.key<='9') {
			 setPage((int)(e.key - '0'));
			 setDraw(true);
			 */
		} else if(doDraw) {
			switch(e.key) {
				case '[': prevPage(); break;
				case ']': nextPage(); break;
					//case 'p': focusPrev(); break;
					//case 'l': focusNext(); break;
				case 'g': 
					setPage(1); 
					setDraw(true);
					break;
				case 'G': gui.setAlignRight(!gui.getAlignRight()); break;
				case OF_KEY_UP:
					gui.currentPage().onKeyUp();
					break;
				case OF_KEY_DOWN:
					//focusNext();
					gui.currentPage().onKeyDown();
					break;
				case OF_KEY_LEFT:
					gui.currentPage().onKeyLeft();
					break;
				case OF_KEY_RIGHT:
					gui.currentPage().onKeyRight();
					break;
				case OF_KEY_RETURN:
					gui.currentPage().onKeyEnter();
					break;
			}
		}
	}
	
	// check and trigger keyboard shortcuts
	if(keyboardShortcutsEnabled){
		map<int,ofxSimpleGuiEventTrigger*>::iterator it = keyboardShortcuts.find(e.key);
		if(it != keyboardShortcuts.end()){ //< if already exists
			it->second->trigger();
		}
	}
	
	if(doDraw) {
		headerPage->keyPressed(e);
		pages[currentPageIndex]->keyPressed(e);
	}
	
}

void ofxSimpleGuiToo::keyReleased(ofKeyEventArgs &e) {
	headerPage->keyReleased(e);
	pages[currentPageIndex]->keyReleased(e);
}

void ofxSimpleGuiToo::enableKeyboardShortcuts(){
	keyboardShortcutsEnabled = true;
}

void ofxSimpleGuiToo::disableKeyboardShortcuts(){
	keyboardShortcutsEnabled = false;
}

void ofxSimpleGuiToo::setDefaultDimensions(ofEventArgs& e){
	setDefaultDimensions();
}

void ofxSimpleGuiToo::setDefaultDimensions(){
	displayRect.x = 5;
	displayRect.y = 5;
	cols = ceilf(ofGetWidth() / config->gridSize.x);
	rows = ceilf(ofGetHeight() / config->gridSize.y);
	//displayRect.width = ofGetWidth() - 5;
	//displayRect.height = ofGetHeight() - 5;
}

void ofxSimpleGuiToo::closeAllTitles(ofEventArgs& e){
	if(doDraw)
		pages[currentPageIndex]->closeAllTitles();
}

void ofxSimpleGuiToo::openAllTitles(ofEventArgs& e){
	if(doDraw)
		pages[currentPageIndex]->openAllTitles();
}

void ofxSimpleGuiToo::moveBy(float dx, float dy){
	if(!doDraw) return;
	displayRect.x += dx;
	displayRect.y += dy;
}

void ofxSimpleGuiToo::resizeBy(float dw, float dh){
	if(!doDraw) return;
	displayRect.width = ofClamp(displayRect.width + dw, 256, 3072);
	displayRect.height = ofClamp(displayRect.height + dh, 256, 3072);
}

void ofxSimpleGuiToo::incrementScroll(ofEventArgs& e){
	if(doDraw)
		pages[currentPageIndex]->incrementScroll();
};

void ofxSimpleGuiToo::decrementScroll(ofEventArgs& e){
	if(doDraw)
		pages[currentPageIndex]->decrementScroll();
};

/*
 //------------------------------------------------------------------------ mouse moved
 void ofxSimpleGuiToo::mouseMoved(int x, int y) {
 mx = x; my = y;
 }
 
 //------------------------------------------------------------------------ mouse released
 void ofxSimpleGuiToo::mouseReleased() {
 if(doAutoSave) {
 saveToXML();
 }
 }
 
 //------------------------------------------------------------------------ key released
 void ofxSimpleGuiToo::keyReleased(int key) {
 if(doAutoSave) {
 saveToXML();
 }
 }
 
 //------------------------------------------------------------------------ key press
 void ofxSimpleGuiToo::keyPressed(int key) {
 
 }
 */
