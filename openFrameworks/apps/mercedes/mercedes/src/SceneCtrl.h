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

#include "IdleScene.h"
#include "PalindromeScene.h"
#include "StepScene.h"
#include "SimpleInteractiveScene.h"
#include "TestScene1.h"

class SceneCtrl : public ofxBaseObject {

public:
	
	vector<Scene*> scenes;
	
	int currentSceneId;
	
	void setup(){
		
		//add scenes
		scenes.push_back(new IdleScene());
		scenes.push_back(new PalindromeScene());
		scenes.push_back(new StepScene());
		scenes.push_back(new SimpleInteractiveScene());
		//scenes.push_back(new TestScene1());
		
		for(int i=0; i<scenes.size(); i++)
			scenes[i]->setup();
		
		currentSceneId = 0;
	}
	
	void setupGUI(){
		gui.page(1).addPageShortcut(gui.addPage("Scenes"));
		
		// add a combox for manual scene selection
		string names[scenes.size()];
		for(int i=0; i<scenes.size(); i++){
			names[i] = scenes[i]->name;
		}
		gui.addComboBox("scene", currentSceneId, scenes.size(), names);
		
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
	
	void exit(){
		for (int i=0; i<scenes.size(); i++) {
			delete scenes[i];
		}
		scenes.clear();
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
