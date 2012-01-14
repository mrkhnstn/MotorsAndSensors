/*
 *  AverageF.cpp
 *  mercedes
 *
 *  Created by Mark Hauenstein on 03/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "AverageF.h"

AverageF::AverageF(){
	size = 0;
	values = NULL;
	setSize(4);
}

AverageF::~AverageF(){
	delete[] values;
}

void AverageF::addValue(float f){
	sum -= values[index];
	index = (index + 1) % size;
	values[index] = f;
	sum += f;
	average = sum / size;
}

float AverageF::getAverage()
{
	return average;
}

int AverageF::getSize(){
	return size;
}

void AverageF::setSize(int i){
	
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
