
#include "EngineManagement.h"
//#include "StdAfx.h"
#include <cstdlib>
#include "TMCCInterface.h"
#include <thread>
#include "json.hpp"
#include <fstream>
#include "interface/throttlemenu.h"

using json = nlohmann::json;

void EngineManagement::ReadEngineRoster(json& engineRoster, const std::string& appDir)
{
  std::string dir = appDir.substr(0, appDir.find_last_of("/\\"));
  std::ifstream i(dir + "/engineRoster.json");
  if (i.fail()) // if we fail, the file doesn't exist, so create our own later
  {
    printf("No roster found. Creating...\n");
    WriteEngineRoster(engineRoster, appDir);
  }
    
  
  else
  {
    printf("Roster found. Reading...\n");
    i >> engineRoster;
  }
}

void EngineManagement::WriteEngineRoster(json& engineRoster, const std::string& appDir)
{
  std::string dir = appDir.substr(0, appDir.find_last_of("/\\"));
  std::ofstream o(dir + "/engineRoster.json");
  o << std::setw(4) << engineRoster << std::endl;
}



void EngineManagement::AddEngineToJson(json& engineRoster, int engineID, int currentEngineType, bool isLegacy, const std::string& appDir)
{
  //if (engineRoster)
  //{
    if(0 <= engineID <= 99)
    {
      engineRoster[std::string("engine") + std::to_string(engineID)] = {
      {"engineID", engineID},
      {"engineType", currentEngineType},
      {"isLegacy", isLegacy} };
      //}
      WriteEngineRoster(engineRoster, appDir);
    }
    }


int EngineManagement::ReadEngineIDFromJson(json& engineRoster, int engineID)
{
  std::string temp = std::string("engine") + std::to_string(engineID);

  if (engineRoster.contains(temp)) // if the json has a key for engineType
  {
    printf("Contains Engine:%d, engine ID is: %d\n", engineID, (int)engineRoster[temp][std::string("engineID")]);
    if (!engineRoster[temp][std::string("engineType")].is_null()) // if the engine ID is not null
    {
      if (0 <= (int)engineRoster[temp][std::string("engineID")] <= 99) // check if engine id is within range, not sure if this could be better
      {
        int engineIDFound = (int)engineRoster[temp][std::string("engineID")];
        printf("Setting engine ID to:%d\n", engineIDFound);
        return engineIDFound;
      }
    }
  }
}

int EngineManagement::ReadEngineTypeFromJson(json& engineRoster, int engineID)
{
  std::string temp = std::string("engine") + std::to_string(engineID);

  if (engineRoster.contains(temp)) // if the json has a key for engineType
  {
    printf("Contains Engine:%d, engine type is: %d\n", engineID, (int)engineRoster[temp][std::string("engineType")]);
    if (!engineRoster[temp][std::string("engineType")].is_null()) // if the engine Type is not null
    {
      if (0 <= (int)engineRoster[temp][std::string("engineType")] <= 12)
      {
        int engineTypeFound = (int)engineRoster[temp][std::string("engineType")];
        printf("Setting engine type:%d\n", engineTypeFound);
        return engineTypeFound;
      }
    }
  }
}

bool EngineManagement::ReadEngineLegacyConditionFromJson(json& engineRoster, int engineID)
{
  std::string temp = std::string("engine") + std::to_string(engineID);

  if (engineRoster.contains(temp)) // if the json has a key for engineType
  {
    printf("Contains Engine:%d, Is a Legacy loco: %d\n", engineID, (bool)engineRoster[temp][std::string("isLegacy")]);
    if (!engineRoster[temp][std::string("isLegacy")].is_null()) // if the engine Type is not null
    {
        bool engineLegacyType = (bool)engineRoster[temp][std::string("isLegacy")];
        printf("Setting Legacy:%d\n", engineLegacyType);
        return engineLegacyType;
    }
  }
}

void EngineManagement::WriteEngineTypeToJson(json& engineRoster, int engineID, int engineType, const std::string& appDir)
{
  std::string temp = std::string("engine") + std::to_string(engineID);
  if (engineRoster.contains(temp)) // if the json has loco
  {
    engineRoster[temp][std::string("engineType")] = engineType;
    WriteEngineRoster(engineRoster, appDir);
    //printf("wrote legacy state\n");
  }
}

void EngineManagement::WriteEngineLegacyConditionToJson(json& engineRoster, int engineID,bool legacyState, const std::string& appDir)
{
  std::string temp = std::string("engine") + std::to_string(engineID);
  if (engineRoster.contains(temp)) // if the json has loco
  {
    engineRoster[temp][std::string("isLegacy")] = legacyState;
    WriteEngineRoster(engineRoster, appDir);
    //printf("wrote legacy state\n");
  }
}

bool EngineManagement::EngineExistsInJson(json& engineRoster, int engineID)
{
  std::string temp = std::string("engine") + std::to_string(engineID);

  if (engineRoster.contains(temp)) // if the json has a key for engineType
  {
    return true;
  }
  else
    return false;
}