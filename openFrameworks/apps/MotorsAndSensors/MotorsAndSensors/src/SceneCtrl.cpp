/*
 *  SceneCtrl.cpp
 *  MotorsAndSensors
 *
 *  Created by Mark Hauenstein on 24/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "SceneCtrl.h"
#include "SensorCtrl.h"

////////////////////////////////////////////////////////////////

void SceneCtrl::setup(){
		
	for(int i=0; i<scenes.size(); i++)
		scenes[i]->setup();
	
	currentSceneId = 0;
	_currentSceneId = 0;
	
	mode = SCENE_CTRL_IDLE;
	_mode = SCENE_CTRL_IDLE;
	
	idleListPos = 0;
	for(int i=0; i<NUM_IDLE_ITEMS; i++)
		idleList[i] = 0;
	
	interactiveListPos = 0;
	for(int i=0; i<NUM_INTERACTIVE_ITEMS; i++)
		interactiveList[i] = 0;
	
	_userInProximity = false;
	lastUserInProximityTime = 0;
	switchToIdleTime = 10;
	
}

void SceneCtrl::setupGUI(){
	gui.page(1).addPageShortcut(gui.addPage("Scenes"));
	
	// add a combobox for manual scene selection
	string names[scenes.size()];
	for(int i=0; i<scenes.size(); i++){
		names[i] = scenes[i]->name;
	}
	
	
	
	gui.addComboBox("Current_Scene", _currentSceneId, scenes.size(), names);
	
	string modeChoices[3] = {"IDLE","INTERACTIVE","MANUAL"};
	gui.addComboBox("Current_Mode", _mode, 3, modeChoices);
	
	gui.addSlider("switchToIdleTime", switchToIdleTime, 0, 30);
	
	string idleListChoices[scenes.size()+1];
	idleListChoices[0] = "EMPTY";
	for(int i=0; i<scenes.size(); i++){
		idleListChoices[i+1] = scenes[i]->name;
	}
	
	gui.addTitle("Scenes");
	for(int i=0; i<scenes.size(); i++)
		scenes[i]->setupGUI();
	
	gui.setPage("Scenes");
	gui.addTitle("Idle_Playlist").setNewColumn(true);
	gui.addSlider("Idle_List_Pos", idleListPos, 0, NUM_IDLE_ITEMS-1);
	for (int i=0; i<NUM_IDLE_ITEMS; i++) {
		gui.addComboBox("Idle_"+ofToString(i), idleList[i], scenes.size()+1, idleListChoices);
	}
	
	gui.addTitle("Interactive_Playlist").setNewColumn(true);
	gui.addSlider("Pnteractive_List_Pos", interactiveListPos, 0, NUM_IDLE_ITEMS-1);
	for (int i=0; i<NUM_INTERACTIVE_ITEMS; i++) {
		gui.addComboBox("Interactive_"+ofToString(i), interactiveList[i], scenes.size()+1, idleListChoices);
	}
	
	
}

void SceneCtrl::postGUI(){
	for(int i=0; i<scenes.size(); i++)
		scenes[i]->postGUI();
	
	for(int i=0; i<NUM_IDLE_ITEMS; i++)
		idleList[i] = ofClamp(idleList[i], 0, scenes.size() + 1);
	
	for(int i=0; i<NUM_INTERACTIVE_ITEMS; i++)
		interactiveList[i] = ofClamp(interactiveList[i], 0, scenes.size() + 1);
	
	idleListPos = 0;
	interactiveListPos = 0;
	
	mode = SCENE_CTRL_IDLE;
	_mode = SCENE_CTRL_IDLE;
	
	currentSceneId = 0;
	_currentSceneId = 0;
	
	//getCurrentScene().start();
}

void SceneCtrl::update(){
	
	if (ofGetElapsedTimef() < 5) {
		return; // don't display any scenes
	}
	
	if (_mode != mode) {
		setMode(_mode);
	}
	
	// check currentSceneId against gui
	if(_currentSceneId != currentSceneId)
		setCurrentScene(_currentSceneId);
	
	// update user activity
	if(userInProximity()){
		lastUserInProximityTime = ofGetElapsedTimef();
	}
	
	switch (mode) {
		case SCENE_CTRL_IDLE:
			// if a user is in proximity then switch to interactive mode
			if(userInProximity())
				setMode(SCENE_CTRL_INTERACTIVE);
			break;
		case SCENE_CTRL_INTERACTIVE:
			if(ofGetElapsedTimef() > lastUserInProximityTime + switchToIdleTime){
				setMode(SCENE_CTRL_IDLE);
			}
			break;
		case SCENE_CTRL_MANUAL: 
		default:
			break;
	}	
	
	switch (mode) {
		case SCENE_CTRL_IDLE:
			// if a user is in proximity then switch to interactive mode
			if (!getCurrentScene().enabled) { // if current scene is not playing (not enabled)
				 startNextIdleScene();
			}
			break;
		case SCENE_CTRL_INTERACTIVE:
			if (!getCurrentScene().enabled) { // if current scene is not playing (not enabled)
				startNextInteractiveScene();
			}
			break;
		case SCENE_CTRL_MANUAL: // if current scene is not playing (not enabled) then restart it
			if (!getCurrentScene().enabled) {
				getCurrentScene().start();
			}		
		default:
			break;
	}
	
	getCurrentScene().update();
}

void SceneCtrl::draw(){}

void SceneCtrl::draw3d(){
	ofPushMatrix();
	ofRotate(-90, 1, 0, 0);
	getCurrentScene().draw();
	ofPopMatrix();
}

void SceneCtrl::exit(){
	//TODO: this is currently not being called on exiting!!
	for (int i=0; i<scenes.size(); i++) {
		delete scenes[i];
	}
	scenes.clear();
}

Scene& SceneCtrl::getCurrentScene(){
	return *scenes[currentSceneId];
}

void SceneCtrl::setCurrentScene(int sceneId){
	getCurrentScene().stop();
	currentSceneId = sceneId;
	_currentSceneId = sceneId;
	getCurrentScene().start();
}

void SceneCtrl::startNextInteractiveScene(){
	// find next scene to playback
	interactiveListPos = (interactiveListPos + 1) % NUM_INTERACTIVE_ITEMS;
	int whileCounter = 0; // to prevent endless loop if all play items empty
	
	while (interactiveList[interactiveListPos] == 0 && whileCounter < NUM_INTERACTIVE_ITEMS) {
		interactiveListPos = (interactiveListPos + 1) % NUM_INTERACTIVE_ITEMS;
		whileCounter++;
	}
	
	if(interactiveList[interactiveListPos] > 0) // if a non empty slot was found
		setCurrentScene(interactiveList[interactiveListPos]-1); // -1 because 0 corresponds to empty slot so scene ids are offset by 1
}

void SceneCtrl::startNextIdleScene(){
	// find next scene to playback
	idleListPos = (idleListPos + 1) % NUM_IDLE_ITEMS;
	int whileCounter = 0; // to prevent endless loop if all play items empty
	
	while (idleList[idleListPos] == 0 && whileCounter < NUM_IDLE_ITEMS) {
		idleListPos = (idleListPos + 1) % NUM_IDLE_ITEMS;
		whileCounter++;
	}
	
	if(idleList[idleListPos] > 0) // if a non empty slot was found
		setCurrentScene(idleList[idleListPos]-1); // -1 because 0 corresponds to empty slot so scene ids are offset by 1
}

void SceneCtrl::setMode(int newMode){
	mode = newMode;
	_mode = newMode;
	switch (mode) {
		case SCENE_CTRL_IDLE:
			startNextIdleScene();
			break;
		case SCENE_CTRL_INTERACTIVE:
			startNextInteractiveScene();
			break;
		case SCENE_CTRL_MANUAL:
			setCurrentScene(currentSceneId); // start current scene if not playing or restart if playing
			break;
		default:
			break;
	}
}

/*
void SceneCtrl::checkUserInProximity(){

	/*
	_userInProximity = false;
	vector<Motor*>& motors = Singleton<MotorCtrl>::instance()->motors;
	// reset all motor proximity udpated flag
	for(int i=0; i<motors.size(); i++){
		if(motors[i]->userInProximity()){
			_userInProximity = true;
			lastUserInProximityTime = ofGetElapsedTimef();
			break;
		}
	}
	*/

/*
	_userInProximity = false;
	vector<int> inProximity;
	for(int i=0; i<motors.size(); i++){
		if (motors[i]->userInProximity()) {
			
		}
	}
}
*/

bool SceneCtrl::userInProximity(){
	return Singleton<SensorCtrl>::instance()->userInProximity();
}