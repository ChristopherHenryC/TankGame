#pragma once
#pragma once
//----------------------------------------------------------------------------------
//  Create a new ParticleSpitter
//	This class functions as a particlesystem that can generate and add its own particles
//	These particles are used to create the explosion effect when rockets hit enemies
//	Chris Conway 3/31/2019

#include "Sprite.h"

class Particle : public BaseObject {
public:
	Particle();
	void draw();
	float age();
	void integrate(); //update physics of a sprite
	//float speed;    //   in pixels/sec
	float mass;
	float lastJitter;
	ofVec3f velocity; // in pixels/sec
	ofVec3f acceleration; //rate of change in velocity
	ofImage image;
	float birthtime; // elapsed time in ms
	float lifespan;  //  time in ms
	bool haveImage;
	float width, height;
};

class ParticleSpitter{
public:
	ParticleSpitter();
	void draw();
	void setLifespan(float);
	void update();
	void addFive();
	ofVec3f velocity;
	ofVec3f pos;
	bool shoot; //true if the tank is shooting, false otherwise
	float lifespan;
	bool started;
	float lastSpawned;

	vector<Particle> particles;

	void add(Particle);
	void remove(int);
};
