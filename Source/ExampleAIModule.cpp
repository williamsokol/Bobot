#include "ExampleAIModule.h"
#include "BuildQueue.h"
#include "UI.h"


using namespace BWAPI;
using namespace Filter;


using namespace BWEM;
using namespace BWEM::BWAPI_ext;
//using namespace BWEM::utils;
namespace { auto& theMap = BWEM::Map::Instance(); }




TilePosition origin;

const Base* thing;
std::vector<Unit> army;
bool attacking = false;

BuildQueue* buildQueue;
UI* ui;


Unit ExampleAIModule::FindWorker(Unit caller)
{
    //Broodwar->sendText("%d", *(&workers + 1) - workers);
    
    for (int i=0; i<workers.size();i++)
    {
        // &&
        
        if (workers[i] != NULL && !(workers)[i]->isGatheringGas() && !(workers)[i]->isConstructing())
        {
            
            //system("pause");
            Unit tmp = workers[i];              //fake name
            workers.erase(workers.begin() + i); // kills real name
            workers.push_back(tmp);             //renames real name

            return tmp;
            
        }
    }
    system("pause");
    //return NULL;
}

bool ExampleAIModule::DoBuilding(UnitType* building, TilePosition* buildPosition,Unit* worker )
{
    if (building->isAddon()) {
        return (*worker)->buildAddon(*building);
    }
    if (*worker == NULL) {
        *worker = FindWorker(NULL);
    }
    if (!buildPosition->isValid()) {
        *buildPosition = Broodwar->getBuildLocation(*building, (*worker)->getTilePosition());
    }
    if (!Broodwar->isVisible(*buildPosition)) {
        (*worker)->move(Position(*buildPosition));
        return true;
    }

    //Broodwar->sendText(" new %s", building.c_str());
    if ((*worker)->build(*building, *buildPosition)) {
        return true;
    }
    else {
        ui->marked.push_back(*worker);
        //Broodwar->sendText("%s", UI::marked.push_back(*worker));
        Broodwar->setLocalSpeed(200);
        return false;
    }
    
    
}



void ExampleAIModule::onStart()
{
    
    try
    {
        // Hello World!
        Broodwar->sendText("test build 4");

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
    buildQueue = new BuildQueue(this);
    ui = new UI(this);
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
    
   
    ui->Display();
  
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

      for (const ChokePoint* chokePoint : area.ChokePoints()) {
          Broodwar->drawTextMap({ chokePoint->Center().x * 8,chokePoint->Center().y * 8 }, "%c ChokePoint", Text::Yellow, i);
      }
      

  }
  // calculate closest chokepoint
  int nearestArea;
  //const ChokePoint *chokePoint = theMap.Areas()[0].ChokePoints()[0];//[0].ChokePointsByArea().begin();
  //Broodwar->drawTextMap({ chokePoint->Center().x * 8,chokePoint->Center().y * 8 }, "%c ChokePoint", Text::Yellow, i);

  Broodwar->drawTextScreen(20, 40, "picked base num %d  main=%d,%d", a, mainBase.x, mainBase.y);
   
  // Return if the game is a replay or is paused
  if ( Broodwar->isReplay() || Broodwar->isPaused() || !Broodwar->self() )
    return;

  // Prevent spamming by only running our onFrame once every number of latency frames.
  // Latency frames are the number of frames before commands are processed.
  if ( Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0 )
    return;

 
  // builds the base
  if (unitQueue.size() >= 1 && Broodwar->getFrameCount() % 10 == 0)
  {
      
      for (int i = 0; i < unitQueue.size(); i++)
      {
          // if we can afford the building
          if (unitQueue[i].valid == false && 
              unitQueue[i].unit.mineralPrice() < InternalResources::GetResource("mineral") && 
              unitQueue[i].unit.gasPrice() < InternalResources::GetResource("gas")  )
          {
              continue;
          }
          else if(unitQueue[i].unit == false)
          {
              unitQueue[i].valid = true;
              InternalResources::UpdateResource("mineral", unitQueue[i].unit.mineralPrice());
              InternalResources::UpdateResource("gas", unitQueue[i].unit.gasPrice());
             
          }

         
          
          if (unitQueue[i].builder == NULL ||!unitQueue[i].builder->isConstructing() && Broodwar->canMake(unitQueue[i].unit) && !unitQueue[i].builder->isMoving())
          {
              
              DoBuilding(&unitQueue[i].unit, &unitQueue[i].pos, &unitQueue[i].builder);
             
          }
         
      }
      
    }
  
   

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
          //if (u == baseBuilder) 
          //{
          //    //Broodwar->sendText("done: %s", u->getTilePosition() == thing->Location() ? "true" : "false");
          //    if (u->isIdle() )
          //    {
          //        DoBuilding(UnitTypes::Terran_Command_Center, thing->Location(),baseBuilder);
          //        //Broodwar->sendText("done: %s", DoBuilding(UnitTypes::Terran_Command_Center, thing->Location())? "true":"false");
          //        baseBuilder = NULL;
          //    }
          //}else 
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
              Unit enemy = u->getClosestUnit(Filter::IsEnemy && IsVisible && !IsDetected);
              if (scanner != NULL && enemy != NULL) {
                  if(enemy->isAttacking())
                      scanner->useTech(TechTypes::Scanner_Sweep, enemy->getPosition());

              }
          }
      }
      else {
          if ( attacking) //checks if unit and if we should attack
          {
              if (u->isIdle()) {
                  Unit enemy = u->getClosestUnit(Filter::IsEnemy);
                  if (enemy != NULL) {
                      u->attack(enemy->getPosition());
                  }
              }
          }
          else {
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
            for (int i = 0; i < unitQueue.size(); i++)
            {
                
                if (unit->getTilePosition() == unitQueue[0].pos)
                {
                    Broodwar->sendText(unit->getType().c_str());
                    unitQueue.erase(unitQueue.begin());
                }
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
    if (unit->getType() == UnitTypes::Terran_Supply_Depot) 
    {
        InternalResources::UpdateResource("supply", -8);
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
        
        for (int i = 0; i < unitQueue.size(); i++)
        {

            if (unit->getTilePosition() == unitQueue[0].pos)
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
                InternalResources::UpdateResource("supply", -8);
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
                PreUnit machineShop(UnitTypes::Terran_Machine_Shop);
                machineShop.builder = unit;
                unitQueue.push_back(machineShop);
                //unit->buildAddon(UnitTypes::Terran_Machine_Shop);
            }
            if ( unit->getType() == UnitTypes::Terran_Academy )
            {
                PreUnit comsat(UnitTypes::Terran_Comsat_Station);
                for (auto& u : Broodwar->self()->getUnits()) 
                {
                    if (u->getType() == UnitTypes::Terran_Command_Center) 
                    {
                        comsat.builder = u;
                        break;
                    }
                }
                
                unitQueue.push_back(comsat);
                //unit->buildAddon(UnitTypes::Terran_Machine_Shop);
            }
        }else{
            
           
            //Broodwar->sendText("this is the unit!!!!!!!!!!!!%s", unit->getType().c_str());
            buildQueue->CheckBuild(unit,thing);
            
            if (unit->getType() == UnitTypes::Terran_SCV)
            {
                workers.push_back(unit);
                
            }
            else
            {
               
                // gets the chokepoints on the map from you to the enemy
                Position pos3;
                /*TilePosition a = Broodwar->self()->getStartLocation();
                TilePosition b = Broodwar->enemy()->getStartLocation();
                int length;
                const CPPath& Path = theMap.GetPath(Position(a), Position(b), &length);
                if (length > 0 && !Path.empty()) {
                    pos3 = Position(Path[1]->Center());
                }*/
                CPPath thing = theMap.GetNearestArea(Broodwar->self()->getStartLocation())->ChokePoints();
                pos3 = Position(thing[army.size() % thing.size()]->Center());

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
