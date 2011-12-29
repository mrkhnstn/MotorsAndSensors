#pragma once
/*
 *  SceneCtrl.h
 *  mercedes
 *
 *  Created by Mark Hauenstein on 24/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Includes.h"
#include "Scene.h"

#include "TestScene1.h"

class SceneCtrl : public ofxBaseObject {

public:
	
	vector<Scene*> scenes;
	
	int currentSceneId;
	
	TestScene1 testScene;
	
	void setup(){
		
		//add scenes
		scenes.push_back(&testScene);
		
		for(int i=0; i<scenes.size(); i++)
			scenes[i]->setup();
		
		currentSceneId = 0;
	}
	
	void setupGUI(){
		for(int i=0; i<scenes.size(); i++)
			scenes[i]->setupGUI();
	}
	
	void postGUI(){
		for(int i=0; i<scenes.size(); i++)
			scenes[i]->postGUI();
	}
	
	void update(){
		//for(int i=0; i<scenes.size(); i++)
		//	scenes[i]->update();
		getCurrentScene().update();
	}
	
	void draw(){
		//for(int i=0; i<scenes.size(); i++)
		//	scenes[i]->draw();
		getCurrentScene().draw();
	}
	
	Scene& getCurrentScene(){
		return *scenes[currentSceneId];
	}
	
	void setCurrentScene(int sceneId)
	{
		if (currentSceneId != sceneId) {
			scenes[currentSceneId]->stop();
			currentSceneId = sceneId;
			scenes[currentSceneId]->stop();
		}
	}

};
