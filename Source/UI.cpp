#include "UI.h"

using namespace BWAPI;
using namespace Filter;


using namespace BWEM;
using namespace BWEM::BWAPI_ext;
//using namespace BWEM::utils;
namespace { auto& theMap = BWEM::Map::Instance(); }

void UI::Display() 
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
    
    //display fps
    Broodwar->drawTextScreen(200, 0, "FPS: %d", Broodwar->getFPS());
    Broodwar->drawTextScreen(200, 20, "Average FPS: %f", Broodwar->getAverageFPS());
    //display time
    Broodwar->drawTextScreen(20, 20, "time %.2d:%.2d", minutes, seconds);

    // display internal resources
    Broodwar->drawTextScreen(500, 40, "internal supply? %d/%d",Broodwar->self()->supplyUsed()/2,InternalResources::GetResource("supply") );
    Broodwar->drawTextScreen(500, 50, "internal mineral? %d", InternalResources::GetResource("mineral"));
    Broodwar->drawTextScreen(500, 60, "need gas? %d",InternalResources::GetResource("gas"));

    //display list of building units
    for (int i = 0; i < bot->unitQueue.size(); i++)
    {
        Broodwar->drawTextScreen(20, i * 10 + 60, "building %s is valid: %s at: (%d,%d)",
            bot->unitQueue[i].unit.c_str(),
            bot->unitQueue[i].valid ? "true" : "false",
            bot->unitQueue[i].pos.x, bot->unitQueue[i].pos.y);

        Broodwar->drawBoxMap(Position(bot->unitQueue[i].pos), Position(bot->unitQueue[i].pos+ UnitType(bot->unitQueue[i].unit).tileSize()), Colors::Teal);
       
    }
    
    for (int i=0; i< marked.size();i++)
    {
        Broodwar->drawTextMap(marked[i]->getPosition(), "%c marked %d", Text::Yellow, i);

        
    }

    // display tags for workers on refineries
    if (bot->ref.refinery) {
        int i=0;
        for (Unit w : bot->ref.refWorkers)
        {
            Broodwar->drawTextMap(w->getPosition(), "%c Invis %d", Text::Yellow, i);

            i++;
        }
    }
}