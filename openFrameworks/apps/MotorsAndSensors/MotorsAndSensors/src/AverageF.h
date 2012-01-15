#pragma once
/*
 *  AverageF.h
 *  MotorsAndSensors
 *
 *  Created by Mark Hauenstein on 03/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

class AverageF {
public:
	
	AverageF();
	~AverageF();
	void addValue(float f);
	float getAverage();
	int getSize();
	void setSize(int i);
	
protected:
	
	float* values;
	int index;
	int size;
	float sum;
	float average;
	
};

