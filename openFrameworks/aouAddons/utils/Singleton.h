#pragma once
/*
 *  Singleton.h
 *  backdrop
 *
 *  Created by Robin Beitra (robin@beitra.net) on 03/03/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
//#include <iostream>

template <class T>
class Singleton{	
public:
	static T* instance(){
		static T* _instance = new T();
		return _instance;
	};
	
	static void setInstance(T* newInstance){
		static T* _instance = newInstance;
	}
	
};