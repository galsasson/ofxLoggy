//
//  ofxLoggy.h
//  modelmath
//
//  Created by Gal Sasson on 7/26/15.
//
//

#ifndef __modelmath__ofxLoggy__
#define __modelmath__ofxLoggy__

#include <stdio.h>
#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxLoggyReceiver.h"


class LoggyLogger;
extern LoggyLogger ofxloggy;
extern ofxLoggyReceiver ofxloggyreceiver;

class LoggyLogger : public ofThread
{
public:
	LoggyLogger();
	~LoggyLogger();
	void setup(const string& hostname, int port);

	void threadedFunction();

	void log(float var);
	void log(const string& who, float var);
	void log(const string& who, const string& str);

private:
	float time;

	class Task
	{
	public:
		Task() : time(0),frame(0),who(NULL),val(0),bUsingVal(false),msg(NULL) {}
		ofMutex lock;
		float time;
		int frame;
		char* who;
		bool bUsingVal;
		float val;
		char* msg;

		void print() {
			cout<<"["<<time<<"> "<<who<<" = "<<val<<endl;
		}
		~Task() {
			if (who != NULL) {
				free(who);
			}
			if (msg != NULL) {
				free(msg);
			}
		}
	};
	vector<Task*> tasks;
	ofMutex tasksLock;
	void addTask(Task* task);

	char* createLoggyMsg(const string& str);

	ofxOscSender sender;
	bool bSenderDead;
};

#endif /* defined(__modelmath__ofxLoggy__) */
