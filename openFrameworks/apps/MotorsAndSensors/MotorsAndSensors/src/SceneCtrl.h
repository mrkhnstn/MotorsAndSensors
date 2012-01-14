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

#define NUM_PLAYLIST_ITEMS		10
#define NUM_INTERACTIVE_ITEMS	3

#define SCENE_CTRL_IDLE			0 // don't update any scene
#define SCENE_CTRL_PLAYLIST		1 // iterate through playlist scenes
#define SCENE_CTRL_INTERACTIVE	2 // iterate through interactive scenes
#define SCENE_CTRL_MANUAL		3 // only play current scene

class SceneCtrl : public ofxBaseObject {

public:
	
	vector<Scene*> scenes;
	
	int playlist[NUM_PLAYLIST_ITEMS]; // contains the ids of scenes to playback in a loop when there is no user interaction
	int playlistIndex;
	int interactivePlaylist[NUM_INTERACTIVE_ITEMS]; // contains the ids of scenes to use when there is user interaction
	
	int currentSceneId; // currently active scene
	int mode; // see SCENE_CTRL_IDLE, SCENE_CTRL_PLAYLIST, SCENE_CTRL_INTERACTIVE, SCENE_CTRL_MANUAL
	
	void setup();
	void setupGUI();
	void postGUI();
	void update();
	void draw();
	void exit();
	
	Scene& getCurrentScene();
	void setCurrentScene(int sceneId);

};
