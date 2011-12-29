#pragma once
/*
 *  FboUtils.h
 *  warp
 *
 *  Created by Robin Beitra (robin@beitra.net) on 25/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <vector>

#include "ofMain.h"
#include "ofxVec2f.h"

#include "Utils.h"

static bool checkShiftKey(){
	//s
	return (glutGetModifiers() & GLUT_ACTIVE_SHIFT);
};
static bool checkControlKey(){
	return (glutGetModifiers() & GLUT_ACTIVE_CTRL);
};
static bool checkAltKey(){
	return (glutGetModifiers() & GLUT_ACTIVE_ALT);
};



static ofxVec2f catmulRom(ofxVec2f& p0, ofxVec2f& p1, ofxVec2f& p2, ofxVec2f& p3, float t){
	return ((2*p1) + 
			(-1*p0 + p2)*t +
			(2*p0 - 5*p1 + 4*p2 - p3)*t*t +
			(-1*p0 + 3*p1 - 3*p2 + p3)*t*t*t
			)*0.5;
//	q(t) = 0.5 *(    	(2 * P1) +
//				 (-P0 + P2) * t +
//				 (2*P0 - 5*P1 + 4*P2 - P3) * t2 +
//				 (-P0 + 3*P1- 3*P2 + P3) * t3)
}


//values between 0 and 1 return values between 0 and ∞
//f(0)   = 0
//f(0.5) = 1;
//f(1)   = ∞
//maybe also try tan(0.5*PI*x)
static float niceRange(float x){
	return x/(1.0 - x);
};

static bool isPowerOfTwo(int x){
	return (x&(x - 1)) == 0;
}

static void glPremultiplyMatrix(GLfloat* m, GLenum mode = GL_PROJECTION_MATRIX){
	GLfloat p[16];
	glGetFloatv(mode, p);//get the current matrix
	glLoadMatrixf(p);
	glMultMatrixf(m);
}
static void glPremultiplyCurrentMatrix(GLfloat* m){
	GLfloat p[16];
	GLint mode;
	glGetIntegerv(GL_MATRIX_MODE, &mode);
	glGetFloatv(mode, p);//get the current matrix
	glLoadMatrixf(m);
	glMultMatrixf(p);
}

static void glTranslateMatrix(GLfloat x, GLfloat y, GLfloat z, GLfloat* m){
	m[0] = 1;	m[4] = 0;	m[8] = 0;	m[12] = x;
	m[1] = 0;	m[5] = 1;	m[9] = 0;	m[13] = y;
	m[2] = 0;	m[6] = 0;	m[10] = 1;	m[14] = z;
	m[3] = 0;	m[7] = 0;	m[11] = 0;	m[15] = 1;
}
static void glIdentityMatrix(GLfloat* m){
	m[0] = 1;	m[4] = 0;	m[8] = 0;	m[12] = 0;
	m[1] = 0;	m[5] = 1;	m[9] = 0;	m[13] = 0;
	m[2] = 0;	m[6] = 0;	m[10] = 1;	m[14] = 0;
	m[3] = 0;	m[7] = 0;	m[11] = 0;	m[15] = 1;
}
static void glZeroMatrix(GLfloat* m){
	m[0] = 0;	m[4] = 0;	m[8] = 0;	m[12] = 0;
	m[1] = 0;	m[5] = 0;	m[9] = 0;	m[13] = 0;
	m[2] = 0;	m[6] = 0;	m[10] = 0;	m[14] = 0;
	m[3] = 0;	m[7] = 0;	m[11] = 0;	m[15] = 0;
}

static bool compareRect(const ofRectangle& a, const ofRectangle& b){
	return
	a.x == b.x && a.y == b.y &&
	a.width == b.width && a.height == b.height;
};

static bool containsPoint(const ofRectangle& a, const ofxVec2f& b){
	return  b.x >= a.x && b.x <= a.x + a.width &&
	b.y >= a.y && b.y <= a.y + a.height;
}

static bool containsRect(const ofRectangle& a, const ofRectangle& b){
	return containsPoint(a, ofxVec2f(b.x, b.y)) && containsPoint(a, ofxVec2f(b.x+b.width, b.y+b.height));
}



static string stringifyVec2f(ofxVec2f v){
	return _S(v.x) + "," + _S(v.y);
};

static void loadStringIntoVec2f(string& vecString, ofxVec2f& vec){
	vector<string> parts;
	string delimiter = ",";
	tokenize(vecString, parts, delimiter, true);
	
	if(parts.size() >= 2){
		vec.x = atof(parts[0].c_str());
		vec.y = atof(parts[1].c_str());
	} else {
		vec.x = 0;
		vec.y = 0;
	}
};

static void loadStringIntoVec2fVec(string& gridString, vector<ofxVec2f>& grid){
	vector<string> parts;
	string delimiter = ",";
	tokenize(gridString, parts, delimiter, true);
	
	for(int i = 1; i < parts.size(); i+=2){
		float x = atof(parts[i-1].c_str());
		float y = atof(parts[i].c_str());
		
		ofxVec2f point;
		point.x = x;
		point.y = y;
		grid.push_back(point);
	}
};

static string stringifyVec2fVec(vector<ofxVec2f>& grid){
	string result = "";
	string delimiter = ",";
	
	for(int i = 0; i < grid.size(); ++i){
		ofxVec2f& point = grid[i];
		result = result + _S(point.x) + delimiter + _S(point.y) + delimiter;
	}
	return result;
};