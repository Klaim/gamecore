#ifndef TESTTASK_H
#define TESTTASK_H

#include <iostream>
#include <stdio.h> 
#include <windows.h> 
#include <time.h>
#include <vector>
using namespace std;
#include "GC_Task.h"
#include "GC_EventTask.h"
#include "GC_Singleton.h"
#include<boost/tokenizer.hpp>


#define SIZE_TASKS 6
#define MAX_RESUME 3
#define SPEED 500
#define END 60

using namespace std;

class TestTask2 : public GCore::Task
{
private:
	int		m_id;
	int		m_time;
public:

	void onActivate();

	void onPaused();

	void onResumed();

	void onTerminate();

	void execute();

	TestTask2();

	int getId(){return (this->m_id);
	}
};

class TestTask : public GCore::Task , public GCore::Singleton<TestTask>
{
private:
	std::vector<TestTask2*>	m_Task2;
	string		cmd[2];
	static bool Run;
public:


	static bool isRunning(){return Run;}

	void onActivate();

	void onPaused();

	void onResumed();

	void onTerminate();

	void execute();

	string* GetCmd();

	TestTask();
};

#endif