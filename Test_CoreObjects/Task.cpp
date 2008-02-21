#include "Task.h"
#include "TaskSystem.h"

void Task::activate()
{
	TaskSystem::GetSingleton().activateTask(this);
}

void Task::pause()
{
	TaskSystem::GetSingleton().pauseTask(this);
}

void Task::resume()
{
	TaskSystem::GetSingleton().resumeTask(this);
}

void Task::terminate()
{
	TaskSystem::GetSingleton().terminateTask(this);
}

void Task::changePriority(const TaskPriority& priority)
{
	TaskSystem::GetSingleton().changePriority(this,priority);

}