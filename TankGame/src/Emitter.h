#pragma once
#pragma once
//----------------------------------------------------------------------------------
// Basic Sprite Emitter Class
//	contains and handles sprite elements such as the enemy jellyfish and the tank
//  Chris Conway 3/31/2019

#include "Sprite.h"


//  General purpose Emitter class for emitting sprites
//  This works similar to a Particle emitter
//	It has been modified so that the sprites have physics similar to particles
class Emitter : public BaseObject {
public:
	Emitter(SpriteSystem *);
	void setMovement(int key); //my movement method for the emitter, takes key input
	void stopMovement(int key); //resets tank movement of given direction, takes key input
	void levelUp();
	void draw();
	void start();
	void stop();
	void setLifespan(float);
	void setVelocity(ofVec3f);
	void setChildImage(ofImage);
	void setChildSize(float w, float h) { childWidth = w; childHeight = h; }
	void setImage(ofImage);
	void setRate(float);
	float maxDistPerFrame();
	void update();
	SpriteSystem *sys;
	float rate;
	ofSoundPlayer datSound; //sound emitter makes when it fires
	bool hasSound;
	bool shoot; //true if the tank is shooting, false otherwise
	bool theTank; //is this emitter specifically the tank
	ofVec3f movementVelocity; // velocity of the tank itself
	int level;

	ofVec3f velocity;
	float lifespan;
	bool started;
	float lastSpawned;
	ofImage childImage;
	ofImage image;
	bool drawable;
	bool haveChildImage;
	bool haveImage;
	float width, height;
	float childWidth, childHeight;
};

