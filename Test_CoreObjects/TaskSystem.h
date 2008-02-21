#ifndef TASKSYSTEM_H
#define TASKSYSTEM_H

#include <map>
#include <list>
#include <string>
#include "Singleton.h"
#include "Task.h"




class TaskSystem : public Singleton<TaskSystem>
{
private:
	friend class Task;

	std::map< std::string , Task*> m_namedTasksIndex;

	std::list<Task*> m_pausedTaskList;
		
	void insertActiveTask(Task* task);
	void removeActiveTask(Task* task);
	
	Task* m_activeTaskList_First;
	Task* m_activeTaskList_Last;

	unsigned long m_activeTaskCount;
	
public:

	void activateTask(Task* task);

	void pauseTask(Task* task);

	void resumeTask(Task* task);

	void terminateTask(Task* task);


	void pauseAllTasks();

	void resumeAllTasks();

	void terminateAllTasks();

	void terminateAllPausedTasks();

	void terminateAllActiveTasks();


	void changePriority(Task* task,const TaskPriority& newPriority);


	Task* getRegisteredTask(const std::string& name);

	TaskSystem();

	~TaskSystem();

	unsigned long countPausedTasks();
	unsigned long countActiveTasks();
	unsigned long countNamedTasks();
	unsigned long countTotalTasks();

	////////

	void executeTasks();

};




#endif