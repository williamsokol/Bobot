#include "ExampleAIModule.h"
#include <iostream>
#include <fstream>
#include "BWEM 1.4.1/src/bwem.h"
#include <cassert> 

using namespace BWAPI;
using namespace Filter;

namespace { auto& theMap = BWEM::Map::Instance(); }

int predictSupply = 0;
Unit workers[100];

int a = 0;


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
            Broodwar->sendText("used workers %d", i );


            return worker;
            
        }
    }
    //system("pause");
    //return NULL;
}

void ExampleAIModule::CheckBuild() 
{
    int currentSupply = Broodwar->self()->supplyUsed() / 2;
    int totalSupply = (Broodwar->self()->supplyTotal() / 2) + predictSupply;

    Unit worker;
    //Broodwar->sendText(" neede %f %d %s",1.16*currentSupply,totalSupply, 1.16 * currentSupply>= totalSupply? "true": "false");
    // all of the scv and building creation, I based it on supply count :)
    if ((1.16 * currentSupply) >= totalSupply)
    {
       
        worker = FindWorker(NULL);
        TilePosition buildPosition = Broodwar->getBuildLocation(UnitTypes::Terran_Supply_Depot, worker->getTilePosition());
        worker->build(UnitTypes::Terran_Supply_Depot, buildPosition);
        Broodwar->sendText(" building needed %s", worker->getTarget()->getType().c_str());

        predictSupply = predictSupply + 8;
    }
    else if (currentSupply == 11 || currentSupply == 13)
    {
        //Broodwar->sendText(" %s", unit->getType().c_str());
        worker = FindWorker(NULL);
        TilePosition buildPosition = Broodwar->getBuildLocation(UnitTypes::Terran_Barracks, worker->getTilePosition());
        worker->build(UnitTypes::Terran_Barracks, buildPosition);
    }
    else if (currentSupply == 18)
    {
        //Broodwar->sendText(" %s", unit->getType().c_str());
        worker = FindWorker(NULL);
        TilePosition buildPosition = Broodwar->getBuildLocation(UnitTypes::Terran_Refinery, worker->getTilePosition());
        worker->build(UnitTypes::Terran_Refinery, buildPosition);
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
        //Broodwar->enableFlag(Flag::CompleteMapInformation);

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

  // Return if the game is a replay or is paused
  if ( Broodwar->isReplay() || Broodwar->isPaused() || !Broodwar->self() )
    return;

  // Prevent spamming by only running our onFrame once every number of latency frames.
  // Latency frames are the number of frames before commands are processed.
  if ( Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0 )
    return;

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
          CheckBuild();

          // if our worker is idle
          if (u->isIdle())
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
              
          }
      }
  
  } // closure: unit iterator
}

void ExampleAIModule::onSendText(std::string text)
{
    BWEM::utils::MapDrawer::ProcessCommand(text);
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
    Broodwar->sendText("%s", text.c_str());


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
    //army
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
                Broodwar->sendText("new barracks %s", unit->getType().c_str());
               
               
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
                        Broodwar->sendText("testing %s", ref.refWorkers[i]);
                        ref.refWorkers[i]->gather(ref.refinery);
                    }
                    

                    ref.refWorkers[i]->gather(ref.refinery);
                }
            }
        }else{
            
            if (unit->getType() == UnitTypes::Terran_SCV)
            {

                workers[workerCount] = unit;
                workerCount++;
            }
            if (unit->getType() == UnitTypes::Terran_Marine) 
            {
                if (army.empty()) 
                {
                    //Broodwar->sendText("empty");
                }
                Position pos = { Broodwar->mapWidth() * 16,Broodwar->mapHeight() * 16 };
                Position pos2 = { Broodwar->self()->getStartLocation().x * 32,Broodwar->self()->getStartLocation().y * 32 };
                Position pos3 = { (pos.x + pos2.x) / 2,(pos.y + pos2.y) / 2 };

                unit->move(pos3);
                //Broodwar->sendText("x:%d y:%d", pos3.x, pos3.y);
            }
            
        }
    }
}
