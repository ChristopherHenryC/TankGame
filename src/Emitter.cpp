//----------------------------------------------------------------------------------
// Basic Sprite Emitter Class
//	contains and handles sprite elements such as the enemy jellyfish and the tank
//  Chris Conway 3/31/2019

#include "ofMain.h"
#include "Emitter.h"

Emitter::Emitter(SpriteSystem *spriteSys) {
	movementVelocity = ofVec3f(0, 0, 0); // velocity of the emitter itself
	shoot = false; // is the emitter currently emitting
	theTank = false; //most emitters are not the tank
	level = 1;
	hasSound = false;

	sys = spriteSys;
	lifespan = 20000;    // milliseconds
	started = false;

	lastSpawned = 0;
	rate = .6;    // sprites/sec
	haveChildImage = false;
	haveImage = false;
	velocity = ofVec3f(100, 0, 0); //default values for emitted sprite, such as jellies
	drawable = true;
	width = 50;
	height = 50;
	childWidth = 100;
	childHeight = 100;
}


void Emitter::setMovement(int key) //changes the velocity of the tank
{
	if (!theTank)
		return;

	switch (key) {
	case OF_KEY_DOWN:
		movementVelocity.y = 10;
		break;
	case OF_KEY_UP:
		movementVelocity.y = -10;
		break;
	case OF_KEY_LEFT:
		movementVelocity.x = -10;
		break;
	case OF_KEY_RIGHT:
		movementVelocity.x = 10;
		break;
	}
}

void Emitter::stopMovement(int key) //resets the momentum of a given direction to zero
{
	if (!theTank)
		return;

	switch (key) {
	case OF_KEY_DOWN:
		if (movementVelocity.y > 0)
			movementVelocity.y = 0;
		break;
	case OF_KEY_UP:
		if (movementVelocity.y < 0)
			movementVelocity.y = 0;
		break;
	case OF_KEY_LEFT:
		if (movementVelocity.x < 0)
			movementVelocity.x = 0;
		break;
	case OF_KEY_RIGHT:
		if (movementVelocity.x > 0)
			movementVelocity.x = 0;
		break;
	}
}

void Emitter::levelUp()
{
	level++;
	rate += 0.1;
}

//  Draw the Emitter if it is drawable. In many cases you would want a hidden emitter
void Emitter::draw() {
	if (drawable) {

		if (haveImage) {
			ofSetColor(255, 255, 255, 255);
			image.draw(-image.getWidth() / 2.0 + trans.x, -image.getHeight() / 2.0 + trans.y);
		}
		else {
			ofSetColor(0, 0, 255);
			ofDrawRectangle(-width / 2 + trans.x, -height / 2 + trans.y, width, height);
		}
	}
	// draw sprite system
	sys->draw();
}

//  Update the Emitter. If it has been started, spawn new sprites with
//  initial velocity, lifespan, birthtime.
void Emitter::update() {

	if (!started) return;

	trans += movementVelocity; //update the position of the emitter
	float time = ofGetElapsedTimeMillis();

	if (shoot && ((time - lastSpawned) > (1000.0 / rate))) { //checks time between spawns and shoot boolean
		if(hasSound)
			datSound.play(); //play shooting sound	

		Sprite sprite;
		if (haveChildImage) sprite.setImage(childImage);
		sprite.velocity = velocity;
		sprite.lifespan = lifespan;
		sprite.setPosition(trans);
		sprite.birthtime = time;
		sprite.width = childWidth;
		sprite.height = childHeight;
		if (theTank)
		{
			sprite.mass = 1;
			sprite.acceleration.y = 5;
			sprite.acceleration.x = 7;
		}
		sys->add(sprite);

		lastSpawned = time;
	}
	if (theTank) {
		if (trans.y > ofGetWindowHeight()) //return the tank to within bounds
			trans.y = ofGetWindowHeight();
		else if (trans.y < 0)
			trans.y = 0;
		if (trans.x > ofGetWindowWidth())
			trans.x = ofGetWindowWidth();
		else if (trans.x < 0)
			trans.x = 0;
	}
	sys->update();
}

// Start/Stop the emitter.
void Emitter::start() {
	started = true;
	lastSpawned = ofGetElapsedTimeMillis();
}

void Emitter::stop() {
	started = false;
}

void Emitter::setLifespan(float life) {
	lifespan = life;
}

void Emitter::setVelocity(ofVec3f v) {
	velocity = v;
}

void Emitter::setChildImage(ofImage img) {
	childImage = img;
	haveChildImage = true;
	childWidth = img.getWidth();
	childHeight = img.getHeight();
}

void Emitter::setImage(ofImage img) {
	image = img;
	height = image.getHeight();
	width = image.getWidth();
}

void Emitter::setRate(float r) {
	rate = r;
}

//gonna use this to detect distance per frame for each missle sprite
float Emitter::maxDistPerFrame() {
	return  velocity.length() / ofGetFrameRate();
}
