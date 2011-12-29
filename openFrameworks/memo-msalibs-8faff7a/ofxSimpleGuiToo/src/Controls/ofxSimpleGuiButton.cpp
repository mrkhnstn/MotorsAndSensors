
#include "ofxSimpleGuiButton.h"

ofxSimpleGuiButton::ofxSimpleGuiButton(string name) : ofxSimpleGuiValueControl<bool>(name, selfValue) {
	beToggle	= false;
	beenPressed = false;
	selfValue = false;
	controlType = "Button";
	setup();
}

ofxSimpleGuiButton::ofxSimpleGuiButton(string name, bool &value) : ofxSimpleGuiValueControl<bool>(name, value) {
	beToggle	= false;
	beenPressed = false;
	selfValue = false;
	controlType = "Button";
	setup();
}

void ofxSimpleGuiButton::setup() {
	setSize(config->gridSize.x - config->padding.x, config->buttonHeight);
}

void ofxSimpleGuiButton::loadFromXML(ofxXmlSettings &XML) {
	if(includeInXml)
		setValue(XML.getValue(controlType + "_" + key + ":value", 0));
}

void ofxSimpleGuiButton::saveToXML(ofxXmlSettings &XML) {
	if(includeInXml){
		XML.addTag(controlType + "_" + key);
		XML.pushTag(controlType + "_" + key);
		XML.addValue("name", name);
		XML.addValue("value", getValue());
		XML.popTag();
	}
}

void ofxSimpleGuiButton::keyPressed( int key ) {
	if(key==keyboardShortcut) toggle();
}



bool ofxSimpleGuiButton::getValue() {
	return (*value);
}

void ofxSimpleGuiButton::setValue(bool b) {
	(*value) = b;
}

void ofxSimpleGuiButton::toggle() {
	(*value) = !(*value); 
}

void ofxSimpleGuiButton::setToggleMode(bool b) {
	beToggle = b;
}

void ofxSimpleGuiButton::onPress(int x, int y, int button) {
	beenPressed = true;	
	if(beToggle) (*value) = !(*value); 
	else (*value) = true;
	
	ofNotifyEvent(onPressed, e, this);
}

void ofxSimpleGuiButton::onRelease(int x, int y, int button) {
	if(!beToggle) (*value) = false;
	
	ofNotifyEvent(onReleased, e, this);
}

void ofxSimpleGuiButton::onKeyEnter() {
	onPress(0, 0, 0);
}

void ofxSimpleGuiButton::onKeyRight() {
	onPress(0, 0, 0);
}

void ofxSimpleGuiButton::onKeyLeft() {
	onPress(0, 0, 0);
}

void ofxSimpleGuiButton::draw(float x, float y) {
	setPos(x, y);
	
	glPushMatrix();
	glTranslatef(x, y, 0);
	
	ofEnableAlphaBlending();
	ofFill();
	setTextBGColor();
	ofRect(0, 0, width, height);
	
	// if a toggle
	if((*value) && beToggle) {
		setTextColor();
		//ofLine(0, 0, box.width, box.height);
		//ofLine(box.width, 0, 0, box.height);
	}
	
	setTextColor();
	ofDrawBitmapString(name, 3, 15);
	
	ofDisableAlphaBlending();
	
	glPopMatrix();
}
