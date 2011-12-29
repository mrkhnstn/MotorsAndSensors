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

#ifndef OFX_SIMPLE_GUI_H
#define OFX_SIMPLE_GUI_H

#include "ofMain.h"
#include "ofxXmlSettings.h"

#include "ofxSimpleGuiIncludes.h"

//#define	OFX_SIMPLEGUITOO_XML_NAME	"ofxSimpleGuiToo.xml"

class ofxSimpleGuiToo {

	
public:
	int							guiFocus;
	ofxSimpleGuiConfig			*config;	
	
	ofxSimpleGuiToo();
	void					setup();
	
	
//	void					loadFromXML(string file = OFX_SIMPLEGUITOO_XML_NAME);
	void					setXMLFolder(string folderpath);
	void					loadFromXML();
	void					saveToXML();	
	void					setAutoSave(bool b);

	void					setAlignRight(bool b);
	bool&					getAlignRight();
	
//	int		getValueI(string nameID);
//	float	getValueF(string nameID);
//	bool	getValueB(string nameID);
	
	void					drawFocus(float x, float y);

	
	void					setDraw(bool b);
	void					toggleDraw();
	void					show();		// simply calls setDraw(true);
	void					hide();		// simply calls setDraw(false);
	bool					isOn();
	
	void					nextPage();
	void					prevPage();
	void					setPage(int i);				// 1 based index of page
	void					setPage(string name);
	
	void					nextPageWithBlank();		// cycles through pages, and closes after last page
	
	ofxSimpleGuiPage		&page(int i);				// 1 based index of page
	ofxSimpleGuiPage		&page(string name);
	
	ofxSimpleGuiPage			&addPage(string name = "");
	ofxSimpleGuiControl			&addControl(ofxSimpleGuiControl& control);
	ofxSimpleGuiContent			&addContent(string name, ofBaseDraws &content, float fixwidth = -1);
	ofxSimpleGuiButton			&addButton(string name, bool &value);
	ofxSimpleGuiFPSCounter		&addFPSCounter();
//	ofxSimpleGuiMovieSlider		&addMovieSlider(string name, ofVideoPlayer& input);
//	ofxSimpleGuiQuadWarp		&addQuadWarper(string name, float x, float y, float sw, float sh, ofPoint &&pts);
	ofxSimpleGuiSliderInt		&addSlider(string name, int &value, int min, int max);
	ofxSimpleGuiSliderFloat		&addSlider(string name, float &value, float min, float max, float smoothing = 0);
	ofxSimpleGuiSliderArrayInt		&addSliderArray(string name, int &value, int min, int max);
	ofxSimpleGuiSliderArrayFloat	&addSliderArray(string name, float &value, float min, float max, float smoothing = 0);
	ofxSimpleGuiSlider2d		&addSlider2d(string name, ofPoint& value, float xmin, float xmax, float ymin, float ymax);
	ofxSimpleGuiTitle			&addTitle(string name);
	ofxSimpleGuiToggle			&addToggle(string name, bool &value);
	ofxSimpleGuiColorPicker		&addColorPicker(string name, float *values);
	
	ofxSimpleGuiPageShortcut			&addPageShortcut(string name, string pageShortcutName);
	
	ofxSimpleGuiPageShortcut			&addPageShortcut(string name, ofxSimpleGuiPage& page){
		return addPageShortcut(name, page.name);
	}
	
	ofxSimpleGuiPageShortcut			&addPageShortcut(string pageShortcutName){
		return addPageShortcut(pageShortcutName, pageShortcutName);
	}
	
	void					draw();
	
	////////////////////////////
	// osc related
	void					setupOsc(int receivePort, string sendIp = "127.0.0.1", int sendPort = 8000){
		osc.setup(receivePort, sendIp, sendPort);
	}
	
	void					addControlToOscReceiver(ofxSimpleGuiControl& guiControl, string oscMessageName=""){
		osc.addControl(guiControl, oscMessageName);
	}
	
	void addOscSendAddress(string sendIp, int sendPort){
	
	}
	
	ofxSimpleGuiButton			&addOscButton(string name, bool &value, string oscName=""){
		ofxSimpleGuiButton& button = addButton(name, value);
		addControlToOscReceiver(button,oscName);
		return button;
	}
	
	ofxSimpleGuiToggle			&addOscToggle(string name, bool &value, string oscName=""){
		ofxSimpleGuiToggle& toggle = addToggle(name, value);
		addControlToOscReceiver(toggle, oscName="");
		return toggle;
	}
	
	ofxSimpleGuiSliderInt		&addOscSlider(string name, int &value, int min, int max, string oscName=""){
		ofxSimpleGuiSliderInt& slider = addSlider(name,value,min,max); 
		addControlToOscReceiver(slider, oscName);
		return slider;
	}
	
	ofxSimpleGuiSliderFloat		&addOscSlider(string name, float &value, float min, float max, string oscName="", float smoothing = 0){
		ofxSimpleGuiSliderFloat& slider = addSlider(name, value, min, max, smoothing);
		addControlToOscReceiver(slider, oscName);
		return slider;
	}
	
	ofxSimpleGuiColorPicker		&addOscColorPicker(string name, float *values, string oscName=""){
		ofxSimpleGuiColorPicker		&colorPicker = addColorPicker(name, values);
		addControlToOscReceiver(colorPicker, oscName);
		return colorPicker;
	}
	
	ofxSimpleGuiDebugInt		&addDebug(string name, int& value){
		if(!config) setup();
		return pages[currentPage]->addDebug(name, value);
	}
	
	ofxSimpleGuiDebugBool		&addDebug(string name, bool& value){
		if(!config) setup();
		return pages[currentPage]->addDebug(name, value);
	}
	
	ofxSimpleGuiDebugFloat		&addDebug(string name, float& value){
		if(!config) setup();
		return pages[currentPage]->addDebug(name, value);
	}
	
	ofxSimpleGuiDebugString		&addDebug(string name, string& value){
		if(!config) setup();
		return pages[currentPage]->addDebug(name, value);
	}
	
	template <class ListenerClass>
	ofxSimpleGuiButton			&addButton(string name, ListenerClass  * listener, void (ListenerClass::*method)(ofEventArgs&)){
		if(!config) setup();
		return pages[currentPage]->addButton(name, listener, method);
	}
	
protected:
	bool							doAutoSave;
	bool							alignRight;
	bool							doSave;//, doSaveBackup;
	bool							changePage;
	int								currentPage;			// 1 based index of page (0 is for global controls)
	
//	ofxXmlSettings					XML;
//	string							xmlFilename;
	
	bool							doDraw;
	float							border;

	ofxSimpleGuiPage				*headerPage;
	ofxSimpleGuiButton				*titleButton;
	vector <ofxSimpleGuiPage*>		pages;				// 0 is for headerPage
	
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
	
	ofxSimpleGuiOsc					osc;
};

#endif


extern ofxSimpleGuiToo gui;



