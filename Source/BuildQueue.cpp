#include"BuildQueue.h"

using namespace BWAPI;
using namespace Filter;



void BuildQueue::CheckBuild( Unit unit,const BWEM::Base* thing)
{
   

    int currentSupply = Broodwar->self()->supplyUsed() / 2;
    int totalSupply = InternalResources::GetResource("supply");

    

    
    //Broodwar->sendText(" neede %f %d %s",1.16*currentSupply,totalSupply, 1.16 * currentSupply>= totalSupply? "true": "false");
    // all of the scv and building creation, I based it on supply count :)
    if ((1.16 * currentSupply) >= totalSupply && Broodwar->self()->minerals() >= UnitTypes::Terran_Supply_Depot.mineralPrice())
    {
        ExampleAIModule::PreUnit building;
        building.valid = true;
        building.unit = UnitTypes::Terran_Supply_Depot;
        bot->unitQueue.push_back(building);
        InternalResources::UpdateResource("supply", 8);
        
        //bot->predictSupply = bot->predictSupply + 8;
    }
    if (currentSupply == 11)
    {
        ExampleAIModule::PreUnit building;
        Broodwar << "making a barracks" << std::endl;
        building.valid = true;
        building.unit = UnitTypes::Terran_Barracks;
        bot->unitQueue.push_back(building);
    }
    if (currentSupply == 12 && Broodwar->enemy()->getRace() == Races::Protoss)
    {
        Broodwar << "making an academy " << std::endl;
        ExampleAIModule::PreUnit building;
        building.valid = true;
        building.unit = UnitTypes::Terran_Academy;
        bot->unitQueue.push_back(building);
        //DoBuilding(UnitTypes::Terran_Barracks);
    }
    if (currentSupply == 17)
    {
        ExampleAIModule::PreUnit building;
        building.pos =thing->Location();
        building.unit = (UnitTypes::Terran_Command_Center);
        bot->unitQueue.push_back(building);
        Broodwar << thing->Location() << std::endl;
    }
    if (currentSupply == 18)
    {
        Broodwar << "making a Refinery--------------------------------------------------" << unit->getType().c_str()<< std::endl;
        ExampleAIModule::PreUnit building;
        //system("pause");
        building.valid = true;
        building.unit = (UnitTypes::Terran_Refinery);
        bot->unitQueue.push_back(building);
    }
    if (currentSupply == 20 || currentSupply == 22)
    {
        ExampleAIModule::PreUnit building;
        Broodwar->sendText("making factory");
        building.valid = true;
        building.unit = (UnitTypes::Terran_Factory);
        bot->unitQueue.push_back(building);

    }
    
    
    //if (building.valid)
    //{
    //    
    //    Broodwar->sendText("supply: %d", currentSupply);
    //    
    //    //Broodwar->sendText(" supply depot %s", building.valid ? "true" : "false");
    //    //system("pause");
    //    
    //    Broodwar << bot->unitQueue[0].pos;
    //    
    //    

    //}

}
