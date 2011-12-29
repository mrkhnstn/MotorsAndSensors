/*
 *  ofxArgs.h
 *  graffitiWall_TUIO
 *
 *  Created by Pat Long on 10/02/09.
 *  Copyright 2009 Tangible Interaction. All rights reserved.
 *
 */
#ifndef _OFXARGS
#define _OFXARGS

#include <iostream>
#include <vector>
#include <map>
using namespace std;

class ofxArgs{
private:
	int argc;
	vector<string> args;
	map<string,string> opts;
	
	int strToInt(string value);
	float strToFloat(string value);
	
public:
	ofxArgs(int argc, char* argv[]);
	
	int getCount();
	
	bool contains(string key);

	float getFloat(string key, float def = 0.0f);
	int getInt(string key, int def = 0);
	string getString(string key, string def = "");
	bool getBool(string key, bool def = false);
	
	float getFloat(int index, float def = 0.0f);
	int getInt(int index, int def = 0);
	string getString(int index, string def = "");
	bool getBool(int index, bool def = false);
	
	void printArgs();
	void printOpts();
};

#endif
