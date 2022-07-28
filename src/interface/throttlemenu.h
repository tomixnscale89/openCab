#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <stdio.h>
#include <TMCCInterface.h>
#include "../json.hpp"
#include "../imgui-knobs.h"



class ThrottleMenu
{
public:
  ThrottleMenu();
  ~ThrottleMenu();
  void Draw();

private:
  std::string m_device;

  int speed = 0; // current speed
  int engineID;
  std::string engineIDStr = "0";
  bool legacyEnabled = true;
  bool consistentBellDing = true;
  int bellDingCount = 2;
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

  void DrawCAB1Keypad();
  void DrawTMCCKeypad();
  void DrawCAB2SteamKeypad();
  void DrawKeypadType(int currentKeypadType);

  ;
};