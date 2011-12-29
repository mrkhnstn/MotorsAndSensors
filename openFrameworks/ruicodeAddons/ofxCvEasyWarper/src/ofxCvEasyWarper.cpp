/* 
 * Copyright (c) 2009, Rui Madeira
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * http://creativecommons.org/licenses/LGPL/2.1/
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */


#include "ofxCvEasyWarper.h"

ofxCvEasyWarper::ofxCvEasyWarper(){
	cornerHitId = -1;
	bEventsAdded = false;
	imageWidth = 0;
	imageHeight = 0;
	bEnabled = false;
	enable();
	setFileName();
	scaleX = scaleY = 1.0f;
}

void ofxCvEasyWarper::setup(ofxCvImage*_sourceImg, ofxCvImage*_destImg){
	sourceImg = _sourceImg;
	destImg = _destImg;
	
	imageWidth = sourceImg->width;
	imageHeight = destImg->height;
	
	reset();
}

void ofxCvEasyWarper::warp(){
	destImg->warpIntoMe(*sourceImg, sourcePts, destPts);
}

void ofxCvEasyWarper::draw(float x, float y){
	ofNoFill();
	imgOffSet.x = x;
	imgOffSet.y = y;
	glPushMatrix();
	glTranslatef(x, y, 0);
	for(int i=0; i<4; i++){
		if(i== cornerHitId)glColor3f(1.0f, 0.0f, 0.0f);
		else glColor3f(0.0f, 1.0f, 0.0f);
		ofCircle(sourcePts[i].x, sourcePts[i].y, CORNER_RADIUS);
	}
	glColor3f(0.0f, 1.0f, 0.0f);
	ofLine(sourcePts[0].x, sourcePts[0].y, sourcePts[1].x, sourcePts[1].y);
	ofLine(sourcePts[1].x, sourcePts[1].y, sourcePts[2].x, sourcePts[2].y);
	ofLine(sourcePts[2].x, sourcePts[2].y, sourcePts[3].x, sourcePts[3].y);
	ofLine(sourcePts[3].x, sourcePts[3].y, sourcePts[0].x, sourcePts[0].y);
	glPopMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	scaleX = scaleY = 1.0f;
}

void ofxCvEasyWarper::draw(float x, float y, float w, float h){
	if(!bEnabled)return;
	if( w == imageWidth and h == imageHeight){
		draw(x, y);
		return;
	}
	scaleX = w / imageWidth; 
	scaleY = h / imageHeight;
	ofNoFill();
	imgOffSet.x = x;
	imgOffSet.y = y;
	glPushMatrix();
	glTranslatef(x, y, 0);
	for(int i=0; i<4; i++){
		if(i== cornerHitId)glColor3f(1.0f, 0.0f, 0.0f);
		else glColor3f(0.0f, 1.0f, 0.0f);
		ofCircle(sourcePts[i].x*scaleX, sourcePts[i].y*scaleY, CORNER_RADIUS);
	}
	glColor3f(0.0f, 1.0f, 0.0f);
	ofLine(sourcePts[0].x*scaleX, sourcePts[0].y*scaleY, sourcePts[1].x*scaleX, sourcePts[1].y*scaleY);
	ofLine(sourcePts[1].x*scaleX, sourcePts[1].y*scaleY, sourcePts[2].x*scaleX, sourcePts[2].y*scaleY);
	ofLine(sourcePts[2].x*scaleX, sourcePts[2].y*scaleY, sourcePts[3].x*scaleX, sourcePts[3].y*scaleY);
	ofLine(sourcePts[3].x*scaleX, sourcePts[3].y*scaleY, sourcePts[0].x*scaleX, sourcePts[0].y*scaleY);
	glPopMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
}

void ofxCvEasyWarper::save(){
	xml.clear();
	string tag;
	for(int i=0; i<4; i++){
		tag = "point" + ofToString(i, 0);
		xml.addTag(tag);
		xml.pushTag(tag);
		xml.addValue("x", sourcePts[i].x);
		xml.addValue("y", sourcePts[i].y);
		xml.popTag();
	}
	xml.saveFile(fileName);
}

void ofxCvEasyWarper::load(){
	xml.loadFile(fileName);
	string tag;
	for(int i=0; i<4; i++){
		tag = "point" + ofToString(i, 0);
		sourcePts[i].x = xml.getValue(tag + ":x", 0);
		sourcePts[i].y = xml.getValue(tag + ":y", 0);
	}
	
}

void ofxCvEasyWarper::reset(){
	sourcePts[0].set(0, 0);
	sourcePts[1].set(imageWidth, 0);
	sourcePts[2].set(imageWidth, imageHeight);
	sourcePts[3].set(0, imageHeight);
	
	for(int i=0; i<4; i++){
		destPts[i].x = sourcePts[i].x;
		destPts[i].y = sourcePts[i].y;
	}
}

void ofxCvEasyWarper::enable(){
	if(bEnabled) return;
	addEvents();
	bEnabled = true;
}

void ofxCvEasyWarper::disable(){
	if(!bEnabled) return;
	removeEvents();
	bEnabled = false;
}

void ofxCvEasyWarper::onMousePressed(ofMouseEventArgs& mouseArgs){
	cornerHitId = checkCornerHit(mouseArgs.x, mouseArgs.y);
	if(cornerHitId > -1){
		sourcePts[cornerHitId].x = (mouseArgs.x - imgOffSet.x)/scaleX;
		sourcePts[cornerHitId].y = (mouseArgs.y - imgOffSet.y)/scaleY;
	}
}

void ofxCvEasyWarper::onMouseReleased(ofMouseEventArgs& mouseArgs){
	cornerHitId = -1;
}

void ofxCvEasyWarper::onMouseDragged(ofMouseEventArgs& mouseArgs){
	if(cornerHitId > -1){
		sourcePts[cornerHitId].x = (mouseArgs.x - imgOffSet.x)/scaleX;
		sourcePts[cornerHitId].y = (mouseArgs.y - imgOffSet.y)/scaleY;
	}
}

void ofxCvEasyWarper::removeEvents(){
	if(bEventsAdded){
		ofRemoveListener(ofEvents.mousePressed, this, &ofxCvEasyWarper::onMousePressed);
		ofRemoveListener(ofEvents.mouseReleased, this, &ofxCvEasyWarper::onMouseReleased);
		ofRemoveListener(ofEvents.mouseDragged, this, &ofxCvEasyWarper::onMouseDragged);
		bEventsAdded = false;
	}
}

void ofxCvEasyWarper::addEvents(){
	if(!bEventsAdded){
		ofAddListener(ofEvents.mousePressed, this, &ofxCvEasyWarper::onMousePressed);
		ofAddListener(ofEvents.mouseReleased, this, &ofxCvEasyWarper::onMouseReleased);
		ofAddListener(ofEvents.mouseDragged, this, &ofxCvEasyWarper::onMouseDragged);
		bEventsAdded = true;
	}
}

void ofxCvEasyWarper::setFileName(string _fileName){
	fileName = _fileName;
}

int ofxCvEasyWarper::checkCornerHit(float x, float y){
	float dx, dy;
	float cornerRadiusSQ = CORNER_RADIUS*CORNER_RADIUS;
	for(int i=0; i<4; i++){
		dx = x - sourcePts[i].x*scaleX - imgOffSet.x;
		dy = y - sourcePts[i].y*scaleY - imgOffSet.y;
		if(dx*dx + dy*dy < cornerRadiusSQ) return i;
	}
	return -1;
}
