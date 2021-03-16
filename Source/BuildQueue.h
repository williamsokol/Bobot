#include "ExampleAIModule.h"
#include "internalResources.h"
#include <BWAPI.h>


class BuildQueue
{
private:
	ExampleAIModule *bot;
public:
	BuildQueue(ExampleAIModule *bot1) 
	{
		bot = bot1;
	}
	virtual void CheckBuild( BWAPI::Unit unit,const BWEM::Base* thing = nullptr);


};



