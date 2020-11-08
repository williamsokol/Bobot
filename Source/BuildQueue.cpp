#include"BuildQueue.h"

using namespace BWAPI;
using namespace Filter;



void BuildQueue::CheckBuild( Unit unit,const BWEM::Base* thing)
{
   

    int currentSupply = Broodwar->self()->supplyUsed() / 2;
    int totalSupply = (Broodwar->self()->supplyTotal() / 2) + bot->predictSupply;

    

    ExampleAIModule::PreUnit building;
    //Broodwar->sendText(" neede %f %d %s",1.16*currentSupply,totalSupply, 1.16 * currentSupply>= totalSupply? "true": "false");
    // all of the scv and building creation, I based it on supply count :)
    if ((1.16 * currentSupply) >= totalSupply && Broodwar->self()->minerals() >= UnitTypes::Terran_Supply_Depot.mineralPrice())
    {
        
        building.valid = true;
        building.unit = UnitTypes::Terran_Supply_Depot;
        //DoBuilding(UnitTypes::Terran_Supply_Depot);


        bot->predictSupply = bot->predictSupply + 8;
    }
    else if (currentSupply == 11)
    {
        Broodwar << "making a barracks" << std::endl;
        building.valid = true;
        building.unit = UnitTypes::Terran_Barracks;
        //DoBuilding(UnitTypes::Terran_Barracks);
    }
    else if (currentSupply == 12)
    {
        Broodwar << "making an academy " << std::endl;
        building.valid = true;
        building.unit = UnitTypes::Terran_Academy;
        //DoBuilding(UnitTypes::Terran_Barracks);
    }
    if (currentSupply == 17)
    {
        bot->baseBuilder = bot->FindWorker(NULL);
        bot->baseBuilder->move({ thing->Location().x * 32, thing->Location().y * 32 });

        Broodwar << thing->Location() << std::endl;
    }
    else if (currentSupply == 18)
    {
        Broodwar << "making a Refinery--------------------------------------------------" << unit->getType().c_str()<< std::endl;
        system("pause");
        building.valid = true;
        building.unit = (UnitTypes::Terran_Refinery);
        //DoBuilding(UnitTypes::Terran_Refinery);
    }
    else if (currentSupply == 20 || currentSupply == 22)
    {
        Broodwar->sendText("making factory");
        building.valid = true;
        building.unit = (UnitTypes::Terran_Factory);

    }
    if (building.valid)
    {
        Broodwar->sendText("supply: %d", currentSupply);

        //Broodwar->sendText(" supply depot %s", building.valid ? "true" : "false");
        bot->unitQueue.push_back(building);
        

    }

}
