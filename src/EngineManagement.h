//#include "StdAfx.h"
#include "TMCCInterface.h"
#include "json.hpp"
#include "interface/throttlemenu.h"

#pragma once

using json = nlohmann::json;


class EngineManagement
{
public:
  static void ReadEngineRoster(json& engineRoster, const std::string& appDir);
  static void ReadEngineRoster(json& engineRoster, std::vector<EngineDef>& enginedefs, std::vector<SwitchDef>& switchdefs, std::vector<AccessoryDef>& accessorydefs, const std::string& appDir);
  static void WriteEngineRoster(json& engineRoster, const std::string& appDir);
  static void AddEngineDataToJson(json& engineRoster, std::vector<EngineDef> enginedefs, const std::string& appDir);
  static void AddSwitchDataToJson(json& engineRoster, std::vector<SwitchDef> switchdefs, const std::string& appDir);
  static void AddAccessoryDataToJson(json& engineRoster, std::vector<AccessoryDef> accdefs, const std::string& appDir);

;
};