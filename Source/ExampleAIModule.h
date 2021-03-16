#pragma once
#include <BWAPI.h>
#include "BWEM 1.4.1/src/bwem.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>
#include "internalResources.h"



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
  bool ExampleAIModule::DoBuilding(BWAPI::UnitType building, BWAPI::TilePosition = {}, BWAPI::Unit worker = NULL);
  
  //virtual void Attack(BWAPI::Position pos, std::vector<BWAPI::Unit> squad);

  class Refinery
  {
  public:
	  BWAPI::Unit refinery;
	  BWAPI::Unit refWorkers[3];
  };
  struct PreUnit 
  {
	  bool valid = false;
	  BWAPI::UnitType unit;
	  BWAPI::Unit builder = NULL;
	  BWAPI::TilePosition pos = { -1,-1 };
	  

  };
  
  int rackCount = 0;
  //BWAPI::Unit *Racks;
  int workerCount = 0;
  int workerGoal = 20;
  
  int predictSupply = 0;

  std::vector<PreUnit> unitQueue;
  BWAPI::Unit baseBuilder;
  BWAPI::Unit scanner;

  std::vector<Unit> workers;
  
  BWAPI::TilePosition mainBase;
  Refinery ref;
  
};
