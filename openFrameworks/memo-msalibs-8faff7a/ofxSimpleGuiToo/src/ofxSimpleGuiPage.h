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

#include "ofxSimpleGuiIncludes.h"

//class ListenerClass;

class ofxSimpleGuiPage : public ofxSimpleGuiControl {
public:
	
	ofxSimpleGuiPage(string name);
	~ofxSimpleGuiPage();
	
	void						draw(float x, float y, float w, float h, bool alignRight);
	
	ofxSimpleGuiPage&			setXMLName(string xmlFilename);
	void						loadFromXML();
	void						saveToXML();	
	
	ofxSimpleGuiControl			&addControl(ofxSimpleGuiControl& control);
	ofxSimpleGuiButton			&addButton(string name, bool &value);

	template<class ListenerClass> 
	ofxSimpleGuiButton			&addButton(string name, ListenerClass  * listener, void (ListenerClass::*method)(ofEventArgs&));
	
	ofxSimpleGuiContent			&addContent(string name, ofBaseDraws &content, float fixwidth = -1);
	ofxSimpleGuiFPSCounter		&addFPSCounter();
	ofxSimpleGuiQuadWarp		&addQuadWarper(string name, ofBaseDraws &baseDraw, ofPoint *pts);
	ofxSimpleGuiSliderInt		&addSlider(string name, int &value, int min, int max);
	ofxSimpleGuiSliderFloat		&addSlider(string name, float &value, float min, float max);
	ofxSimpleGuiSlider2d		&addSlider2d(string name, ofPoint& value, float xmin, float xmax, float ymin, float ymax);
	ofxSimpleGuiTitle			&addTitle(string name="", float height = 0);
	ofxSimpleGuiToggle			&addToggle(string name, bool &value);
	ofxSimpleGuiColorPicker2	&addColorPicker(string name, float *values, bool useFloat=true, bool displayInteger=false);
	ofxSimpleGuiComboBox		&addComboBox(string name, int &choice_out, int numChoices, string* choiceTitles=NULL);
	ofxSimpleGuiDebugInt		&addDebug(string name, int& value);
	ofxSimpleGuiDebugFloat		&addDebug(string name, float& value);
	ofxSimpleGuiDebugBool		&addDebug(string name, bool& value);
	ofxSimpleGuiDebugString		&addDebug(string name, string& value);
	ofxSimpleGuiPageShortcut	&addPageShortcut(string name, string shortcutPageName);
	ofxSimpleGuiPageShortcut	&addPageShortcut(string shortcutPageName);
	ofxSimpleGuiPageShortcut	&addPageShortcut(ofxSimpleGuiPage& shortcutPage);
	ofxSimpleGuiTextInput		&addTextInput(string name, string &value);
	
	void SetEventStealingControl(ofxSimpleGuiControl &control);
	void ReleaseEventStealingControl();
	
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
	
	void onKeyUp();
	void onKeyDown();
	void onKeyLeft();
	void onKeyRight();
	void onKeyEnter();
	void onJoystick(float x, float y, int id);
	
	void setActiveControl(int id);
	bool focusNextControl();
	bool focusPreviousControl();
	
	void closeAllTitles();
	void openAllTitles();
	
	void decrementScroll();
	void incrementScroll();
	
	vector <ofxSimpleGuiControl*>&	getControls();
	
	static string folderPath; // mark: xml of page gets stored in this path //TODO: this should be part of ofxSimpleGuiToo rather!!!
		
protected:
	vector <ofxSimpleGuiControl*>	controls;
	
	//some controls can take over focus (e.g. combo box,) which means events should only be passed to them
	ofxSimpleGuiControl*			eventStealingControl;
	float getNextY(float y);
	string createUniqueKey(string key, int i);
	
	
	ofxXmlSettings					XML;
	string							xmlFilename;
	
	int activeControlId;
	
	int scroll;
	
};

template<class ListenerClass> 
inline 
ofxSimpleGuiButton &ofxSimpleGuiPage::addButton(string name, ListenerClass  * listener, void (ListenerClass::*method)(ofEventArgs&)){
	ofxSimpleGuiButton* button = new ofxSimpleGuiButton(name);
	addControl(*button);
	ofAddListener(button->onPressed, listener,method);
	return *button;		
}
