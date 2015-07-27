//
//  ofxLoggyReceiver.cpp
//  modelmath
//
//  Created by Gal Sasson on 7/26/15.
//
//

#include "ofxLoggyReceiver.h"


ofxLoggyReceiver::ofxLoggyReceiver()
{
	bSetup = false;
	bDraw = false;
}

void ofxLoggyReceiver::setup(int port)
{
	receiver.setup(port);

	ofAddListener(ofEvents().update, this, &ofxLoggyReceiver::update);
	ofAddListener(ofEvents().draw, this, &ofxLoggyReceiver::draw);
	bSetup = true;
	bDraw = true;
}


void ofxLoggyReceiver::update(ofEventArgs & args)
{
	ofxOscMessage msg;
	while (receiver.getNextMessage(&msg)) {
		Entity* ent = getEntityWithName(msg.getAddress());
		if (ent == NULL) {
			// create new entity
			ent = new Entity();
			ent->name = msg.getAddress();

			ent->plot = new ofxHistoryPlot(&(ent->lastValue), ent->name, ofGetWidth(), true);
			ent->plot->setBackgroundColor(ofColor(0, 0, 0, 255));
			ent->plot->setColor( ofColor(0,255,0) );
			ent->plot->setShowNumericalInfo(true);
			ent->plot->setLineWidth(1);
			entities.push_back(ent);
		}

		if (msg.getArgType(0) == OFXOSC_TYPE_FLOAT) {
			ent->lastValue = msg.getArgAsFloat(0);
		}
	}
}

void ofxLoggyReceiver::draw(ofEventArgs & args)
{
	if (!bDraw) {
		return;
	}

//	ofSetColor(0);
//	ofFill();
//	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

//	ofSetColor(255, 255);
	ofFill();
	height = 0;
	for (Entity* ent : entities) {
		ent->plot->draw(0, 0, ofGetWidth(), ent->plot->getHeight());
		ofTranslate(0, ent->plot->getHeight());
		height += ent->plot->getHeight();
	}
}

ofxLoggyReceiver::Entity* ofxLoggyReceiver::getEntityWithName(const string &name)
{
	for (Entity* ent : entities) {
		if (ent->name == name) {
			return ent;
		}
	}

	return NULL;
}
