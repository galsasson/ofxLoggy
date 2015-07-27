//
//  ofxLoggy.cpp
//  modelmath
//
//  Created by Gal Sasson on 7/26/15.
//
//

#include "ofxLoggy.h"


LoggyLogger ofxloggy;
ofxLoggyReceiver ofxloggyreceiver;

LoggyLogger::~LoggyLogger()
{
	stopThread();
}

LoggyLogger::LoggyLogger()
{
	time = 0;
	bSenderDead = true;

	startThread();
}

void LoggyLogger::setup(const string& hostname, int port)
{
//	sender.setup("localhost", 10991);
	sender.setup(hostname, port);
	bSenderDead = false;
}

void LoggyLogger::threadedFunction()
{
	while (isThreadRunning()) {
		usleep(1000);
		time += 1.0/960;

		if (tasks.empty() || bSenderDead) {
			continue;
		}

		ofxOscBundle bundle;

		tasksLock.lock();
		vector<Task*> performTasks = tasks;
		tasks.clear();
		tasksLock.unlock();

		for (Task* task: performTasks) {
			ofxOscMessage msg;
			msg.setAddress(task->who);
			if (task->bUsingVal) {
				msg.addFloatArg(task->val);
			}
			if (task->msg != NULL) {
				msg.addStringArg(task->msg);
			}

			bundle.addMessage(msg);
			delete task;
		}

	   sender.sendBundle(bundle);
	}
}

void LoggyLogger::log(float var)
{
	Task* task = new Task();
	task->time = time;
	task->frame = ofGetFrameNum();
	task->val = var;
	task->bUsingVal = true;
	task->who = createLoggyMsg("/generic");
	addTask(task);
}

void LoggyLogger::log(const string &who, float var)
{
	Task* task = new Task();
	task->time = time;
	task->frame = ofGetFrameNum();
	task->val = var;
	task->bUsingVal = true;
	task->who = createLoggyMsg(who);
	addTask(task);
}

void LoggyLogger::log(const string &who, const string &str)
{
	Task* task = new Task();
	task->time = time;
	task->frame = ofGetFrameNum();
	task->who = createLoggyMsg(who);
	task->msg = createLoggyMsg(str);
	addTask(task);
}


void LoggyLogger::addTask(Task* task)
{
	if (bSenderDead) {
		delete task;
		return;
	}

	tasksLock.lock();
	tasks.push_back(task);
	tasksLock.unlock();
}


char* LoggyLogger::createLoggyMsg(const string& str)
{
	char* msg = (char*)malloc(str.length()+1);
	memcpy(msg, str.c_str(), str.length());
	msg[str.length()] = NULL;
	return msg;
}
