#include "ExampleAIModule.h"
#include <iostream>
#include <fstream>
#include "BWEM 1.4.1/src/bwem.h"
#include <cassert>
#include <cmath>

using namespace BWAPI;
using namespace Filter;


using namespace BWEM;
using namespace BWEM::BWAPI_ext;
//using namespace BWEM::utils;
namespace { auto& theMap = BWEM::Map::Instance(); }

int predictSupply = 0;
Unit workers[100];

TilePosition origin;

const Base* thing;
std::vector<Unit> army;
bool attacking = false;
//int a = 0;


Unit ExampleAIModule::FindWorker(Unit caller)
{
    //Broodwar->sendText("%d", *(&workers + 1) - workers);
    
    for (int i=0; i<100;i++)
    {
        // &&
        
        if (workers[i] != NULL && !(workers)[i]->isGatheringGas() && !(workers)[i]->isConstructing())
        {
            
            //system("pause");
            Unit worker = workers[i];
            workers[i] = NULL;
            //Broodwar->sendText("used workers %d", i );
            

            return worker;
            
        }
    }
    system("pause");
    //return NULL;
}

bool ExampleAIModule::DoBuilding(UnitType building, TilePosition buildPosition,Unit worker )
{
    if (building.isAddon()) {
        return worker->buildAddon(building);
    }
    if (worker == NULL) {
        worker = FindWorker(NULL);
    }
    if (buildPosition.x == NULL) {
        buildPosition = Broodwar->getBuildLocation(building, worker->getTilePosition());
    }

    //Broodwar->sendText(" new %s", building.c_str());
    return (worker->build(building, buildPosition));
    
}

void ExampleAIModule::CheckBuild() 
{
    int currentSupply = Broodwar->self()->supplyUsed() / 2;
    int totalSupply = (Broodwar->self()->supplyTotal() / 2) + predictSupply;

    
    PreUnit building;
    //Broodwar->sendText(" neede %f %d %s",1.16*currentSupply,totalSupply, 1.16 * currentSupply>= totalSupply? "true": "false");
    // all of the scv and building creation, I based it on supply count :)
    if ((1.16 * currentSupply) >= totalSupply && Broodwar->self()->minerals() >= UnitTypes::Terran_Supply_Depot.mineralPrice())
    {
        building.valid = true;
        building.unit = UnitTypes::Terran_Supply_Depot;
        //DoBuilding(UnitTypes::Terran_Supply_Depot);
        

        predictSupply = predictSupply +8;
    }
    else if (currentSupply == 11  )
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
        baseBuilder = FindWorker(NULL);
        baseBuilder->move({ thing->Location().x * 32, thing->Location().y * 32 });

        Broodwar << thing->Location() << std::endl;
    }else if (currentSupply == 18 )
    {
        Broodwar << "making a Refinery--------------------------------------------------" << std::endl;
        building.valid = true;
        building.unit = (UnitTypes::Terran_Refinery);
        //DoBuilding(UnitTypes::Terran_Refinery);
    }
    else if (currentSupply == 20 || currentSupply == 22 ) 
    {
        Broodwar->sendText("making factory");
        building.valid = true;
        building.unit = (UnitTypes::Terran_Factory);
        
    }
    if (building.valid) 
    {
        //Broodwar->sendText(" supply depot %s", building.valid ? "true" : "false");
        unitQueue.push_back(building);
    }
   
}

void ExampleAIModule::onStart()
{
    
    try
    {
        // Hello World!
        Broodwar->sendText("test build 2");

        // Print the map name.
        // BWAPI returns std::string when retrieving a string, don't forget to add .c_str() when printing!
        Broodwar << "The map is " << Broodwar->mapName() << "!" << std::endl;

        // Enable the UserInput flag, which allows us to control the bot and type messages.
        Broodwar->enableFlag(Flag::UserInput);
        Broodwar->setLocalSpeed(5);
        // Uncomment the following line and the bot will know about everything through the fog of war (cheat).
        Broodwar->enableFlag(Flag::CompleteMapInformation);

        // Set the command optimization level so that common commands can be grouped
        // and reduce the bot's APM (Actions Per Minute).
        Broodwar->setCommandOptimizationLevel(2);

        // Check if this is a replay
        if (Broodwar->isReplay())
        {

            // Announce the players in the replay
            Broodwar << "The following players are in this replay:" << std::endl;

            // Iterate all the players in the game using a std:: iterator
            Playerset players = Broodwar->getPlayers();
            for (auto p : players)
            {
                // Only print the player if they are not an observer
                if (!p->isObserver())
                    Broodwar << p->getName() << ", playing as " << p->getRace() << std::endl;
            }

        }
        else // if this is not a replay
        {
            // Retrieve you and your enemy's races. enemy() will just return the first enemy.
            // If you wish to deal with multiple enemies then you must use enemies().
            if (Broodwar->enemy()) // First make sure there is an enemy
                Broodwar << "The matchup is " << Broodwar->self()->getRace() << " vs " << Broodwar->enemy()->getRace() << std::endl;

            
            mainBase = Broodwar->self()->getStartLocation();
            Broodwar->sendText("dist = 0 base x=%d y=%d", mainBase.x, mainBase.y);


            Broodwar << "Map initialization..." << std::endl;

            theMap.Initialize();
            theMap.EnableAutomaticPathAnalysis();
            bool startingLocationsOK = theMap.FindBasesForStartingLocations();
            //assert(startingLocationsOK);

            BWEM::utils::MapPrinter::Initialize(&theMap);
            //BWEM::utils::printMap(theMap);      // will print the map into the file <StarCraftFolder>bwapi-data/map.bmp
            //BWEM::utils::pathExample(theMap);   // add to the printed map a path between two starting locations

            BWEM::utils::MapDrawer::ProcessCommand("all");  //will disable seeing all of the map stuff on start

            Broodwar << "gg" << std::endl;

            std::ofstream file;
            file.open("cout.txt");
            std::streambuf* sbuf = std::cout.rdbuf();
            std::cout.rdbuf(file.rdbuf());

            //this is stuff for testing down here
            
        }

    }
    catch (const std::exception& e)
    {
       // Broodwar << "EXCEPTION: " << e.what() << std::endl;
    }
}

void ExampleAIModule::onEnd(bool isWinner)
{
  // Called when the game ends
  if ( isWinner )
  {
    // Log your win here!
  }
}

void ExampleAIModule::onFrame()
{
    
    try
    {
        //BWEM::utils::gridMapExample(theMap);
        BWEM::utils::drawMap(theMap);

        // Display the game frame rate as text in the upper left area of the screen
        Broodwar->drawTextScreen(200, 0, "FPS: %d", Broodwar->getFPS());
        Broodwar->drawTextScreen(200, 20, "Average FPS: %f", Broodwar->getAverageFPS());

        // ...
    }
    catch (const std::exception& e)
    {
        Broodwar << "EXCEPTION: " << e.what() << std::endl;
    }
  // Called once every game frame

  // Display the game frame rate as text in the upper left area of the screen
    int seconds = Broodwar->getFrameCount() / 24;
    int minutes = seconds / 60;
    seconds %= 60;
    Broodwar->drawTextScreen(20,20,"time %.2d:%.2d", minutes, seconds);
    for (int i = 0; i < unitQueue.size(); i++) 
    {
        Broodwar->drawTextScreen(20, i*10+60, "will make: %s onit? %s",
            unitQueue[i].unit.c_str(),
            unitQueue[i].builder != NULL? "true":"false");
    }
    Broodwar->drawTextScreen(200, 0,  "FPS: %d", Broodwar->getFPS() );
    Broodwar->drawTextScreen(200, 20, "Average FPS: %f", Broodwar->getAverageFPS() );

  if (ref.refinery) {
      int i = 0;
      for (Unit w : ref.refWorkers)
      {
          Broodwar->drawTextMap(w->getPosition(), "%c Invis %d", Text::Yellow, i);
          
          i++;
      }
  }
  
  
  int i = 0, a = 0;
  float bigDist = 100000;
  //Broodwar->sendText("dist = 0 base x=%d y=%d", mainBase.x, mainBase.y);

  for (const Area& area : theMap.Areas())
  {
      for (const Base& base : area.Bases())
      {
          origin = base.Location();

          if ( mainBase == origin)
              continue;
          
          WalkPosition size(UnitType(UnitTypes::Terran_Command_Center).tileSize());	// same size for other races

          int dist = sqrt(pow(origin.x - mainBase.x, 2) + pow(origin.y - mainBase.y, 2));
          Broodwar->drawTextMap({ origin.x * 32,origin.y * 32 }, "%c base number: %d dist=%d", Text::Yellow, i,dist);
          

          if (bigDist > dist)
          {
              bigDist = dist;
              //Broodwar->sendText("dist = %d base x=%d y=%d", dist, mainBase.x, mainBase.y);
              thing=&base;
              a = i;
          }
          i++;
      }
  }
  Broodwar->drawTextScreen(20, 40, "picked base num %d  main=%d,%d", a, mainBase.x, mainBase.y);
   
  // Return if the game is a replay or is paused
  if ( Broodwar->isReplay() || Broodwar->isPaused() || !Broodwar->self() )
    return;

  // Prevent spamming by only running our onFrame once every number of latency frames.
  // Latency frames are the number of frames before commands are processed.
  if ( Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0 )
    return;


  // builds the base
    if (unitQueue.size() >= 1)
    {
        TilePosition go(unitQueue[0].pos);
        if (unitQueue[0].builder == NULL) 
        {
            unitQueue[0].builder = FindWorker(NULL);
        }
        if (!unitQueue[0].builder->isConstructing() && Broodwar->canMake(unitQueue[0].unit))
        {
            DoBuilding(unitQueue[0].unit, go, unitQueue[0].builder);
            Broodwar->sendText("all done!");
        }
       
        
    }else {
        
        
    }
  // look at enemy stuff too
   

  // Iterate through all the units that we own
  for (auto& u : Broodwar->self()->getUnits())
  {
      // Ignore the unit if it no longer exists
      // Make sure to include this block when handling any Unit pointer!
      if (!u->exists()) {
          Broodwar->sendText("unit lost");
          continue;
      }
      // Ignore the unit if it has one of the following status ailments
      if (u->isLockedDown() || u->isMaelstrommed() || u->isStasised())
          continue;

      // Ignore the unit if it is in one of the following states
      if (u->isLoaded() || !u->isPowered() || u->isStuck())
          continue;

      // Ignore the unit if it is incomplete or busy constructing
      if (!u->isCompleted() || u->isConstructing())
          continue;


      // Finally make the unit do some stuff!



      // If the unit is a worker unit
      if (u->getType().isWorker())
      {
          if (u == baseBuilder) 
          {
              //Broodwar->sendText("done: %s", u->getTilePosition() == thing->Location() ? "true" : "false");
              if (u->isIdle() )
              {
                  DoBuilding(UnitTypes::Terran_Command_Center, thing->Location(),baseBuilder);
                  //Broodwar->sendText("done: %s", DoBuilding(UnitTypes::Terran_Command_Center, thing->Location())? "true":"false");
                  baseBuilder = NULL;
              }
          }else if (u->isIdle())
          {
              // Order workers carrying a resource to return them to the center,
              // otherwise find a mineral patch to harvest.
              if (u->isCarryingGas() || u->isCarryingMinerals())
              {
                  u->returnCargo();
              }
              else if (!u->getPowerUp())  // The worker cannot harvest anything if it
              {                             // is carrying a powerup such as a flag
                // Harvest from the nearest mineral patch or gas refinery
                  if (!u->gather(u->getClosestUnit(IsMineralField /*|| IsRefinery*/)))
                  {
                      // If the call fails, then print the last error message
                      Broodwar << Broodwar->getLastError() << std::endl;
                  }

              } // closure: has no powerup
          } // closure: if idle
          if (u == workers[0]) 
          {
              //TilePositions::Unknown.x;
          }
          

      }
      else if (u->getType().isBuilding()) 
      {
          if (u->getType().isResourceDepot()) // A resource depot is a Command Center, Nexus, or Hatchery
          {

              // Order the depot to construct more workers! But only when it is idle.
              if (u->isIdle() && workerCount <= workerGoal &&!u->train(u->getType().getRace().getWorker()))
              {
                  //Broodwar->sendText("%d", workerCount);
              } // closure: failed to train idle unit

          }
          else if (u->getType() == UnitTypes::Terran_Barracks )
          {
              if (u->isIdle() && u->train(UnitTypes::Terran_Marine)) 
              {
                  //
              }
              
          }else if (u->getType() == UnitTypes::Terran_Factory)
          {
              if(u->isIdle() && u->getAddon() &&u->train(UnitTypes::Terran_Siege_Tank_Tank_Mode))
              {
                  //
              }
          }
          else if (u->getType() == UnitTypes::Terran_Comsat_Station) 
          {
              Unit enemy = u->getClosestUnit(Filter::IsEnemy && !IsVisible);
              if(scanner != NULL && enemy != NULL)
                  scanner->useTech(TechTypes::Scanner_Sweep, enemy->getPosition());
          }
      }
      else {
          if (u->isIdle() && attacking)
          {
              Unit enemy = u->getClosestUnit(Filter::IsEnemy);
              if (enemy != NULL) {
                  u->attack(enemy->getPosition());
              }
          }
      }
  
  } // closure: unit iterator
  
}

void ExampleAIModule::onSendText(std::string text)
{
    Broodwar->sendText("%s", text.c_str());

    BWEM::utils::MapDrawer::ProcessCommand(text);
    //exits to surrender screen (can compile from there)
    if (text == "done") { Broodwar->leaveGame(); }
    // number reader
    if (text[0] >= '0' && text[0] <= '9') 
    {
        int num = 0;
        for (int c : text) 
        {
            num = (num * 10) + (c - '0');
        }
        Broodwar->setLocalSpeed(num);
    }
    
  // Send the text to the game if it is not being processed.
    


  // Make sure to use %s and pass the text as a parameter,
  // otherwise you may run into problems when you use the %(percent) character!

}

void ExampleAIModule::onReceiveText(BWAPI::Player player, std::string text)
{
  // Parse the received text
  Broodwar << player->getName() << " said \"" << text << "\"" << std::endl;
}

void ExampleAIModule::onPlayerLeft(BWAPI::Player player)
{
  // Interact verbally with the other players in the game by
  // announcing that the other player has left.
  Broodwar->sendText("Goodbye %s!", player->getName().c_str());
}

void ExampleAIModule::onNukeDetect(BWAPI::Position target)
{

  // Check if the target is a valid position
  if ( target )
  {
    // if so, print the location of the nuclear strike target
    Broodwar << "Nuclear Launch Detected at " << target << std::endl;
  }
  else 
  {
    // Otherwise, ask other players where the nuke is!
    Broodwar->sendText("Where's the nuke?");
  }

  // You can also retrieve all the nuclear missile targets using Broodwar->getNukeDots()!
}

void ExampleAIModule::onUnitDiscover(BWAPI::Unit unit)
{
    
   
}

void ExampleAIModule::onUnitEvade(BWAPI::Unit unit)
{
}

void ExampleAIModule::onUnitShow(BWAPI::Unit unit)
{
}

void ExampleAIModule::onUnitHide(BWAPI::Unit unit)
{
}

void ExampleAIModule::onUnitCreate(BWAPI::Unit unit)
{
    if (unit->getPlayer() == Broodwar->self()) {
        
        if (unitQueue.size() > 0) {
            if (unit->getType() == unitQueue[0].unit)
            {
                Broodwar->sendText(unit->getType().c_str());
                unitQueue.erase(unitQueue.begin());
            }
        }
    }
       


  if ( Broodwar->isReplay() )
  {
    // if we are in a replay, then we will print out the build order of the structures
    if ( unit->getType().isBuilding() && !unit->getPlayer()->isNeutral() )
    {
      int seconds = Broodwar->getFrameCount()/24;
      int minutes = seconds/60;
      seconds %= 60;
      //Broodwar->sendText("%.2d:%.2d: %s creates a %s", minutes, seconds, unit->getPlayer()->getName().c_str(), unit->getType().c_str());
    }
  }
}

void ExampleAIModule::onUnitDestroy(BWAPI::Unit unit)
{
    for (int i = 0; i < army.size(); i++) 
    {
        if (unit == army[i]) 
        {
            army.erase(army.begin() + i);
        }
    }
    
        
    try
    {
        if (unit->getType().isMineralField())    theMap.OnMineralDestroyed(unit);
        else if (unit->getType().isSpecialBuilding()) theMap.OnStaticBuildingDestroyed(unit);
    }
    catch (const std::exception& e)
    {
        Broodwar << "EXCEPTION: " << e.what() << std::endl;
    }
}

void ExampleAIModule::onUnitMorph(BWAPI::Unit unit)
{
    if (unit->getPlayer() == Broodwar->self()) {

        if (unitQueue.size() > 0) {
            if (unit->getType() == unitQueue[0].unit)
            {
                Broodwar->sendText(unit->getType().c_str());
                unitQueue.erase(unitQueue.begin());
            }
        }
    }

  if ( Broodwar->isReplay() )
  {
    // if we are in a replay, then we will print out the build order of the structures
    if ( unit->getType().isBuilding() && !unit->getPlayer()->isNeutral() )
    {
      int seconds = Broodwar->getFrameCount()/24;
      int minutes = seconds/60;
      seconds %= 60;
      Broodwar->sendText("%.2d:%.2d: %s morphs a %s", minutes, seconds, unit->getPlayer()->getName().c_str(), unit->getType().c_str());
    }
  }
}

void ExampleAIModule::onUnitRenegade(BWAPI::Unit unit)
{
}

void ExampleAIModule::onSaveGame(std::string gameName)
{
  Broodwar << "The game was saved to \"" << gameName << "\"" << std::endl;
}

void ExampleAIModule::onUnitComplete(BWAPI::Unit unit)
{
    
    if (unit->getPlayer() == Broodwar->self())
    {
        
        // Check If Buildings Are Created
        if (unit->getType().isBuilding())
        {
           
            if (unit->getType() == UnitTypes::Terran_Supply_Depot)
            {
                predictSupply -= 8;
                //Broodwar->sendText(" %d ", predictSupply);
            }
            if (unit->getType() == UnitTypes::Terran_Barracks) 
            {
                //Broodwar->sendText("new barracks %s", unit->getType().c_str());
               
               
            }
            if (unit->getType() == UnitTypes::Terran_Refinery)
            {
                

                ref.refinery = unit;
                ref.refWorkers[0] = unit->getClosestUnit();
                ref.refWorkers[0]->gather(ref.refinery);

                for (int i = 1; i < 3; i++) {
                    ref.refWorkers[i] = FindWorker(ref.refinery);


                    if (!ref.refWorkers[i]->isGatheringGas()) 
                    {
                        //Broodwar->sendText("testing %s", ref.refWorkers[i]);
                        ref.refWorkers[i]->gather(ref.refinery);
                    }
                    

                    ref.refWorkers[i]->gather(ref.refinery);
                }
            }
            if (unit->getType() == UnitTypes::Terran_Comsat_Station) { scanner = unit;}
            if (unit->getType() == UnitTypes::Terran_Factory)
            {
                PreUnit machineShop;
                machineShop.builder = unit;
                machineShop.unit = UnitTypes::Terran_Machine_Shop;
                unitQueue.push_back(machineShop);
                //unit->buildAddon(UnitTypes::Terran_Machine_Shop);
            }
            if ( unit->getType() == UnitTypes::Terran_Academy )
            {
                PreUnit comsat;
                for (auto& u : Broodwar->self()->getUnits()) 
                {
                    if (u->getType() == UnitTypes::Terran_Command_Center) 
                    {
                        comsat.builder = u;
                        break;
                    }
                }
                comsat.unit = UnitTypes::Terran_Comsat_Station;
                unitQueue.push_back(comsat);
                //unit->buildAddon(UnitTypes::Terran_Machine_Shop);
            }
        }else{
            CheckBuild();
            
            if (unit->getType() == UnitTypes::Terran_SCV)
            {

                workers[workerCount] = unit;
                workerCount++;
            }
            else
            {
                Position pos = { Broodwar->mapWidth() * 16,Broodwar->mapHeight() * 16 };
                Position pos2 = { Broodwar->self()->getStartLocation().x * 32,Broodwar->self()->getStartLocation().y * 32 };
                Position pos3 = { (pos.x + pos2.x) / 2,(pos.y + pos2.y) / 2 };

                army.push_back(unit);
                if (army.size() >= 25)
                {
                    if (attacking != true){
                        Broodwar->sendText("this is a army size: %d", army.size());
                        attacking = true;
                    }
                }
                else { 
                    Broodwar->sendText("this is attacking: %s", attacking? "true" : "false");
                    attacking = false;
                }
                

                if (unit->getType() == UnitTypes::Terran_Marine)
                {



                    unit->move(pos3);
                    //Broodwar->sendText("x:%d y:%d", pos3.x, pos3.y);
                }
                if (unit->getType() == UnitTypes::Terran_Siege_Tank_Tank_Mode)
                {

                   
                    unit->move(pos3);
                    //Broodwar->sendText("x:%d y:%d", pos3.x, pos3.y);
                }
            }
        }
    }
}
