#include "StdAfx.h"
#include "TMCCInterface.h"
#include <json.hpp>
using json = nlohmann::json;


class EngineManagement
{
public:
  static void AddEngineToJson(json& j, int engineID, int currentEngineType, bool isLegacy, const std::string& appDir);
  static void ReadEngineRoster(json& engineRoster, const std::string& appDir);
  static int ReadEngineIDFromJson(json& engineRoster, int engineID);
  static int ReadEngineTypeFromJson(json& engineRoster, int engineID);
  static bool ReadEngineLegacyConditionFromJson(json& engineRoster, int engineID);
  static void WriteEngineTypeToJson(json& engineRoster, int engineID, int engineType, const std::string& appDir);
  static void WriteEngineLegacyConditionToJson(json& engineRoster, int engineID, bool legacyState, const std::string& appDir);
  static bool EngineExistsInJson(json& engineRoster, int engineID);
  static void WriteEngineRoster(json& engineRoster, const std::string& appDir);
;
};