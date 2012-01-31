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
	
	string modeChoices[SCENE_CTRL_NUM_MODES] = {"IDLE","INTERACTIVE","MANUAL","PLAYLIST_ONLY","INTERACTIVE_ONLY"};
	gui.addComboBox("Current_Mode", _mode, SCENE_CTRL_NUM_MODES, modeChoices);
	
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
	
	gui.addTitle("keyboard shortcuts").setNewColumn(true);
	gui.addTitle("playlist");
	gui.addButton("Shortcut_0", '1', this, &SceneCtrl::shortcut_1);
	gui.addButton("Shortcut_1", '2', this, &SceneCtrl::shortcut_2);
	gui.addButton("Shortcut_2", '3', this, &SceneCtrl::shortcut_3);
	gui.addButton("Shortcut_3", '4', this, &SceneCtrl::shortcut_4);
	gui.addButton("Shortcut_4", '5', this, &SceneCtrl::shortcut_5);
	gui.addButton("Shortcut_5", '6', this, &SceneCtrl::shortcut_6);
	gui.addButton("Shortcut_6", '7', this, &SceneCtrl::shortcut_7);
	gui.addButton("Shortcut_7", '8', this, &SceneCtrl::shortcut_8);
	gui.addButton("Shortcut_8", '9', this, &SceneCtrl::shortcut_9);
	gui.addButton("Shortcut_9", '0', this, &SceneCtrl::shortcut_0);
	
	/*
	gui.addTitle("modes");
	gui.addButton("Shortcut_8", 'i', this, &SceneCtrl::setToInteractiveOnlyMode);
	gui.addButton("Shortcut_9", 'p', this, &SceneCtrl::setToPlaylistOnlyMode);
	*/
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
	
	mode = SCENE_CTRL_PLAYLIST_ONLY;
	_mode = SCENE_CTRL_PLAYLIST_ONLY;
	
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
	
	
	// automatic mode switching (IDLE <-> INTERACTIVE)
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
			// if  no user in proximity for a while then switch back to idle mod
			if(ofGetElapsedTimef() > lastUserInProximityTime + switchToIdleTime){
				setMode(SCENE_CTRL_IDLE);
			}
			break;
		default:
			break;
	}	
	
	// if current scene is not playing (not enabled)
	if (!getCurrentScene().enabled) { 
		switch (mode) {
			case SCENE_CTRL_IDLE:
			case SCENE_CTRL_PLAYLIST_ONLY:
				startNextIdleScene(); // start next scene from idle list
				break;
			case SCENE_CTRL_INTERACTIVE:
			case SCENE_CTRL_INTERACTIVE_ONLY:
				startNextInteractiveScene(); // start next scene from interactive list
				break;
			case SCENE_CTRL_MANUAL: // restart same scene
				getCurrentScene().start();
			default:
				break;
		}
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
	if(sceneId < 0 || sceneId > scenes.size()) return;
	
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
		case SCENE_CTRL_PLAYLIST_ONLY:
			startNextIdleScene();
			break;
		case SCENE_CTRL_INTERACTIVE:
		case SCENE_CTRL_INTERACTIVE_ONLY:
			startNextInteractiveScene();
			break;
		case SCENE_CTRL_MANUAL:
			setCurrentScene(currentSceneId); // start current scene if not playing or restart if playing
			break;
		default:
			break;
	}
}

void SceneCtrl::setToInteractiveOnlyMode(ofEventArgs& e){
	setMode(SCENE_CTRL_INTERACTIVE_ONLY);
}

void SceneCtrl::setToPlaylistOnlyMode(ofEventArgs& e){
	setMode(SCENE_CTRL_PLAYLIST_ONLY);
}

bool SceneCtrl::userInProximity(){
	return Singleton<SensorCtrl>::instance()->userInProximity();
}

void SceneCtrl::setShortcutScene(int i){
	switch (mode) {
		case SCENE_CTRL_IDLE:
		case SCENE_CTRL_PLAYLIST_ONLY:
		case SCENE_CTRL_MANUAL: // restart same scene
			setCurrentScene(idleList[i]-1);
			idleListPos = i;
			break;
		case SCENE_CTRL_INTERACTIVE:
		case SCENE_CTRL_INTERACTIVE_ONLY:
			setCurrentScene(interactiveList[i]-1);
			interactiveListPos = i;
			break;
		default:
			break;
	}
}

void SceneCtrl::shortcut_1(ofEventArgs& e){
	setShortcutScene(0);
}

void SceneCtrl::shortcut_2(ofEventArgs& e){
	setShortcutScene(1);
}

void SceneCtrl::shortcut_3(ofEventArgs& e){
	setShortcutScene(2);
}

void SceneCtrl::shortcut_4(ofEventArgs& e){
	setShortcutScene(3);
}

void SceneCtrl::shortcut_5(ofEventArgs& e){
	setShortcutScene(4);
}

void SceneCtrl::shortcut_6(ofEventArgs& e){
	setShortcutScene(5);
}

void SceneCtrl::shortcut_7(ofEventArgs& e){
	setShortcutScene(6);
}

void SceneCtrl::shortcut_8(ofEventArgs& e){
	setShortcutScene(7);
}

void SceneCtrl::shortcut_9(ofEventArgs& e){
	setShortcutScene(8);
}

void SceneCtrl::shortcut_0(ofEventArgs& e){
	setShortcutScene(9);
}

