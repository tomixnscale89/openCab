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
  ENGINE_TYPE_CRANE_CAR = 0x6
};

enum EngineTypeTMCC : uint8
{
  ENGINE_TYPE_TMCC_STEAM = 0x0,
  ENGINE_TYPE_TMCC_DIESEL = 0x1,
  ENGINE_TYPE_TMCC_ELECTRIC = 0x2,
  ENGINE_TYPE_TMCC_CAR = 0x3,
  ENGINE_TYPE_TMCC_CRANE = 0x4,
  ENGINE_TYPE_TMCC_ACELA = 0x5,
  ENGINE_TYPE_TMCC_BREAKDOWN_UNIT = 0x6
};



struct EngineDef
 {
    int engineID;
    int engineType;
    bool legacyEngine;
    std::string engineName;
    // add temp engine speed
    int legacy_speed = 0;
    int tmcc_speed = 0;
    int steam_labour_intensity = 0;
    int diesel_electric_rev_lvl = 0;
    int smoke_state_legacy = 0;
    int keyPadPage = 0;
    bool useHorn2 = false;
  };

class ThrottleMenu
{
public:
  ThrottleMenu(const std::string& appDir);
  ~ThrottleMenu();
  void Draw(const std::string& appDir);
  std::vector<EngineDef> m_enginedefs;

private:

  std::string m_device;
  float lastQuillTime = 0.0f;
  bool whistleEnabled = false;
  int speed = 0; // current speed
  int engineID;
  int engineID_sound_menu = 0;
  int engineID_voice_menu = 0;
  std::string engineIDStr = "0";
  bool legacyEnabled = true;
  bool consistentBellDing = true;
  bool bellOn = false; 
  int bellDingCount = 2;


  int m_selected_engine = 0;
  int m_selected_engine_sound_menu = 0;
  int m_selected_engine_voice_menu = 0;

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
  int currentQuill = 0;
  int currentLocoBrake = 0;
  int currentPantoPosition = 0;


  int m_dialog_index = 0;


  void DrawCAB1Keypad();
  void DrawTMCCKeypad();
  void DrawCAB2SteamKeypad();
  void DrawCAB2ElectricKeypad();
  void DrawKeypadType(int currentKeypadType, bool isLegacy, int engineType);
  void PlayWhistle(bool enabled, float curTime, int currentQuill, int engineID);
  void PlayWhistleTMCC(bool enabled, float curTime, int currentQuill, int engineID, bool horn2Enabled);
  void ShowSoundWindow(bool* p_open);
  void ShowVoiceWindow(bool* p_open);
  void ShowDinerVoiceWindow(bool* p_open);
  void AddEngineWindow(bool* p_open, const std::string& appDir);
  void ThrottleWindow(bool* p_open, float curTime);

  char newEngineNameChar[64] = " ";
  std::string newEngineIDStr = "0";
  int newEngineID = 0;
  std::string newEngineName  = "0";
  int newEngineType = 0;
  bool newEngineisLegacy = false;

  int type_index = 0;



  void ThrottleMenu::Tooltip(const char* text)
  {
    if (ImGui::IsItemHovered())
      ImGui::SetTooltip(text);
  }
  
};