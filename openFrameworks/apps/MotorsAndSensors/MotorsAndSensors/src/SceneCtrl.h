#pragma once
/*
 *  SceneCtrl.h
 *  MotorsAndSensors
 *
 *  Created by Mark Hauenstein on 24/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Includes.h"
#include "Scene.h"

#define NUM_IDLE_ITEMS		10
#define NUM_INTERACTIVE_ITEMS	10

#define SCENE_CTRL_IDLE				0	// iterate through idleList scenes
#define SCENE_CTRL_INTERACTIVE		1	// iterate through interactive scenes
#define SCENE_CTRL_MANUAL			2	// only play current scene
#define SCENE_CTRL_PLAYLIST_ONLY	3	// play playlist without switching to interactive mode
#define SCENE_CTRL_INTERACTIVE_ONLY 4	// play only interactive scene
#define SCENE_CTRL_NUM_MODES		5


class SceneCtrl : public ofxBaseObject {

public:
	
	vector<Scene*> scenes;
	
	int idleList[NUM_IDLE_ITEMS]; // contains the ids of scenes to playback in a loop when there is no user interaction
	int idleListPos;
	
	int interactiveList[NUM_INTERACTIVE_ITEMS]; // contains the ids of scenes to use when there is user interaction
	int interactiveListPos;
	
	int currentSceneId;		// currently active scene
	int _currentSceneId;	// currently active scene used by gui
	
	int mode;				// see SCENE_CTRL_IDLE, SCENE_CTRL_INTERACTIVE, SCENE_CTRL_MANUAL
	int _mode;				// mode used by gui
	
	void setup();
	void setupGUI();
	void postGUI();
	void update();
	void draw();
	void draw3d();
	void exit();
	
	Scene& getCurrentScene();
	void setCurrentScene(int sceneId);
	void startNextInteractiveScene();
	void startNextIdleScene();
	void setMode(int newMode);
	void setToInteractiveOnlyMode(ofEventArgs& e);
	void setToPlaylistOnlyMode(ofEventArgs& e);
	
	//void checkUserInProximity();
	bool userInProximity();
	bool _userInProximity;
	float lastUserInProximityTime;
	float switchToIdleTime;

	void setShortcutScene(int i);	
	void shortcut_1(ofEventArgs& e);
	void shortcut_2(ofEventArgs& e);
	void shortcut_3(ofEventArgs& e);
	void shortcut_4(ofEventArgs& e);
	void shortcut_5(ofEventArgs& e);
	void shortcut_6(ofEventArgs& e);
	void shortcut_7(ofEventArgs& e);
	void shortcut_8(ofEventArgs& e);
	void shortcut_9(ofEventArgs& e);
	void shortcut_0(ofEventArgs& e);
};
