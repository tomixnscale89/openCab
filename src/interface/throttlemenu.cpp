#include "throttlemenu.h"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <stdio.h>
#include "TMCCInterface.h"
#include "PDIInterface.h"
#include "../json.hpp"
#include "../imgui-knobs.h"
#include "../EngineManagement.h"
#include <fstream>
#include "KeypadTypes.h"
//#include "../surface//DeviceListener.h"

using json = nlohmann::json;

struct engine_type_legacy
{
  std::string engineTypeName;
  EngineTypeLegacy engineType; // engine type
};

struct engine_type_tmcc
{
  std::string engineTypeName;
  EngineTypeTMCC engineType; // engine type
};

struct dialog_entry
{
  std::string name;
  DialogCommandParams value; // use the enum type or whatever here
};


static engine_type_legacy engineTypesLegacy[]{
    {"Steam Locomotive", ENGINE_TYPE_STEAM},
    {"Diesel Locomotive", ENGINE_TYPE_DIESEL},
    {"Electric Locomotive", ENGINE_TYPE_ELECTRIC},
    {"Subway", ENGINE_TYPE_SUBWAY},
    {"Passenger Car", ENGINE_TYPE_STATIONSOUND_CAR},
    {"Freight Car", ENGINE_TYPE_STOCK_CAR},
    {"Crane Car", ENGINE_TYPE_CRANE_CAR}
};

static engine_type_tmcc engineTypes[]{
    {"Steam Locomotive", ENGINE_TYPE_TMCC_STEAM},
    {"Diesel Locomotive", ENGINE_TYPE_TMCC_DIESEL},
    {"Electric Locomotive", ENGINE_TYPE_TMCC_ELECTRIC},
    {"TMCC Enabled Car", ENGINE_TYPE_TMCC_CAR},
    {"Crane Car", ENGINE_TYPE_TMCC_CRANE},
    {"Acela", ENGINE_TYPE_TMCC_ACELA},
    {"Breakdown Unit", ENGINE_TYPE_TMCC_BREAKDOWN_UNIT}
};

static dialog_entry dialog_map[]{
  {"Tower Initial Start-up", DC_TOWER_INITIAL_ENGINE_STARTUP},
  {"Tower: Non Emergency Stop", DC_TOWER_NON_EMG_STOP},
  {"Tower: Go to Restricted Speed", DC_TOWER_RESTRICTED_SPEED},
  {"Tower: Go to Slow Speed", DC_TOWER_SLOW_SPEED},
  {"Tower: Go to Limited Speed", DC_TOWER_LIMITED_SPEED},
  {"Tower: Go to Normal Speed", DC_TOWER_NORMAL_SPEED},
  {"Tower: Go to High Ball", DC_TOWER_HIGH_BALL_SPEED},
  {"Tower Context Dependent", DC_TOWER_CONTEXT_DEPENDENT},
  {"Tower: Departure Denied", DC_TOWER_DEPARTURE_DENIED},
  {"Tower: Departure Granted", DC_TOWER_DEPARTURE_GRANTED},
  {"Tower: Have Departed", DC_TOWER_HAVE_DEPARTED},
  {"Tower: All Clear", DC_TOWER_ALL_CLEAR},
  {"Tower: Arriving Soon",DC_TOWER_ARRIVING_SOON},
  {"Tower: Have Arrived",DC_TOWER_HAVE_ARRIVED},
  {"Tower: Shut Down",DC_TOWER_SHUT_DOWN},
  {"Engineer: Departure Granted", DC_ENGINEER_DEPARTURE_DENIED},
  {"Engineer: Departure Denied", DC_ENGINEER_DEPARTURE_GRANTED},
  {"Engineer: Have Departed", DC_ENGINEER_HAVE_DEPARTED},
  {"Engineer: All Clear", DC_ENGINEER_ALL_CLEAR},
  {"Engineer: Arriving Soon", DC_ENGINEER_ARRVING_SOON},
  {"Engineer: Have Arrived", DC_ENGINEER_HAVE_ARRIVE},
  {"Engineer: First Engine Startup", DC_ENGINEER_FIRST_ENGINE_STARTUP},
  {"Engineer: Shut down", DC_ENGINEER_SHUT_DOWN},
  {"Engineer: Identify oneself", DC_ENGINEER_IDENTIFY},
  {"Engineer: Acknowledge ", DC_ENGINEER_ACK_COMM},
  {"Engineer: Acknowledge Speed to Stop", DC_ENGINEER_ACK_SPEED_TO_STOP},
  {"Engineer: Acknowledge to Restricted Speed", DC_ENGINEER_ACK_SPEED_TO_RESTRICTED},
  {"Engineer: Acknowledge to Slow Speed", DC_ENGINEER_ACK_SPEED_TO_SLOW},
  {"Engineer: Acknowledge to Medium Speed", DC_ENGINEER_ACK_SPEED_TO_MEDIUM},
  {"Engineer: Acknowledge to Limited Speed", DC_ENGINEER_ACK_SPEED_TO_LIMITED},
  {"Engineer: Acknowledge to Normal Speed", DC_ENGINEER_ACK_SPEED_TO_NORMAL},
  {"Engineer: Acknowledging Standing by",DC_ENGINEER_ACK_STANDING_BY},
  {"Engineer: Acknowledging Cleared to Go",DC_ENGINEER_ACK_CLEARED_TO_GO},
  {"Engineer: Acknowledging Clear Ahead",DC_ENGINEER_ACK_CLEAR_AHEAD},
  {"Engineer: Acknowledging Clear Inbound",DC_ENGINEER_ACK_CLEAR_INBOUND},
  {"Engineer: Acknowledging Welcome Back",DC_ENGINEER_ACK_WELCOME_BACK},
  {"Engineer: Acknowledging Identify and Out",DC_ENGINEER_ACK_IDENTIFY_OUT},
  {"Engineer: Speak Fuel Level",DC_ENGINEER_SPEAK_FUEL_LVL},
  {"Engineer: Speak Fuel Refilled",DC_ENGINEER_SPEAK_FUEL_REFILL},
  {"Engineer: Speak Speed",DC_ENGINEER_SPEAK_SPEED},
  {"Engineer: Speak Water Level",DC_ENGINEER_SPEAK_WATER_LEVEL},
  {"Engineer: Speak Water Refilled",DC_ENGINEER_SPEAK_WATER_REFILL},
  {"Bluetooth Mode - Start Up Sequence", DC_BLE_STARTUP_SEQUENCE},
  {"Bluetooth Mode - Standby Hold", DC_BLE_DIALOGUE_STANDBY_HOLD},
  {"Bluetooth Mode - Clear Outbound", DC_BLE_DIALOGUE_CLEAR_OUTBOUND},
  {"Bluetooth Mode - Have Departed",DC_BLE_DIALOGUE_HAVE_DEPARTED},
  {"Bluetooth Mode - Clear Ahead",DC_BLE_DIALOGUE_CLEAR_AHEAD},
  {"Bluetooth Mode - Clear Inbound",DC_BLE_DIALOGUE_CLEAR_INBOUND},
  {"Bluetooth Mode - Have Arrived",DC_BLE_DIALOGUE_HAVE_ARRIVED},
  {"Bluetooth Mode - Engineer Acknowledge",DC_BLE_DIALOGUE_ENGINEER_ACK},
  {"Polar Express Conductor: All Aboard", DC_POLAR_EXPRESS_CONDUCTOR_ALL_ABOARD},
  {"Scene 2 Key Context Dependent", DC_SCENE_2_KEY_CONTEXT_DEPENDENT},
  {"Scene 7 Key Context Dependent", DC_SCENE_7_KEY_CONTEXT_DEPENDENT},
  {"Scene 5 Key Context Dependent", DC_SCENE_5_KEY_CONTEXT_DEPENDENT},
  {"Scene 2 Context Dependent",DC_SCENE_2_CONTEXT_DEPENDANT},
  {"Scene Aux 1 - 2 Context Dependent",DC_SCENE_AUX1_2_CONTEXT_DEPENDANT},
  {"Scene 7 Context Dependent",DC_SCENE_7_CONTEXT_DEPENDANT},
  {"Scene Aux 1 - 7 Context Dependent",DC_SCENE_AUX1_7_CONTEXT_DEPENDANT},
  {"Scene 5 Context Dependent",DC_SCENE_5_CONTEXT_DEPENDANT},
  {"Scene Aux 1 - 5 Context Dependent",DC_SCENE_AUX1_5_CONTEXT_DEPENDANT},
  {"Emergency Context Dependent", DC_EMERGENCY_CONTEXT_DEPENDENT},
  {"Conventional Short Horn Trigger", DC_CONVENTIONAL_SHORT_HORN_TRIGGER},
  {"Conventional Mode Shutdown", DC_CONVENTIONAL_MODE_SHUTDOWN},
  {"Sequence Control - Phase 1", DC_SEQ_CONTROL_CLEARED},
  {"Sequence Control - Phase 2", DC_SEQ_CONTROL_HAVE_DEPARTED},
  {"Sequence Control - Phase 3", DC_SEQ_CONTROL_IN_TRANSIT},
  {"Sequence Control - Phase 4", DC_SEQ_CONTROL_MAX_SPEED},
  {"Sequence Control - Phase 5", DC_SEQ_CONTROL_CLEAR_AHEAD},
  {"Sequence Control - Phase 6", DC_SEQ_CONTROL_RANDOM},
  {"Sequence Control - Phase 7", DC_SEQ_CONTROL_ARRIVING_SHORT_TRIP},
  {"Sequence Control - Phase 8", DC_SEQ_CONTROL_ARRIVING_LONG_TRIP},
  {"Sequence Control - Phase 9", DC_SEQ_CONTROL_RESERVED},
  {"Sequence Control - Phase 10", DC_SEQ_CONTROL_RESERVED2},
  {"Sequence Control - Phase 11", DC_SEQ_CONTROL_ARRIVED_SHORT_TRIP},
  {"Sequence Control - Phase 12", DC_SEQ_CONTROL_ARRIVED_LONG_TRIP},
  {"Sequence Control - Phase 13", DC_SEQ_CONTROL_RESERVED3},
  {"Sequence Control - Phase 14", DC_SEQ_CONTROL_RESERVED4}
};

static dialog_entry diner_dialog_map[]{
  {"Startup", DC_STATION_SOUND_CAR_STARTUP},
  {"Shutdown", DC_STATION_SOUND_CAR_SHUTDOWN},
  {"Conductor: Next Stop", DC_STATION_SOUND_CAR_CONDUCTOR_NEXT_STOP},
  {"Conductor: Watch your step", DC_STATION_SOUND_CAR_CONDUCTOR_WATCH_STEP},
  {"Conductor: All Aboard 2", DC_STATION_SOUND_CAR_CONDUCTOR_ALL_ABOARD},
  {"Conductor: Tickets Please", DC_STATION_SOUND_CAR_CONDUCTOR_TICKETS_PLZ},
  {"Conductor: Premature Stop", DC_STATION_SOUND_CAR_CONDUCTOR_PREMATURE_STOP},
  {"Steward: Welcome", DC_STATION_SOUND_CAR_STEWARD_WELCOME},
  {"Steward: First Seating now Open", DC_STATION_SOUND_CAR_STEWARD_FIRST_SEATING},
  {"Steward: Second Seating now Open", DC_STATION_SOUND_CAR_STEWARD_SECOND_SEATING},
  {"Steward: Lounge now Open", DC_STATION_SOUND_CAR_STEWARD_LOUNGE_OPEN},
  {"PA System: Train Arriving", DC_STATION_SOUND_CAR_PA_TRAIN_ARRIVING},
  {"PA System: Train Arrived", DC_STATION_SOUND_CAR_PA_TRAIN_ARRIVED},
  {"PA System: Train Boarding", DC_STATION_SOUND_CAR_PA_TRAIN_BOARDING},
  {"PA System: Train Departing", DC_STATION_SOUND_CAR_PA_TRAIN_DEPARTING},
  {"Sequence Control - Phase 1", DC_STATION_SOUND_CAR_SEQCRTL_PHASE_1},
  {"Sequence Control - Phase 2", DC_STATION_SOUND_CAR_SEQCRTL_PHASE_2},
  {"Sequence Control - Phase 3", DC_STATION_SOUND_CAR_SEQCRTL_PHASE_3},
  {"Sequence Control - Phase 4", DC_STATION_SOUND_CAR_SEQCRTL_PHASE_4},
  {"Sequence Control - Phase 5", DC_STATION_SOUND_CAR_SEQCRTL_PHASE_5},
  {"Sequence Control - Phase 6", DC_STATION_SOUND_CAR_SEQCRTL_PHASE_6},
  {"Sequence Control - Phase 7", DC_STATION_SOUND_CAR_SEQCRTL_PHASE_7},
  {"Sequence Control - Phase 8", DC_STATION_SOUND_CAR_SEQCRTL_PHASE_8},
  {"Sequence Control - Phase 9", DC_STATION_SOUND_CAR_SEQCRTL_PHASE_9},
  {"Sequence Control - Phase 10", DC_STATION_SOUND_CAR_SEQCRTL_PHASE_10},
  {"Sequence Control - Phase 11", DC_STATION_SOUND_CAR_SEQCRTL_PHASE_11},
  {"Sequence Control - Phase 12", DC_STATION_SOUND_CAR_SEQCRTL_PHASE_12},
  {"Sequence Control - Phase 13", DC_STATION_SOUND_CAR_SEQCRTL_PHASE_13},
  {"Sequence Control - Phase 14", DC_STATION_SOUND_CAR_SEQCRTL_PHASE_14}
};


ThrottleMenu::ThrottleMenu(const std::string& appDir, SDL_GameController* gGameController, std::string gGameControllerName)
{
  speed = 0;
  currentTrainBrake = 0;
  currentQuill = 0;
  engineID = 1;

  ImVec4* colors = ImGui::GetStyle().Colors;
  colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
  colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
  colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
  colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
  colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
  colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_FrameBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
  colors[ImGuiCol_FrameBgHovered] = ImVec4(0.71f, 0.71f, 0.71f, 0.40f);
  colors[ImGuiCol_FrameBgActive] = ImVec4(0.51f, 0.51f, 0.51f, 0.67f);
  colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
  colors[ImGuiCol_TitleBgActive] = ImVec4(0.54f, 0.09f, 0.09f, 1.00f);
  colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
  colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
  colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
  colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
  colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 0.45f, 0.45f, 1.00f);
  colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.66f);
  colors[ImGuiCol_SliderGrabActive] = ImVec4(0.67f, 0.67f, 0.67f, 1.00f);
  colors[ImGuiCol_Button] = ImVec4(0.66f, 0.16f, 0.16f, 1.00f);  
  colors[ImGuiCol_ButtonHovered] = ImVec4(0.99f, 0.14f, 0.14f, 1.00f);
  colors[ImGuiCol_ButtonActive] = ImVec4(1.00f, 0.29f, 0.29f, 1.00f);
  colors[ImGuiCol_Header] = ImVec4(1.00f, 0.29f, 0.29f, 0.31f);
  colors[ImGuiCol_HeaderHovered] = ImVec4(1.00f, 0.00f, 0.00f, 0.80f);
  colors[ImGuiCol_HeaderActive] = ImVec4(0.99f, 0.24f, 0.24f, 1.00f);
  colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
  colors[ImGuiCol_SeparatorHovered] = ImVec4(0.75f, 0.00f, 0.00f, 0.78f);
  colors[ImGuiCol_SeparatorActive] = ImVec4(0.83f, 0.22f, 0.22f, 1.00f);
  colors[ImGuiCol_ResizeGrip] = ImVec4(0.98f, 0.26f, 0.26f, 0.20f);
  colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.98f, 0.26f, 0.26f, 0.67f);
  colors[ImGuiCol_ResizeGripActive] = ImVec4(0.98f, 0.26f, 0.26f, 0.95f);
  colors[ImGuiCol_Tab] = ImVec4(0.58f, 0.18f, 0.18f, 0.86f);
  colors[ImGuiCol_TabHovered] = ImVec4(0.98f, 0.26f, 0.26f, 0.80f);
  colors[ImGuiCol_TabActive] = ImVec4(0.68f, 0.20f, 0.20f, 1.00f);
  colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.07f, 0.07f, 0.97f);
  colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.42f, 0.14f, 0.14f, 1.00f);
  colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
  colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
  colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
  colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
  colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
  colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
  colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
  colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
  colors[ImGuiCol_TextSelectedBg] = ImVec4(0.98f, 0.26f, 0.26f, 0.35f);
  colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
  colors[ImGuiCol_NavHighlight] = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
  colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
  colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
  colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);



  //ImGui::GetBackgroundDrawList()->AddImage();

  dir = appDir.substr(0, appDir.find_last_of("/\\"));

  //engineTestIcon = std::make_shared<Image>(dir + "/engine_picture/Sleepy Hollow Casket Co. #31.png");

  blowdownIcon = std::make_shared<Image>(dir + "/graphics/blowdown.png");
  volUpIcon = std::make_shared<Image>(dir + "/graphics/volup.png");
  volDownIcon = std::make_shared<Image>(dir + "/graphics/voldown.png");  
  rpmUPIcon = std::make_shared<Image>(dir + "/graphics/rpm_up.png");
  rpmDownIcon = std::make_shared<Image>(dir + "/graphics/rpm_down.png");
  towerComIcon = std::make_shared<Image>(dir + "/graphics/towercom.png");
  crewTalkIcon = std::make_shared<Image>(dir + "/graphics/crewtalk.png");
  smokeOnIcon = std::make_shared<Image>(dir + "/graphics/smokeon.png");
  smokeOffIcon = std::make_shared<Image>(dir + "/graphics/smokeoff.png");
  resetIcon = std::make_shared<Image>(dir + "/graphics/reset.png");
  tenderMarkerOnIcon = std::make_shared<Image>(dir + "/graphics/marker_on.png");
  tenderMarkerOffIcon = std::make_shared<Image>(dir + "/graphics/marker_off.png");
  waterIcon = std::make_shared<Image>(dir + "/graphics/water.png");
  startUpIcon = std::make_shared<Image>(dir + "/graphics/startup.png");
  shutDownIcon = std::make_shared<Image>(dir + "/graphics/shutdown.png");  
  extstartUpIcon = std::make_shared<Image>(dir + "/graphics/extended_startup.png");
  extshutDownIcon = std::make_shared<Image>(dir + "/graphics/extended_shutdown.png");
  rule17onIcon = std::make_shared<Image>(dir + "/graphics/rule17on.png");
  rule17offIcon = std::make_shared<Image>(dir + "/graphics/rule17off.png");
  blankIcon = std::make_shared<Image>(dir + "/graphics/blank.png");
  coalIcon = std::make_shared<Image>(dir + "/graphics/coal_refill.png");

  leftArrowIcon = std::make_shared<Image>(dir + "/graphics/left_arrow.png");
  rightArrowIcon = std::make_shared<Image>(dir + "/graphics/right_arrow.png");
  panto_front_upIcon = std::make_shared<Image>(dir + "/graphics/panto_front_up.png");
  panto_front_downIcon = std::make_shared<Image>(dir + "/graphics/panto_front_down.png");
  panto_rear_upIcon = std::make_shared<Image>(dir + "/graphics/panto_rear_up.png");
  panto_rear_downIcon = std::make_shared<Image>(dir + "/graphics/panto_rear_down.png");
  
  front_couplerIcon = std::make_shared<Image>(dir + "/graphics/front_coupler.png");
  rear_couplerIcon = std::make_shared<Image>(dir + "/graphics/rear_coupler.png");

  fwdIcon = std::make_shared<Image>(dir + "/graphics/forward_direction.png");
  reverseIcon = std::make_shared<Image>(dir + "/graphics/reverse_direction.png");
  
  bellIcon = std::make_shared<Image>(dir + "/graphics/bell.png");
  bellWideIcon = std::make_shared<Image>(dir + "/graphics/bell_wide.png");
  bellWideOneShot_1Icon = std::make_shared<Image>(dir + "/graphics/bell_wide_oneshot_1.png");
  bellWideOneShot_2Icon = std::make_shared<Image>(dir + "/graphics/bell_wide_oneshot_2.png");
  bellWideOneShot_3Icon = std::make_shared<Image>(dir + "/graphics/bell_wide_oneshot_3.png");

  aux1arrowIcon = std::make_shared<Image>(dir + "/graphics/aux1_arrow.png");
  aux2arrowIcon = std::make_shared<Image>(dir + "/graphics/aux2_arrow.png");
  aux3arrowIcon = std::make_shared<Image>(dir + "/graphics/aux3.png");
  swapHornIcon = std::make_shared<Image>(dir + "/graphics/swap_horn.png");
  
  num1Icon = std::make_shared<Image>(dir + "/graphics/1.png");
  num2Icon = std::make_shared<Image>(dir + "/graphics/2.png");
  num3Icon = std::make_shared<Image>(dir + "/graphics/3.png");
  num4Icon = std::make_shared<Image>(dir + "/graphics/4.png");
  num5Icon = std::make_shared<Image>(dir + "/graphics/5.png");
  num6Icon = std::make_shared<Image>(dir + "/graphics/6.png");
  num7Icon = std::make_shared<Image>(dir + "/graphics/7.png");
  num8Icon = std::make_shared<Image>(dir + "/graphics/8.png");
  num9Icon = std::make_shared<Image>(dir + "/graphics/9.png");
  num0Icon = std::make_shared<Image>(dir + "/graphics/0.png");

  minusIcon = std::make_shared<Image>(dir + "/graphics/minus_button.png");
  plusIcon = std::make_shared<Image>(dir + "/graphics/plus_button.png");
  minusThrottleIcon = std::make_shared<Image>(dir + "/graphics/mph_-.png");
  plusThrottleIcon = std::make_shared<Image>(dir + "/graphics/mph_+.png");
  stopThrottleIcon = std::make_shared<Image>(dir + "/graphics/stop_throttle.png");



  stopSlow0Icon = std::make_shared<Image>(dir + "/graphics/stopslowspeed.png");
  marsonIcon = std::make_shared<Image>(dir + "/graphics/mars_on.png");
  marsoffIcon = std::make_shared<Image>(dir + "/graphics/mars_off.png");
  doglightoffIcon = std::make_shared<Image>(dir + "/graphics/doghouse_off.png");
  doglightonIcon = std::make_shared<Image>(dir + "/graphics/doghouse_on.png");
  groundlightonIcon = std::make_shared<Image>(dir + "/graphics/groundlights_on.png");
  groundlightoffIcon = std::make_shared<Image>(dir + "/graphics/groundlights_off.png");
  groundlightAutoIcon = std::make_shared<Image>(dir + "/graphics/groundlights_auto.png");
  ditchlightoffIcon = std::make_shared<Image>(dir + "/graphics/ditch_off.png");
  ditchlightonIcon = std::make_shared<Image>(dir + "/graphics/ditch_on.png");
  ditchlightautoIcon = std::make_shared<Image>(dir + "/graphics/ditch_on_auto.png");
  ditchlightHornIcon = std::make_shared<Image>(dir + "/graphics/ditch_on_horn.png");
  locomarkeronIcon = std::make_shared<Image>(dir + "/graphics/loco_markers_on.png");
  locomarkeroffIcon = std::make_shared<Image>(dir + "/graphics/loco_markers_off.png");
  strobeOnIcon = std::make_shared<Image>(dir + "/graphics/strobe_on.png");
  strobeOffIcon = std::make_shared<Image>(dir + "/graphics/strobe_off.png");  
  hazardOnIcon = std::make_shared<Image>(dir + "/graphics/hazard_on.png");
  hazardOffIcon = std::make_shared<Image>(dir + "/graphics/hazard_off.png");
  doorcloseIcon = std::make_shared<Image>(dir + "/graphics/door_close.png");
  dooropenIcon = std::make_shared<Image>(dir + "/graphics/door_open.png");
  buttonAnnouncementIcon = std::make_shared<Image>(dir + "/graphics/button_announcement.png");
  nextStopIcon = std::make_shared<Image>(dir + "/graphics/next_stop.png");
  prevStopIcon = std::make_shared<Image>(dir + "/graphics/prev_stop.png");
  lastStopIcon = std::make_shared<Image>(dir + "/graphics/last_stop.png");
  conductorAnnouncementIcon = std::make_shared<Image>(dir + "/graphics/conductor_announcement.png");
  railClatterOnIcon = std::make_shared<Image>(dir + "/graphics/rail_clatter_on.png");
  railClatterOffIcon = std::make_shared<Image>(dir + "/graphics/rail_clatter_off.png");
  curveGrindOffIcon = std::make_shared<Image>(dir + "/graphics/curve_grind_off.png");
  curveGrindOnIcon = std::make_shared<Image>(dir + "/graphics/curve_grind_on.png");
  lightOnIcon = std::make_shared<Image>(dir + "/graphics/light_on.png");
  lightOffIcon = std::make_shared<Image>(dir + "/graphics/light_off.png");

  craneBoomIcon = std::make_shared<Image>(dir + "/graphics/boom.png");
  craneHookDownIcon = std::make_shared<Image>(dir + "/graphics/lower_hook.png");
  craneHookUpIcon = std::make_shared<Image>(dir + "/graphics/raise_hook.png");
  craneBigHookIcon = std::make_shared<Image>(dir + "/graphics/large_hook.png");
  craneSmallHookIcon = std::make_shared<Image>(dir + "/graphics/small_hook.png");
  craneAutoRigsIcon = std::make_shared<Image>(dir + "/graphics/autoriggers.png");

  frontLightIcon = std::make_shared<Image>(dir + "/graphics/front_lights.png");
  rearLightIcon = std::make_shared<Image>(dir + "/graphics/rear_lights.png");
  soundOnIcon = std::make_shared<Image>(dir + "/graphics/sound_on.png");
  soundOffIcon = std::make_shared<Image>(dir + "/graphics/sound_off.png");

  stallVoltageIcon = std::make_shared<Image>(dir + "/graphics/stall_voltage.png");
  lowMomentumIcon = std::make_shared<Image>(dir + "/graphics/low_momentum.png");
  mediumMomentumIcon = std::make_shared<Image>(dir + "/graphics/med_momentum.png");
  highMomentumIcon = std::make_shared<Image>(dir + "/graphics/hi_momentum.png");
  letOffIcon = std::make_shared<Image>(dir + "/graphics/let_off.png");
  setIcon = std::make_shared<Image>(dir + "/graphics/set.png");
  boostIcon = std::make_shared<Image>(dir + "/graphics/boost.png");
  brakeIcon = std::make_shared<Image>(dir + "/graphics/brake.png");
  emergencyIcon = std::make_shared<Image>(dir + "/graphics/emergency.png");

  acelaArriveIcon = std::make_shared<Image>(dir + "/graphics/arriving_acela.png");
  acelaDepartIcon = std::make_shared<Image>(dir + "/graphics/departing_acela.png");

  //ImGui::GetBackgroundDrawList()->AddImage((void*)(intptr_t)num2Icon->GetGLHandle(), ImVec2(70, 70), ImVec2(70, 70));

  // Controller stuff

  if (gGameControllerName == "Guitar (Xbox 360 Wireless Receiver for Windows)" && SDL_GameControllerGetType(gGameController) == SDL_CONTROLLER_TYPE_XBOXONE || SDL_GameControllerGetType(gGameController) == SDL_CONTROLLER_TYPE_XBOX360)
  {
    // we have a guitar controller
    guitarController = true;
  }
}



ThrottleMenu::~ThrottleMenu()
{

}

void ThrottleMenu::LoadRosterLaunch(const std::string& appDir)
{
  if (m_enginedefs.size()) // if size is real, then load the first entry and store it so the icon, keypad, and engine id are loaded properly
  {
    dir = appDir.substr(0, appDir.find_last_of("/\\"));
    engineID = m_enginedefs[m_selected_engine].engineID;
    legacyEnabled = m_enginedefs[m_selected_engine].legacyEngine;
    Image o;
    if (o.Load(dir + "/engine_picture/" + m_enginedefs[m_selected_engine].engineName + ".png"))
    {
      m_enginedefs[m_selected_engine].locoIcon = std::make_shared<Image>(dir + "/engine_picture/" + m_enginedefs[m_selected_engine].engineName + ".png");
      m_enginedefs[m_selected_engine].engineHasCustomIcon = true;
    }
    else
    {
      printf("Using default image\n");

      switch (m_enginedefs[m_selected_engine].engineType)
      {
      case EngineTypeLegacy::ENGINE_TYPE_STEAM:
      case EngineTypeTMCC::ENGINE_TYPE_TMCC_STEAM:
        m_enginedefs[m_selected_engine].locoIcon = std::make_shared<Image>(dir + "/engine_picture/" + "default_" + "steam" + ".png");
        break;

      case EngineTypeLegacy::ENGINE_TYPE_DIESEL:
      case EngineTypeTMCC::ENGINE_TYPE_TMCC_DIESEL:
        m_enginedefs[m_selected_engine].locoIcon = std::make_shared<Image>(dir + "/engine_picture/" + "default_" + "diesel" + ".png");
        break;

      case EngineTypeLegacy::ENGINE_TYPE_ELECTRIC:
      case EngineTypeTMCC::ENGINE_TYPE_TMCC_ELECTRIC:
        m_enginedefs[m_selected_engine].locoIcon = std::make_shared<Image>(dir + "/engine_picture/" + "default_" + "electric" + ".png");
        break;

      case EngineTypeTMCC::ENGINE_TYPE_TMCC_ACELA:
        m_enginedefs[m_selected_engine].locoIcon = std::make_shared<Image>(dir + "/engine_picture/" + "default_" + "acela" + ".png");
        break;

      case EngineTypeLegacy::ENGINE_TYPE_STATIONSOUND_CAR:
        m_enginedefs[m_selected_engine].locoIcon = std::make_shared<Image>(dir + "/engine_picture/" + "default_" + "station_sound_diner" + ".png");
        break;

      case EngineTypeLegacy::ENGINE_TYPE_STOCK_CAR:
        m_enginedefs[m_selected_engine].locoIcon = std::make_shared<Image>(dir + "/engine_picture/" + "default_" + "stock_car" + ".png");
        break;

      case EngineTypeLegacy::ENGINE_TYPE_SUBWAY:
        m_enginedefs[m_selected_engine].locoIcon = std::make_shared<Image>(dir + "/engine_picture/" + "default_" + "subway" + ".png");
        break;

      case EngineTypeLegacy::ENGINE_TYPE_CRANE_CAR:
      case EngineTypeTMCC::ENGINE_TYPE_TMCC_CRANE:
        m_enginedefs[m_selected_engine].locoIcon = std::make_shared<Image>(dir + "/engine_picture/" + "default_" + "default_crane" + ".png");
        break;

      }
      m_enginedefs[m_selected_engine].engineHasCustomIcon = true;
    }
    printf("Engine ID to use: %d\n", engineID);
  }
}

void ThrottleMenu::Draw(const std::string& appDir, SDL_GameController* gGameController, int leftStickXDeadZone, int leftStickYDeadZone, int rightStickXDeadZone, int rightStickYDeadZone)
{

  curTime = (float)clock() / CLOCKS_PER_SEC;

  HandleGameControllerEvents(gGameController, curTime,appDir, leftStickXDeadZone,leftStickYDeadZone,rightStickXDeadZone, rightStickYDeadZone);

  if (soundMenuVisible)              ShowSoundWindow(&soundMenuVisible);
  if (voiceClipMenuVisible)              ShowVoiceWindow(&voiceClipMenuVisible);
  if (dinerVoiceClipMenuVisible)              ShowDinerVoiceWindow(&dinerVoiceClipMenuVisible);
  if (addEngineMenuVisible)              AddEngineWindow(&addEngineMenuVisible, appDir);
  if (addSwitchMenuVisible)              AddSwitchWindow(&addSwitchMenuVisible, appDir);
  if (addAccessoryMenuVisible)              AddAccessoryWindow(&addAccessoryMenuVisible, appDir);
  if (secondaryEngineWindow)              ShowSecondaryEngineWindow(&secondaryEngineWindow, appDir);
  if (cab1Menu)              CAB1Window(&cab1Menu,curTime);
  if (serialDeviceWindow)              SerialDeviceWindow(&serialDeviceWindow, appDir);
  if (engineAddressUpdateWindowVisible)              UpdateEngineAddressWindow(&engineAddressUpdateWindowVisible, appDir);
  if (consistBuilderVisible)              ConsistBuilderWindow(&consistBuilderVisible);

  if (accessoryMenuVisible)              AccessoryWindow(&accessoryMenuVisible);
  if (switchMenuVisible)              SwitchWindow(&switchMenuVisible);
  if (show_demo_window)
    ImGui::ShowDemoWindow(&show_demo_window);

  // Debug Windows
  if (soundMaskWindowVisible)              SoundMaskWindow(&soundMaskWindowVisible);
  if (lightingWindowVisible)              DebugLightingWindow(&lightingWindowVisible);
  if (serialTermainalVisible)              ShowSoundWindow(&serialTermainalVisible);
  if (bellDebugWindowVisible)              DebugBellWindow(&bellDebugWindowVisible);
  if (whistleDebugWindowVisible)              ShowSoundWindow(&whistleDebugWindowVisible);
  if (pdiTestingWindowVisible)                 DebugPDIWindow(&pdiTestingWindowVisible);
  if (surfaceDialEnabled)
  {
  }
  
  // Menu Bar
  if (ImGui::BeginMenuBar())
  {
    if (ImGui::BeginMenu("Menu"))
    {
      ImGui::MenuItem("Select Serial Device", NULL, &serialDeviceWindow);

      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Engine Management"))
    {
      ImGui::MenuItem("Add Engine", NULL, &addEngineMenuVisible);
      ImGui::MenuItem("Add Switch", NULL, &addSwitchMenuVisible);
      ImGui::MenuItem("Add Accessory", NULL, &addAccessoryMenuVisible);
      ImGui::MenuItem("Consist Builder", NULL, &consistBuilderVisible);
      ImGui::MenuItem("Update Engine ID", NULL, &engineAddressUpdateWindowVisible);
      //ImGui::MenuItem("View Engine Roster", NULL, &test);
      //ImGui::MenuItem("Delete Engine", NULL, &test);

      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Switches/Accessories"))
    {
      ImGui::MenuItem("Switch Menu", NULL, &switchMenuVisible);
      ImGui::MenuItem("Accessory Menu", NULL, &accessoryMenuVisible);
      ImGui::MenuItem("Route Menu", NULL, &test);
      ImGui::MenuItem("Group Menu", NULL, &test);
      ImGui::EndMenu();
    }
    //if (ImGui::MenuItem("MenuItem")) {} // You can also use MenuItem() inside a menu bar!
    if (ImGui::BeginMenu("Feature Windows"))
    {
      ImGui::MenuItem("Launch Sound Car Helper", NULL, &secondaryEngineWindow);
      ImGui::MenuItem("Sound Menu", NULL, &soundMenuVisible);
      ImGui::MenuItem("CrewTalk/TowerCom Announcements", NULL, &voiceClipMenuVisible);
      ImGui::MenuItem("StationSounds Diner Announcements", NULL, &dinerVoiceClipMenuVisible);
      ImGui::MenuItem("Lighting Menu", NULL, &lightingWindowVisible);
      //ImGui::MenuItem("Enable Surface Dial", NULL, &surfaceDialEnabled);
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Other"))
    {
      //ImGui::MenuItem("CAB1 Menu", NULL, &cab1Menu);
      if(&m_enginedefs[m_selected_engine])
        ImGui::MenuItem("Enable Brake Sound for Current Locomotive", NULL, &m_enginedefs[m_selected_engine].brakeSoundEnabled);
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Debug"))
    {
      ImGui::MenuItem("Sound Mask Menu", NULL, &soundMaskWindowVisible);
      //ImGui::MenuItem("Lighting Menu", NULL, &lightingWindowVisible);
      ImGui::MenuItem("Bell Menu", NULL, &bellDebugWindowVisible);
      ImGui::MenuItem("Whistle/Horn Menu", NULL, &whistleDebugWindowVisible);      
      ImGui::MenuItem("ImGui Demo Menu", NULL, &show_demo_window);
      //ImGui::MenuItem("Terminal Window", NULL, &serialTermainalVisible);
      //ImGui::MenuItem("PDI Window", NULL, &pdiTestingWindowVisible);
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }

    std::string engineHint = "No Engines Found. Please add some engines.";
    if (m_enginedefs.size())
      engineHint = m_enginedefs[m_selected_engine].engineName;
    // second argument is the text hint for the current item
    if (ImGui::BeginCombo("Engine", engineHint.c_str()))
    {
      // combobox has been clicked, list the options
      for (int i = 0; i < m_enginedefs.size(); i++)
      {
        // first argument is the option name, second argument is whether to display this item as selected
        if (ImGui::Selectable(m_enginedefs[i].engineName.c_str(), m_selected_engine == i))
        {
          // combobox item has been clicked, set the new selected engine
          m_selected_engine = i;
          //SetUpEngineFromRoster(engineID, legacyEnabled, dir);
          engineID = m_enginedefs[m_selected_engine].engineID;
          legacyEnabled = m_enginedefs[m_selected_engine].legacyEngine;
          Image o;
          if(o.Load(dir + "/engine_picture/" + m_enginedefs[m_selected_engine].engineName + ".png"))
          {
            m_enginedefs[m_selected_engine].locoIcon = std::make_shared<Image>(dir + "/engine_picture/" + m_enginedefs[m_selected_engine].engineName + ".png");
            m_enginedefs[m_selected_engine].engineHasCustomIcon = true;
          }
          else
          {
            printf("Using default image\n");

            switch (m_enginedefs[m_selected_engine].engineType)
            {
              case EngineTypeLegacy::ENGINE_TYPE_STEAM:
              case EngineTypeTMCC::ENGINE_TYPE_TMCC_STEAM:
                m_enginedefs[m_selected_engine].locoIcon = std::make_shared<Image>(dir + "/engine_picture/" + "default_" + "steam"+ ".png");
                break;

              case EngineTypeLegacy::ENGINE_TYPE_DIESEL:
              case EngineTypeTMCC::ENGINE_TYPE_TMCC_DIESEL:
                m_enginedefs[m_selected_engine].locoIcon = std::make_shared<Image>(dir + "/engine_picture/" + "default_" + "diesel" + ".png");
                break;

              case EngineTypeLegacy::ENGINE_TYPE_ELECTRIC:
              case EngineTypeTMCC::ENGINE_TYPE_TMCC_ELECTRIC:
                m_enginedefs[m_selected_engine].locoIcon = std::make_shared<Image>(dir + "/engine_picture/" + "default_" + "electric" + ".png");
                break;

              case EngineTypeTMCC::ENGINE_TYPE_TMCC_ACELA:
                m_enginedefs[m_selected_engine].locoIcon = std::make_shared<Image>(dir + "/engine_picture/" + "default_" + "acela" + ".png");
                break;
                
              case EngineTypeLegacy::ENGINE_TYPE_STATIONSOUND_CAR:
                m_enginedefs[m_selected_engine].locoIcon = std::make_shared<Image>(dir + "/engine_picture/" + "default_" + "station_sound_diner" + ".png");
                break;
                
              case EngineTypeLegacy::ENGINE_TYPE_STOCK_CAR:
                m_enginedefs[m_selected_engine].locoIcon = std::make_shared<Image>(dir + "/engine_picture/" + "default_" + "stock_car" + ".png");
                break;

              case EngineTypeLegacy::ENGINE_TYPE_SUBWAY:
                m_enginedefs[m_selected_engine].locoIcon = std::make_shared<Image>(dir + "/engine_picture/" + "default_" + "subway" + ".png");
                break;

              case EngineTypeLegacy::ENGINE_TYPE_CRANE_CAR:
              case EngineTypeTMCC::ENGINE_TYPE_TMCC_CRANE:
                m_enginedefs[m_selected_engine].locoIcon = std::make_shared<Image>(dir + "/engine_picture/" + "default_" + "default_crane" + ".png");
                break;

            }
            m_enginedefs[m_selected_engine].engineHasCustomIcon = true;
          }
          printf("Engine ID to use: %d\n", m_enginedefs[m_selected_engine].engineID);

        }
      }
      ImGui::EndCombo();
    }

    //static int value = 0;
    static float f = 0.0f;
    static int counter = 0;
    

    //ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

    //ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
    //if (ImGui::Checkbox("Demo Window", &show_demo_window))
    //{

    //}      // Edit bools storing our window open/close state
    //ImGui::Checkbox("Another Window", &show_another_window);

    //ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    //ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color


    //ImGui::Text("counter = %d", counter);
    //ImGui::ProgressBar(speed/200, ImVec2(0.0f, 0.0f));

    if (m_enginedefs.size())
    {
      ThrottleWindow(&test, curTime);
    }



    //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);







    ImGui::End();

    SDL_Delay(40);
  };

  void ThrottleMenu::DebugPDIWindow(bool* p_open)
  {
    ImGui::SetNextWindowSize(ImVec2(455, 300), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Debug PDI Window", p_open, ImGuiWindowFlags_MenuBar))
    {
      if (ImGui::BeginCombo("PDI Devices", m_pdi_device.c_str()))
      {
        DeviceInfo* devices;
        
        int numDevices = PDIInterface::EnumerateDevicesPDI(&devices);
        for (int i = 0; i < numDevices; i++)
        {
          DeviceInfo* device = &devices[i];
          std::string deviceName = device->GetFriendlyName();
          bool is_selected = (deviceName == m_pdi_device);
          if (ImGui::Selectable(deviceName.c_str(), is_selected))
          {
            m_pdi_device = deviceName;
            PDIInterface::InitPDI(i);
          }

          // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
          if (is_selected)
            ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
      }

      if(ImGui::Button("Test", ImVec2(76, 76)))
      {
        PDIInterface::EngineBlowHorn1(20);
      }

      ImGui::End();

    }
  };

  void ThrottleMenu::ConsistBuilderWindow(bool* p_open)
  {
    ImGui::SetNextWindowSize(ImVec2(455, 300), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Consist Builder Window", p_open, ImGuiWindowFlags_MenuBar))
    {

      const char* engineLocationitems[] = { "SINGLE", "FRONT", "MIDDLE", "REAR"};
      static int item_current_idx = 0; // Here we store our selection data as an index.
      const char* combo_preview_value = engineLocationitems[item_current_idx];  // Pass in the preview value visible before opening the combo (it could be anything)
      if (ImGui::BeginCombo("Engine Location", combo_preview_value, 0))
      {
        for (int n = 0; n < IM_ARRAYSIZE(engineLocationitems); n++)
        {
          const bool is_selected = (item_current_idx == n);
          if (ImGui::Selectable(engineLocationitems[n], is_selected))
            item_current_idx = n;

          // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
          if (is_selected)
            ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
      }

      if (ImGui::Button("Configure Consist", ImVec2(76, 76)))
      {
        printf("TMCC1 BELL\n");
        TMCCInterface::EngineRingBell(engineID);
      };

      ImGui::End();

    }
  };

  void ThrottleMenu::DebugBellWindow(bool* p_open)
  {
    ImGui::SetNextWindowSize(ImVec2(455, 300), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Debug Bell Window", p_open, ImGuiWindowFlags_MenuBar))
    {
      ImGui::BulletText("Bell Slider Position is for Swinging Bell Locos.");

      if (ImGui::Button("TMCC1 BELL", ImVec2(76, 76)))
      {
        printf("TMCC1 BELL\n");
        TMCCInterface::EngineRingBell(engineID);
      };
      ImGui::SameLine();
      if (ImGui::Button("TMCC2 BELL\nON", ImVec2(76, 76)))
      {
        printf("TMCC2 BELL ON\n");
        TMCCInterface::EngineSetBell(engineID, TMCC_ON);
      };

      ImGui::SameLine();
      if (ImGui::Button("TMCC2 BELL\nOFF", ImVec2(76, 76)))
      {
        printf("TMCC2 BELL OFF\n");
        TMCCInterface::EngineSetBell(engineID, TMCC_OFF);
      };
      ImGui::SameLine();
      if (ImGui::Button("TMCC2 BELL SLIDER\nPOSITION 2", ImVec2(76, 76)))
      {
        printf("TMCC2 BELL SLIDER POSITION 2\n");
        TMCCInterface::EngineSetBellSliderPosition(engineID, 2);
      };
      ImGui::SameLine();
      if (ImGui::Button("TMCC2 BELL SLIDER\nPOSITION 3", ImVec2(76, 76)))
      {
        printf("TMCC2 BELL SLIDER POSITION 3\n");
        TMCCInterface::EngineSetBellSliderPosition(engineID, 3);
      };

      if (ImGui::Button("TMCC2 BELL SLIDER\nPOSITION 4", ImVec2(76, 76)))
      {
        printf("TMCC2 BELL SLIDER POSITION 4\n");
        TMCCInterface::EngineSetBellSliderPosition(engineID, 4);
      };
      ImGui::SameLine();
      if (ImGui::Button("TMCC2 BELL SLIDER\nPOSITION 5", ImVec2(76, 76)))
      {
        printf("TMCC2 BELL SLIDER POSITION 5\n");
        TMCCInterface::EngineSetBellSliderPosition(engineID, 5);
      };

      ImGui::SameLine();
      if (ImGui::Button("TMCC2 BELL SLIDER\nONE SHOT 1", ImVec2(76, 76)))
      {
        printf("TMCC2 BELL SLIDER ONE SHOT 1\n");
        TMCCInterface::EngineBellOneShotDing(engineID, 1);
      };

      ImGui::SameLine();
      if (ImGui::Button("TMCC2 BELL SLIDER\nONE SHOT 2", ImVec2(76, 76)))
      {
        printf("TMCC2 BELL SLIDER ONE SHOT 2\n");
        TMCCInterface::EngineBellOneShotDing(engineID, 2);
      };

      ImGui::SameLine();
      if (ImGui::Button("TMCC2 BELL SLIDER\nONE SHOT 3", ImVec2(76, 76)))
      {
        printf("TMCC2 BELL SLIDER ONE SHOT 3\n");
        TMCCInterface::EngineBellOneShotDing(engineID, 3);
      };

      ImGui::End();

    }
  };
  void ThrottleMenu::DebugLightingWindow(bool* p_open)
  {
    ImGui::SetNextWindowSize(ImVec2(455, 300), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Lighting Menu", p_open, ImGuiWindowFlags_MenuBar))
    {
      ImGui::BulletText("Works with Legacy Locomotives/Cars only.");

      if (ImGui::Button("DOG HOUSE\nOFF", ImVec2(76, 76)))
      {
        printf("Dog House Off\n");
        TMCCInterface::EngineSetDoghouseLight(engineID, TMCC_OFF);
      };
      ImGui::SameLine();
      if (ImGui::Button("DOG HOUSE\nON", ImVec2(76, 76)))
      {
        printf("Dog House On\n");
        TMCCInterface::EngineSetDoghouseLight(engineID, TMCC_ON);
      };

      ImGui::SameLine();
      if (ImGui::Button("HAZARD\nON", ImVec2(76, 76)))
      {
        printf("Hazard Light On\n");
        TMCCInterface::EngineSetHazardLight(engineID, TMCC_ON);
      };
      ImGui::SameLine();
      if (ImGui::Button("HAZARD\nOFF", ImVec2(76, 76)))
      {
        printf("Hazard Light Off\n");
        TMCCInterface::EngineSetHazardLight(engineID, TMCC_OFF);
      };
      ImGui::SameLine();
      if (ImGui::Button("HAZARD\nAUTO", ImVec2(76, 76)))
      {
        printf("Hazard Light Auto\n");
        TMCCInterface::EngineSetHazardLightAuto(engineID);
      };

      // line break 
      if (ImGui::Button("DITCH OFF\nPULSE ON", ImVec2(76, 76)))
      {
        printf("Ditch Light OFF\n");
        TMCCInterface::EngineSetDitchLights(engineID, TMCC_OFF, (TMCCActiveState)LT_DITCH_LIGHTS_OFF_PULSE_ON);
      };
      ImGui::SameLine();
      if (ImGui::Button("DITCH ON\nPULSE OFF", ImVec2(76, 76)))
      {
        printf("Ditch Light OFF\n");
        TMCCInterface::EngineSetDitchLights(engineID, TMCC_ON, (TMCCActiveState)LT_DITCH_LIGHTS_ON_PULSE_OFF);
      };

      ImGui::SameLine();
      if (ImGui::Button("DITCH ON", ImVec2(76, 76)))
      {
        printf("Ditch Light OFF\n");
        TMCCInterface::EngineSetDitchLights(engineID, TMCC_ON, (TMCCActiveState)LT_DITCH_LIGHTS_OFF_PULSE_ON);
      };

      ImGui::SameLine();
      if (ImGui::Button("LOCO\nMARKER OFF", ImVec2(76, 76)))
      {
        printf("Loco Marker OFF\n");
        TMCCInterface::EngineSetLocoMarkerLight(engineID, TMCC_OFF);
      };

      ImGui::SameLine();
      if (ImGui::Button("LOCO\nMARKER ON", ImVec2(76, 76)))
      {
        printf("Loco Marker OFF\n");
        TMCCInterface::EngineSetLocoMarkerLight(engineID, TMCC_ON);
      };

      // line break

      if (ImGui::Button("TENDER\nMARKER OFF", ImVec2(76, 76)))
      {
        printf("Tender Marker OFF\n");
        TMCCInterface::EngineSetTenderMarkerLight(engineID, TMCC_OFF);
      };
      ImGui::SameLine();
      if (ImGui::Button("TENDER\nMARKER ON", ImVec2(76, 76)))
      {
        printf("Tender Marker OFF\n");
        TMCCInterface::EngineSetTenderMarkerLight(engineID, TMCC_ON);
      };

      ImGui::SameLine();
      if (ImGui::Button("GROUND\nLIGHT ON", ImVec2(76, 76)))
      {
        printf("Ground Light On\n");
        TMCCInterface::EngineSetGroundLights(engineID, TMCC_ON);
      };

      ImGui::SameLine();
      if (ImGui::Button("GROUND\nLIGHT OFF", ImVec2(76, 76)))
      {
        printf("Ground Light OFF\n");
        TMCCInterface::EngineSetGroundLights(engineID, TMCC_OFF);
      };

      ImGui::SameLine();
      if (ImGui::Button("GROUND\nLIGHT AUTO", ImVec2(76, 76)))
      {
        printf("Ground Light Auto\n");
        TMCCInterface::EngineSetGroundLightsAuto(engineID);
      };

      // line break

      if (ImGui::Button("STROBE\nOFF", ImVec2(76, 76)))
      {
        printf("Strobe OFF\n");
        TMCCInterface::EngineSetStrobeLight(engineID, 0);
      };
      ImGui::SameLine();
      if (ImGui::Button("STROBE ON\n SINGLE\nFLASH", ImVec2(76, 76)))
      {
        printf("Strobe Single Flash On\n");
        TMCCInterface::EngineSetStrobeLight(engineID, 1);
      };

      ImGui::SameLine();
      if (ImGui::Button("STROBE ON\n DOUBLE\nFLASH", ImVec2(76, 76)))
      {
        printf("Strobe Double Flash On\n");
        TMCCInterface::EngineSetStrobeLight(engineID, 2);
      };

      ImGui::SameLine();
      if (ImGui::Button("MARS LIGHT\nOFF", ImVec2(76, 76)))
      {
        printf("Mars Light OFF\n");
        TMCCInterface::EngineSetMarsLight(engineID, TMCC_OFF);
      };

      ImGui::SameLine();
      if (ImGui::Button("MARS LIGHT\nON", ImVec2(76, 76)))
      {
        printf("Mars Light On\n");
        TMCCInterface::EngineSetMarsLight(engineID, TMCC_ON);
      };

      // line break

      if (ImGui::Button("CAB LIGHT\nOFF", ImVec2(76, 76)))
      {
        printf("Cab Light OFF\n");
        TMCCInterface::EngineSetCabLight(engineID, TMCC_OFF);
      };
      ImGui::SameLine();
      if (ImGui::Button("CAB LIGHT\nON", ImVec2(76, 76)))
      {
        printf("Cab Light On\n");
        TMCCInterface::EngineSetCabLight(engineID, TMCC_ON);
      };

      ImGui::SameLine();
      if (ImGui::Button("CAB LIGHT\nAUTO", ImVec2(76, 76)))
      {
        printf("Cab Light Auto\n");
        TMCCInterface::EngineSetCabLightAuto(engineID);
      };

      ImGui::SameLine();
      if (ImGui::Button("RULE 17\nOFF", ImVec2(76, 76)))
      {
        printf("Rule 17 OFF\n");
        TMCCInterface::EngineSetRule17(engineID, TMCC_OFF);
      };

      ImGui::SameLine();
      if (ImGui::Button("RULE 17\nON", ImVec2(76, 76)))
      {
        printf("Rule 17 On\n");
        TMCCInterface::EngineSetRule17(engineID, TMCC_ON);
      };

      // line break

      if (ImGui::Button("RULE 17\nAUTO", ImVec2(76, 76)))
      {
        printf("Rule 17 Auto\n");
        TMCCInterface::EngineSetRule17Auto(engineID);
      };
      ImGui::SameLine();
      if (ImGui::Button("CAR CABIN\nLIGHT ON", ImVec2(76, 76)))
      {
        printf("Car Cabin Light On\n");
        TMCCInterface::EngineSetCarCabinLight(engineID, TMCC_ON);
      };

      ImGui::SameLine();
      if (ImGui::Button("CAR CABIN\nLIGHT AUTO", ImVec2(76, 76)))
      {
        printf("Car Cabin Light Auto\n");
        TMCCInterface::EngineSetCarCabinLightAuto(engineID);
      };

      ImGui::SameLine();
      if (ImGui::Button("CAR CABIN\nLIGHT OFF", ImVec2(76, 76)))
      {
        printf("Car Cabin Light OFF\n");
        TMCCInterface::EngineSetCarCabinLight(engineID, TMCC_OFF);
      };

      ImGui::End();

    }
  };
  void ThrottleMenu::SoundMaskWindow(bool* p_open)
  {
    ImGui::SetNextWindowSize(ImVec2(455, 300), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Sound Mask Window", p_open, ImGuiWindowFlags_MenuBar))
    {

      if (ImGui::Button("Dialog:Always\nSignal:Always", ImVec2(125, 76)))
      {
        printf("DIALOG_PLAY_ALWAYS, SIGNAL_PLAY_ALWAYS\n");
        TMCCInterface::EngineSoundMaskControl(engineID, DIALOG_PLAY_ALWAYS, SIGNAL_PLAY_ALWAYS);
      };
      ImGui::SameLine();
      if (ImGui::Button("Dialog:Never\nSignal:Never", ImVec2(125, 76)))
      {
        printf("DIALOG_PLAY_NEVER, SIGNAL_PLAY_NEVER\n");
        TMCCInterface::EngineSoundMaskControl(engineID, DIALOG_PLAY_NEVER, SIGNAL_PLAY_NEVER);
      };
      ImGui::SameLine();
      if (ImGui::Button("Dialog:Default\nSignal:Default", ImVec2(125, 76)))
      {
        printf("DIALOG_DEFAULT, SIGNAL_DEFAULT\n");
        TMCCInterface::EngineSoundMaskControl(engineID, DIALOG_DEFAULT, SIGNAL_DEFAULT);
      };

      //
      
      if (ImGui::Button("Dialog:Never\nSignal:Always", ImVec2(125, 76)))
      {
        printf("DIALOG_PLAY_NEVER, SIGNAL_PLAY_ALWAYS\n");
        TMCCInterface::EngineSoundMaskControl(engineID, DIALOG_PLAY_NEVER, SIGNAL_PLAY_ALWAYS);
      };

      ImGui::SameLine();
      if (ImGui::Button("Dialog:Default\nSignal:Always", ImVec2(125, 76)))
      {
        printf("DIALOG_DEFAULT, SIGNAL_PLAY_ALWAYS\n");
        TMCCInterface::EngineSoundMaskControl(engineID, DIALOG_DEFAULT, SIGNAL_PLAY_ALWAYS);
      };

      ImGui::SameLine();
      if (ImGui::Button("Dialog:Default\nSignal:Never", ImVec2(125, 76)))
      {
        printf("DIALOG_DEFAULT, SIGNAL_PLAY_NEVER\n");
        TMCCInterface::EngineSoundMaskControl(engineID, DIALOG_DEFAULT, SIGNAL_PLAY_NEVER);
      };

      //

      if (ImGui::Button("Dialog:Never\nSignal:Default", ImVec2(125, 76)))
      {
        printf("DIALOG_PLAY_NEVER, SIGNAL_PLAY_ALWAYS\n");
        TMCCInterface::EngineSoundMaskControl(engineID, DIALOG_PLAY_NEVER, SIGNAL_DEFAULT);
      };

      ImGui::SameLine();
      if (ImGui::Button("Dialog:Always\nSignal:Default", ImVec2(125, 76)))
      {
        printf("DIALOG_DEFAULT, SIGNAL_PLAY_ALWAYS\n");
        TMCCInterface::EngineSoundMaskControl(engineID, DIALOG_PLAY_ALWAYS, SIGNAL_DEFAULT);
      };

      ImGui::SameLine();
      if (ImGui::Button("Dialog:Always\nSignal:Never", ImVec2(125, 76)))
      {
        printf("DIALOG_PLAY_ALWAYS, SIGNAL_PLAY_NEVER\n");
        TMCCInterface::EngineSoundMaskControl(engineID, DIALOG_PLAY_ALWAYS, SIGNAL_PLAY_NEVER);
      };

      ImGui::BulletText("DIALOG_NO_CHANGE = 0x0");
      ImGui::BulletText("DIALOG_PLAY_ALWAYS = 0x1, // always talk");
      ImGui::BulletText("DIALOG_PLAY_NEVER = 0x2, // never talk");
      ImGui::BulletText("DIALOG_DEFAULT = 0x3,");
      ImGui::BulletText("SIGNAL_NO_CHANGE = 0x0,");
      ImGui::BulletText("SIGNAL_PLAY_ALWAYS = 0x1, // always play horn");
      ImGui::BulletText("SIGNAL_PLAY_NEVER = 0x2, // never play horn");
      ImGui::BulletText("SIGNAL_DEFAULT = 0x3,");

      if (ImGui::InputText("##soundDialogMasking", &soundDialogMaskStr, 1))
      {
        soundDialogMask = std::atoi(soundDialogMaskStr.c_str());
      }
      ImGui::SameLine();
      if (ImGui::InputText("##soundSignalMasking", &soundSignalMaskStr, 1))
      {
        soundSignalMask = std::atoi(soundSignalMaskStr.c_str());
      }
      if ((soundSignalMask < 0 || soundSignalMask >> 3) || (soundDialogMask < 0 || soundDialogMask >> 3))
      {
        ImGui::Text("Please enter a valid Sound or Dialog bit.");

      }
      else
      {
        if (ImGui::Button("Send CMD", ImVec2(80, 60)))
        {
          printf("SoundMaskControl: DIALOG:%d SIGNAL %d\n",soundDialogMask,soundSignalMask);
          TMCCInterface::EngineSoundMaskControl(engineID, (SoundMaskingControl)soundDialogMask, (SoundMaskingControl)soundSignalMask);
        }
      }
        

      ImGui::End();

    }
  };

  void ThrottleMenu::UpdateEngineAddressWindow(bool* p_open, const std::string& appDir)
  {
    ImGui::SetNextWindowSize(ImVec2(455, 300), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Update Engine TMCC Address", p_open, ImGuiWindowFlags_MenuBar))
    {
      
      ImGui::Text("Update Engine in CABPC Database? :");
      ImGui::SameLine();
      ImGui::Checkbox("##UpdateEngineInCABPCDataBase", &updateInCABPCDB);

      if (updateInCABPCDB) // Update a loco in our local CABPC Database
      {
        std::string engineHint = "No Engines Found.";
        if (m_enginedefs.size())
          engineHint = m_enginedefs[m_selected_engine_cabpc_programming_menu].engineName;
        // second argument is the text hint for the current item
        if (ImGui::BeginCombo("Engine", engineHint.c_str()))
        {
          // combobox has been clicked, list the options
          for (int i = 0; i < m_enginedefs.size(); i++)
          {
            // first argument is the option name, second argument is whether to display this item as selected
            if (ImGui::Selectable(m_enginedefs[i].engineName.c_str(), m_selected_engine_cabpc_programming_menu == i))
            {
              // combobox item has been clicked, set the new selected engine
              m_selected_engine_cabpc_programming_menu = i;
              engineID_cabpc_programming_menu = m_enginedefs[m_selected_engine_cabpc_programming_menu].engineID;
              printf("Engine ID to use for TMCC Address Programming: %d\n", engineID_cabpc_programming_menu);
            }
          }
          ImGui::EndCombo();
        }

        ImGui::Text("Set Program Switch to PGM.");
        ImGui::Text("Enter a new TMCC Address from 1-99: ");
        if (ImGui::InputText("##newEngineTMCCAddress", &newEngineTMCCAddressStr, 64))
        {
          //printf("New Engine NAME: %s\n", newEngineName.c_str());
          newEngineTMCCAddress = std::atoi(newEngineTMCCAddressStr.c_str());
        }
        if (newEngineTMCCAddress < 1 || newEngineTMCCAddress > 99)
        {
          ImGui::Text("Please enter a valid TMCC Address from 1-99.");
        }
        else
        {
          if (ImGui::Button("Set Engine Address", ImVec2(80, 60)))
          {
            TMCCInterface::EngineSetEngineAddress(newEngineTMCCAddress);
            printf("Set a Engine TMCC ID of: %d", newEngineTMCCAddress);

            EngineDef engine = EngineDef();
            engine.engineID = newEngineTMCCAddress;
            engine.engineName = m_enginedefs[m_selected_engine_cabpc_programming_menu].engineName;
            engine.engineType = m_enginedefs[m_selected_engine_cabpc_programming_menu].engineType;
            engine.legacyEngine = m_enginedefs[m_selected_engine_cabpc_programming_menu].legacyEngine;
            m_enginedefs.push_back(engine);
            printf("Engine ID: %d, EngineName: %s, engineType: %d, Legacy: %d\n", newEngineID, newEngineName.c_str(), newEngineType, newEngineisLegacy);
            printf("%s\n", appDir.c_str());
            //EngineManagement::AddEngineToJson(test, newEngineID, newEngineType, newEngineisLegacy, appDir.c_str());
            json j;

            EngineManagement::AddEngineDataToJson(j, m_enginedefs, appDir.c_str());
          }
          ImGui::Text("After clicking the button,\nchange the Program Switch back to RUN.");
        }



        



      }
      else
      {
        ImGui::Text("Set Program Switch to PGM.");
        ImGui::Text("Enter a new TMCC Address from 1-99: ");
        if (ImGui::InputText("##newEngineTMCCAddress", &newEngineTMCCAddressStr, 64))
        {
          //printf("New Engine NAME: %s\n", newEngineName.c_str());
          newEngineTMCCAddress = std::atoi(newEngineTMCCAddressStr.c_str());
        }
        if (newEngineTMCCAddress < 1 || newEngineTMCCAddress > 99)
        {
            ImGui::Text("Please enter a valid TMCC Address from 1-99.");
        }
        else
        {
          if (ImGui::Button("Set Engine Address", ImVec2(80, 60)))
          {
            TMCCInterface::EngineSetEngineAddress(newEngineTMCCAddress);
            printf("Set a Engine TMCC ID of: %d", newEngineTMCCAddress);

          }
          ImGui::Text("After clicking the button,\nchange the Program Switch back to RUN.");
        }
      
      }

      ImGui::End();

    }
  };

  void ThrottleMenu::SerialDeviceWindow(bool* p_open, const std::string& appDir)
  {
    ImGui::SetNextWindowSize(ImVec2(455, 300), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Serial Devices", p_open, ImGuiWindowFlags_MenuBar))
    {
      if (ImGui::BeginCombo("Devices", m_device.c_str()))
      {
        DeviceInfo* devices;
        int numDevices = TMCCInterface::EnumerateDevices(&devices);
        for (int i = 0; i < numDevices; i++)
        {
          DeviceInfo* device = &devices[i];
          std::string deviceName = device->GetFriendlyName();
          bool is_selected = (deviceName == m_device);
          if (ImGui::Selectable(deviceName.c_str(), is_selected))
          {
            m_device = deviceName;
            TMCCInterface::Init(i);
          }

          // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
          if (is_selected)
            ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
      }

      ImGui::End();

    }
  };

void ThrottleMenu::AddEngineWindow(bool* p_open, const std::string& appDir)
{
  ImGui::SetNextWindowSize(ImVec2(455, 300), ImGuiCond_FirstUseEver);
  if (ImGui::Begin("Add New Engine", p_open, ImGuiWindowFlags_MenuBar))
  {
    ImGui::Text("Engine Name: ");
    ImGui::SameLine();
    if (ImGui::InputText("##NewEngineName", &newEngineName, 64))
    {
      //printf("New Engine NAME: %s\n", newEngineName.c_str());
    }
    ImGui::Text("Engine ID: ");
    ImGui::SameLine();
    if (ImGui::InputText("##NewEngineID", &newEngineIDStr, 64))
    {
      newEngineID = std::atoi(newEngineIDStr.c_str());
      //printf("New Engine ID: %d\n", (int)newEngineID);
    }
    ImGui::Text("Engine has Legacy? :");
    ImGui::SameLine();
    ImGui::Checkbox("##", &newEngineisLegacy);
    ImGui::Text("Select Engine Type:");
    if (newEngineisLegacy)
    {
      if (ImGui::BeginListBox("##LegacyEngList", ImVec2(256, 128)))
      {
        for (int i = 0; i < IM_ARRAYSIZE(engineTypesLegacy); i++)
        {
          const bool is_selected = (type_index == i);
          if (ImGui::Selectable(engineTypesLegacy[i].engineTypeName.c_str(), is_selected))
            type_index = i;
        }
        ImGui::EndListBox();
      }
    }
    else
    {
      if (ImGui::BeginListBox("##TMCCEngList", ImVec2(256, 128)))
      {
        for (int i = 0; i < IM_ARRAYSIZE(engineTypes); i++)
        {
          const bool is_selected = (type_index == i);
          if (ImGui::Selectable(engineTypes[i].engineTypeName.c_str(), is_selected))
            type_index = i;
        }
        ImGui::EndListBox();
        newEngineType = engineTypes[type_index].engineType;
      }
    }
    ImGui::SameLine();

    if (ImGui::Button("Add Engine", ImVec2(80, 60)))
    {
      if (newEngineisLegacy)
        newEngineType = engineTypesLegacy[type_index].engineType;
      else
        newEngineType = engineTypes[type_index].engineType;

      EngineDef engine = EngineDef();
      engine.engineID = newEngineID;
      engine.engineName = newEngineName.c_str();
      engine.engineType = newEngineType;
      engine.legacyEngine = newEngineisLegacy;
      m_enginedefs.push_back(engine);
      printf("Engine ID: %d, EngineName: %s, engineType: %d, Legacy: %d\n", newEngineID, newEngineName.c_str(), newEngineType, newEngineisLegacy);
      printf("%s\n", appDir.c_str());
      //EngineManagement::AddEngineToJson(test, newEngineID, newEngineType, newEngineisLegacy, appDir.c_str());
      json j;

      EngineManagement::AddEngineDataToJson(j, m_enginedefs, appDir.c_str());
    }


    ImGui::End();

  }
};

void ThrottleMenu::AddSwitchWindow(bool* p_open, const std::string& appDir)
{
  ImGui::SetNextWindowSize(ImVec2(455, 300), ImGuiCond_FirstUseEver);
  if (ImGui::Begin("Add New Switch", p_open, ImGuiWindowFlags_MenuBar))
  {

    ImGui::Text("Switch Name: ");
    ImGui::SameLine();
    if (ImGui::InputText("##NewSwitchName", &newSwitchName, 64))
    {
      //printf("New Engine NAME: %s\n", newEngineName.c_str());
    }
    ImGui::Text("Switch ID: ");
    ImGui::SameLine();
    if (ImGui::InputText("##NewSwitchID", &newSwitchIDStr, 64))
    {
      newSwitchID = std::atoi(newSwitchIDStr.c_str());
      //printf("New Engine ID: %d\n", (int)newEngineID);
    }

    if (ImGui::Button("Add Switch", ImVec2(80, 60)))
    {
      SwitchDef switch_ = SwitchDef();
      switch_.switchID = newSwitchID;
      switch_.switchName = newSwitchName.c_str();
      m_switchdefs.push_back(switch_);
      printf("Switch ID: %d, SwitchName: %s\n", newSwitchID, newSwitchName.c_str());
      printf("%s\n", appDir.c_str());
      //EngineManagement::AddEngineToJson(test, newEngineID, newEngineType, newEngineisLegacy, appDir.c_str());
      json j;

      EngineManagement::AddSwitchDataToJson(j, m_switchdefs, appDir.c_str());
    }


    ImGui::End();

  }
};

void ThrottleMenu::AddAccessoryWindow(bool* p_open, const std::string& appDir)
{
  ImGui::SetNextWindowSize(ImVec2(455, 300), ImGuiCond_FirstUseEver);
  if (ImGui::Begin("Add New Accessory", p_open, ImGuiWindowFlags_MenuBar))
  {

    ImGui::Text("Accessory Name: ");
    ImGui::SameLine();
    if (ImGui::InputText("##NewAccessoryName", &newAccessoryName, 64))
    {
      //printf("New Engine NAME: %s\n", newEngineName.c_str());
    }
    ImGui::Text("Accessory ID: ");
    ImGui::SameLine();
    if (ImGui::InputText("##NewAccessoryID", &newAccessoryIDStr, 64))
    {
      newAccessoryID = std::atoi(newAccessoryIDStr.c_str());
      //printf("New Engine ID: %d\n", (int)newEngineID);
    }

    if (ImGui::Button("Add Accessory", ImVec2(80, 60)))
    {
      AccessoryDef accessory_ = AccessoryDef();
      accessory_.accessoryID = newAccessoryID;
      accessory_.accessoryName = newAccessoryName.c_str();
      m_accessorydefs.push_back(accessory_);
      printf("Accessory ID: %d, AccessoryName: %s\n", newAccessoryID, newAccessoryName.c_str());
      printf("%s\n", appDir.c_str());
      //EngineManagement::AddEngineToJson(test, newEngineID, newEngineType, newEngineisLegacy, appDir.c_str());
      json j;

      EngineManagement::AddAccessoryDataToJson(j, m_accessorydefs, appDir.c_str());
    }


    ImGui::End();

  }
};


void ThrottleMenu::ThrottleWindow(bool* p_open, float curTime)
{
  //const auto& engine = m_enginedefs[m_selected_engine];

  if (ImGui::BeginTable("split", 4))
  {
    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 80.0f);
    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 256.0f);
    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 256.0f);
    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 80.0f);

    ImGui::TableNextColumn();
    
    // brakeslidervalue from 0.0f to 1.0f
    if (ImGui::VSliderFloat("##int", ImVec2(78, 258), &m_enginedefs[m_selected_engine].currentTrainBrake, 1.0f, 0.0f, "Train\nBrake", ImGuiSliderFlags_AlwaysClamp))
    {
        m_enginedefs[m_selected_engine].SetSpeedMultiplier(1.0 - m_enginedefs[m_selected_engine].currentTrainBrake, m_enginedefs[m_selected_engine].legacyEngine, true);
    }

    // Push our invisible button style
    ImGui::PushStyleColor(ImGuiCol_Button, invisible);

    if (ImGui::ImageButton((void*)(intptr_t)fwdIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      printf("Forward\n");
      TMCCInterface::EngineSetDirection(engineID, TMCC_FORWARD);
    };

    Tooltip("Forward Direction");

    if (ImGui::ImageButton((void*)(intptr_t)front_couplerIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      printf("Fire Front coupler\n");
      TMCCInterface::EngineOpenFrontCoupler(engineID);
    };


    //ImGui::Text("Aux 1");
    ImGui::ImageButton((void*)(intptr_t)aux1arrowIcon->GetGLHandle(), ImVec2(70, 70));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("Aux 1: Pressed\n");
      TMCCInterface::EngineAux1Option1(engineID);
    };

    //ImGui::Text("Aux 2");
    ImGui::ImageButton((void*)(intptr_t)aux2arrowIcon->GetGLHandle(), ImVec2(70, 70));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("Aux 2: Pressed\n");
      TMCCInterface::EngineAux2Option1(engineID);
    };

    // Then Pop the Invisible Style Back
    ImGui::PopStyleColor();
    
    

    ImGui::TableNextColumn();
    ImGui::SetNextItemWidth(50);

    if (m_enginedefs[m_selected_engine].engineHasCustomIcon)
    {
      // 250 98
      ImGui::Image((void*)(intptr_t)m_enginedefs[m_selected_engine].locoIcon->GetGLHandle(), ImVec2(250, 98));

    }

    DrawKeypadType(currentKeypadStyle, m_enginedefs[m_selected_engine].legacyEngine, m_enginedefs[m_selected_engine].engineType, m_enginedefs[m_selected_engine].engineID);


        // if we have the stationsound diner or the stock car, don't show the throttle column.
        if ((m_enginedefs[m_selected_engine].engineType != EngineTypeLegacy::ENGINE_TYPE_STOCK_CAR && m_enginedefs[m_selected_engine].engineType != EngineTypeLegacy::ENGINE_TYPE_STATIONSOUND_CAR))
        {
          ImGui::TableNextColumn();
          ImGui::SetNextItemWidth(50);
          //////////////////////////////////////////////////////
        // DIESEL RUN GAUGE AND STEAM LABOUR GAUGE
        // IF STATIONSOUND DINER OR STOCK CAR, DON'T SHOW THESE
        //////////////////////////////////////////////////////
          if (m_enginedefs[m_selected_engine].legacyEngine)
            {
              // value was changed
                  // if steam, use labour
                  // else, use diesel run level
              if (m_enginedefs[m_selected_engine].engineType == EngineTypeLegacy::ENGINE_TYPE_STEAM)
              {
                if (ImGui::VSliderInt("##Labour", ImVec2(113, 160), &m_enginedefs[m_selected_engine].steam_labour_intensity, 0, 31, "%f", ImGuiSliderFlags_AlwaysClamp)) {

                  TMCCInterface::EngineSetLabor(engineID, m_enginedefs[m_selected_engine].steam_labour_intensity);
                };
                Tooltip("Labour Level");
              }
              else
              {
                if (ImGui::VSliderInt("##Diesel Run Lvl.", ImVec2(113, 160), &m_enginedefs[m_selected_engine].diesel_electric_rev_lvl, 0, 8, "%f", ImGuiSliderFlags_AlwaysClamp)) {

                  if (m_enginedefs[m_selected_engine].engineType == EngineTypeLegacy::ENGINE_TYPE_DIESEL)
                    TMCCInterface::EngineSetDieselRunLevel(engineID, m_enginedefs[m_selected_engine].diesel_electric_rev_lvl);
                  else if (m_enginedefs[m_selected_engine].engineType == EngineTypeLegacy::ENGINE_TYPE_ELECTRIC)
                    TMCCInterface::EngineSetDieselRunLevel(engineID, m_enginedefs[m_selected_engine].diesel_electric_rev_lvl);
                  else if (m_enginedefs[m_selected_engine].engineType == EngineTypeLegacy::ENGINE_TYPE_SUBWAY)
                    TMCCInterface::EngineSetDieselRunLevel(engineID, m_enginedefs[m_selected_engine].diesel_electric_rev_lvl);
                };
                Tooltip("Diesel Run Lvl.");
              }

              //////////////////////////////////////////////////////
              // FUEL LEVEL GAUGES
              // WILL REQUIRE PDI COMMANDS FROM SENSOR TRACK
              //////////////////////////////////////////////////////
              ImGui::SameLine();

              if (m_enginedefs[m_selected_engine].engineType == EngineTypeLegacy::ENGINE_TYPE_STEAM)
              {
                if (ImGui::VSliderInt("##Coal Level", ImVec2(62, 160), &m_enginedefs[m_selected_engine].currentCoalLevel, 0, 31, "%f")) {

                  // todo
                };
                Tooltip("Coal Level");

                ImGui::SameLine();
                if (ImGui::VSliderInt("##Water Level", ImVec2(62, 160), &m_enginedefs[m_selected_engine].currentWaterLevel, 0, 31, "%f")) {

                  // todo
                };
                Tooltip("Water Level");
              }
              else
              {
                if (ImGui::VSliderInt("##Diesel Fuel", ImVec2(113, 160), &m_enginedefs[m_selected_engine].currentDieselFuel, 0, 8, "%f")) {
                  // todo
                };
                Tooltip("Diesel Fuel Level");
              }

              // Push our invisible button style
              ImGui::PushStyleColor(ImGuiCol_Button, invisible);

              ImGui::PushButtonRepeat(true);

              // Old Size ImVec2(52,28)
              if (ImGui::ImageButton((void*)(intptr_t)minusIcon->GetGLHandle(), ImVec2(52, 28)))
              {
                if (m_enginedefs[m_selected_engine].engineType == EngineTypeLegacy::ENGINE_TYPE_STEAM)
                {
                  m_enginedefs[m_selected_engine].steam_labour_intensity--;
                  if (m_enginedefs[m_selected_engine].steam_labour_intensity < 0)
                    m_enginedefs[m_selected_engine].steam_labour_intensity = 0;

                  TMCCInterface::EngineSetLabor(engineID, m_enginedefs[m_selected_engine].steam_labour_intensity);
                }

                else
                {
                  m_enginedefs[m_selected_engine].diesel_electric_rev_lvl--;
                  if (m_enginedefs[m_selected_engine].diesel_electric_rev_lvl < 0)
                    m_enginedefs[m_selected_engine].diesel_electric_rev_lvl = 0;

                  if (m_enginedefs[m_selected_engine].engineType == EngineTypeLegacy::ENGINE_TYPE_DIESEL)
                    TMCCInterface::EngineSetDieselRunLevel(engineID, m_enginedefs[m_selected_engine].diesel_electric_rev_lvl);
                  else if (m_enginedefs[m_selected_engine].engineType == EngineTypeLegacy::ENGINE_TYPE_ELECTRIC)
                    TMCCInterface::EngineSetDieselRunLevel(engineID, m_enginedefs[m_selected_engine].diesel_electric_rev_lvl);
                  else if (m_enginedefs[m_selected_engine].engineType == EngineTypeLegacy::ENGINE_TYPE_SUBWAY)
                    TMCCInterface::EngineSetDieselRunLevel(engineID, m_enginedefs[m_selected_engine].diesel_electric_rev_lvl);
                }

              };
              ImGui::PopButtonRepeat();

              ImGui::SameLine();

              ImGui::PushButtonRepeat(true);
              if (ImGui::ImageButton((void*)(intptr_t)plusIcon->GetGLHandle(), ImVec2(52, 28)))
              {
                if (m_enginedefs[m_selected_engine].engineType == EngineTypeLegacy::ENGINE_TYPE_STEAM)
                {
                  m_enginedefs[m_selected_engine].steam_labour_intensity++;
                  if (m_enginedefs[m_selected_engine].steam_labour_intensity > 31)
                    m_enginedefs[m_selected_engine].steam_labour_intensity = 31;

                  TMCCInterface::EngineSetLabor(engineID, m_enginedefs[m_selected_engine].steam_labour_intensity);


                }

                else
                {
                  m_enginedefs[m_selected_engine].diesel_electric_rev_lvl++;
                  if (m_enginedefs[m_selected_engine].diesel_electric_rev_lvl > 8)
                    m_enginedefs[m_selected_engine].diesel_electric_rev_lvl = 8;

                  if (m_enginedefs[m_selected_engine].engineType == EngineTypeLegacy::ENGINE_TYPE_DIESEL)
                    TMCCInterface::EngineSetDieselRunLevel(engineID, m_enginedefs[m_selected_engine].diesel_electric_rev_lvl);
                  else if (m_enginedefs[m_selected_engine].engineType == EngineTypeLegacy::ENGINE_TYPE_ELECTRIC)
                    TMCCInterface::EngineSetDieselRunLevel(engineID, m_enginedefs[m_selected_engine].diesel_electric_rev_lvl);
                  else if (m_enginedefs[m_selected_engine].engineType == EngineTypeLegacy::ENGINE_TYPE_SUBWAY)
                    TMCCInterface::EngineSetDieselRunLevel(engineID, m_enginedefs[m_selected_engine].diesel_electric_rev_lvl);
                }
              };
              ImGui::PopButtonRepeat();

              if (m_enginedefs[m_selected_engine].engineType == EngineTypeLegacy::ENGINE_TYPE_STEAM)
              {
                ImGui::SameLine();
                ImGui::Image((void*)(intptr_t)coalIcon->GetGLHandle(), ImVec2(25, 25), uv_min, uv_max, tint_col, border_col);
                ImGui::SameLine();
                ImGui::Image((void*)(intptr_t)waterIcon->GetGLHandle(), ImVec2(25, 25), uv_min, uv_max, tint_col, border_col);
              }

              else
              {
                ImGui::SameLine();
                ImGui::Image((void*)(intptr_t)waterIcon->GetGLHandle(), ImVec2(25, 25), uv_min, uv_max, tint_col, border_col);
              }

              // Pop back our invisible button style
              ImGui::PopStyleColor();


            }

          float uiSpeed = m_enginedefs[m_selected_engine].legacy_speed * m_enginedefs[m_selected_engine].legacy_speed_multiplier;
          float uiSpeedTMCC = m_enginedefs[m_selected_engine].tmcc_speed * m_enginedefs[m_selected_engine].legacy_speed_multiplier;
          // THROTTLE COLUMN
          if (m_enginedefs[m_selected_engine].legacyEngine)
          {
            // Legacy Version
              if (ImGuiKnobs::Knob("Throttle", &uiSpeed, 0.0, 200.0, 0.3, "%f MPH", ImGuiKnobVariant_Stepped, 256, ImGuiKnobFlags_NoInput | ImGuiKnobFlags_ValueTooltip, 50))
              {
                float realSpeed = uiSpeed / std::max(m_enginedefs[m_selected_engine].legacy_speed_multiplier, 0.01f);
                m_enginedefs[m_selected_engine].legacy_speed = std::min(std::max(realSpeed, 0.0f), 200.0f);
                m_enginedefs[m_selected_engine].UpdateSpeed();
                printf("UI Speed: %f, Real Speed: %f, Brake Multiplier: %f\n", uiSpeed, realSpeed, m_enginedefs[m_selected_engine].legacy_speed_multiplier);
              }
          }

          else
          {
            // TMCC Version
            if (m_enginedefs[m_selected_engine].engineType != EngineTypeTMCC::ENGINE_TYPE_TMCC_CRANE)
            {
              if (ImGuiKnobs::Knob("Throttle", &uiSpeedTMCC, 0.0, 32.0, 0.3, "%f MPH", ImGuiKnobVariant_Stepped, 256, ImGuiKnobFlags_NoInput | ImGuiKnobFlags_ValueTooltip, 32))
              {
                float realSpeed = uiSpeedTMCC / std::max(m_enginedefs[m_selected_engine].legacy_speed_multiplier, 0.01f);
                m_enginedefs[m_selected_engine].tmcc_speed = std::min(std::max(realSpeed, 0.0f), 32.0f);
                m_enginedefs[m_selected_engine].UpdateSpeed();
                printf("UI Speed: %f, Real Speed: %f, Brake Multiplier: %f\n", uiSpeedTMCC, realSpeed, m_enginedefs[m_selected_engine].legacy_speed_multiplier);
              }
            }
            else
            {
              // TMCC Version For Crane
            }
          }
          // Push our invisible button style
          ImGui::PushStyleColor(ImGuiCol_Button, invisible);

          // Old Button ImVec2(78,28)
            ImGui::PushButtonRepeat(true);
            if (ImGui::ImageButton((void*)(intptr_t)minusThrottleIcon->GetGLHandle(), ImVec2(70, 32)))
            {
              printf("Speed -1\n");
              if (m_enginedefs[m_selected_engine].legacyEngine)
              {
                m_enginedefs[m_selected_engine].legacy_speed--;
                if (m_enginedefs[m_selected_engine].legacy_speed < 0)
                  m_enginedefs[m_selected_engine].legacy_speed = 0;

                m_enginedefs[m_selected_engine].UpdateSpeed();

              }

              else
              {
                m_enginedefs[m_selected_engine].tmcc_speed--;
                if (m_enginedefs[m_selected_engine].tmcc_speed < 0)
                  m_enginedefs[m_selected_engine].tmcc_speed = 0;

                m_enginedefs[m_selected_engine].UpdateSpeed();

              }
            };
            ImGui::PopButtonRepeat();

            ImGui::SameLine();
            if (ImGui::ImageButton((void*)(intptr_t)stopThrottleIcon->GetGLHandle(), ImVec2(70, 32)))
            {
              printf("STOP\n");
              if (m_enginedefs[m_selected_engine].legacyEngine)
              {
                m_enginedefs[m_selected_engine].legacy_speed = 0;
                m_enginedefs[m_selected_engine].UpdateSpeed();
              }
              else
              {
                m_enginedefs[m_selected_engine].tmcc_speed = 0;
                m_enginedefs[m_selected_engine].UpdateSpeed();

              }
            };
            ImGui::SameLine();

            ImGui::PushButtonRepeat(true);
            if (ImGui::ImageButton((void*)(intptr_t)plusThrottleIcon->GetGLHandle(), ImVec2(70, 32)))
            {
              printf("Speed +1\n");
              if (m_enginedefs[m_selected_engine].legacyEngine)
              {
                m_enginedefs[m_selected_engine].legacy_speed++;
                if (m_enginedefs[m_selected_engine].legacy_speed > 200)
                  m_enginedefs[m_selected_engine].legacy_speed = 200;
                m_enginedefs[m_selected_engine].UpdateSpeed();

              }

              else
              {
                m_enginedefs[m_selected_engine].tmcc_speed++;
                if (m_enginedefs[m_selected_engine].tmcc_speed > 32)
                  m_enginedefs[m_selected_engine].tmcc_speed = 32;

                m_enginedefs[m_selected_engine].UpdateSpeed();

              }
            };
            ImGui::PopButtonRepeat();
          

            

            // Aux 3 and TMCC Let off
            if (m_enginedefs[m_selected_engine].legacyEngine)
            {
              ImGui::ImageButton((void*)(intptr_t)aux3arrowIcon->GetGLHandle(), ImVec2(70, 70));
              if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
              {
                printf("Aux 3: Pressed\n");
                TMCCInterface::EngineAux3Trigger(engineID);
              };
            }
            else
            {
              ImGui::PushButtonRepeat(true);
              if (ImGui::ImageButton((void*)(intptr_t)letOffIcon->GetGLHandle(), ImVec2(70, 70)))
              {
                printf("TMCC Let off Pressed\n");
                TMCCInterface::EngineLetOffSound(engineID);
              };
              ImGui::PopButtonRepeat();
            }
            ImGui::SameLine();
            // Boost  and brake
            ImGui::PushButtonRepeat(true);
            if (ImGui::ImageButton((void*)(intptr_t)boostIcon->GetGLHandle(), ImVec2(70, 70)))
            {
              printf("Boost\n");
              TMCCInterface::EngineBoostSpeed(engineID);
            };
            ImGui::PopButtonRepeat();
            ImGui::SameLine();
            ImGui::PushButtonRepeat(true);
            if (ImGui::ImageButton((void*)(intptr_t)brakeIcon->GetGLHandle(), ImVec2(70, 70)))
            {
              printf("Brake\n");
              TMCCInterface::EngineBrakeSpeed(engineID);
            };
            ImGui::PopButtonRepeat();

            // Then Pop our style back
            ImGui::PopStyleColor();
        }

          


        else
        {
          // No Throttle column for Stationsound Car or Stock Car
        }

        ImGui::TableNextColumn();
        ImGui::SetNextItemWidth(50);
    // Whistle
        if (m_enginedefs[m_selected_engine].legacyEngine)
        {
          if (ImGui::VSliderFloat("##intLegacyHorn", ImVec2(80, 250), &currentQuill, 1.0f, 0.0f, "Whistle"))
          {
            if (currentQuill > 0)
            {
              whistleEnabled = true;
            }
            else
            {
              whistleEnabled = false;
              TMCCInterface::EngineSetQuillingHornIntensity(engineID, 0);
            }
          };
          PlayWhistle(whistleEnabled, curTime, currentQuill, engineID);
        }
        else
        {
          if (ImGui::VSliderFloat("##intTMCCHorn", ImVec2(80, 250), &currentQuill, 1.0f, 0.0f, "Whistle"))
          {
            if (currentQuill > 0.2f)
            {
              whistleEnabled = true;
            }
            else
            {
              whistleEnabled = false;
            }

          };
          PlayWhistleTMCC(whistleEnabled, curTime, currentQuill, engineID, m_enginedefs[m_selected_engine].useHorn2);

          //ImGui::Text("Horn 2 Disabled:");
          //ImGui::SameLine();
          //ImGui::Checkbox("##TMCC2Horn2Enabler", &m_enginedefs[m_selected_engine].useHorn2);
          //Tooltip("Horn 1 is the normal TMCC horn. Horn 2 is the same sound, but will not enable ditch lights on diesel locos when blown.");

        }
    
        // Push our invisible button style
        ImGui::PushStyleColor(ImGuiCol_Button, invisible);
    
        if (ImGui::ImageButton((void*)(intptr_t)reverseIcon->GetGLHandle(), ImVec2(70, 70)))
        {
          printf("Reverse\n");
          TMCCInterface::EngineSetDirection(engineID, TMCC_REVERSE);
        };

    if (m_enginedefs[m_selected_engine].legacyEngine)
    {
      //if (ImGui::ImageButton((void*)(intptr_t)bellIcon->GetGLHandle(), ImVec2(70, 70)))
      //{
        //if (!m_enginedefs[m_selected_engine].oneShotBellEnabled)
        //{
          //printf("Bell Toggle\n");
          //m_enginedefs[m_selected_engine].bellOn = !m_enginedefs[m_selected_engine].bellOn;
          //if (m_enginedefs[m_selected_engine].bellOn)
            //TMCCInterface::EngineSetBell(engineID, TMCC_ON);
          //else
            //TMCCInterface::EngineSetBell(engineID, TMCC_OFF);
        //}
        //else
        //{
          //printf("One Shot Bell Ding: %d\n", m_enginedefs[m_selected_engine].bellDingCount);
          //TMCCInterface::EngineBellOneShotDing(engineID, m_enginedefs[m_selected_engine].bellDingCount);
        //}
      //}
      ImGui::PushButtonRepeat(true);
      if (ImGui::ImageButton((void*)(intptr_t)bellWideOneShot_3Icon->GetGLHandle(), ImVec2(70, 32)))
      {
        printf("EngineBellOneShotDing: 3\n");
        TMCCInterface::EngineBellOneShotDing(engineID, 3);
      };
      if (ImGui::ImageButton((void*)(intptr_t)bellWideOneShot_2Icon->GetGLHandle(), ImVec2(70, 32)))
      {
        printf("EngineBellOneShotDing: 2\n");
        TMCCInterface::EngineBellOneShotDing(engineID, 2);
      };
      if (ImGui::ImageButton((void*)(intptr_t)bellWideOneShot_1Icon->GetGLHandle(), ImVec2(70, 32)))
      {
        printf("EngineBellOneShotDing: 1\n");
        TMCCInterface::EngineBellOneShotDing(engineID, 1);
      };
      ImGui::PopButtonRepeat();
      if (ImGui::ImageButton((void*)(intptr_t)bellWideIcon->GetGLHandle(), ImVec2(70, 35)))
      {
        printf("EngineToggleBell\n");
        TMCCInterface::EngineRingBell(engineID);
      };

    }
    else
    {

      if (ImGui::ImageButton((void*)(intptr_t)bellIcon->GetGLHandle(), ImVec2(70, 70)))
      {
        if (m_enginedefs[m_selected_engine].legacyEngine)
        {
          printf("TMCC2 Bell Toggle\n");
          m_enginedefs[m_selected_engine].bellOn = !m_enginedefs[m_selected_engine].bellOn;
          if (m_enginedefs[m_selected_engine].bellOn)
            TMCCInterface::EngineSetBell(engineID, TMCC_ON);
          else
            TMCCInterface::EngineSetBell(engineID, TMCC_OFF);
        }
        else
        {
          printf("TMCC1 Bell Toggle\n");
          TMCCInterface::EngineRingBell(engineID);
        }
          
        }
    }
    

    if (ImGui::ImageButton((void*)(intptr_t)rear_couplerIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      printf("Fire Rear coupler\n");
      TMCCInterface::EngineOpenRearCoupler(engineID);
    };

    
    // Then Pop our style back
    ImGui::PopStyleColor();
    

    //ImGui::Image();

    ImGui::EndTable();
  }
};
void ThrottleMenu::ShowSecondaryEngineWindow(bool* p_open, const std::string& appDir)
{
  ImGui::SetNextWindowSize(ImVec2(480, 574), ImGuiCond_FirstUseEver);
  if (ImGui::Begin("Secondary Engine Window", p_open, ImGuiWindowFlags_MenuBar))
  {
    dir = appDir.substr(0, appDir.find_last_of("/\\"));

    std::string engineHint = "No Engines Found.";
    if (m_enginedefs.size())
      engineHint = m_enginedefs[m_selected_engine_secondary_window].engineName;
    // second argument is the text hint for the current item
    if (ImGui::BeginCombo("Secondary Engine", engineHint.c_str()))
    {
      // combobox has been clicked, list the options
      for (int i = 0; i < m_enginedefs.size(); i++)
      {
        // first argument is the option name, second argument is whether to display this item as selected
        if (ImGui::Selectable(m_enginedefs[i].engineName.c_str(), m_selected_engine_secondary_window == i))
        {
          // combobox item has been clicked, set the new selected engine
          m_selected_engine_secondary_window = i;
          //SetUpEngineFromRoster(engineID, legacyEnabled, dir);
          engineID_secondary = m_enginedefs[m_selected_engine_secondary_window].engineID;
          Image p;
          if (p.Load(dir + "/engine_picture/" + m_enginedefs[m_selected_engine_secondary_window].engineName + ".png"))
          {
            m_enginedefs[m_selected_engine_secondary_window].locoIcon = std::make_shared<Image>(dir + "/engine_picture/" + m_enginedefs[m_selected_engine_secondary_window].engineName + ".png");
            m_enginedefs[m_selected_engine_secondary_window].engineHasCustomIcon = true;
          }
          else
          {
            m_enginedefs[m_selected_engine_secondary_window].locoIcon = std::make_shared<Image>(dir + "/engine_picture/" + "default" + ".png");

          }
          printf("Secondary Engine ID to use: %d\n", engineID_secondary);

        }
      }
      ImGui::EndCombo();
    }

    if (m_enginedefs[m_selected_engine_secondary_window].engineHasCustomIcon)
    {
      // 250 98
      ImGui::Image((void*)(intptr_t)m_enginedefs[m_selected_engine_secondary_window].locoIcon->GetGLHandle(), ImVec2(250, 98));

    }
    if (ImGui::BeginTable("split", 2))
    {
      ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 250.0f);
      ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 80.0f);
      ImGui::TableNextColumn();
      DrawKeypadType(currentKeypadStyle, m_enginedefs[m_selected_engine_secondary_window].legacyEngine, m_enginedefs[m_selected_engine_secondary_window].engineType,m_enginedefs[m_selected_engine_secondary_window].engineID);
      ImGui::TableNextColumn();

      if (m_enginedefs[m_selected_engine_secondary_window].legacyEngine)
      {
        if (ImGui::VSliderFloat("##intLegacyHorn2", ImVec2(80, 250), &m_enginedefs[m_selected_engine_secondary_window].currentQuill, 1.0f, 0.0f, "Whistle"))
        {
          if (m_enginedefs[m_selected_engine_secondary_window].currentQuill > 0)
          {
            whistleEnabled = true;
          }
          else
          {
            whistleEnabled = false;
          }
        };
        PlayWhistle(whistleEnabled, curTime, m_enginedefs[m_selected_engine_secondary_window].currentQuill, engineID_secondary);
      }
      else
      {
        if (ImGui::VSliderFloat("##intTMCCHorn2", ImVec2(80, 250), &m_enginedefs[m_selected_engine_secondary_window].currentQuill, 1.0f, 0.0f, "Whistle"))
        {
          if (m_enginedefs[m_selected_engine_secondary_window].currentQuill > 0.2f)
          {
            whistleEnabled = true;
          }
          else
          {
            whistleEnabled = false;
          }

        };
        PlayWhistleTMCC(whistleEnabled, curTime, m_enginedefs[m_selected_engine_secondary_window].currentQuill, engineID_secondary, m_enginedefs[m_selected_engine_secondary_window].useHorn2);

        //ImGui::Text("Horn 2 Disabled:");
        //ImGui::SameLine();
        //ImGui::Checkbox("##TMCC2Horn2Enabler", &m_enginedefs[m_selected_engine].useHorn2);
        //Tooltip("Horn 1 is the normal TMCC horn. Horn 2 is the same sound, but will not enable ditch lights on diesel locos when blown.");

      }

      //ImGui::Text("Aux 1");
      ImGui::ImageButton((void*)(intptr_t)aux1arrowIcon->GetGLHandle(), ImVec2(70, 70));
      if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
      {
        printf("Aux 1: Pressed\n");
        TMCCInterface::EngineAux1Option1(engineID_secondary);
      };

      //ImGui::Text("Aux 2");
      ImGui::ImageButton((void*)(intptr_t)aux2arrowIcon->GetGLHandle(), ImVec2(70, 70));
      if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
      {
        printf("Aux 2: Pressed\n");
        TMCCInterface::EngineAux2Option1(engineID_secondary);
      };

      // Aux 3 and TMCC Let off
      if (m_enginedefs[m_selected_engine_secondary_window].legacyEngine)
      {
        ImGui::ImageButton((void*)(intptr_t)aux3arrowIcon->GetGLHandle(), ImVec2(70, 70));
        if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
        {
          printf("Aux 3: Pressed\n");
          TMCCInterface::EngineAux3Trigger(engineID_secondary);
        };
      }
      else
      {
        ImGui::PushButtonRepeat(true);
        if (ImGui::Button("Let Off", ImVec2(78, 76)))
        {
          printf("TMCC Let off Pressed\n");
          TMCCInterface::EngineLetOffSound(engineID_secondary);
        };
        ImGui::PopButtonRepeat();
      }

      ImGui::EndTable();
    }





    ImGui::End();
  }
}

void ThrottleMenu::SwitchWindow(bool* p_open)
{
  ImGui::SetNextWindowSize(ImVec2(480, 574), ImGuiCond_FirstUseEver);
  if (ImGui::Begin("Switches", p_open, ImGuiWindowFlags_MenuBar))
  {
    if (m_switchdefs.size() > 0) // if size of the accessory def array is bigger than one, show our options
    {
      for (int i = 0; i < m_switchdefs.size(); i++)
      {
        ImGui::Text(m_switchdefs[i].switchName.c_str());
        ImGui::PushID(i);
        ImGui::PushButtonRepeat(true);
        if (ImGui::Button("Throw\nOut", ImVec2(100, 60)))
        {
          printf("%s Throw Out\n", m_switchdefs[i].switchName);

          TMCCInterface::SwitchThrowOut(m_switchdefs[i].switchID);
        };
        ImGui::PopButtonRepeat();

        ImGui::SameLine();

        ImGui::PushButtonRepeat(true);
        if (ImGui::Button("Throw\nThrough", ImVec2(100, 60)))
        {
          printf("%s Throw Through\n", m_switchdefs[i].switchName);

          TMCCInterface::SwitchThrowThrough(m_switchdefs[i].switchID);
        };
        ImGui::PopButtonRepeat();
        ImGui::PopID();
      }
      
     

    }

    else
      ImGui::Text("No Switches Found.");

    ImGui::End();
  }
}

void ThrottleMenu::AccessoryWindow(bool* p_open)
{
  ImGui::SetNextWindowSize(ImVec2(480, 574), ImGuiCond_FirstUseEver);
  if (ImGui::Begin("Accessories", p_open, ImGuiWindowFlags_MenuBar))
  {
    std::string hint = "No Accessories Found.";
    if (m_accessorydefs.size())
    {
      hint = m_accessorydefs[m_selected_accessory].accessoryName;
      accessoryID = m_accessorydefs[m_selected_accessory].accessoryID;
    }
    // second argument is the text hint for the current item
    if (ImGui::BeginCombo("Accessory", hint.c_str()))
    {
      // combobox has been clicked, list the options
      for (int i = 0; i < m_accessorydefs.size(); i++)
      {
        // first argument is the option name, second argument is whether to display this item as selected
        if (ImGui::Selectable(m_accessorydefs[i].accessoryName.c_str(), m_selected_accessory == i))
        {
          // combobox item has been clicked, set the new selected engine
          m_selected_accessory = i;
          accessoryID = m_accessorydefs[m_selected_accessory].accessoryID;
          printf("Accessory ID to use: %d\n", accessoryID);

        }
      }
      ImGui::EndCombo();
    }

    if (m_accessorydefs.size() > 0) // if size of the accessory def array is bigger than one, show our options
    {
      ImGui::PushButtonRepeat(true);
      if (ImGui::Button("Aux 1 Off\n", ImVec2(100, 60)))
      {
        printf("Aux 1 Off\n");

        TMCCInterface::AccessoryAux1Off(accessoryID);
      };
      ImGui::PopButtonRepeat();

      ImGui::SameLine();

      ImGui::PushButtonRepeat(true);
      if (ImGui::Button("Aux 1 On\n", ImVec2(100, 60)))
      {
        printf("Aux 1 On\n");

        TMCCInterface::AccessoryAux1On(accessoryID);
      };
      ImGui::PopButtonRepeat();

      ImGui::SameLine();

      ImGui::PushButtonRepeat(true);
      if (ImGui::Button("Aux 1 Option 1\n", ImVec2(100, 60)))
      {
        printf("Aux 1 Option 1\n");

        TMCCInterface::AccessoryAux1Option1(accessoryID);
      };
      ImGui::PopButtonRepeat();

      ImGui::SameLine();

      ImGui::PushButtonRepeat(true);
      if (ImGui::Button("Aux 1 Option 2\n", ImVec2(100, 60)))
      {
        printf("Aux 1 Option 2\n");

        TMCCInterface::AccessoryAux1Option2(accessoryID);
      };
      ImGui::PopButtonRepeat();

      ImGui::PushButtonRepeat(true);
      if (ImGui::Button("Aux 2 Off\n", ImVec2(100, 60)))
      {
        printf("Aux 2 Off\n");

        TMCCInterface::AccessoryAux2Off(accessoryID);
      };
      ImGui::PopButtonRepeat();

      ImGui::SameLine();

      ImGui::PushButtonRepeat(true);
      if (ImGui::Button("Aux 2 On\n", ImVec2(100, 60)))
      {
        printf("Aux 2 On\n");

        TMCCInterface::AccessoryAux2On(accessoryID);
      };
      ImGui::PopButtonRepeat();

      ImGui::SameLine();

      ImGui::PushButtonRepeat(true);
      if (ImGui::Button("Aux 2 Option 1\n", ImVec2(100, 60)))
      {
        printf("Aux 2 Option 1\n");

        TMCCInterface::AccessoryAux2Option1(accessoryID);
      };
      ImGui::PopButtonRepeat();

      ImGui::SameLine();

      ImGui::PushButtonRepeat(true);
      if (ImGui::Button("Aux 2 Option 2\n", ImVec2(100, 60)))
      {
        printf("Aux 2 Option 2\n");

        TMCCInterface::AccessoryAux2Option2(accessoryID);
      };
      ImGui::PopButtonRepeat();
      
    }
    ImGui::End();
  }
}

void ThrottleMenu::ShowSoundWindow(bool* p_open)
{
  ImGui::SetNextWindowSize(ImVec2(480, 574), ImGuiCond_FirstUseEver);
  if (ImGui::Begin("Sounds and more!", p_open, ImGuiWindowFlags_MenuBar))
  {
    std::string engineHint = "No Engines Found.";
    if (m_enginedefs.size())
      engineHint = m_enginedefs[m_selected_engine_sound_menu].engineName;
    // second argument is the text hint for the current item
    if (ImGui::BeginCombo("Engine", engineHint.c_str()))
    {
      // combobox has been clicked, list the options
      for (int i = 0; i < m_enginedefs.size(); i++)
      {
        // first argument is the option name, second argument is whether to display this item as selected
        if (ImGui::Selectable(m_enginedefs[i].engineName.c_str(), m_selected_engine_sound_menu == i))
        {
          // combobox item has been clicked, set the new selected engine
          m_selected_engine_sound_menu = i;
          engineID_sound_menu = m_enginedefs[m_selected_engine_sound_menu].engineID;
          printf("Engine ID to use for Sound Clips: %d\n", engineID_sound_menu);

        }
      }
      ImGui::EndCombo();
    }

    if (m_enginedefs[m_selected_engine_sound_menu].legacyEngine) // if selected engine is legacy, show legacy menu options
    {
        ImGui::Text("One Shot Bell Enabled:");
        ImGui::SameLine();
        ImGui::Checkbox("##OneShotBellEnabler", &m_enginedefs[m_selected_engine_sound_menu].oneShotBellEnabled);

        if (ImGui::ArrowButton("##left", ImGuiDir_Left)) { m_enginedefs[m_selected_engine_sound_menu].bellDingCount--; }
        if (m_enginedefs[m_selected_engine_sound_menu].bellDingCount < 1)
          m_enginedefs[m_selected_engine_sound_menu].bellDingCount = 3;
        ImGui::SameLine();
        ImGui::Text("One Shot Bell Volume:%d", m_enginedefs[m_selected_engine_sound_menu].bellDingCount);
        ImGui::SameLine();
        if (ImGui::ArrowButton("##right", ImGuiDir_Right)) { m_enginedefs[m_selected_engine_sound_menu].bellDingCount++; }
        if (m_enginedefs[m_selected_engine_sound_menu].bellDingCount > 3)
          m_enginedefs[m_selected_engine_sound_menu].bellDingCount = 1;


        ImGui::PushButtonRepeat(true);
        if (ImGui::Button("Quill Intens:\n1", ImVec2(100, 60)))
        {
          printf("Quill Intensity:\n1\n");

          TMCCInterface::EngineSetQuillingHornIntensity(engineID_sound_menu,1);
        };
        ImGui::PopButtonRepeat();

        ImGui::SameLine();

        ImGui::PushButtonRepeat(true);
        if (ImGui::Button("Quill Intens:\n2", ImVec2(100, 60)))
        {
          printf("Quill Intensity:\n2\n");

          TMCCInterface::EngineSetQuillingHornIntensity(engineID_sound_menu, 2);
        };
        ImGui::PopButtonRepeat();

        ImGui::SameLine();

        ImGui::PushButtonRepeat(true);
        if (ImGui::Button("Quill Intens:\n3", ImVec2(100, 60)))
        {
          printf("Quill Intensity:\n3\n");

          TMCCInterface::EngineSetQuillingHornIntensity(engineID_sound_menu, 3);
        };
        ImGui::PopButtonRepeat();

        ImGui::SameLine();

        ImGui::PushButtonRepeat(true);
        if (ImGui::Button("Quill Intens:\n4", ImVec2(100, 60)))
        {
          printf("Quill Intensity:\n4\n");

          TMCCInterface::EngineSetQuillingHornIntensity(engineID_sound_menu, 4);
        };
        ImGui::PopButtonRepeat();

        ImGui::PushButtonRepeat(true);
        if (ImGui::Button("Quill Intens:\n5", ImVec2(100, 60)))
        {
          printf("Quill Intensity:\n5\n");

          TMCCInterface::EngineSetQuillingHornIntensity(engineID_sound_menu, 5);
        };
        ImGui::PopButtonRepeat();

        ImGui::SameLine();

        ImGui::PushButtonRepeat(true);
        if (ImGui::Button("Quill Intens:\n6", ImVec2(100, 60)))
        {
          printf("Quill Intensity:\n6\n");

          TMCCInterface::EngineSetQuillingHornIntensity(engineID_sound_menu, 6);
        };
        ImGui::PopButtonRepeat();

        ImGui::SameLine();

        ImGui::PushButtonRepeat(true);
        if (ImGui::Button("Quill Intens:\n7", ImVec2(100, 60)))
        {
          printf("Quill Intensity:\n7\n");

          TMCCInterface::EngineSetQuillingHornIntensity(engineID_sound_menu, 7);
        };
        ImGui::PopButtonRepeat();

        ImGui::SameLine();

        ImGui::PushButtonRepeat(true);
        if (ImGui::Button("Quill Intensity:\n8", ImVec2(100, 60)))
        {
          printf("Quill Intensity:\n8\n");

          TMCCInterface::EngineSetQuillingHornIntensity(engineID_sound_menu, 8);
        };
        ImGui::PopButtonRepeat();

      if (ImGui::Button("Cycle Horn", ImVec2(100, 60)))
      {
        printf("Cycle Legacy Horn\n");
        TMCCInterface::EngineAux1Option1(engineID_sound_menu);
        TMCCInterface::EngineSetQuillingHornIntensity(engineID_sound_menu, 1);
      };
      ImGui::SameLine();
      if (ImGui::Button("Cycle Bell", ImVec2(100, 60)))
      {
        printf("Cycle Legacy Bell\n");
        TMCCInterface::EngineAux1Option1(engineID_sound_menu);
        TMCCInterface::EngineBellOneShotDing(engineID_sound_menu, 3);
      };
      ImGui::SameLine();
      ImGui::PushButtonRepeat(true);
      if (ImGui::Button("Blow Horn 1", ImVec2(100, 60)))
      {
        printf("Legacy Horn 1\n");

        TMCCInterface::EngineBlowHorn1TMCC2(engineID_sound_menu);
      };
      ImGui::PopButtonRepeat();

      ImGui::SameLine();

      ImGui::PushButtonRepeat(true);
      if (ImGui::Button("Blow Horn 2", ImVec2(100, 60)))
      {
        printf("Legacy Horn 2 (doesn't seem to work)\n");

        TMCCInterface::EngineBlowHorn2TMCC2(engineID_sound_menu);
      };
      ImGui::PopButtonRepeat();



      ImGui::PushButtonRepeat(true);
      if (ImGui::Button("Engine Refuel", ImVec2(100, 60)))
      {
        printf("Engine refuel\n");

        TMCCInterface::EngineLocoRefuelSound(engineID_sound_menu);
      };
      ImGui::PopButtonRepeat();

      ImGui::SameLine();
      ImGui::PushButtonRepeat(true);
      if (ImGui::Button("Water Injector", ImVec2(100, 60)))
      {
        printf("Water injector\n");

        TMCCInterface::EngineRSTriggerWaterInjector(engineID_sound_menu);
      };
      ImGui::PopButtonRepeat();

      ImGui::SameLine();

      ImGui::PushButtonRepeat(true);
      if (ImGui::Button("Air Horn", ImVec2(100, 60)))
      {
        TMCCInterface::EngineBlowAirHorn(engineID_sound_menu);
      };
      ImGui::PopButtonRepeat();

      ImGui::SameLine();

      ImGui::PushButtonRepeat(true);
      if (ImGui::Button("Brake Squeal", ImVec2(100, 60)))
      {
        printf("Brake squeal sound\n");

        TMCCInterface::EngineBrakeSquealSound(engineID_sound_menu);
      };
      ImGui::PopButtonRepeat();


      ImGui::PushButtonRepeat(true);
      if (ImGui::Button("Auger Sound", ImVec2(100, 60)))
      {
        printf("Auger sound\n");

        TMCCInterface::EngineAugerSound(engineID_sound_menu);
      };
      ImGui::PopButtonRepeat();

      ImGui::SameLine();

      ImGui::PushButtonRepeat(true);
      if (ImGui::Button("Air Release", ImVec2(100, 60)))
      {
        printf("RSTriggerBrakeAirRelease\n");

        TMCCInterface::EngineRSTriggerBrakeAirRelease(engineID_sound_menu);
      };
      ImGui::PopButtonRepeat();

      ImGui::SameLine();

      ImGui::PushButtonRepeat(true);
      if (ImGui::Button("Short Let-Off", ImVec2(100, 60)))
      {
        printf("RSTriggerShortLetOff\n");

        TMCCInterface::EngineRSTriggerShortLetOff(engineID_sound_menu);
      };
      ImGui::PopButtonRepeat();

      ImGui::SameLine();

      ImGui::PushButtonRepeat(true);
      if (ImGui::Button("Long Let-off", ImVec2(100, 60)))
      {
        printf("RSTriggerLongLetOff\n");

        TMCCInterface::EngineRSTriggerLongLetOff(engineID_sound_menu);
      };
      ImGui::PopButtonRepeat();


      ImGui::PushButtonRepeat(true);
      if (ImGui::Button("TMCC Horn 1", ImVec2(100, 60)))
      {
        printf("tmcc horn 1\n");

        TMCCInterface::EngineBlowHorn1(engineID_sound_menu);
      };
      ImGui::PopButtonRepeat();

      ImGui::SameLine();

      ImGui::PushButtonRepeat(true);
      if (ImGui::Button("TMCC Horn 2", ImVec2(100, 60)))
      {
        printf("tmcc horn 2\n");

        TMCCInterface::EngineBlowHorn2(engineID_sound_menu);
      };
      ImGui::PopButtonRepeat();

      ImGui::SameLine();

      ImGui::PushButtonRepeat(true);
      if (ImGui::Button("TMCC Let-off", ImVec2(100, 60)))
      {
        printf("tmcc letoff\n");

        TMCCInterface::EngineLetOffSound(engineID_sound_menu);
      };
      ImGui::PopButtonRepeat();

      ImGui::SameLine();

      ImGui::PushButtonRepeat(true);
      if (ImGui::Button("Legacy Chuff", ImVec2(100, 60)))
      {
        TMCCInterface::SendEngineCommand2(engineID_sound_menu, EC2_CHUFF);
      };
      ImGui::PopButtonRepeat();

      ImGui::PushButtonRepeat(true);
      if (ImGui::Button("Cyl. Cock Off", ImVec2(100, 60)))
      {
        printf("Cyl Cock off\n");

        TMCCInterface::EngineToggleCylinderCock (engineID_sound_menu,TMCC_OFF);
      };
      ImGui::PopButtonRepeat();

      ImGui::SameLine();

      ImGui::PushButtonRepeat(true);
      if (ImGui::Button("Cyl. Cock ON", ImVec2(100, 60)))
      {
        printf("Cyl Cock on\n");
        TMCCInterface::EngineToggleCylinderCock(engineID_sound_menu, TMCC_ON);
      };
      ImGui::PopButtonRepeat();

      ImGui::SameLine();

      ImGui::PushButtonRepeat(true);
      if (ImGui::Button("Standby\nBell On", ImVec2(100, 60)))
      {
        TMCCInterface::EngineLetOffSound(engineID_sound_menu);
      };
      ImGui::PopButtonRepeat();

      

      ImGui::SameLine();

      ImGui::PushButtonRepeat(true);
      if (ImGui::Button("Legacy Secondary Chuff", ImVec2(100, 60)))
      {
        TMCCInterface::SendEngineCommand2(engineID_sound_menu, EC2_SECONDARY_CHUFF);
      };
      ImGui::PopButtonRepeat();
    }
    

    ImGui::End();
  }
}

void ThrottleMenu::ShowVoiceWindow(bool* p_open)
{
  ImGui::SetNextWindowSize(ImVec2(480, 574), ImGuiCond_FirstUseEver);
  if (ImGui::Begin("CrewTalk/TowerCom Voice Testing", p_open, ImGuiWindowFlags_MenuBar))
  {
    std::string engineHint = "No Engines Found.";
    if (m_enginedefs.size())
      engineHint = m_enginedefs[m_selected_engine_voice_menu].engineName;
    // second argument is the text hint for the current item
    if (ImGui::BeginCombo("Engine", engineHint.c_str()))
    {
      // combobox has been clicked, list the options
      for (int i = 0; i < m_enginedefs.size(); i++)
      {
        // first argument is the option name, second argument is whether to display this item as selected
        if (ImGui::Selectable(m_enginedefs[i].engineName.c_str(), m_selected_engine_voice_menu == i))
        {
          // combobox item has been clicked, set the new selected engine
          m_selected_engine_voice_menu = i;
          engineID_voice_menu = m_enginedefs[m_selected_engine_voice_menu].engineID;
          printf("Engine ID to use for Voice Clips: %d\n", engineID_voice_menu);

        }
      }
      ImGui::EndCombo();
    }

    if (ImGui::BeginListBox("##", ImVec2(320, 512)))
    {
      for (int i = 0; i < IM_ARRAYSIZE(dialog_map); i++)
      {
        const bool is_selected = (m_dialog_index == i);
        if (ImGui::Selectable(dialog_map[i].name.c_str(), is_selected))
          m_dialog_index = i;
      }
      ImGui::EndListBox();
    }
    ImGui::SameLine();
    if (ImGui::Button("Play Clip", ImVec2(80, 60)))
    {
      //printf("Voice clip: " + dialog_map[dialog_map[m_dialog_index].value].name.c_str());
      TMCCInterface::EngineDialogCommand(engineID_voice_menu, dialog_map[m_dialog_index].value);
    };

    ImGui::End();
  }
}

void ThrottleMenu::ShowDinerVoiceWindow(bool* p_open)
{
  ImGui::SetNextWindowSize(ImVec2(480, 574), ImGuiCond_FirstUseEver);
  if (ImGui::Begin("StationSounds Diner Voice Clips", p_open, ImGuiWindowFlags_MenuBar))
  {
    std::string engineHint = "No Engines Found.";
    if (m_enginedefs.size())
      engineHint = m_enginedefs[m_selected_engine_diner_voice_menu].engineName;
    // second argument is the text hint for the current item
    if (ImGui::BeginCombo("Engine", engineHint.c_str()))
    {
      // combobox has been clicked, list the options
      for (int i = 0; i < m_enginedefs.size(); i++)
      {
        // first argument is the option name, second argument is whether to display this item as selected
        if (ImGui::Selectable(m_enginedefs[i].engineName.c_str(), m_selected_engine_diner_voice_menu == i))
        {
          // combobox item has been clicked, set the new selected engine
          m_selected_engine_diner_voice_menu = i;
          engineID_diner_voice_menu = m_enginedefs[m_selected_engine_diner_voice_menu].engineID;
          printf("Engine ID to use for Voice Clips: %d\n", engineID_diner_voice_menu);

        }
      }
      ImGui::EndCombo();
    }

    if (ImGui::BeginListBox("##", ImVec2(320, 512)))
    {
      for (int i = 0; i < IM_ARRAYSIZE(diner_dialog_map); i++)
      {
        const bool is_selected = (m_dialog_index == i);
        if (ImGui::Selectable(diner_dialog_map[i].name.c_str(), is_selected))
          m_dialog_index = i;
      }
      ImGui::EndListBox();
    }
    ImGui::SameLine();
    if (ImGui::Button("Play Clip", ImVec2(80, 60)))
    {
      //printf("Voice clip: " + dialog_map[dialog_map[m_dialog_index].value].name.c_str());
      TMCCInterface::EngineDialogCommand(engineID_diner_voice_menu, diner_dialog_map[m_dialog_index].value);
    };

    ImGui::End();
  }
}

void ThrottleMenu::PlayWhistle(bool enabled , float curTime, float currentQuill, int engineID)
{
  float quillInterval = 0.2f;
  if (enabled)
  {
    if (curTime - ThrottleMenu::lastQuillTime >= quillInterval)
    {
      int value = (int)(currentQuill * 15.0f);
      if (value > 0)
      {
        ThrottleMenu::lastQuillTime = curTime;
        printf("Quilling: %d\n", value);
        TMCCInterface::EngineSetQuillingHornIntensity(engineID, value);
      }
      
    }
  }
  
}

void ThrottleMenu::PlayWhistleTMCC(bool enabled, float curTime, float currentQuill, int engineID , bool horn2Enabled)
{
  float quillInterval = 0.2f;
  if (enabled)
  {
    if (curTime - ThrottleMenu::lastQuillTime >= quillInterval)
    {
      ThrottleMenu::lastQuillTime = curTime;
      if (horn2Enabled)
      {
        printf("TMCC Horn 2\n");
        TMCCInterface::EngineBlowHorn2(engineID);

      }
      else
      {
        printf("TMCC Horn 1\n");
        TMCCInterface::EngineBlowHorn1(engineID);

      }

    }
  }

}



void ThrottleMenu::DrawKeypadType(int currentKeypadType, bool isLegacy, int engineType, int m_selected_engine)
{
  if (isLegacy) // Legacy Locos
  {
    switch (engineType)
    {
    case EngineTypeLegacy::ENGINE_TYPE_STEAM:
    default:
      DrawCAB2SteamKeypad(m_selected_engine);
      break;
    
    case EngineTypeLegacy::ENGINE_TYPE_DIESEL:
      DrawCAB2DieselKeypad(m_selected_engine);
      break;

    case EngineTypeLegacy::ENGINE_TYPE_ELECTRIC:
      DrawCAB2ElectricKeypad(m_selected_engine);
      break;
    case EngineTypeLegacy::ENGINE_TYPE_STATIONSOUND_CAR:
      DrawStationDinerKeypad(m_selected_engine);
      break;
    case EngineTypeLegacy::ENGINE_TYPE_STOCK_CAR:
      DrawFreightCarKeypad(m_selected_engine);
      break;
    case EngineTypeLegacy::ENGINE_TYPE_SUBWAY:
      DrawCAB2SteamKeypad(m_selected_engine);
      break;
    }
    
  }

  else // TMCC Locos
  {
    switch (engineType)
    {
    case EngineTypeTMCC::ENGINE_TYPE_TMCC_STEAM:
      DrawTMCCSteamKeypad(m_selected_engine);
      break;
    case EngineTypeTMCC::ENGINE_TYPE_TMCC_DIESEL:
      DrawTMCCDieselKeypad(m_selected_engine);
      break;
    case EngineTypeTMCC::ENGINE_TYPE_TMCC_ELECTRIC:
      DrawTMCCAcelaKeypad(m_selected_engine);
      break;
    case EngineTypeTMCC::ENGINE_TYPE_TMCC_ACELA:
      DrawTMCCAcelaKeypad(m_selected_engine);
      break;
    case EngineTypeTMCC::ENGINE_TYPE_TMCC_CRANE:
      DrawTMCCCraneKeypad(m_selected_engine);
      break;
    default:
      DrawCAB1Keypad(m_selected_engine);
      break;
    }
  }
}

void ThrottleMenu::SetUpEngineFromRoster(int engineID,bool legacyEnabled, const std::string& dir)
{
  engineID = m_enginedefs[m_selected_engine].engineID;
  legacyEnabled = m_enginedefs[m_selected_engine].legacyEngine;
  Image o;
  if (o.Load(dir + "/engine_picture/" + m_enginedefs[m_selected_engine].engineName + ".png"))
  {
    m_enginedefs[m_selected_engine].locoIcon = std::make_shared<Image>(dir + "/engine_picture/" + m_enginedefs[m_selected_engine].engineName + ".png");
    m_enginedefs[m_selected_engine].engineHasCustomIcon = true;
  }
  else
  {
    m_enginedefs[m_selected_engine].locoIcon = std::make_shared<Image>(dir + "/engine_picture/" + "default" + ".png");

  }
  printf("Engine ID to use: %d\n", engineID);
}

void ThrottleMenu::HandleGameControllerEvents(SDL_GameController* gGameController, float curTime, const std::string& dir, int leftStickXDeadZone, int leftStickYDeadZone, int rightStickXDeadZone, int rightStickYDeadZone)
{
  //SDL_Delay(50);
  if (gGameController)
  {
    float interval = 0.1f;
    if (curTime - ThrottleMenu::lastTime >= interval)
    {
      ThrottleMenu::lastTime = curTime;
      if (SDL_GameControllerGetAxis(gGameController, SDL_CONTROLLER_AXIS_RIGHTY))
      {
        // The speed here will also update the UI Throttle. Yay!
        float value = (float)SDL_GameControllerGetAxis(gGameController, SDL_CONTROLLER_AXIS_RIGHTY);
        if (value > rightStickYDeadZone + 10000)
        {
          
          //printf("Down Right Stick Y: %d\n", (int)value);
          m_enginedefs[m_selected_engine].legacy_speed--;
          if (m_enginedefs[m_selected_engine].legacy_speed < 0)
            m_enginedefs[m_selected_engine].legacy_speed = 0;
        
          m_enginedefs[m_selected_engine].UpdateSpeed();
        }
        else if (value < rightStickYDeadZone - 10000)
        {
          //printf("UP Right Stick Y: %d\n", (int)value);
          m_enginedefs[m_selected_engine].legacy_speed++;
          if (m_enginedefs[m_selected_engine].legacy_speed > 200)
            m_enginedefs[m_selected_engine].legacy_speed = 200;
          
          m_enginedefs[m_selected_engine].UpdateSpeed();
        }
      }
      if (SDL_GameControllerGetAxis(gGameController, SDL_CONTROLLER_AXIS_LEFTY) && !guitarController)
      {
        float value = (float)SDL_GameControllerGetAxis(gGameController, SDL_CONTROLLER_AXIS_LEFTY);
        if (value < leftStickYDeadZone - 10000)
        {
          value = (value / 32767);
          //value = floorf(value * 10) / 10;
          //printf("Controller Brake %f\n", value);

          if (value < -0.9)
          {
            if (m_enginedefs[m_selected_engine].legacyEngine)
            {
              // if stick is pulled down enough, decrement the gamePadTrainBrake variable
              // and check if it's less than zero. if so, then set it min to zero.
              // Then re run through the same code the train brake slider uses. 

              m_enginedefs[m_selected_engine].gamepadTrainBrake -= 0.1f;

              if (m_enginedefs[m_selected_engine].gamepadTrainBrake < 0.0f)
                m_enginedefs[m_selected_engine].gamepadTrainBrake = 0.0f;

              int brakevalue = (int)(m_enginedefs[m_selected_engine].gamepadTrainBrake * 8.0f);
              m_enginedefs[m_selected_engine].currentTrainBrake = m_enginedefs[m_selected_engine].gamepadTrainBrake;
              m_enginedefs[m_selected_engine].SetSpeedMultiplier(1.0 - m_enginedefs[m_selected_engine].gamepadTrainBrake, m_enginedefs[m_selected_engine].legacyEngine, true);
              //TMCCInterface::EngineSetTrainBrake2(engineID, brakevalue);
              //if (brakeSoundEnabledAllTypes)
              //  TMCCInterface::EngineBrakeSquealSound(engineID);

              //if (m_enginedefs[m_selected_engine].engineType == EngineTypeLegacy::ENGINE_TYPE_STEAM)
              //{
              //  int labourvalue = (int)(m_enginedefs[m_selected_engine].gamepadTrainBrake * 32.0f);
              //  m_enginedefs[m_selected_engine].steam_labour_intensity = labourvalue;
              //  TMCCInterface::EngineSetLabor(engineID, labourvalue);
              //}
              //else
              //{
              //  int dieselRun = (int)(m_enginedefs[m_selected_engine].gamepadTrainBrake * 8.0f);
              //  m_enginedefs[m_selected_engine].diesel_electric_rev_lvl = dieselRun;
              //  TMCCInterface::EngineSetDieselRunLevel(engineID, dieselRun);
              //}
            }
          }
        }
        else if (value > leftStickYDeadZone + 10000)
        {
          //printf("Left Stick Y: %d\n", (int)value);
          value = (value / 32767);
          value = floorf(value * 10) / 10;
          //printf("Controller Brake %f\n", value);
          
          if (value > 0.9) 
          {
            if (m_enginedefs[m_selected_engine].legacyEngine)
            {
              // if stick is pulled down enough, increment the gamePadTrainBrake variable
              // and check if it's over 1. if so, then set it max to one.
              // Then re run through the same code the train brake slider uses. 

              m_enginedefs[m_selected_engine].gamepadTrainBrake += 0.1f;

              if (m_enginedefs[m_selected_engine].gamepadTrainBrake > 1.0f)
                m_enginedefs[m_selected_engine].gamepadTrainBrake = 1.0f;

              int brakevalue = (int)(m_enginedefs[m_selected_engine].gamepadTrainBrake * 8.0f);
              m_enginedefs[m_selected_engine].currentTrainBrake = m_enginedefs[m_selected_engine].gamepadTrainBrake;
              m_enginedefs[m_selected_engine].SetSpeedMultiplier(1.0 - m_enginedefs[m_selected_engine].gamepadTrainBrake, m_enginedefs[m_selected_engine].legacyEngine, true);
              /*TMCCInterface::EngineSetTrainBrake2(engineID, brakevalue);
              if (brakeSoundEnabledAllTypes)
                TMCCInterface::EngineBrakeSquealSound(engineID);

              if (m_enginedefs[m_selected_engine].engineType == EngineTypeLegacy::ENGINE_TYPE_STEAM)
              {
                int labourvalue = (int)(m_enginedefs[m_selected_engine].gamepadTrainBrake * 32.0f);
                m_enginedefs[m_selected_engine].steam_labour_intensity = labourvalue;
                TMCCInterface::EngineSetLabor(engineID, labourvalue);
              }
              else
              {
                int dieselRun = (int)(m_enginedefs[m_selected_engine].gamepadTrainBrake * 8.0f);
                m_enginedefs[m_selected_engine].diesel_electric_rev_lvl = dieselRun;
                TMCCInterface::EngineSetDieselRunLevel(engineID, dieselRun);
              }*/
            }
          }
        }
      }

      //if (SDL_GameControllerGetButton(gGameController, SDL_CONTROLLER_BUTTON_RIGHTSTICK))
      //{
       // m_enginedefs[m_selected_engine].legacy_speed = 0;
       // m_enginedefs[m_selected_engine].SetSpeed(m_enginedefs[m_selected_engine].legacy_speed, m_enginedefs[m_selected_engine].legacyEngine);

     // }
      if (SDL_GameControllerGetButton(gGameController, SDL_CONTROLLER_BUTTON_A))
      {
        printf("Game controller Bell\n");
        if (!m_enginedefs[m_selected_engine].oneShotBellEnabled)
        {
          //printf("Bell Toggle\n");
          m_enginedefs[m_selected_engine].bellOn = !m_enginedefs[m_selected_engine].bellOn;
          if (m_enginedefs[m_selected_engine].bellOn)
            TMCCInterface::EngineSetBell(engineID, TMCC_ON);
          else
            TMCCInterface::EngineSetBell(engineID, TMCC_OFF);
        }
        else
        {
          printf("Controller One Shot Bell Ding: %d\n", m_enginedefs[m_selected_engine].bellDingCount);
          TMCCInterface::EngineBellOneShotDing(engineID, m_enginedefs[m_selected_engine].bellDingCount);
        }
      }
      if (SDL_GameControllerGetButton(gGameController, SDL_CONTROLLER_BUTTON_B))
      {
        printf("Controller Front Coupler\n");
        TMCCInterface::EngineOpenFrontCoupler(engineID);
      }
      if (SDL_GameControllerGetButton(gGameController, SDL_CONTROLLER_BUTTON_X))
      {
        
        m_enginedefs[m_selected_engine].oneShotBellEnabled = !m_enginedefs[m_selected_engine].oneShotBellEnabled;
        printf("Controller: One Shot bell: %d\n", m_enginedefs[m_selected_engine].oneShotBellEnabled);
      }
      if (SDL_GameControllerGetButton(gGameController, SDL_CONTROLLER_BUTTON_Y))
      {
        printf("Controller Rear Coupler\n");
        TMCCInterface::EngineOpenRearCoupler(engineID);
      }

      if (SDL_GameControllerGetButton(gGameController, SDL_CONTROLLER_BUTTON_START))
      {
        printf("Controller Numerical 3\n");
        TMCCInterface::EngineNumericCommand(engineID, 3);
      }

      if (SDL_GameControllerGetButton(gGameController, SDL_CONTROLLER_BUTTON_TOUCHPAD))
      {
        printf("Controller Numerical 6\n");
        TMCCInterface::EngineNumericCommand(engineID,6);
      }

      if (SDL_GameControllerGetButton(gGameController, SDL_CONTROLLER_BUTTON_GUIDE))
      {
        printf("Controller Extended Start Up\n");
        TMCCInterface::EngineStartupSequence1(engineID);
        TMCCInterface::EngineStartupSequence2(engineID);
      }

      if (SDL_GameControllerGetButton(gGameController, SDL_CONTROLLER_BUTTON_BACK))
      {
        printf("Controller Extended Shut Down\n");
        TMCCInterface::EngineShutdownSequence1(engineID);
        TMCCInterface::EngineShutdownSequence2(engineID);
      }

      if (SDL_GameControllerGetButton(gGameController, SDL_CONTROLLER_BUTTON_DPAD_UP))
      {
        if (guitarController)
        {
          m_enginedefs[m_selected_engine].legacy_speed--;
          if (m_enginedefs[m_selected_engine].legacy_speed < 0)
            m_enginedefs[m_selected_engine].legacy_speed = 0;
          //m_enginedefs[m_selected_engine].SetSpeed(m_enginedefs[m_selected_engine].legacy_speed, m_enginedefs[m_selected_engine].legacyEngine);
          m_enginedefs[m_selected_engine].UpdateSpeed();
        }
        else
        {
          printf("Controller Crewtalk\n");
          TMCCInterface::EngineNumericCommand(engineID, 2);
        }
        
      }

      if (SDL_GameControllerGetButton(gGameController, SDL_CONTROLLER_BUTTON_DPAD_DOWN))
      {
        if (guitarController)
        {
          //printf("UP Right Stick Y: %d\n", (int)value);
          m_enginedefs[m_selected_engine].legacy_speed++;
          if (m_enginedefs[m_selected_engine].legacy_speed > 200)
            m_enginedefs[m_selected_engine].legacy_speed = 200;
          //m_enginedefs[m_selected_engine].SetSpeed(m_enginedefs[m_selected_engine].legacy_speed, m_enginedefs[m_selected_engine].legacyEngine);
          m_enginedefs[m_selected_engine].UpdateSpeed();
        }
        else
        {
          printf("Controller Aux 3\n");
          TMCCInterface::EngineAux3Trigger(engineID);
        }
      }
      
      if (SDL_GameControllerGetButton(gGameController, SDL_CONTROLLER_BUTTON_DPAD_LEFT))
      {
        printf("Controller TowerCom\n");
        TMCCInterface::EngineNumericCommand(engineID,7);
      }

      if (SDL_GameControllerGetButton(gGameController, SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
      {
        printf("Controller Aux 1\n");
        TMCCInterface::EngineAux1Option1(engineID);
      }

      if (SDL_GameControllerGetButton(gGameController, SDL_CONTROLLER_BUTTON_LEFTSTICK))
      {
        
      }

      if (SDL_GameControllerGetButton(gGameController, SDL_CONTROLLER_BUTTON_RIGHTSTICK))
      {
        

        printf("Change Direction\n");
        TMCCInterface::EngineToggleDirection(engineID);
      }

      if (SDL_GameControllerGetButton(gGameController, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER))
      {
        gameControllerSelectedEngine++;
        printf("gameControllerEng :%d\n", gameControllerSelectedEngine);
        if (gameControllerSelectedEngine < m_enginedefs.size())
          m_selected_engine = gameControllerSelectedEngine;
        else
        {
          gameControllerSelectedEngine = 0;
          m_selected_engine = 0;
        }
        //SetUpEngineFromRoster(engineID, legacyEnabled, dir);
        engineID = m_enginedefs[m_selected_engine].engineID;
        legacyEnabled = m_enginedefs[m_selected_engine].legacyEngine;
        Image o;
        if (o.Load(dir + "/engine_picture/" + m_enginedefs[m_selected_engine].engineName + ".png"))
        {
          m_enginedefs[m_selected_engine].locoIcon = std::make_shared<Image>(dir + "/engine_picture/" + m_enginedefs[m_selected_engine].engineName + ".png");
          m_enginedefs[m_selected_engine].engineHasCustomIcon = true;
        }
        else
        {
          m_enginedefs[m_selected_engine].locoIcon = std::make_shared<Image>(dir + "/engine_picture/" + "default" + ".png");

        }
      }


      if (SDL_GameControllerGetButton(gGameController, SDL_CONTROLLER_BUTTON_LEFTSHOULDER))
      {
        gameControllerSelectedEngine--;
        printf("gameControllerEng :%d\n", gameControllerSelectedEngine);
        if (gameControllerSelectedEngine < 0)
        {
          gameControllerSelectedEngine = m_enginedefs.size() - 1;
          m_selected_engine = m_enginedefs.size() - 1;
        }

        else
        {
          m_selected_engine = gameControllerSelectedEngine;
        }

        engineID = m_enginedefs[m_selected_engine].engineID;
        legacyEnabled = m_enginedefs[m_selected_engine].legacyEngine;
        Image o;
        if (o.Load(dir + "/engine_picture/" + m_enginedefs[m_selected_engine].engineName + ".png"))
        {
          m_enginedefs[m_selected_engine].locoIcon = std::make_shared<Image>(dir + "/engine_picture/" + m_enginedefs[m_selected_engine].engineName + ".png");
          m_enginedefs[m_selected_engine].engineHasCustomIcon = true;
        }
        else
        {
          m_enginedefs[m_selected_engine].locoIcon = std::make_shared<Image>(dir + "/engine_picture/" + "default" + ".png");

        }
        //SetUpEngineFromRoster(engineID, legacyEnabled, dir);
      }
    }
    else
    {
    if (guitarController)
    {
      float value = (float)SDL_GameControllerGetAxis(gGameController, SDL_CONTROLLER_AXIS_LEFTY);
      if (value < leftStickYDeadZone - 10000)
      {

      }
      else if (value > leftStickYDeadZone + 10000)
      {
        //printf("Left Stick Y: %d\n", (int)value);
        value = (value / 32767) * 15.0f;
        if ((int)value > 0)
        {
          TMCCInterface::EngineSetQuillingHornIntensity(engineID, (int)value);
        }
        else
        {
          TMCCInterface::EngineSetQuillingHornIntensity(engineID, 0);
        }
        printf("Guitar Game controller Quilling: %d\n", (int)value);
      }
    }
      else if (SDL_GameControllerGetAxis(gGameController, SDL_CONTROLLER_AXIS_TRIGGERRIGHT))
      {
        float value = (float)SDL_GameControllerGetAxis(gGameController, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
        value = (value / 32767) * 15.0f;
        if ((int)value > 0)
        {
          TMCCInterface::EngineSetQuillingHornIntensity(engineID, (int)value);
        }
        else
        {
          TMCCInterface::EngineSetQuillingHornIntensity(engineID, 0);
        }
        printf("Game controller Quilling: %d\n", (int)value);
        //printf("Right Trigger value: %d\n", SDL_GameControllerGetAxis(gGameController, SDL_CONTROLLER_AXIS_TRIGGERRIGHT));
      }

    else if (SDL_GameControllerGetAxis(gGameController, SDL_CONTROLLER_AXIS_TRIGGERLEFT))
    {
      float value = (float)SDL_GameControllerGetAxis(gGameController, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
      value = (value / 32767);

      
      if (m_enginedefs[m_selected_engine].legacyEngine)
      {
        int brakevalue = (int)(value * 8.0f);
        m_enginedefs[m_selected_engine].SetSpeedMultiplier(1.0 - value, m_enginedefs[m_selected_engine].legacyEngine, true);
        //TMCCInterface::EngineSetTrainBrake2(engineID, brakevalue);
        //if (brakeSoundEnabledAllTypes)
        //  TMCCInterface::EngineBrakeSquealSound(engineID);

        //if (m_enginedefs[m_selected_engine].engineType == EngineTypeLegacy::ENGINE_TYPE_STEAM)
        //{
        //  int labourvalue = (int)(value * 32.0f);
        //  m_enginedefs[m_selected_engine].steam_labour_intensity = labourvalue;
        //  TMCCInterface::EngineSetLabor(engineID, labourvalue);
        //}
        //else
        //{
        //  int dieselRun = (int)(value * 8.0f);
        //  m_enginedefs[m_selected_engine].diesel_electric_rev_lvl = dieselRun;
        //  TMCCInterface::EngineSetDieselRunLevel(engineID, dieselRun);
        //}
      }

      printf("Game controller braking: %f\n", value);
      //printf("Right Trigger value: %d\n", SDL_GameControllerGetAxis(gGameController, SDL_CONTROLLER_AXIS_TRIGGERRIGHT));
    }

      
    }
    
  }
};


