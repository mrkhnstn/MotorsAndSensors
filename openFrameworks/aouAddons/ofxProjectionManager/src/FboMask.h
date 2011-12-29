#pragma once
/*
 *  FboMask.h
 *  warp
 *
 *  Created by Robin Beitra (robin@beitra.net) on 23/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"
#include "ofxSimpleGuiToo.h"
#include "ofxFBOTexture.h"
#include "ofxVec2f.h"
#include "ofxXmlSettings.h"
#include "FboMaskSelection.h"
#include "FboMaskShape.h"
#include "FboUtils.h"

/*
 TODO: draw into gamma mask fbos
 */

class FboMask{
	
public:
	
	vector<FboMaskShape> shapes;
	
	void draw(bool editing, bool invert);
	
	void drawSelection(FboMaskSelection& selection, bool active, bool mouseOver);
	
	void shiftSelection(FboMaskSelection& selection, float dx, float dy);
	
	FboMaskSelection getNearestPoint(ofxVec2f& target);
	
	void insertShape(FboMaskSelection& selection);
	
	void insertPoint(FboMaskSelection& selection);
	
	void replacePoint(FboMaskSelection& selection);
	
	void deletePoint(FboMaskSelection& selection);

	bool isValidShape(FboMaskSelection& selection);
	
	void save(string path);
	
	void load(string path);
	
};
