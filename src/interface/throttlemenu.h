#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <stdio.h>
#include <TMCCInterface.h>
#include "../json.hpp"
#include "../imgui-knobs.h"
#include "types.h"
#include "../Image.h"
#include "KeypadTypes.h"

#pragma once

using json = nlohmann::json;


enum EngineTypeLegacy : uint8
{
  ENGINE_TYPE_STEAM = 0x0,
  ENGINE_TYPE_DIESEL = 0x1,
  ENGINE_TYPE_ELECTRIC = 0x2,
  ENGINE_TYPE_STATIONSOUND_CAR = 0x3,
  ENGINE_TYPE_SUBWAY = 0x4,
  ENGINE_TYPE_STOCK_CAR = 0x5,
  ENGINE_TYPE_CRANE_CAR = 0xD
};

enum EngineTypeTMCC : uint8
{
  ENGINE_TYPE_TMCC_STEAM = 0x6,
  ENGINE_TYPE_TMCC_DIESEL = 0x7,
  ENGINE_TYPE_TMCC_ELECTRIC = 0x8,
  ENGINE_TYPE_TMCC_CAR = 0x9,
  ENGINE_TYPE_TMCC_CRANE = 0xA,
  ENGINE_TYPE_TMCC_ACELA = 0xB,
  ENGINE_TYPE_TMCC_BREAKDOWN_UNIT = 0xC
};



struct EngineDef
{
  int engineID;
  int engineType;
  bool legacyEngine = false;
  std::string engineName;
  // add temp engine speed
  float legacy_speed = 0;
  float temp_legacy_speed = 0;
  float tmcc_speed = 0;
  int steam_labour_intensity = 0;
  int diesel_electric_rev_lvl = 0;
  int smoke_state_legacy = 0;
  int currentDieselFuel = 0;
  int currentCoalLevel = 0;
  int currentWaterLevel = 0;
  int keyPadPage = 0;
  bool useHorn2 = false;
  bool oneShotBellEnabled = false;
  int bellDingCount = 3;
  bool bellOn = false;
  float currentTrainBrake;
  float currentQuill = 0;

  float gamepadTrainBrake = 0.0f;

  std::shared_ptr<Image> locoIcon;
  bool engineHasCustomIcon = false;

  float legacy_speed_multiplier = 1.0f;

  bool brakeSoundEnabled = true;

  int previousFinalSpeed = 0;

  //inline int GetFinalSpeed(bool tmcc2, bool prevValBigger)
  //{
  //    return (int)(legacy_speed * legacy_speed_multiplier);
  //}

  //void EngineDef::SetSpeed(float value, bool tmcc2)
  //{
  //  // don't send a command if the speed hasn't changed
  //  //if (value != legacy_speed)
  //  //{
  //    legacy_speed = value;
  //    float val = GetFinalSpeed(tmcc2,false);
  //    TMCCInterface::EngineSetAbsoluteSpeed2(engineID, val);
  //    printf("speed: %f, new Speed: %f\n", legacy_speed, val);

  //  //}
  //}

  void EngineDef::UpdateSpeed()
  {
    
    if (legacyEngine)
    {
      int finalSpeed = (int)(legacy_speed * legacy_speed_multiplier);
      printf("FinalSpeed: %d\n", finalSpeed);
      TMCCInterface::EngineSetAbsoluteSpeed2(engineID, finalSpeed);
    }
    else
    {
      // TMCC variant
      int finalSpeed = (int)(tmcc_speed * legacy_speed_multiplier);
      // check if the previous speed is less than the new, 
      // this is to accomodate some behaviour with some tmcc engines and legacy
      // the relative speed command should do nothing, since we will send another absolute command after
      // it's to fix the rpm sounds not working

      if (previousFinalSpeed < finalSpeed) 
      {
        TMCCInterface::EngineSetRelativeSpeed(engineID, 1);
        printf("Relative Speed: %d\n", 1);

      }
      else if(previousFinalSpeed > finalSpeed)
      {
        TMCCInterface::EngineSetRelativeSpeed(engineID, -1);
        printf("Relative Speed: %d\n", -1);
      }
      previousFinalSpeed = finalSpeed;
      printf("FinalSpeed TMCC: %d\n", finalSpeed);
      TMCCInterface::EngineSetAbsoluteSpeed(engineID, finalSpeed);
    }
    
  }

  void EngineDef::SetSpeedMultiplier(float value, bool tmcc2, bool braking)
  {
    // don't send a command if the speed hasn't changed
    if (floorf(value * 10) / 10 != legacy_speed_multiplier)
    {
      if (braking && tmcc2)
      {
        TMCCInterface::EngineSetTrainBrake2(engineID, (int)(currentTrainBrake * 8.0f));
        if (brakeSoundEnabled)
          TMCCInterface::EngineBrakeSquealSound(engineID);

        if (engineType == EngineTypeLegacy::ENGINE_TYPE_STEAM)
        {
          steam_labour_intensity = (int)(currentTrainBrake * 32.0f);
          TMCCInterface::EngineSetLabor(engineID, steam_labour_intensity);
        }
        else
        {
          diesel_electric_rev_lvl = (int)(currentTrainBrake * 8.0f);
          TMCCInterface::EngineSetDieselRunLevel(engineID, diesel_electric_rev_lvl);
        }
      }
      //printf("legacy_speed_multiplier:%f\n", value);
      legacy_speed_multiplier = floorf(value * 10) / 10;
      UpdateSpeed();
      //printf("New speed: %f, multipler: %f\n", legacy_speed, legacy_speed_multiplier);
      
    }
  }
};

struct TrainDef
{
  int trainID;
  int engineType;
  bool legacyTrain = false;
  std::string trainName;
  // add temp engine speed
  float legacy_speed = 0;
  float temp_legacy_speed = 0;
  float tmcc_speed = 0;
  int steam_labour_intensity = 0;
  int diesel_electric_rev_lvl = 0;
  int smoke_state_legacy = 0;
  int keyPadPage = 0;
  bool useHorn2 = false;
  bool oneShotBellEnabled = false;
  int bellDingCount = 3;
  bool bellOn = false;
  float currentTrainBrake;
  float currentQuill = 0;


  std::shared_ptr<Image> locoIcon;
  bool engineHasCustomIcon = false;

  float legacy_speed_multiplier = 1.0f;

  inline int GetFinalSpeed(bool tmcc2)
  {
    if (tmcc2) return temp_legacy_speed;
    return (int)(temp_legacy_speed * legacy_speed_multiplier * 200.0f);
  }

  void TrainDef::SetSpeed(float value, bool tmcc2)
  {
    // don't send a command if the speed hasn't changed
    if (value != temp_legacy_speed)
    {
      temp_legacy_speed = value;
      TMCCInterface::TrainSetAbsoluteSpeed2(trainID, GetFinalSpeed(tmcc2));
      printf("speed: %f, new Speed: %f\n", temp_legacy_speed, legacy_speed_multiplier, GetFinalSpeed(tmcc2));

    }
  }

  void TrainDef::SetSpeedMultiplier(float value, bool tmcc2)
  {
    // don't send a command if the speed hasn't changed
    if (value != legacy_speed_multiplier)
    {
      //printf("legacy_speed_multiplier:%f\n", value);
      legacy_speed_multiplier = value;
      TMCCInterface::TrainSetAbsoluteSpeed(trainID, GetFinalSpeed(tmcc2));
      printf("speed: %f, multipler: %f, new Speed: %d\n", temp_legacy_speed, legacy_speed_multiplier, GetFinalSpeed(tmcc2));
    }
  }
};

struct SwitchDef
{
  int switchID;
  std::string switchName;
};

struct AccessoryDef
{
  int accessoryID;
  std::string accessoryName;
  bool isSensorTrack;
};

struct RouteDef
{
  int routeID;
  std::string routeName;
  int switchIDs[];
};

struct GroupDef
{
  int groupID;
  std::string groupName;
  int accessoryIDs[];
};



class ThrottleMenu
{
public:
  ThrottleMenu(const std::string& appDir, SDL_GameController* gGameController, std::string gGameControllerName);
  void LoadRosterLaunch(const std::string& appDir);
  ~ThrottleMenu();
  void Draw(const std::string& appDir, SDL_GameController* gGameController, int leftStickXDeadZone, int leftStickYDeadZone, int rightStickXDeadZone, int rightStickYDeadZone);
  std::vector<EngineDef> m_enginedefs;
  std::vector<TrainDef> m_traindefs;
  std::vector<SwitchDef> m_switchdefs;
  std::vector<AccessoryDef> m_accessorydefs;


private:
  std::byte objectIDByte;
  std::byte currentCommandTypeByte;

  ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
  ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
  ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
  ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white


  float curTime;
  bool guitarController = false;
  std::string controllerName;
  std::string dir;

  std::string m_device;
  std::string m_pdi_device;
  float lastQuillTime = 0.0f;
  bool whistleEnabled = false;
  int speed = 0; // current speed
  int engineID = 0;
  int engineID_secondary = 0;
  int engineID_cabpc_programming_menu = 0;
  int engineID_sound_menu = 0;
  int engineID_voice_menu = 0;
  int engineID_diner_voice_menu = 0;
  std::string engineIDStr = "0";
  bool legacyEnabled = true;
  bool consistentBellDing = true;
  bool bellOn = false; 
  int bellDingCount = 2;

  
  int accessoryID = 0;
  int switchID = 0;

  int m_selected_engine = 0;
  int m_selected_engine_secondary_window = 0;
  int m_selected_engine_cabpc_programming_menu = 0;
  int m_selected_engine_sound_menu = 0;
  int m_selected_engine_voice_menu = 0;
  int m_selected_engine_diner_voice_menu = 0;

  int m_selected_switch = 0;
  int m_selected_accessory = 0;

  std::shared_ptr<Image> engineTestIcon;
  std::shared_ptr<Image> blowdownIcon;
  std::shared_ptr<Image> volUpIcon;
  std::shared_ptr<Image> volDownIcon;
  std::shared_ptr<Image> towerComIcon;
  std::shared_ptr<Image> crewTalkIcon;
  std::shared_ptr<Image> smokeOnIcon;
  std::shared_ptr<Image> smokeOffIcon;
  std::shared_ptr<Image> resetIcon;
  std::shared_ptr<Image> tenderMarkerOnIcon;
  std::shared_ptr<Image> tenderMarkerOffIcon;
  std::shared_ptr<Image> waterIcon;
  std::shared_ptr<Image> startUpIcon;
  std::shared_ptr<Image> shutDownIcon;
  std::shared_ptr<Image> extstartUpIcon;
  std::shared_ptr<Image> extshutDownIcon;
  std::shared_ptr<Image> rule17onIcon;
  std::shared_ptr<Image> rule17offIcon;
  std::shared_ptr<Image> blankIcon;
  std::shared_ptr<Image> coalIcon;

  std::shared_ptr<Image> leftArrowIcon;
  std::shared_ptr<Image> rightArrowIcon;

  std::shared_ptr<Image> panto_front_upIcon;
  std::shared_ptr<Image> panto_front_downIcon;
  std::shared_ptr<Image> panto_rear_upIcon;
  std::shared_ptr<Image> panto_rear_downIcon;

  std::shared_ptr<Image> front_couplerIcon;
  std::shared_ptr<Image> rear_couplerIcon;

  std::shared_ptr<Image> fwdIcon;
  std::shared_ptr<Image> reverseIcon;
  std::shared_ptr<Image> aux1arrowIcon;
  std::shared_ptr<Image> aux2arrowIcon;
  std::shared_ptr<Image> aux3arrowIcon;  
  
  std::shared_ptr<Image> swapHornIcon;  
  std::shared_ptr<Image> bellIcon;  

  std::shared_ptr<Image> num1Icon;
  std::shared_ptr<Image> num2Icon;
  std::shared_ptr<Image> num3Icon;
  std::shared_ptr<Image> num4Icon;
  std::shared_ptr<Image> num5Icon;
  std::shared_ptr<Image> num6Icon;
  std::shared_ptr<Image> num7Icon;
  std::shared_ptr<Image> num8Icon;
  std::shared_ptr<Image> num9Icon;
  std::shared_ptr<Image> num0Icon;

  std::shared_ptr<Image> stopSlow0Icon;
  std::shared_ptr<Image> marsonIcon;
  std::shared_ptr<Image> marsoffIcon;
  std::shared_ptr<Image> doglightoffIcon;
  std::shared_ptr<Image> doglightonIcon;
  std::shared_ptr<Image> groundlightonIcon;
  std::shared_ptr<Image> groundlightoffIcon;
  std::shared_ptr<Image> ditchlightoffIcon;
  std::shared_ptr<Image> ditchlightonIcon;
  std::shared_ptr<Image> ditchlightautoIcon;
  std::shared_ptr<Image> locomarkeronIcon;
  std::shared_ptr<Image> locomarkeroffIcon;
  std::shared_ptr<Image> doorcloseIcon;
  std::shared_ptr<Image> dooropenIcon;
  std::shared_ptr<Image> buttonAnnouncementIcon;
  std::shared_ptr<Image> nextStopIcon;
  std::shared_ptr<Image> prevStopIcon;
  std::shared_ptr<Image> lastStopIcon;
  std::shared_ptr<Image> conductorAnnouncementIcon;
  std::shared_ptr<Image> railClatterOnIcon;
  std::shared_ptr<Image> railClatterOffIcon;

  std::shared_ptr<Image> rpmUPIcon;
  std::shared_ptr<Image> rpmDownIcon;

  std::shared_ptr<Image> curveGrindOffIcon;
  std::shared_ptr<Image> curveGrindOnIcon;
  std::shared_ptr<Image> lightOnIcon;
  std::shared_ptr<Image> lightOffIcon;

  // Crane icons
  std::shared_ptr<Image> craneBoomIcon;
  std::shared_ptr<Image> craneHookDownIcon;
  std::shared_ptr<Image> craneHookUpIcon;
  std::shared_ptr<Image> craneBigHookIcon;
  std::shared_ptr<Image> craneSmallHookIcon;
  std::shared_ptr<Image> craneAutoRigsIcon;
  std::shared_ptr<Image> frontLightIcon;
  std::shared_ptr<Image> rearLightIcon;

  std::shared_ptr<Image> soundOnIcon;
  std::shared_ptr<Image> soundOffIcon;


  std::shared_ptr<Image> acelaArriveIcon;
  std::shared_ptr<Image> acelaDepartIcon;


  int gameControllerSelectedEngine = 0;
  float lastTime = 0.0f;

  int currentKeypadStyle = 0;
  // 0 - CAB1
  // 1 - TMCC
  // 2 - Legacy Steam
  // 3 - Page 2 Steam
  // 4 - Legacy Diesel
  // 5 - Page 2 Diesel
  // 6 - Legacy Electric
  // 7 - Page 2 Electric
  // 5 - Legacy Freight Sounds
  // 6 - Legacy Station Sounds Diner
  // 7 - TMCC Crane


  bool test = true;

  bool soundMenuVisible = false;
  bool voiceClipMenuVisible = false;
  bool dinerVoiceClipMenuVisible = false;
  bool addEngineMenuVisible = false;
  bool addSwitchMenuVisible = false;
  bool addAccessoryMenuVisible = false;
  bool secondaryEngineWindow = false;
  bool serialDeviceWindow = false;
  bool cab1Menu = false;
  bool engineAddressUpdateWindowVisible = false;
  bool consistBuilderVisible = false;

  bool switchMenuVisible = false;
  bool accessoryMenuVisible = false;

  bool updateInCABPCDB = false;

  // DEBUG WINDOWS
  bool soundMaskWindowVisible = false;
  bool lightingWindowVisible = false;
  bool serialTermainalVisible = false;
  bool bellDebugWindowVisible = false;
  bool whistleDebugWindowVisible = false;
  bool pdiTestingWindowVisible = false;


  bool surfaceDialEnabled = false;

  bool legacyBellOn = false;
  bool smokeOn = true; // This is a bit tricky, since a locomotive may already have it's smoke unit on, or the switch is set to the off switch already. Will try with auto set to true.
  bool controlCanChange = false;
  bool debugEnabled = false;
  bool headlightCrane = true;
  bool sequenceCtrl = false;
  bool flatWheel = false;
  bool stockCarGame = false;

  int currentDir = 0;
  int currentThrottle = 0;
  int currentTrainBrake = 0;
  int currentBoost = 0;
  int currentLetOff = 0;
  int currentDieselRunLvl = 0;
  int currentEngineLabour = 0;
  int currentLegacyLightState = 0;
  int currentEngineType = 0;
  int currentLegacySmoke = 0;
  int currentLegacyMomentum = 0;
  float currentQuill = 0;
  int currentLocoBrake = 0;
  int currentPantoPosition = 0;

 


  std::string soundDialogMaskStr, soundSignalMaskStr;
  int soundDialogMask, soundSignalMask;


  int m_dialog_index = 0;


  void DrawCAB1Keypad(int m_selected_engine);
  void DrawCAB2SteamKeypad(int m_selected_engine);
  void DrawCAB2DieselKeypad(int m_selected_engine);
  void DrawCAB2ElectricKeypad(int m_selected_engine);
  void DrawStationDinerKeypad(int m_selected_engine);
  void DrawFreightCarKeypad(int m_selected_engine);
  void DrawTMCCSteamKeypad(int m_selected_engine);
  void DrawTMCCDieselKeypad(int m_selected_engine);
  void DrawTMCCCraneKeypad(int m_selected_engine);
  void DrawTMCCAcelaKeypad(int m_selected_engine);
  void DrawKeypadType(int currentKeypadType, bool isLegacy, int engineType,int m_selected_engine);
  void PlayWhistle(bool enabled, float curTime, float currentQuill, int engineID);
  void PlayWhistleTMCC(bool enabled, float curTime, float currentQuill, int engineID, bool horn2Enabled);
  void SerialDeviceWindow(bool* p_open, const std::string& appDir);
  void ShowSecondaryEngineWindow(bool* p_open, const std::string& appDir);
  void UpdateEngineAddressWindow(bool* p_open, const std::string& appDir);
  
  void ConsistBuilderWindow(bool* p_open);
  void AccessoryWindow(bool* p_open);
  void SwitchWindow(bool* p_open);
  void ShowSoundWindow(bool* p_open);
  void ShowVoiceWindow(bool* p_open);
  void DebugBellWindow(bool* p_open);
  void DebugPDIWindow(bool* p_open);
  void SoundMaskWindow(bool* p_open);
  void ShowDinerVoiceWindow(bool* p_open);
  void AddEngineWindow(bool* p_open, const std::string& appDir);
  void AddSwitchWindow(bool* p_open, const std::string& appDir);
  void AddAccessoryWindow(bool* p_open, const std::string& appDir);
  void ThrottleWindow(bool* p_open, float curTime);
  void CAB1Window(bool* p_open, float curTime);
  void HandleGameControllerEvents(SDL_GameController* gGameController, float curTime, const std::string& dir, int leftStickXDeadZone, int leftStickYDeadZone, int rightStickXDeadZone, int rightStickYDeadZone);
  void SetUpEngineFromRoster(int engineID, bool legacyEnabled, const std::string& dir);


  std::string newEngineIDStr = "0";
  int newEngineID = 0;
  std::string newEngineName  = "0";
  int newEngineType = 0;
  bool newEngineisLegacy = false;

  std::string newEngineTMCCAddressStr = "0";
  int newEngineTMCCAddress = 0;

  std::string newSwitchName = "0";
  std::string newSwitchIDStr = "0";
  int newSwitchID = 0;

  std::string newAccessoryName = "0";
  std::string newAccessoryIDStr = "0";
  int newAccessoryID = 0;

  int type_index = 0;



  void ThrottleMenu::Tooltip(const char* text)
  {
    if (ImGui::IsItemHovered())
      ImGui::SetTooltip(text);
  }

  void ThrottleMenu::PlayDialogCommandAfterSeconds(float curTime, float delay, EngineHandle id , DialogCommandParams x)
  {
    float delayTime = curTime + delay;
    if((float)clock() / CLOCKS_PER_SEC > delayTime)
    {
      // do something
    }
  }
  
};