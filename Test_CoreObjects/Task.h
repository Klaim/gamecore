#ifndef TASK_H
#define TASK_H

#include "Named.h"

typedef long TaskPriority;

enum TASK_STATE
{
	TS_UNACTIVE = 0,
	TS_ACTIVE,
	TS_PAUSED,
	
};

class Task : public Named
{
private:

	TaskPriority m_priority;

	TASK_STATE m_state;

	friend class TaskSystem;

	Task* m_next;
	Task* m_prev;

protected:

	virtual void onActivate() = 0;

	virtual void onPaused(){};

	virtual void onResumed(){};

	virtual void onTerminate() = 0;

	virtual void execute() = 0;

public:


	void activate();

	void pause();

	void resume();

	void terminate();

	void changePriority(const TaskPriority& priority);

	inline const TaskPriority& getPriority(){return m_priority;}

	inline const TASK_STATE& getState(){return m_state;}

	Task(const std::string& name = "" , TaskPriority priority = 0 ):
	  Named(name), m_priority(priority), m_state(TS_UNACTIVE),m_next(NULL),m_prev(NULL)
	{}

	  virtual ~Task(){ if(m_state!=TS_UNACTIVE)terminate();  }
};


#endif