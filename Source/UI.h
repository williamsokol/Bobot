#include "ExampleAIModule.h"
#include "internalResources.h"
#include <BWAPI.h>

class UI
{
private:
	ExampleAIModule* bot;
public:
	UI(ExampleAIModule* bot1)
	{
		bot = bot1;
	}
	


	void Display();

};