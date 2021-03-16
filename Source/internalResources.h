#include <BWAPI.h>
#include <string>
#pragma once

class InternalResources
{
private:
	static int m_Mineral;
	static int m_Gas;
	static int m_Supply;
	
public:
	
	static int GetResource(std::string reasource);
	static void UpdateResource(std::string type, int resource);
	
	static int gasDiff;
	static int mineralDiff;
	static int supplyDiff;

	

}; 

