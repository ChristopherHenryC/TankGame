#include "ofMain.h"
#include "ParticleSpitter.h"

//  Create a new ParticleSpitter
//	This class functions as a particlesystem that can generate and add its own particles
//	These particles are used to create the explosion effect when rockets hit enemies
//	Chris Conway 3/31/2019

Particle::Particle() {
	velocity = ofVec3f(400, 100, 0);
	acceleration = ofVec3f(1.5, 2, 0); // begins expiriencing gravity
	lastJitter = 0;
	mass = .5;
	lifespan = 1200;      // lifespan of -1 => immortal 
	birthtime = ofGetElapsedTimeMillis();
	bSelected = false;
	haveImage = false;
	width = 20;
	height = 20;
}

float Particle::age() {
	return (ofGetElapsedTimeMillis() - birthtime);
}

void Particle::integrate() {
	velocity += acceleration;
}

void Particle::draw() {
	ofSetColor(299, 0, 0);
	ofDrawRectangle(trans.x, trans.y, 5, 5);	
}


ParticleSpitter::ParticleSpitter() {
	velocity = ofVec3f(0, 0, 0); // velocity of the ParticleSpitter itself
	pos = ofVec3f(0, 0, 0);

	lifespan = 1500;    // milliseconds
	started = false;

	lastSpawned = 0;
	velocity = ofVec3f(100, 0, 0); //default values for emitted sprite, such as jellies
}

//  Draw the Particles
void ParticleSpitter::draw() {
	for (int i = 0; i < particles.size(); i++) {
		particles[i].draw();
	}
}

//  Update the ParticleSpitter.
void ParticleSpitter::update() {
	if (particles.size() == 0) return;
	vector<Particle>::iterator s = particles.begin();
	vector<Particle>::iterator tmp;

	// check which sprites have exceed their lifespan and delete from list.
	while (s != particles.end()) {
		if (s->lifespan != -1 && s->age() > s->lifespan) {
			//			cout << "deleting sprite: " << s->name << endl;
			tmp = particles.erase(s);
			s = tmp;
		}
		else s++;
	}

	for (int i = 0; i < particles.size(); i++) {
		if (particles[i].velocity.x > 0)
			particles[i].acceleration.x = 0;
		particles[i].integrate();
		particles[i].trans += particles[i].velocity / ofGetFrameRate();
	}
}

void ParticleSpitter::add(Particle s) {
	s.trans = pos;
	ofVec3f v = s.velocity;
	v.y = ofRandom(-v.y, v.y);
	v.x = ofRandom(-v.x, 0);
	s.velocity = ofVec3f(v.x, v.y, 0);

	particles.push_back(s);
}

void ParticleSpitter::addFive()
{
	for(int i = 0; i < 10; i++) { add(Particle()); }
}

void ParticleSpitter::remove(int i) {
	if (particles.size() >= i)
		particles.erase(particles.begin() + i);
}

void ParticleSpitter::setLifespan(float life) {
	lifespan = life;
}



