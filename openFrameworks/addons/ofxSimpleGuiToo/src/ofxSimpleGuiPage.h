/***********************************************************************

 Copyright (c) 2008, 2009, Memo Akten, www.memo.tv
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

class ofxSimpleGuiPage : public ofxSimpleGuiControl {
public:
	ofxSimpleGuiPage(string name);
	~ofxSimpleGuiPage();

	void					draw(float x, float y, bool alignRight);

//	void					loadFromXML(ofxXmlSettings &XML);
//	void					saveToXML(ofxXmlSettings &XML);

	void					setXMLName(string xmlFilename);
	void					loadFromXML();
	void					saveToXML();	
	

	ofxSimpleGuiControl			&addControl(ofxSimpleGuiControl& control);
	ofxSimpleGuiButton			&addButton(string name, bool &value);
	ofxSimpleGuiContent			&addContent(string name, ofBaseDraws &content, float fixwidth = -1);
	ofxSimpleGuiFPSCounter		&addFPSCounter();
//	ofxSimpleGuiQuadWarp		&addQuadWarper(string name, float x, float y, float sw, float sh, ofPoint &&pts);
//	ofxSimpleGuiMovieSlider		&addMovieSlider(string name, ofVideoPlayer& input);
	ofxSimpleGuiSliderInt		&addSlider(string name, int &value, int min, int max);
	ofxSimpleGuiSliderFloat		&addSlider(string name, float &value, float min, float max, float smoothing = 0);
	ofxSimpleGuiSliderArrayInt		&addSliderArray(string name, int &value, int min, int max);
	ofxSimpleGuiSliderArrayFloat		&addSliderArray(string name, float &value, float min, float max, float smoothing = 0);
	ofxSimpleGuiSlider2d		&addSlider2d(string name, ofPoint& value, float xmin, float xmax, float ymin, float ymax);
	ofxSimpleGuiTitle			&addTitle(string name);
	ofxSimpleGuiToggle			&addToggle(string name, bool &value);
	ofxSimpleGuiColorPicker		&addColorPicker(string name, float *values);
	ofxSimpleGuiDebugInt		&addDebug(string name, int& value);
	ofxSimpleGuiDebugFloat		&addDebug(string name, float& value);
	ofxSimpleGuiDebugBool		&addDebug(string name, bool& value);
	ofxSimpleGuiDebugString		&addDebug(string name, string& value);
	ofxSimpleGuiPageShortcut	&addPageShortcut(string name, string shortcutPageName);
	ofxSimpleGuiPageShortcut	&addPageShortcut(string shortcutPageName){
		return addPageShortcut(shortcutPageName,shortcutPageName);
	}
	ofxSimpleGuiPageShortcut	&addPageShortcut(ofxSimpleGuiPage& shortcutPage){
		return addPageShortcut(shortcutPage.name,shortcutPage.name);
	}
	
	template <class ListenerClass>
	ofxSimpleGuiButton			&addButton(string name, ListenerClass  * listener, void (ListenerClass::*method)(ofEventArgs&)){
		ofxSimpleGuiButton* button = new ofxSimpleGuiButton(name);
		addControl(*button);
		ofAddListener(button->onPressed, listener,method);
		return *button;
	}
	
	void enable();
	void disable();
	
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
	
	static string			folderPath;

protected:
	vector <ofxSimpleGuiControl*>	controls;
	float getNextY(float y);
	ofxXmlSettings					XML;
	string							xmlFilename;
};
