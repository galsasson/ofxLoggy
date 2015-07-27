//
//  ofxLoggyReceiver.h
//  modelmath
//
//  Created by Gal Sasson on 7/26/15.
//
//

#ifndef __modelmath__ofxLoggyReceiver__
#define __modelmath__ofxLoggyReceiver__

#include <stdio.h>
#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxHistoryPlot.h"

class ofxLoggyReceiver
{
public:
	ofxLoggyReceiver();

	void setup(int port);

	void update(ofEventArgs & args);
	void draw(ofEventArgs & args);

	bool bDraw;
	float height;

private:
	bool bSetup;

	class Entity {
	public:
		string name;
		float lastValue;
		ofxHistoryPlot* plot;
	};
	vector<Entity*> entities;
	void renderEntity(float x, float y, Entity* ent);
	Entity* getEntityWithName(const string& name);

	ofxOscReceiver receiver;
};
#endif /* defined(__modelmath__ofxLoggyReceiver__) */
