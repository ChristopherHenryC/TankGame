#pragma once

//----------------------------------------------------------------------------------
// Main body of the app which contains the emitters and other elements that make 
//	up the game, and calls their update() and draw() methods
//	it also handles key press inputs to move and fire the tank, as well as start the game
// Chris Conway 3/31/2019

#include "ofMain.h"
#include "ofxGui.h"
#include "Sprite.h"
#include "ParticleSpitter.h"


typedef enum { MoveStop, MoveLeft, MoveRight, MoveUp, MoveDown } MoveDir;

class Emitter;

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();
	void checkCollisions();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	void updateScore(int n);

	Emitter *emitter;
	Emitter *invaders;
	Emitter *invaders2;

	ofImage defaultImage;
	ofImage emitterImage;
	ofImage invaderImage;
	ofImage invaderLaser;
	ofImage alium;
	ofImage alium2;
	ofSoundPlayer hit;

	ofVec3f mouse_last;
	bool imageLoaded;

	bool bHide;
	bool appStarted; // flase when the game has not begun running, true when it has
	int score; //keeps track of score
	int level;
	bool gameOver;
	ParticleSpitter spit;
	ofxLabel screenSize;
	ofxPanel gui;
};
