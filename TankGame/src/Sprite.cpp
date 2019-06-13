//----------------------------------------------------------------------------------
//  Sprite Class
//  Chris Conway 3/31/2019
#include "Sprite.h"

BaseObject::BaseObject() {
	trans = ofVec3f(0, 0, 0);
	scale = ofVec3f(1, 1, 1);
	rot = 0;
}

void BaseObject::setPosition(ofVec3f pos) {
	trans = pos;
}

// Basic Sprite Object
Sprite::Sprite() {
	speed = 0;
	velocity = ofVec3f(0, 0, 0);
	acceleration = ofVec3f(-1.5, 2, 0); // begins expiriencing gravity
	lastJitter = 0;
	mass = .5;
	lifespan = -1;      // lifespan of -1 => immortal 
	birthtime = 0;
	bSelected = false;
	haveImage = false;
	//name = "UnamedSprite";
	width = 20;
	height = 20;
}

// Return a sprite's age in milliseconds
float Sprite::age() {
	return (ofGetElapsedTimeMillis() - birthtime);
}

//  Set an image for the sprite. If you don't set one, a rectangle gets drawn.
void Sprite::setImage(ofImage img) {
	image = img;
	haveImage = true;
	width = image.getWidth();
	height = image.getHeight();
}

//  Render the sprite
void Sprite::draw() {

	ofSetColor(255, 255, 255, 255);

	// draw image centered and add in translation amount
	if (haveImage) {
		image.draw(-width / 2.0 + trans.x, -height / 2.0 + trans.y);
	}
	else {
		// in case no image is supplied, draw something.
		ofNoFill();
		ofSetColor(255, 0, 0);
		ofDrawRectangle(-width / 2.0 + trans.x, -height / 2.0 + trans.y, width, height);
	}
}

void Sprite::integrate() {
	//acceleration *= (1.0 / mass);
	velocity += acceleration;
}


//  Add a Sprite to the Sprite System
void SpriteSystem::add(Sprite s) {
	sprites.push_back(s);
}

// Remove a sprite from the sprite system. Note that this function is not currently
// used. The typical case is that sprites automatically get removed when the reach
// their lifespan.
void SpriteSystem::remove(int i) {
	if(sprites.size() >= i)
		sprites.erase(sprites.begin() + i);
}

// remove all sprites within a given dist of point
void SpriteSystem::removeNear(ofVec3f point, float dist) {
	vector<Sprite>::iterator s = sprites.begin();
	vector<Sprite>::iterator tmp;

	while (s != sprites.end()) {
		ofVec3f v = s->trans - point;
		if (v.length() < dist) {
			tmp = sprites.erase(s);
			s = tmp;
			tangoDown = true;
		}
		else s++;
	}
}


//  Update the SpriteSystem by checking which sprites have exceeded their
//  lifespan (and deleting).  Also the sprite is moved to it's next
//  location based on velocity and direction.
void SpriteSystem::update() {

	if (sprites.size() == 0) return;
	vector<Sprite>::iterator s = sprites.begin();
	vector<Sprite>::iterator tmp;

	// check which sprites have exceed their lifespan and delete from list.
	while (s != sprites.end()) {
		if (s->lifespan != -1 && s->age() > s->lifespan) {
			//			cout << "deleting sprite: " << s->name << endl;
			tmp = sprites.erase(s);
			s = tmp;
		}
		else s++;
	}

	//  Move sprites
	if (jellies)
	{
		float time = ofGetElapsedTimeMillis();
		for (int i = 0; i < sprites.size(); i++) {
			if (time - sprites[i].lastJitter > 2000.0)
			{
				sprites[i].velocity.x += 150 + (theLevel * 100);
				sprites[i].acceleration.x = -4 - theLevel;
				sprites[i].velocity.y += -160;
				sprites[i].lastJitter = time + ofRandom(-1000, 1500);
			}
			sprites[i].integrate();
			if (sprites[i].velocity.x < 0)
				sprites[i].acceleration.x = 0;
			if (sprites[i].velocity.y > 200)
				sprites[i].acceleration.y = 0;
			sprites[i].trans += sprites[i].velocity / ofGetFrameRate();
		}
	}
	else
	{
		for (int i = 0; i < sprites.size(); i++) {
			if (sprites[i].velocity.x > 0)
				sprites[i].acceleration.x = 0;
			sprites[i].integrate();
			sprites[i].trans += sprites[i].velocity / ofGetFrameRate();
		}
	}

}

//  Render all the sprites
void SpriteSystem::draw() {
	for (int i = 0; i < sprites.size(); i++) {
		sprites[i].draw();
	}
}

ofVec3f SpriteSystem::getPos(int i)
{
	return sprites[i].trans;
}