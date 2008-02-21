
//#include <boost/thread.hpp>

#include "GC_Task.h"
#include "GC_TaskManager.h"
#include "GC_EventManager.h"
#include "GC_EventTask.h"
#include "GC_Exception.h"
#include "GC_Console.h"
#include "GC_LogManager.h"

#include "TestTask.h"

bool ThreadRunning;
std::string lastEntry;

class MyConsole : public gcore::Console
{
public:
	/** Draw the text.
			User defined because rendering dependent.
			@param text Text to render.
		*/
		void renderText(const std::wstring& text)
		{
			std::wcout<< text <<std::endl;
		}

};

class MyCommand : public gcore::ConsoleCommand
{
	
public:
	void execute( gcore::Console & console , const std::vector< std::wstring >& parametterList)
	{
		std::wcout<< "MyCommand!" <<std::endl;
	}

};

void testThread()
{
	//TODO : finir cette fonction thread qui test les events selon la valeur de lastEntry
	using namespace gcore;

	MyConsole console;

	console.addCommand( L"test", gcore::ConsoleCommandPtr( new MyCommand() ) );

	console.setEntry(L"/test");

	console.executeEntry();

	//EventManager eventManager(;
/*
	ThreadRunning = true;
	TaskManager taskManager;
	while(ThreadRunning)
		taskManager.executeTasks();
*/
	

}

int main()
{
	using namespace gcore;

	LogManager::create();

	TaskManager* taskManager=new TaskManager();

	srand( (unsigned)time( NULL ));

	TestTask::create();


	TestTask::getSingleton().activate(taskManager);

	//boost::thread tThread( &testThread);
	testThread();
	
	cout << "commands : "<< endl;
	cout << "\tpause 'taskname'" << endl;
	cout << "\tresumed 'taskname'" << endl;
	cout << "\tterminate 'taskname'" << endl;
	cout << "\texit" << endl;
	try
	{
		
		while(TestTask::isRunning()) 
		{
			taskManager->executeTasks();
		}
	}
	catch(gcore::Exception& ex)
	{
		GC_LOGMSG( ex.getMessage() );
	}

	cout << "END WHILE" <<endl;
	cout << "NB task : "<< taskManager->countTotalTasks() << endl;

	

	delete taskManager;


	LogManager::destroy();

	std::system("PAUSE");

	return 0;

}