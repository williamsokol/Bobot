#include "internalResources.h"

using namespace BWAPI;



int InternalResources::gasDiff = 0;
int InternalResources::mineralDiff = 0;
int InternalResources::supplyDiff =0;

int InternalResources::GetResource(std::string resource)
{
	if (resource == "mineral") {
		return Broodwar->self()->minerals() + mineralDiff;
	}
	if(resource == "gas"){
		return Broodwar->self()->gas() + gasDiff;
	}
	if (resource == "supply") {
		return (Broodwar->self()->supplyTotal() / 2) + supplyDiff; //idk why but supply is natrally doubled, I guess for zerglings which are 1/2 supply each
	}
}

void InternalResources::UpdateResource(std::string type,int resource)
{
	if (type == "mineral") {
		gasDiff += resource;
	}
	if (type == "gas") {
		mineralDiff += resource;
	}
	if (type == "supply") {
		supplyDiff += resource;
	}

}