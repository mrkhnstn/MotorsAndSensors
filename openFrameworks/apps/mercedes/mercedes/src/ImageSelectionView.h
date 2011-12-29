#pragma once
/*
 *  ImageSelectionView.h
 *  TestBed
 *
 *  Created by Mark Hauenstein on 14/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Includes.h"

class ImageSelectionView : public UI::View {
public:
	
	UIElement cancelButton;
	UIElement helpTextSprite;
	
	void setup();
	void draw();	
	void onPress(int x, int y, int button); // used for selecting image

private:
	
	int cols;	// number of columns in selection grid
	int rows;	// number of rows in selection grid
	float imgW;	// width of single image in selection grid
	float imgH; // height of single image in selection grid
	int imgCount; // number of images in selection grid
	int gridFrames[9]; //TODO: 
};