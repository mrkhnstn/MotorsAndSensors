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

#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxSimpleGuiIncludes.h"
#include "ofxSimpleGuiEventTrigger.h"

#define GUI_MODE_CLASSIC		0
#define GUI_MODE_COLUMN			1
#define GUI_MODE_COUNT			2

class ofxSimpleGuiToo {
	
public:
	int							guiFocus;
	
	ofxSimpleGuiConfig			*config;	
	
	ofxSimpleGuiToo();
	void						setup();
	
	void						loadFromXML();
	void						saveToXML();	
	void						setFolderPath(string folderPath);
	string						getFolderPath();
	void						setAutoSave(bool b);
	void						setAlignRight(bool b);
	bool						getAlignRight();
	void						setDefaultKeys(bool b);
	
	//	int		getValueI(string nameID);
	//	float	getValueF(string nameID);
	//	bool	getValueB(string nameID);
	
	void						drawFocus(float x, float y);
	
	void						setDraw(bool b);
	void						toggleDraw();
	void						show();		// simply calls setDraw(true);
	void						hide();		// simply calls setDraw(false);
	bool						isOn();
	
	void						nextPage();
	void						prevPage();
	void						setPage(int i);				// 1 based index of page
	void						setPage(string name);
	
	void						nextPageWithBlank();		// cycles through pages, and closes after last page
	
	ofxSimpleGuiPage&			page(int i);				// 1 based index of page
	ofxSimpleGuiPage&			page(string name);			// returns page by name
	ofxSimpleGuiPage&			currentPage();				// returns current page
	vector <ofxSimpleGuiPage*>&	getPages();
	
	void						focusNext(); // increments focus on current page
	void						focusPrev(); // decrements focus on current page
	
	ofxSimpleGuiControl			&control(string name);		// returns control by name
	
	ofxSimpleGuiPage			&addPage(string name = "");
	ofxSimpleGuiControl			&addControl(ofxSimpleGuiControl& control);
	ofxSimpleGuiContent			&addContent(string name, ofBaseDraws &content, float fixwidth = -1);
	ofxSimpleGuiButton			&addButton(string name, bool &value);
	
	template <class ListenerClass>
	ofxSimpleGuiButton			&addButton(string name, ListenerClass  * listener, void (ListenerClass::*method)(ofEventArgs&));
	
	template <class ListenerClass>
	ofxSimpleGuiButton			&addButton(string name, char keyboardShortcut, ListenerClass  * listener, void (ListenerClass::*method)(ofEventArgs&));
	
	template <class ListenerClass>
	void addKeyboardShortcut(int keyboardShortcut, ListenerClass  * listener, void (ListenerClass::*method)(ofEventArgs&));	
	
	ofxSimpleGuiFPSCounter		&addFPSCounter();
	//	ofxSimpleGuiMovieSlider		&addMovieSlider(string name, ofVideoPlayer& input);
	ofxSimpleGuiQuadWarp		&addQuadWarper(string name, ofBaseDraws &baseDraw, ofPoint *pts);
	ofxSimpleGuiSliderInt		&addSlider(string name, int &value, int min, int max);
	ofxSimpleGuiSliderFloat		&addSlider(string name, float &value, float min, float max);
	ofxSimpleGuiSlider2d		&addSlider2d(string name, ofPoint& value, float xmin, float xmax, float ymin, float ymax);
	ofxSimpleGuiTitle			&addTitle(string name="", float height = 0);
	ofxSimpleGuiToggle			&addToggle(string name, bool &value);
	ofxSimpleGuiColorPicker2	&addColorPicker(string name, float *values, bool useFloat=true, bool displayInteger=false);
	void						addColorPicker2(string name, float *values, bool useFloat=true, bool displayInteger=false);
	
	ofxSimpleGuiComboBox        &addComboBox(string name, int &value, int numChoices, string* choiceTitles=NULL);
	
	ofxSimpleGuiDebugInt		&addDebug(string name, int& value);	
	ofxSimpleGuiDebugBool		&addDebug(string name, bool& value);	
	ofxSimpleGuiDebugFloat		&addDebug(string name, float& value);	
	ofxSimpleGuiDebugString		&addDebug(string name, string& value);	
	
	ofxSimpleGuiPageShortcut	&addPageShortcut(string name, string pageShortcutName);
	ofxSimpleGuiPageShortcut	&addPageShortcut(string name, ofxSimpleGuiPage& page);	
	ofxSimpleGuiPageShortcut	&addPageShortcut(ofxSimpleGuiPage& page);	
	ofxSimpleGuiPageShortcut	&addPageShortcut(string pageShortcutName);
	
	ofxSimpleGuiTextInput		&addTextInput(string name, string &value);
	
	void						draw();
	
	void						enableKeyboardShortcuts();
	void						disableKeyboardShortcuts();
	
	void moveBy(float dx, float dy);
	void resizeBy(float dw, float dh);
	
	void closeAllTitles(ofEventArgs& e);
	void openAllTitles(ofEventArgs& e);
	
	void incrementScroll(ofEventArgs& e);
	void decrementScroll(ofEventArgs& e);
	
	ofRectangle						displayRect; ///< only used by GUI_MODE_COLUMN
	
protected:
	//int								priority;
	int								displayMode; ///< GUI_MODE_....
	
	
	bool							doAutoSave;
	bool							alignRight;
	bool							doDefaultKeys;
	bool							doSave;//, doSaveBackup;
	bool							changePage;
	int								currentPageIndex;			// 1 based index of page (0 is for global controls)
	
	int								cols;
	int								rows;
	
	//	ofxXmlSettings					XML;
	//	string							xmlFilename;
	
	bool							doDraw;
	float							border;
	bool							keyboardShortcutsEnabled;
	bool							doDrawPageBoundary;
	
	ofxSimpleGuiPage				*headerPage;
	ofxSimpleGuiButton				*titleButton;
	vector <ofxSimpleGuiPage*>		pages;				// 0 is for headerPage
	
	map<int,ofxSimpleGuiEventTrigger*> keyboardShortcuts;
	
	void addListeners();
	void removeListeners();
	
	//	void setup(ofEventArgs &e);
	void update(ofEventArgs &e);
	//  void draw(ofEventArgs &e);
	//	void exit(ofEventArgs &e);
	
	void mouseMoved(ofMouseEventArgs &e);
	void mousePressed(ofMouseEventArgs &e);	
	void mouseDragged(ofMouseEventArgs &e);	
	void mouseReleased(ofMouseEventArgs &e);
	
	void keyPressed(ofKeyEventArgs &e);
	void keyReleased(ofKeyEventArgs &e);
	
	void setDefaultDimensions(ofEventArgs& e);
	void setDefaultDimensions();
	

};


template <class ListenerClass>
inline
ofxSimpleGuiButton			&ofxSimpleGuiToo::addButton(string name, ListenerClass  * listener, void (ListenerClass::*method)(ofEventArgs&)){
	if(!config) setup();
	return pages[currentPageIndex]->addButton(name, listener, method);
}

template <class ListenerClass>
inline
ofxSimpleGuiButton			&ofxSimpleGuiToo::addButton(string name, char keyboardShortcut, ListenerClass  * listener, void (ListenerClass::*method)(ofEventArgs&)){
	if(!config) setup();
	addKeyboardShortcut(keyboardShortcut, listener, method);
	
	// add extra button to keyboard shortcuts page
	ofxSimpleGuiButton* button = new ofxSimpleGuiButton(name);
	ofAddListener(button->onPressed, listener,method);
	button->setName(name + " ["+keyboardShortcut+"]");
	page("Keyboard Shortcuts").addControl(*button);
	pages[currentPageIndex]->addControl(*button);
	return *button;//pages[currentPageIndex]->addButton(name + " _"+keyboardShortcut+"_", listener, method);
}

template <class ListenerClass>
inline
void ofxSimpleGuiToo::addKeyboardShortcut(int keyboardShortcut, ListenerClass  * listener, void (ListenerClass::*method)(ofEventArgs&)){
	map<int,ofxSimpleGuiEventTrigger*>::iterator it = keyboardShortcuts.find(keyboardShortcut);
	if(it != keyboardShortcuts.end()){ //< if already exists
		//ofLog(OF_LOG_VERBOSE, "keyboard shortcut already");
		it->second->addListener(listener,method); //< add to existing
	} else { // otherwise create a new event trigger and add to keyboard shortcuts
		ofxSimpleGuiEventTrigger* eventTrigger = new ofxSimpleGuiEventTrigger();
		eventTrigger->addListener(listener,method);
		keyboardShortcuts[keyboardShortcut] = eventTrigger;
		//ofLog(OF_LOG_VERBOSE, "keyboard shortcut does not exist yet");
	}
}	


extern ofxSimpleGuiToo gui;



