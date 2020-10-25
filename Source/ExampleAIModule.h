#pragma once
#include <BWAPI.h>




// Remember not to use "Broodwar" in any global class constructor!

class ExampleAIModule : public BWAPI::AIModule
{
public:
  // Virtual functions for callbacks, leave these as they are.
	
  virtual void onStart();
  virtual void onEnd(bool isWinner);
  virtual void onFrame();
  virtual void onSendText(std::string text);
  virtual void onReceiveText(BWAPI::Player player, std::string text);
  virtual void onPlayerLeft(BWAPI::Player player);
  virtual void onNukeDetect(BWAPI::Position target);
  virtual void onUnitDiscover(BWAPI::Unit unit);
  virtual void onUnitEvade(BWAPI::Unit unit);
  virtual void onUnitShow(BWAPI::Unit unit);
  virtual void onUnitHide(BWAPI::Unit unit);
  virtual void onUnitCreate(BWAPI::Unit unit);
  virtual void onUnitDestroy(BWAPI::Unit unit);
  virtual void onUnitMorph(BWAPI::Unit unit);
  virtual void onUnitRenegade(BWAPI::Unit unit);
  virtual void onSaveGame(std::string gameName);
  virtual void onUnitComplete(BWAPI::Unit unit);
  // Everything below this line is safe to modify.
  BWAPI::Unit ExampleAIModule::FindWorker(BWAPI::Unit caller);
  void ExampleAIModule::DoBuilding(BWAPI::UnitType building, BWAPI::TilePosition = {});
  virtual void CheckBuild();

  class Refinery
  {
  public:
	  BWAPI::Unit refinery;
	  BWAPI::Unit refWorkers[3];
  };

  int rackCount = 0;
  //BWAPI::Unit *Racks;
  int workerCount = 0;
  int workerGoal = 20;
  
  
  BWAPI::TilePosition mainBase;
  Refinery ref;
  
};
