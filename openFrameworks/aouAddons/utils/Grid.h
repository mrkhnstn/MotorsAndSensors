#pragma once

#include "ofMain.h"
#include "ofxSimpleGuiToo.h"

class Grid : public ofRectangle {
	
public: 
	
	int cols; //< columns: has to be minimum 1
	int rows; //< rows: has to be minium 1
	
	Grid(){
		x = 0;
		y = 0;
		width = 256;
		height = 256;
		cols = 8;
		rows = 8;
	}
	
	/**
	 returns horizontal span between grid columns
	 */
	float getColSpan(){
		return width / (cols-1);
	}
	
	/**
	 returns horizontal span between grid columns
	 */	
	float getRowSpan(){
		return height / (rows-1);
	}
	
	/**
	 returns a coordinate of the intersection between entred c
	 olumn and row 
	 no boundary checking!!!
	 */
	ofPoint getPoint(int col, int row){
		return ofPoint(x + col * getColSpan(),y + row * getRowSpan());
	}
	
	/**
	 returns number of intersections
	 */
	int getPointCount(){
		return cols * rows;
	}
	
	void setupGUI(){
		gui.addSlider("cols",cols,1,128);
		gui.addSlider("rows",rows,1,128);
	}
	
	void draw(){
		ofRect(x,y,width,height);
		
		float colSpan = getColSpan();
		float rowSpan = getRowSpan();
		
		// draw vertical lines
		for (int col=1; col<cols-1; col++) {
			float tempX = x + col * colSpan;
			ofLine(tempX, y, tempX, y + height);
		}
		// draw horizontal lines
		for (int row=1; row<rows-1; row++) {
			float tempY = y + row * rowSpan;
			ofLine(x, tempY, x + width, tempY);
		}
		
	}
	
};
