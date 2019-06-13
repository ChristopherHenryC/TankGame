//----------------------------------------------------------------------------------
// Main body of the app which contains the emitters and other elements that make 
//	up the game, and calls their update() and draw() methods
//	it also handles key press inputs to move and fire the tank, as well as start the game
// Chris Conway 3/31/2019

#include "ofApp.h"
#include "Emitter.h"
#include "ParticleSpitter.h"

void ofApp::setup() {
	gameOver = false;
	score = 0;
	level = 1;
	appStarted = false;
	ofSetVerticalSync(true);
	ofBackground(0, 0, 0);

	spit = ParticleSpitter();

	emitter = new Emitter(new SpriteSystem()); //emitter is the tank
	emitter->theTank = true; //it is indeed the tank
	emitter->datSound.load("sounds/rocket.wav");
	emitter->hasSound = true;
	emitter->sys->jellies = false;
	emitter->setLifespan(8000);
	emitter->setVelocity(ofVec3f(-700, -200, 0));
		//ofVec3f(-370, -200, 0));

	hit.load("sounds/popped.wav");

	invaders = new Emitter(new SpriteSystem()); //emitter creating aliums
	invaders->shoot = true;
	invaders->setRate(0.5);
	//invaders->datSound.load("sounds/invadeSound.wav");


	invaders2 = new Emitter(new SpriteSystem()); //emitter creating aliums
	invaders2->shoot = true;
	invaders2->setRate(0.25);
	//invaders2->datSound.load("sounds/invadeSound.wav");
	invaders->sys->jellies = true;
	invaders2->sys->jellies = true;
	invaders->sys->theLevel = 1;
	invaders2->sys->theLevel = 1;
	invaders2->lastSpawned = 1000;
	

	emitter->setChildSize(20, 10);

	if (defaultImage.loadImage("images/smallMissle.png")) { // sets the sprite images to that of the missle
		emitter->setChildImage(defaultImage);
		imageLoaded = true;
	}

	if (emitterImage.loadImage("images/tankyboy.png")) { // sets the emitter sprite to that of the tank
		emitter->setImage(emitterImage);
		emitter->haveImage = true;
	}
	emitter->setPosition(ofVec3f(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2, 0));

	if (invaderImage.loadImage("images/worldPeace.png")) {
		/*invaders->setImage(invaderImage);
		invaders2->setImage(invaderImage);
		invaders2->setImage(invaderImage);
		invaders->haveImage = true; */
	}
	invaders->setPosition(ofVec3f(-50, (ofGetWindowHeight() / 2) - 200, 0));
	invaders2->setPosition(ofVec3f(-50, (ofGetWindowHeight() / 2) + 200, 0));
	invaders2->setRate(.5);

	if (alium.loadImage("images/alium.png")) {
		invaders->setChildImage(alium);
		invaders2->setChildImage(alium);
		invaders2->haveChildImage = true;
		invaders->haveImage = true;
	}

	/*gui.setup();
	gui.add(rate.setup("rate", 1, 1, 10));
	gui.add(life.setup("life", 8, .1, 10));
	gui.add(velocity.setup("velocity", ofVec3f(-370, 0, 0), ofVec3f(-1000, -1000, -1000), ofVec3f(1000, 1000, 1000)));
	*/
	//sets the default velocity of the missle projetiles to head to the left
	bHide = true; // hides the gui slider which controls direction of fire. pressing 'h' will reveal it again
}

//--------------------------------------------------------------
void ofApp::update() {

	if (gameOver) return;
	//emitter->setRate(rate);
	//emitter->setLifespan(life * 1000);    // convert to milliseconds 
	//emitter->setVelocity(ofVec3f(velocity));
	emitter->update();
	spit.update();

	ofVec3f v = invaders->velocity;
	ofVec3f v2 = invaders2->velocity;
	v.y = ofRandom(-v.x, v.x);
	v2.y = ofRandom(-v2.x, v2.x);
	invaders->setVelocity(ofVec3f(v.x, v.y, v.z));
	invaders->update();
	invaders2->setVelocity(ofVec3f(v2.x, v2.y, v2.z));
	invaders2->update();

	checkCollisions();
}


//--------------------------------------------------------------
void ofApp::draw() {

	if (!appStarted) {
		ofDrawBitmapString("PRESS SPACE TO START\n1000 POINTS TO PROGRESS A LEVEL", 
			ofPoint(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2));
	}
	else if (gameOver) {
		ofDrawBitmapString("GAME OVER\nSCORE: " 
			+ std::to_string(score), ofPoint(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2));
	}
	else 
	{

	ofColor colorOne(46 + (level*50), 157, 255 - (level*50));
	ofColor colorTwo(0, 0, 0);
	ofBackgroundGradient(colorOne, colorTwo, OF_GRADIENT_LINEAR);

	// draw current score
	string scoreText;
	scoreText += "Score: " + std::to_string(score) + " \nLevel: " + std::to_string(level);
	ofDrawBitmapString(scoreText, ofPoint(10, 20));



	invaders->draw();
	invaders2->draw();
	emitter->draw();
	spit.draw();

	if (!bHide) {
		gui.draw();
	}
	}
}

void ofApp::checkCollisions() {

	float dist = 70.0; //emitter->maxDistPerFrame(); // distance sprite can travel in one frame.
	vector<Sprite> missles = emitter->sys->sprites; //missle sprite vector
	vector<Sprite>::iterator s = missles.begin(); 
	vector<Sprite>::iterator tmp;

	int i = 0; //counter

	invaders->sys->removeNear(emitter->trans, 100.0); //should clear nearby aliums
	invaders2->sys->removeNear(emitter->trans, 100.0);
	if (invaders->sys->tangoDown || invaders2->sys->tangoDown)
	{
		//emitter->sys->remove(i);
		invaders->sys->tangoDown = false;
		invaders2->sys->tangoDown = false;
		gameOver = true;
	}


	while (s != missles.end()) {
		invaders->sys->removeNear(missles[i].trans, dist); //should clear nearby aliums
		invaders2->sys->removeNear(missles[i].trans, dist);

		if (invaders->sys->tangoDown || invaders2->sys->tangoDown)
		{
			spit.pos = emitter->sys->getPos(i);
			spit.addFive();

			emitter->sys->remove(i); 
			invaders->sys->tangoDown = false;
			invaders2->sys->tangoDown = false;
			score += 100;
			hit.play(); 
			if (score / 1000 > level - 1)
			{
				level++;
				invaders->sys->theLevel = level;
				invaders2->sys->theLevel = level;
			}
		}
		else {
			s++;
			i++;
		}
	}
}
//--------------------------------------------------------------
void ofApp::updateScore(int n) {score += n;}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
	//	cout << "mouse( " << x << "," << y << ")" << endl;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	/*ofPoint mouse_cur = ofPoint(x, y);
	ofVec3f delta = mouse_cur - mouse_last;
	emitter->trans += delta;
	mouse_last = mouse_cur;*/ 
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	mouse_last = ofPoint(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

void ofApp::keyPressed(int key) {
	switch (key) {
	case 'C':
	case 'c':
		break;
	case 'F':
	case 'f':
		ofToggleFullscreen();
		break;
	case 'H':
	case 'h':
		bHide = !bHide;
		break;
	case 'r':
		break;
	case 's':
		break;
	case 'u':
		break;
	case ' ':
		if (!appStarted) {
			appStarted = true;
			emitter->start();
			invaders->start();
			invaders2->start();
		}
		else
			emitter->shoot = true;
		break;
	case OF_KEY_LEFT:
	case OF_KEY_RIGHT:
	case OF_KEY_UP:
	case OF_KEY_DOWN:
		emitter->setMovement(key); //set momentum in direction of arrow
		break;
	case OF_KEY_ALT:
		break;
	case OF_KEY_CONTROL:
		break;
	case OF_KEY_SHIFT:
		break;
	case OF_KEY_DEL:
		break;
	}
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	switch (key) {
	case ' ':
		emitter->shoot = false;
		break;
	case OF_KEY_LEFT:
	case OF_KEY_RIGHT:
	case OF_KEY_UP:
	case OF_KEY_DOWN:
		emitter->stopMovement(key); //reset momentum of given direction
		break;
	case OF_KEY_ALT:
		break;
	case OF_KEY_CONTROL:
		break;
	case OF_KEY_SHIFT:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

