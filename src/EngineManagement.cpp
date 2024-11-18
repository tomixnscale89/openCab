
#include "EngineManagement.h"
//#include "StdAfx.h"
#include <cstdlib>
#include "TMCCInterface.h"
#include <thread>
#include "json.hpp"
#include <fstream>
#include "interface/throttlemenu.h"

using json = nlohmann::json;

// RD2TMCC ReadEngineRoster
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

// OpenCAB ReadEngineRoster

void EngineManagement::ReadEngineRoster(json & engineRoster, json& switchRoster, json& accessoryRoster, json& groupRoster, std::vector<EngineDef>& enginedefs, std::vector<SwitchDef>& switchdefs, std::vector<AccessoryDef>& accessorydefs, const std::string& appDir)
{
  std::string dir = appDir.substr(0, appDir.find_last_of("/\\"));
  std::ifstream i(dir + "/engineRoster.json");
  if (i.fail()) // if we fail, the file doesn't exist, so create our own later
  {
    printf("No roster found. Please add some engines, switches, or accessories.\n");
    EngineManagement::WriteEngineRoster(engineRoster, appDir);
  }

  else
  {
    printf("Engine Roster found. Reading...");
    i >> engineRoster;

    for (const auto& engine : engineRoster["engines"])
    {
      EngineDef enginedef = EngineDef();
      enginedef.engineID = engine["engineID"].get<int>();
      enginedef.engineType = engine["engineType"].get<int>();
      enginedef.engineName = engine["engineName"];
      enginedef.engineRoadNumber = engine["engineRoadNumber"];
      enginedef.legacyEngine = engine["isLegacy"].get<bool>(); // remove this 

      enginedef.engineCtrlType = engine["engineCtrlType"].get<int>();
      enginedef.engineClassType = engine["engineClassType"].get<int>();
      enginedef.engineSndType = engine["engineSndType"].get<int>();
      enginedef.engineTouchPadLeftIconSetting = engine["engineTouchPadLeftIconSetting"].get<int>();
      enginedef.engineTouchPadRightSetting = engine["engineTouchPadRightSetting"].get<int>();
      enginedef.speedLimit = engine["speedLimit"].get<int>();
      enginedef.steam_labour_intensity = engine["steam_labour_intensity"].get<int>();
      enginedef.diesel_electric_rev_lvl = engine["diesel_electric_rev_lvl"].get<int>();
      enginedef.currentFuelLevel = engine["currentFuelLevel"].get<int>();
      enginedef.currentWaterLevel = engine["currentWaterLevel"].get<int>();
      enginedef.trainAddressRecorded = engine["trainAddressRecorded"].get<int>();
      enginedef.trainPosition = engine["trainPosition"].get<int>();
      enginedef.smoke_state_legacy = engine["smoke_state_legacy"].get<int>();
      enginedef.ditchLightState = engine["ditchLightState"].get<int>();
      enginedef.base_speed_record = engine["base_speed_record"].get<int>();
      enginedef.momentum = engine["momentum"].get<int>();
      enginedef.trainBrake = engine["trainBrake"].get<int>();
      enginedefs.push_back(enginedef);

    }
    printf("Engines done...");

    //for (const auto& switch_ : engineRoster["switches"])
    //{
    //  SwitchDef data = SwitchDef();
    //  data.switchID = switch_["switchID"].get<int>();
    //  data.switchName = switch_["switchName"];
    //  switchdefs.push_back(data);
    //}
    //printf("Switches done...");

    


  }

  std::ifstream i2(dir + "/accessoryRoster.json");
  if (i2.fail()) // if we fail, the file doesn't exist, so create our own later
  {
    printf("No roster found. Please add some accessories.\n");
    //EngineManagement::WriteEngineRoster(engineRoster, appDir);
  }

  else
  {
    printf("Accessory Roster found. Reading...");
    i2 >> accessoryRoster;

    for (const auto& accessory : accessoryRoster["accessories"])
    {
      AccessoryDef data = AccessoryDef();
      data.accessoryID = accessory["accessoryID"].get<int>();
      data.accessoryName = accessory["accessoryName"];
      data.isSensorTrack = accessory["isSensorTrack"].get<bool>();
      accessorydefs.push_back(data);
    }
    printf("Accessories done...");
  }

  std::ifstream i3(dir + "/switchRoster.json");
  if (i3.fail()) // if we fail, the file doesn't exist, so create our own later
  {
    printf("No roster found. Please add some switches.\n");
    //EngineManagement::WriteEngineRoster(engineRoster, appDir);
  }

  else
  {
    printf("Switch Roster found. Reading...");
    i3 >> switchRoster;

    for (const auto& switch_ : switchRoster["switches"])
    {
        SwitchDef data = SwitchDef();
        data.switchID = switch_["switchID"].get<int>();
        data.switchName = switch_["switchName"];
        switchdefs.push_back(data);
    }
    printf("Switches done...");
  }

  printf("\n");

}


void EngineManagement::WriteEngineRoster(json& engineRoster, const std::string& appDir)
{
  std::string dir = appDir.substr(0, appDir.find_last_of("/\\"));
  std::ofstream o(dir + "/engineRoster.json");
  o << std::setw(4) << engineRoster << std::endl;
}

void EngineManagement::WriteSwitchRoster(json& switchRoster, const std::string& appDir)
{
  std::string dir = appDir.substr(0, appDir.find_last_of("/\\"));
  std::ofstream o(dir + "/switchRoster.json");
  o << std::setw(4) << switchRoster << std::endl;
}

void EngineManagement::WriteAccessoryRoster(json& accessoryRoster, const std::string& appDir)
{
  std::string dir = appDir.substr(0, appDir.find_last_of("/\\"));
  std::ofstream o(dir + "/accessoryRoster.json");
  o << std::setw(4) << accessoryRoster << std::endl;
}

void EngineManagement::WriteGroupRoster(json& groupRoster, const std::string& appDir)
{
  std::string dir = appDir.substr(0, appDir.find_last_of("/\\"));
  std::ofstream o(dir + "/groupRoster.json");
  o << std::setw(4) << groupRoster << std::endl;
}

void EngineManagement::AddTrainDataToJson(json& engineRoster, std::vector<EngineDef> enginedefs, const std::string& appDir)
{
  std::vector<json> engines;
  for (const auto& engine : enginedefs)
  {
    json enginedata;
    enginedata["trainID"] = engine.engineID;
    enginedata["trainName"] = engine.engineName;
    enginedata["isLegacy"] = engine.legacyEngine;
    engines.push_back(enginedata);
  }
  engineRoster["engines"] = engines;
  //engineRoster["engines"] = engines;
  EngineManagement::WriteEngineRoster(engineRoster, appDir);

}

void EngineManagement::AddEngineDataToJson(json& engineRoster, std::vector<EngineDef> enginedefs, const std::string& appDir)
{
  std::vector<json> engines;
  for (const auto& engine : enginedefs)
  {
    json enginedata;
    enginedata["engineID"] = engine.engineID;
    enginedata["engineName"] = engine.engineName;
    enginedata["engineRoadNumber"] = engine.engineRoadNumber;
    enginedata["engineType"] = engine.engineType;
    enginedata["engineCtrlType"] = engine.engineCtrlType;
    enginedata["engineClassType"] = engine.engineClassType;
    enginedata["engineSndType"] = engine.engineSndType;
    enginedata["engineTouchPadLeftIconSetting"] = engine.engineTouchPadLeftIconSetting;
    enginedata["engineTouchPadRightSetting"] = engine.engineTouchPadRightSetting;
    enginedata["speedLimit"] = engine.speedLimit;
    enginedata["steam_labour_intensity"] = engine.steam_labour_intensity;
    enginedata["diesel_electric_rev_lvl"] = engine.diesel_electric_rev_lvl;
    enginedata["currentFuelLevel"] = engine.currentFuelLevel;
    enginedata["currentWaterLevel"] = engine.currentWaterLevel;
    enginedata["trainAddressRecorded"] = engine.trainAddressRecorded;
    enginedata["trainPosition"] = engine.trainPosition;
    enginedata["smoke_state_legacy"] = engine.smoke_state_legacy;
    enginedata["ditchLightState"] = engine.ditchLightState;
    enginedata["base_speed_record"] = engine.base_speed_record;
    enginedata["momentum"] = engine.momentum;
    enginedata["trainBrake"] = engine.trainBrake;
    enginedata["isLegacy"] = engine.legacyEngine;
    engines.push_back(enginedata);
  }
  engineRoster["engines"] = engines;
  //engineRoster["engines"] = engines;
  EngineManagement::WriteEngineRoster(engineRoster, appDir);

}

void EngineManagement::AddSwitchDataToJson(json& switchRoster, std::vector<SwitchDef> switchdefs, const std::string& appDir)
{
  std::vector<json> switches;
  for (const auto& switchT : switchdefs)
  {
    json data;
    data["switchID"] = switchT.switchID;
    data["switchName"] = switchT.switchName;
    switches.push_back(data);
  }
  switchRoster["switches"] = switches;
  EngineManagement::WriteSwitchRoster(switchRoster, appDir);

}

void EngineManagement::AddAccessoryDataToJson(json& groupRoster, std::vector<AccessoryDef> accdefs, const std::string& appDir)
{
  std::vector<json> accessories;
  for (const auto& accessory : accdefs)
  {
    json data;
    data["accessoryID"] = accessory.accessoryID;
    data["accessoryName"] = accessory.accessoryName;
    data["isSensorTrack"] = accessory.isSensorTrack;
    accessories.push_back(data);
  }
  groupRoster["accessories"] = accessories;
  EngineManagement::WriteAccessoryRoster(groupRoster, appDir);

}

void EngineManagement::WriteEngineIDToJson(json& engineRoster, int engineID, std::string engineName, const std::string& appDir)
{
  std::string temp = std::string("engine") + std::to_string(engineID);
  if (engineRoster.contains(temp)) // if the json has loco
  {
    engineRoster[temp][std::string("engineID")] = engineID;
    EngineManagement::WriteEngineRoster(engineRoster, appDir);
    //printf("wrote legacy state\n");
  }
}