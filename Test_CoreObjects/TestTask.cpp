#include <sstream>
#include "TestTask.h"

bool TestTask::Run = true;

TestTask::TestTask()
	:Task("TestTask",10000)
{
}

void TestTask::onActivate()
{ 
	std::cout<<"->Test task activated."<<std::endl;
	int i;

	for (i = 0; i < SIZE_TASKS; i++)
	{
		TestTask2* t = new TestTask2();
		t->activate(m_taskManager);
		m_Task2.push_back( t );
	}
}

void TestTask::onPaused()
{ 
	std::cout<<"->Test task paused."<<std::endl; 
}

void TestTask::onResumed()
{ 
	std::cout<<"->Test task resumed."<<std::endl; 
}

void TestTask::onTerminate()
{ 
	std::cout<<"->Test task terminate."<<std::endl; 

	for( std::vector<TestTask2*>::iterator it = m_Task2.begin(); it != m_Task2.end(); ++it)
	{
		if((*it)->getState() == gcore::TS_ACTIVE)
		{
			(*it)->terminate();
		}
	}

	m_Task2.clear();
	Run = false;
	destroy();
}

void TestTask::execute()
{
	/*static int k=1000;
	static char entry[256];
	if(k > 1000)
	{
		
		std::cout<<std::endl<<"?> ";
		std::cin.getline(entry,256);
		
		if( strcmp(entry,"exit")==0 )
		{
			terminate();
			return;
		}
		if( strcmp(entry,"pause")==0 )
		{
			pause();
			return;
		}

		k=0;
	}

	++k;

	std::cout<<"Yeah! You did : "<<entry<<std::endl;

	*/
	
	std::cout << "------ Execute Main TASK -----" << std::endl;
	
	char entry[256];
	cout << ">";
	cin.getline(entry, 256);
	this->cmd[0].clear();
	this->cmd[1].clear();
	typedef boost::tokenizer<boost::char_separator<char> > my_tok;
	boost::char_separator<char> sep( " " );
	string tmp = entry;
	my_tok tok( tmp, sep );
	int pos = 0;
	for ( my_tok::const_iterator i = tok.begin();i != tok.end() && pos < 2;++i, ++pos)
		this->cmd[pos] = *i;
	if (this->cmd && this->cmd[0] == "exit")
		TestTask::getSingleton().terminate();
	else if (cmd[0] != "" && cmd[1] != "")
	{
		int i = 0;
		for (; i < SIZE_TASKS && this->m_Task2[i]->getName() != cmd[1]; ++i)
			;
		if (i != SIZE_TASKS){
		if (cmd[0] == "resumed" && this->m_Task2[i]->getState() == gcore::TS_PAUSED)
			this->m_Task2[i]->resume();
		else if (cmd[0] == "terminate")
			this->m_Task2[i]->terminate();
		}
	}
	
}

string* TestTask::GetCmd()
{
	return (this->cmd);
}

string ran = "TestTask2";
TestTask2::TestTask2() : Task(ran)
{
	
	static int i = 0;
	ran = "TestTask2";
	ran += i + '0';
	this->m_id = i;
	setName(ran);
	i++;
	this->m_time = 0;
	setPriority(i);
}

void TestTask2::onActivate()
{
	std::cout<<"->Test task activated." + this->getName() <<std::endl;
}

void TestTask2::onPaused()
{ 
	std::cout<<"->Test task paused." + this->getName() <<std::endl; 
}

void TestTask2::onResumed()
{ 
	std::cout<<"Ca te fais dresser le zizi ? ->Test task Resumed." + this->getName() <<std::endl; 
}

void TestTask2::onTerminate()
{ 
	std::cout<<" !!!WTF!!! WARNING NO MORE THIS TASKS->Test task terminate." + this->getName() <<std::endl;
	
//	delete this;
}

void TestTask2::execute()
{
	string *cmd = TestTask::getSingleton().GetCmd();

	if (cmd[1] != "" && cmd[1] == this->getName())
	{
		if (cmd[0] == "pause" && this->getState() == gcore::TS_ACTIVE)
			pause();
		else
			std::cout << "Execute task " + this->getName() << std::endl;
	}
	else
		std::cout << "Execute task " + this->getName() << std::endl;
}