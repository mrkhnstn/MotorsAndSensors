#pragma once
/*
 *  AverageF.h
 *  mercedes
 *
 *  Created by Mark Hauenstein on 03/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

class AverageF {
public:
	
	AverageF(){
		size = 0;
		values = NULL;
		setSize(4);
	}
	
	~AverageF(){
		delete[] values;
	}
	
	void addValue(float f){
		sum -= values[index];
		index = (index + 1) % size;
		values[index] = f;
		sum += f;
		average = sum / size;
	}
	
	float getAverage()
	{
		return average;
	}
	
	int getSize(){
		return size;
	}
	
	void setSize(int i){
		
		if(i <= 0) return;
		if(i == size) return;
		
		size = i;
		
		// delete previous array if exists
		if(values != NULL)
			delete[] values;
		
		// create new array
		values = new float[size];
		
		// reset all values to zero
		// TODO: transfer old values if exist
		for(int i=0; i<size; i++)
			values[i] = 0;
		
		index = 0;
		sum = 0;
		average = 0;
	}
	
protected:
	
	float* values;
	int index;
	int size;
	float sum;
	float average;
	
};

