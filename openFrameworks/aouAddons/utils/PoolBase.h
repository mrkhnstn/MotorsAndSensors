#pragma once
/*
 *  PoolBase.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 06/02/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"

template<class T> class PoolBase {
	
public:
	
	void update(){
		alive.clear();
		dead.clear();
		
		T* item;
		//for(vector<T*>::iterator it = all.begin(); it != all.end(); ++it){
		for (int i=0; i<all.size(); i++) {
			item = all[i];
			if(isAlive(item)){
				alive.push_back(item);
			} else {
				dead.push_back(item);
			}
		}
	};
	
	virtual bool isAlive(T* item) = 0;
	
	virtual vector<T*>& getAll(){
		return all;
	}
	
	virtual vector<T*>& getAlive(){
		return alive;
	}
	
	virtual vector<T*>& getDead(){
		return dead;
	}
	
	T* popAlive(){
		if(alive.size() == 0) return NULL;
		T* item = alive.back();
		alive.pop_back();
		dead.push_back(item); //!!
		return item;
	}
	
	T* popDead(){
		if(dead.size() == 0) return NULL;
		T* item = dead.back();
		dead.pop_back();
		alive.push_back(item); //!!
		return item;
	}
	
protected:
	
	vector<T*> all;
	vector<T*> alive;
	vector<T*> dead;
};

