#pragma once
#pragma once
//----------------------------------------------------------------------------------
//  Sprite Class
//  Chris Conway 3/31/2019

#include "ofMain.h"

// This is a base object that all drawable object inherit from
class BaseObject {
public:
	BaseObject();
	ofVec2f trans, scale;
	float	rot;
	bool	bSelected;
	void setPosition(ofVec3f);
};

//  General Sprite class
class Sprite : public BaseObject {
public:
	Sprite();
	void draw();
	float age();
	void setImage(ofImage);
	void integrate(); //update physics of a sprite

	float speed;    //   in pixels/sec
	float mass;
	float lastJitter;
	ofVec3f velocity; // in pixels/sec
	ofVec3f acceleration; //rate of change in velocity
	ofImage image;
	float birthtime; // elapsed time in ms
	float lifespan;  //  time in ms
	//string name;
	bool haveImage;
	float width, height;
};

//  Manages all Sprites in a system.  You can create multiple systems
class SpriteSystem {
public:
	void add(Sprite);
	void remove(int);
	void update();
	void removeNear(ofVec3f point, float dist);
	void draw();
	ofVec3f getPos(int i);

	vector<Sprite> sprites;
	int theLevel;
	bool tangoDown = false; //was a sprite destroyed by removeNear
	bool jellies; // true if it contains jellyfish, false for rockets
};
