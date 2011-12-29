/*
 *  ofxSimpleGuiColorPicker.cpp
 *  OpenCL Particles 3. single segment trails
 *
 *  Created by Mehmet Akten on 01/11/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxSimpleGuiColorPicker.h"

ofxSimpleGuiColorPicker::ofxSimpleGuiColorPicker(string name, float* value, float max, bool displayInteger) : ofxSimpleGuiControl(name) {
	this->value = value;
	this->min	= 0;
	this->max	= max;
	
	for (int i=0; i<4; i++) {
		defaultValues[i] = value[i];
	}
	
	this->displayInteger = displayInteger;
	
	controlType = "ColorPicker";
	setup();
}

void ofxSimpleGuiColorPicker::setup() {
	setSize(config->gridSize.x - config->padding.x, config->sliderTextHeight * 5);
	for(int i=0; i<4; i++) {
		pct[i] = ofMap(getValue(i), 0, max, 0.0, width);
		barwidth[i] = pct[i];
	}
}

void ofxSimpleGuiColorPicker::loadFromXML(ofxXmlSettings &XML) {
	if(includeInXml){
		for(int i=0; i<4; i++) {
			setValue(XML.getValue(controlType + "_" + key + ":values_" + ofToString(i), defaultValues[i]), i);
		}
	}
}

void ofxSimpleGuiColorPicker::saveToXML(ofxXmlSettings &XML) {
	if(includeInXml){
		XML.addTag(controlType + "_" + key);
		XML.pushTag(controlType + "_" + key);
		XML.addValue("name", name);
		for(int i=0; i<4; i++) {
			XML.addValue("values_" + ofToString(i), getValue(i));
		}
		XML.popTag();
	}
}

float ofxSimpleGuiColorPicker::getValue(int i) {
	return (value)[i];
}

void ofxSimpleGuiColorPicker::setValue(float f, int i) {
	if(f < min) f = min;
	else if(f > max) f = max;
	(value)[i] = f;
}

void ofxSimpleGuiColorPicker::updateSlider() {
	if(!enabled) return;
	
	int i= (getMouseY() - y) / config->sliderTextHeight;
	if(i<0 || i>=4) return;
	
	if(pct[i] > width) {
		pct[i] = width;
	}
	else {
		pct[i] = getMouseX() - x;
		setValue(ofMap(pct[i], 0.0, (float)width, 0, max), i);
	}
}

void ofxSimpleGuiColorPicker::onPress(int x, int y, int button) {
	updateSlider();
}

void ofxSimpleGuiColorPicker::onDragOver(int x, int y, int button) {
	updateSlider();
}

void ofxSimpleGuiColorPicker::onDragOutside(int x, int y, int button) {
	updateSlider();
}

//--------------------------------------------------------------------- update
void ofxSimpleGuiColorPicker::update() {
	if(!enabled) return;
	
	if(lock) {
		updateSlider();
	}
	
//	enabled = false;
}

//--------------------------------------------------------------------- draw
void ofxSimpleGuiColorPicker::draw(float x, float y) {
	
//	enabled = true;
	
	//update postion of gui object
	setPos(x, y);
	glPushMatrix();
	glTranslatef(x, y, 0);
	
	int startY = 0;
	for(int i=0; i<4; i++) {
		
		barwidth[i] = ofMap(getValue(i), 0, max, 0.0, (float)width);
		if(barwidth[i] > width)	barwidth[i] = width;
		else if(barwidth[i] < 0) barwidth[i] = 0;
		
		ofEnableAlphaBlending();
		ofFill();
		setEmptyColor();
		ofRect(0, startY, width, config->sliderTextHeight);
		
	if(max == 1){ 
		switch(i) {
			case 0:glColor3f(getValue(i), 0, 0); break;
			case 1:glColor3f(0, getValue(i), 0); break;
			case 2:glColor3f(0, 0, getValue(i)); break;
			case 3:glColor3f(getValue(i), getValue(i), getValue(i)); break;
		}
	} else {
		switch(i) {
			case 0:ofSetColor(getValue(i), 0, 0); break;
			case 1:ofSetColor(0, getValue(i), 0); break;
			case 2:ofSetColor(0, 0, getValue(i)); break;
			case 3:ofSetColor(getValue(i), getValue(i), getValue(i)); break;
		}
	}

		
		ofRect(0, startY, barwidth[i], config->sliderTextHeight);
		
		int iover = (getMouseY() - y) / (config->sliderTextHeight);
		bool isOver = iover == i;
		if(isOver) {
			glColor3f(1, 1, 1);
		} else {
			glColor3f(0.5, 0.5, 0.5);
		}
		
		if (max == 1 && displayInteger) {
			ofDrawBitmapString(ofToString(getValue(i)*255, 0), 3, startY + 14);
		} else {
			if(max == 255){
				ofDrawBitmapString(ofToString(getValue(i), 0), 3, startY + 14);
			} else {
				ofDrawBitmapString(ofToString(getValue(i), 4), 3, startY + 14);
			}
		}

		
		
		startY += config->sliderTextHeight;
	}
	
	ofFill();
	
	setTextBGColor();
	ofRect(0, startY, width, config->sliderTextHeight);

	if(max == 1){ 
		glColor3f(getValue(0), getValue(1), getValue(2));
	} else {
		ofSetColor(getValue(0), getValue(1), getValue(2));
	}
//	ofRect(0, startY+config->sliderTextHeight, width, config->sliderTextHeight * 1.5);
	ofRect(150, startY + 3, width - 150 -3, config->sliderTextHeight - 8);
	
	setTextColor();
	string s = name;
	ofDrawBitmapString(s, 3, startY + 14);
	ofDisableAlphaBlending();
	glPopMatrix();
}

// COLOR PICKER 2 ////////////////////////////////////////////////////////

ofxSimpleGuiColorSlider::ofxSimpleGuiColorSlider(string name, float &value, float min, float max, bool displayInteger) : ofxSimpleGuiSliderBase<float>(name, value, min, max){
	controlType = "SliderColor";
	this->displayInteger = displayInteger;
}

void ofxSimpleGuiColorSlider::setFromString(string& s){
	setValue(ofToFloat(s));
}

string ofxSimpleGuiColorSlider::getString(){
	return ofToString(*value, 2);
}

void ofxSimpleGuiColorSlider::draw(float x, float y) {
	
	//		enabled = true;
	
	//update postion of gui object
	setPos(x, y);
	
	//VALUE CLAMP
	barwidth = ofMap((*value), min, max, 0.0, (float)width);
	if(barwidth > width) barwidth = width;
	else if(barwidth < 0) barwidth = 0;
	
	ofEnableAlphaBlending();
	glPushMatrix();
	glTranslatef(x, y, 0);
	ofFill();
	
	setEmptyColor();
	ofRect(0, 0, width, config->sliderHeight);
	
	
	//setFullColor();
	ofSetColor(color.r, color.g, color.b,color.a);
	ofRect(0, 0, barwidth, config->sliderHeight);
	
	setTextBGColor();
	
	ofRect(0, config->sliderHeight, width, config->sliderTextHeight);
	
	//string s = name + ": " + ofToString((*value));
	//ofDrawBitmapString(s, 3, config->sliderHeight + 14);
	
	
	if (keyboardEdit) {
		ofSetColor(255, 0, 0); //< set text to red while editing with keyboard. TODO: Make this more generic using something like setTextEditColor()
		string s = name + ": " + keyboardInputString;
		ofDrawBitmapString(s, 3, config->sliderHeight + 14);
	} else {
		setTextColor();
		string s = name + ": ";//
		if (displayInteger) {
			if (max == 255) {
				s += ofToString(getValue(),0);
			} else {
				s += ofToString(getValue() * 255, 0);
			}
		} else {
			if (max == 255) {
				s += ofToString(getValue(),0);
			} else {
				s += ofToString(getValue(), 4);
			}
		}
		ofDrawBitmapString(s, 3, config->sliderHeight + 14);
	}
	
	ofDisableAlphaBlending();
	glPopMatrix();
}

//

ofxSimpleGuiColorTitle::ofxSimpleGuiColorTitle(string name, float height) : ofxSimpleGuiTitle(name, height){
}

void ofxSimpleGuiColorTitle::draw(float x, float y) {
	setPos(x, y);
	
	if(hasTitle == false) return;
	
	glPushMatrix();
	glTranslatef(x, y, 0);
	
	ofEnableAlphaBlending();
	ofFill();
	//		setTextBGColor(value != NULL);
	//		ofSetColor(0, 0, 0);
	setEmptyColor();
	ofRect(0, 5, width, height-5);
	
	if(isInteger){
		ofSetColor(value[0], value[1], value[2], value[3]);
	} else {
		glColor4f(value[0], value[1], value[2], value[3]);	
	}
	
	ofRect(0, 0, width, 5);
	
	// if a toggle
	//if(value && (*value) && beToggle) {
	//	setTextColor();
		//ofLine(0, 0, box.width, box.height);
		//ofLine(box.width, 0, 0, box.height);
	//}
	
	setTextColor(value != NULL);
	if(isOpen){
		ofDrawBitmapString(name, 3, 18);
	} else {
		ofDrawBitmapString(name+" ...", 3, 18);
	}
	ofDisableAlphaBlending();
	
	glPopMatrix();
	glColor4f(1,1,1,1);
}

//

ofxSimpleGuiColorPicker2::ofxSimpleGuiColorPicker2(string name, float* value, float max, bool displayInteger) : ofxSimpleGuiControl(name) {
	
	this->value = value;
	this->min	= 0;
	this->max	= max;
	focus = 0;
	
	for (int i=0; i<4; i++) {
		defaultValues[i] = value[i];
	}
	
	this->displayInteger = displayInteger;
	
	controlType = "ColorPicker";
	setup();
}

ofxSimpleGuiColorPicker2::~ofxSimpleGuiColorPicker2(){
	delete[] sliders;
	delete title;
}

void ofxSimpleGuiColorPicker2::setup() {
	
	string names[4] = {"red","green","blue","alpha"};
	ofColor sliderColors[4];
	
	sliderColors[0].r = 255;
	sliderColors[0].g = 0;
	sliderColors[0].b = 0;
	sliderColors[0].a = 255;
	
	sliderColors[1].r = 0;
	sliderColors[1].g = 255;
	sliderColors[1].b = 0;
	sliderColors[1].a = 255;
	
	sliderColors[2].r = 0;
	sliderColors[2].g = 0;
	sliderColors[2].b = 255;
	sliderColors[2].a = 255;
	
	sliderColors[3].r = 128;
	sliderColors[3].g = 128;
	sliderColors[3].b = 128;
	sliderColors[3].a = 255;
	
	for(int i=0; i<4; i++) {
		ofxSimpleGuiColorSlider* slider = new ofxSimpleGuiColorSlider(names[i],value[i],0.f,max);
		slider->color = sliderColors[i];
		slider->displayInteger = displayInteger;
		sliders[i] = slider;
	}
	
	title = new ofxSimpleGuiColorTitle(name,sliders[0]->height);
	title->value = value;
	title->isInteger = max == 255;
	
	setSize(config->gridSize.x - config->padding.x, sliders[0]->height * 5 + 5 * 1);
}

void ofxSimpleGuiColorPicker2::loadFromXML(ofxXmlSettings &XML) {
	if(includeInXml){
		for(int i=0; i<4; i++) {
			setValue(XML.getValue(controlType + "_" + key + ":values_" + ofToString(i), defaultValues[i]), i);
		}
	}
}

void ofxSimpleGuiColorPicker2::saveToXML(ofxXmlSettings &XML) {
	if(includeInXml){
		XML.addTag(controlType + "_" + key);
		XML.pushTag(controlType + "_" + key);
		XML.addValue("name", name);
		for(int i=0; i<4; i++) {
			XML.addValue("values_" + ofToString(i), getValue(i));
		}
		XML.popTag();
	}
}

float ofxSimpleGuiColorPicker2::getValue(int i) {
	return (value)[i];
}

void ofxSimpleGuiColorPicker2::setValue(float f, int i) {
	if(f < min) f = min;
	else if(f > max) f = max;
	(value)[i] = f;
}

void ofxSimpleGuiColorPicker2::onPress(int x, int y, int button) {
	//if(!enabled) return;
	
	for (int i=0; i<1; i++) {
		//sliders[i]->onPress(x,y,button);
	}
}

void ofxSimpleGuiColorPicker2::onRelease(int x, int y, int button) {
	//if(!enabled) return;
	
	for (int i=0; i<1; i++) {
		//sliders[i]->onRelease(x,y,button);
	}
}

void ofxSimpleGuiColorPicker2::onDragOver(int x, int y, int button) {
	//if(!enabled) return;
	
	for (int i=0; i<1; i++) {
		//sliders[i]->onDragOver(x,y,button);
	}
}

void ofxSimpleGuiColorPicker2::onDragOutside(int x, int y, int button) {
	//if(!enabled) return;
	
	for (int i=0; i<1; i++) {
		//sliders[i]->onDragOutside(x,y,button);
	}
}

void ofxSimpleGuiColorPicker2::onJoystick(float x, float y, int id) {
	if(!enabled) return;
	sliders[focus]->onJoystick(x,y,id);
}

void ofxSimpleGuiColorPicker2::onKeyRight() {
	if(!enabled) return;
	sliders[focus]->onKeyRight();
}

void ofxSimpleGuiColorPicker2::onKeyLeft() {
	if(!enabled) return;
	
	//for (int i=0; i<4; i++) {
		sliders[focus]->onKeyLeft();
	//}
}

void ofxSimpleGuiColorPicker2::update() {
	//if(!enabled) return;
	
	for (int i=0; i<4; i++) {
		sliders[i]->update();
	}
}

void ofxSimpleGuiColorPicker2::draw(float x, float y) {
	setPos(x, y);
	title->draw(x,y);
	for (int i=0; i<4; i++) {
		sliders[i]->draw(x,y+(i+1)*(sliders[i]->height + 1));
	}
}

void ofxSimpleGuiColorPicker2::_mouseMoved(ofMouseEventArgs &e) {
	for(int i=0; i<4; i++)
		sliders[i]->_mouseMoved(e);
}

void ofxSimpleGuiColorPicker2::_mousePressed(ofMouseEventArgs &e) {
	for(int i=0; i<4; i++)
		sliders[i]->_mousePressed(e);
}

void ofxSimpleGuiColorPicker2::_mouseDragged(ofMouseEventArgs &e) {
	for(int i=0; i<4; i++)
		sliders[i]->_mouseDragged(e);
}

void ofxSimpleGuiColorPicker2::_mouseReleased(ofMouseEventArgs &e) {
	for(int i=0; i<4; i++)
		sliders[i]->_mouseReleased(e);
}

void ofxSimpleGuiColorPicker2::_keyReleased(ofKeyEventArgs &e) {
	for(int i=0; i<4; i++)
		sliders[i]->_keyReleased(e);
}

void ofxSimpleGuiColorPicker2::_keyPressed(ofKeyEventArgs &e) {
	for(int i=0; i<4; i++)
		sliders[i]->_keyPressed(e);
}

/** returns true if control focused an control internally */
bool ofxSimpleGuiColorPicker2::focusNextControl(){	
	for(int i=0; i<4; i++)
		sliders[i]->focused = false;
	
	if (!focused) {
		focused = true;
		focus = 0;
		sliders[focus]->focused = true;
		return true;
	}
	
	if (focus < 3) {
		focus++;
		sliders[focus]->focused = true;
		return true;
	} else {
		return false;
	}
}

/** returns true if control focused an control internally */
bool ofxSimpleGuiColorPicker2::focusPrevControl(){
	for(int i=0; i<4; i++)
		sliders[i]->focused = false;
	
	if (!focused) {
		focused = true;
		focus = 3;
		sliders[focus]->focused = true;
		return true;
	}
	
	if (focus > 0) {
		focus--;
		sliders[focus]->focused = true;
		return true;
	} else {
		return false;
	}
}

void ofxSimpleGuiColorPicker2::setFocus(bool newFocus){
	if (focused != newFocus) {
		focused = newFocus;
		if(focused){
			for(int i=0; i<4; i++)
				sliders[i]->focused = false;
			focus = 0;
			sliders[focus]->focused = true;
		}
	}
}
