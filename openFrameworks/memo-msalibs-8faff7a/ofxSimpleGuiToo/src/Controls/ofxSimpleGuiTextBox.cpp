/*
 *  ofxSimpleGuiTextBox.cpp
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 06/05/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxSimpleGuiTextBox.h"

ofTrueTypeFont	ofxSimpleGuiTextBox::font;

ofxSimpleGuiTextBox::ofxSimpleGuiTextBox(string name, float height) : ofxSimpleGuiControl(name) {
	beToggle	= false;
	beenPressed = false;
	isOpen = true;
	//		this->value	= &value;
	value		= NULL;
	controlType = "Title";
	//		newColumn	= true;
	
	if(height == 0) height = config->titleHeight;
	if(hasTitle == false) height/=2;
	
	if (!font.bLoadedOk) {
		//font.loadFont("AndaleMondo.ttf", 10);
	}
	
	
	
	textBlock.init("AndaleMono.ttf",8);
	textBlock.setText(name);
	textBlock.wrapTextX(config->gridSize.x - 1 * config->padding.x-3);
		
	closedHeight = height;
	expandedHeight = textBlock.getHeight() + config->padding.y;
	setSize(config->gridSize.x - config->padding.x, expandedHeight);	
	
	fbo = NULL;
	
	setup();
}

void ofxSimpleGuiTextBox::setup() {
}

void ofxSimpleGuiTextBox::loadFromXML(ofxXmlSettings &XML) {
	if(!value) return;
	setValue(XML.getValue(controlType + "_" + key + ":value", 0));
}

void ofxSimpleGuiTextBox::saveToXML(ofxXmlSettings &XML) {
	if(!value) return;
	XML.addTag(controlType + "_" + key);
	XML.pushTag(controlType + "_" + key);
	XML.addValue("name", name);
	XML.addValue("value", getValue());
	XML.popTag();
}

bool ofxSimpleGuiTextBox::getValue() {
	if(!value) return false;
	return (*value);
}

void ofxSimpleGuiTextBox::setValue(bool b) {
	if(!value) return;
	(*value) = b;
}

void ofxSimpleGuiTextBox::toggle() {
	if(!value) return;
	(*value) = !(*value); 
}

void ofxSimpleGuiTextBox::setToggleMode(bool b) {
	if(!value) return;
	beToggle = b;
}

void ofxSimpleGuiTextBox::onPress(int x, int y, int button) {
	isOpen = !isOpen;
	if(!value) return;
	beenPressed = true;	
	if(beToggle) (*value) = !(*value); 
	else (*value) = true;
}

void ofxSimpleGuiTextBox::onRelease(int x, int y, int button) {
	if(!value) return;
	if(!beToggle) (*value) = false;
}

void ofxSimpleGuiTextBox::draw(float x, float y) {
	setPos(x, y);
	
	if (isOpen) {
		setSize(config->gridSize.x - config->padding.x, expandedHeight);	
	} else {
		setSize(config->gridSize.x - config->padding.x, closedHeight);		
	}
	
	if (fbo == NULL) {
		fbo = new ofxFBOTexture();
		fbo->allocate(textBlock.getWidth(),textBlock.getHeight()+5);
		fbo->begin();
		fbo->clear(0,0,0,1);
		//glColor4f(0,0,0,1);
		//setTextBGColor(false);
		ofSetColor(config->textBGColor);
		ofRect(0, 0, fbo->getWidth(), fbo->getHeight());
		textBlock.setColor(255, 255, 255, 255);
		textBlock.draw(0, 0);
		fbo->end();
	}	
	
	if(hasTitle == false) return;
	
	glPushMatrix();
	glTranslatef(x, y, 0);
	
	ofEnableAlphaBlending();
	ofFill();
			
	
	//		ofSetColor(0, 0, 0);

	//setTextBGColor(false);
	ofSetColor(config->textBGColor);
	ofRect(0, 0, width, height);
	
	//setTextColor(value != NULL);
	
	
	/*
	if(isOpen){
		ofDrawBitmapString(name, 3, 15);
	} else {
		ofDrawBitmapString(name+" ...", 3, 15);
	}
	*/
	
	ofDisableAlphaBlending();
	ofSetColor(255, 255, 255);
	if (isOpen) {
		fbo->draw(3,0);
	} else {
		//fbo->draw(3,0,fbo->getWidth(),height);
		
		glPushMatrix(); 
		
		glTranslatef(3,0,0.0f);
		
		GLfloat px0 = 0;
		GLfloat py0 = 0;
		GLfloat px1 = fbo->getWidth();
		GLfloat py1 = height - 3;
		
		GLfloat tx0 = 0;
		GLfloat ty0 = fbo->getHeight();
		GLfloat tx1 = fbo->getWidth();
		GLfloat ty1 = fbo->getHeight() - height +3;
		
		GLfloat tex_coords[] = {
			tx0,ty0,
			tx1,ty0,
			tx1,ty1,
			tx0,ty1
		};
		GLfloat verts[] = {
			px0,py0,
			px1,py0,
			px1,py1,
			px0,py1
		};
		fbo->bind();
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		glTexCoordPointer(2, GL_FLOAT, 0, tex_coords );
		glEnableClientState(GL_VERTEX_ARRAY);		
		glVertexPointer(2, GL_FLOAT, 0, verts );
		glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );
		fbo->unbind();
		glPopMatrix();
		
	}
	glPopMatrix();
	
	//glScissor(x, y, width, height);
	//glEnable(GL_SCISSOR_TEST);
	
	//glScissor(0, 0, ofGetWidth(), ofGetHeight());
	//glDisable(GL_SCISSOR_TEST);
	
	ofDisableAlphaBlending();
}

void ofxSimpleGuiTextBox::onKeyEnter() {
	onPress(0, 0, 0);
}

void ofxSimpleGuiTextBox::onKeyRight() {
	onPress(0, 0, 0);
}

void ofxSimpleGuiTextBox::onKeyLeft() {
	onPress(0, 0, 0);
}