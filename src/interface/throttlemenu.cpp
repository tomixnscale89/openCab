#include "throttlemenu.h"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <stdio.h>
#include "TMCCInterface.h"
#include "../json.hpp"
#include "../imgui-knobs.h"
#include "../EngineManagement.h"
#include <fstream>
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
  {"Reserved", DC_RESERVED},
  {"Conventional Mode Shutdown", DC_CONVENTIONAL_MODE_SHUTDOWN},
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
  {"Conventional Short Horn Trigger", DC_CONVENTIONAL_SHORT_HORN_TRIGGER}
};

static dialog_entry diner_dialog_map[]{
  {"Conductor: All Aboard", DC_CONDUCTOR_ALL_ABOARD},
  {"Conductor: Next Stop", STATION_SOUND_CAR_CONDUCTOR_NEXT_STOP},
  {"Conductor: Watch your step", STATION_SOUND_CAR_CONDUCTOR_WATCH_STEP},
  {"Conductor: All Aboard 2", STATION_SOUND_CAR_CONDUCTOR_ALL_ABOARD},
  {"Conductor: Tickets Please", STATION_SOUND_CAR_CONDUCTOR_TICKETS_PLZ},
  {"Conductor: Premature Stop", STATION_SOUND_CAR_CONDUCTOR_PREMATURE_STOP},
  {"Steward: Welcome", STATION_SOUND_CAR_STEWARD_WELCOME},
  {"Steward: First Seating now Open", STATION_SOUND_CAR_STEWARD_FIRST_SEATING},
  {"Steward: Second Seating now Open", STATION_SOUND_CAR_STEWARD_SECOND_SEATING},  
  {"Steward: Lounge now Open", STATION_SOUND_CAR_STEWARD_LOUNGE_OPEN},
  {"PA System: Train Arriving", STATION_SOUND_CAR_PA_TRAIN_ARRIVING},
  {"PA System: Train Arrived", STATION_SOUND_CAR_PA_TRAIN_ARRIVED},  
  {"PA System: Train Boarding", STATION_SOUND_CAR_PA_TRAIN_BOARDING},
  {"PA System: Train Departing", STATION_SOUND_CAR_PA_TRAIN_DEPARTING},
  {"Startup", STATION_SOUND_CAR_STARTUP},
  {"Shutdown", STATION_SOUND_CAR_SHUTDOWN}
};


ThrottleMenu::ThrottleMenu(const std::string& appDir)
{
  speed = 0;
  currentTrainBrake = 0;
  currentQuill = 0;
  engineID = 1;

  dir = appDir.substr(0, appDir.find_last_of("/\\"));

  //engineTestIcon = std::make_shared<Image>(dir + "/engine_picture/Sleepy Hollow Casket Co. #31.png");

  blowdownIcon = std::make_shared<Image>(dir + "/graphics/blowdown.png");
  volUpIcon = std::make_shared<Image>(dir + "/graphics/volup.png");
  volDownIcon = std::make_shared<Image>(dir + "/graphics/voldown.png");
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




  stopSlow0Icon = std::make_shared<Image>(dir + "/graphics/stopslowspeed.png");
  marsonIcon = std::make_shared<Image>(dir + "/graphics/mars_on.png");
  marsoffIcon = std::make_shared<Image>(dir + "/graphics/mars_off.png");
  doglightoffIcon = std::make_shared<Image>(dir + "/graphics/doghouse_off.png");
  doglightonIcon = std::make_shared<Image>(dir + "/graphics/doghouse_on.png");
  groundlightonIcon = std::make_shared<Image>(dir + "/graphics/groundlights_on.png");
  groundlightoffIcon = std::make_shared<Image>(dir + "/graphics/groundlights_off.png");
  ditchlightoffIcon = std::make_shared<Image>(dir + "/graphics/ditch_off.png");
  ditchlightonIcon = std::make_shared<Image>(dir + "/graphics/ditch_on.png");
  ditchlightautoIcon = std::make_shared<Image>(dir + "/graphics/0.png");
  locomarkeronIcon = std::make_shared<Image>(dir + "/graphics/loco_markers_on.png");
  locomarkeroffIcon = std::make_shared<Image>(dir + "/graphics/loco_markers_off.png");
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

  acelaArriveIcon = std::make_shared<Image>(dir + "/graphics/arriving_acela.png");
  acelaDepartIcon = std::make_shared<Image>(dir + "/graphics/departing_acela.png");


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
      m_enginedefs[m_selected_engine].locoIcon = std::make_shared<Image>(dir + "/engine_picture/" + "default" + ".png");

    }
    printf("Engine ID to use: %d\n", engineID);
  }
}

void ThrottleMenu::Draw(const std::string& appDir)
{

  float curTime = (float)clock() / CLOCKS_PER_SEC;

  if (soundMenuVisible)              ShowSoundWindow(&soundMenuVisible);
  if (voiceClipMenuVisible)              ShowVoiceWindow(&voiceClipMenuVisible);
  if (dinerVoiceClipMenuVisible)              ShowDinerVoiceWindow(&dinerVoiceClipMenuVisible);
  if (addEngineMenuVisible)              AddEngineWindow(&addEngineMenuVisible, appDir);

  if (surfaceDialEnabled)
  {
  }
  // Menu Bar
  if (ImGui::BeginMenuBar())
  {
    if (ImGui::BeginMenu("Menu"))
    {

      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Engine Management"))
    {
      ImGui::MenuItem("Add Engine", NULL, &addEngineMenuVisible);
      ImGui::MenuItem("View Engine Roster", NULL, &test);
      ImGui::MenuItem("Delete Engine", NULL, &test);

      ImGui::EndMenu();
    }
    //if (ImGui::MenuItem("MenuItem")) {} // You can also use MenuItem() inside a menu bar!
    if (ImGui::BeginMenu("Feature Windows"))
    {
      ImGui::MenuItem("Sound Menu", NULL, &soundMenuVisible);
      ImGui::MenuItem("CrewTalk/TowerCom Announcements", NULL, &voiceClipMenuVisible);
      ImGui::MenuItem("StationSounds Diner Announcements", NULL, &dinerVoiceClipMenuVisible);
      ImGui::MenuItem("Lighting Menu", NULL, &test);
      ImGui::MenuItem("Enable Surface Dial", NULL, &surfaceDialEnabled);
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }

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

    std::string engineHint = "No Engines Found.";
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
            m_enginedefs[m_selected_engine].locoIcon = std::make_shared<Image>(dir + "/engine_picture/" + "default" + ".png");

          }
          printf("Engine ID to use: %d\n", engineID);

        }
      }
      ImGui::EndCombo();
    }

    //static int value = 0;
    static float f = 0.0f;
    static int counter = 0;
    bool show_demo_window = false;
    if (show_demo_window)
      ImGui::ShowDemoWindow(&show_demo_window);

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

    ThrottleWindow(&test, curTime);



    //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);







    ImGui::End();
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



void ThrottleMenu::ThrottleWindow(bool* p_open, float curTime)
{
  const auto& engine = m_enginedefs[m_selected_engine];

  if (ImGui::BeginTable("split", 4))
  {
    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 80.0f);
    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 256.0f);
    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 256.0f);
    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 80.0f);

    ImGui::TableNextColumn();
    
    // brakeslidervalue from 0.0f to 1.0f
    if (ImGui::VSliderFloat("##int", ImVec2(80, 250), &m_enginedefs[m_selected_engine].currentTrainBrake, 1.0f, 0.0f, "Train\nBrake"))
    {
      int brakevalue = (int)(m_enginedefs[m_selected_engine].currentTrainBrake * 8.0f); // or, if you want a tolerance, (int)(engine.currentTrainBrake * 8.0f + 0.5f)
      printf("brake value: %d\n", brakevalue);
      m_enginedefs[m_selected_engine].SetSpeedMultiplier(1.0 - m_enginedefs[m_selected_engine].currentTrainBrake);
      TMCCInterface::EngineSetTrainBrake2(engineID, brakevalue);
      if (m_enginedefs[m_selected_engine].engineType == EngineTypeLegacy::ENGINE_TYPE_STEAM)
      {
        int labourvalue = (int)(m_enginedefs[m_selected_engine].currentTrainBrake * 32.0f);
        TMCCInterface::EngineSetLabor(engineID, labourvalue);
      }
      else
      {
        int dieselRun = (int)(m_enginedefs[m_selected_engine].currentTrainBrake * 8.0f);
        TMCCInterface::EngineSetDieselRunLevel(engineID, dieselRun);
      }
    }


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
      if (ImGui::Button("Let Off", ImVec2(78, 76)))
      {
        printf("TMCC Let off Pressed\n");
        TMCCInterface::EngineLetOffSound(engineID);
      };
      ImGui::PopButtonRepeat();
    }

    // Boost  and brake
      ImGui::PushButtonRepeat(true);
      if (ImGui::Button("Boost", ImVec2(78, 76)))
      {
        printf("Bosot\n");
        TMCCInterface::EngineBoostSpeed(engineID);
      };
      ImGui::PopButtonRepeat();

      ImGui::PushButtonRepeat(true);
      if (ImGui::Button("Brake", ImVec2(78, 76)))
      {
        printf("Brake\n");
        TMCCInterface::EngineBrakeSpeed(engineID);
      };
      ImGui::PopButtonRepeat();
    

    ImGui::TableNextColumn();
    ImGui::SetNextItemWidth(50);

    if (m_enginedefs[m_selected_engine].engineHasCustomIcon)
    {
      // 250 98
      ImGui::Image((void*)(intptr_t)m_enginedefs[m_selected_engine].locoIcon->GetGLHandle(), ImVec2(250, 98));

    }

    DrawKeypadType(currentKeypadStyle, m_enginedefs[m_selected_engine].legacyEngine, m_enginedefs[m_selected_engine].engineType);

    if (m_enginedefs[m_selected_engine].legacyEngine)
    {
      // Legacy Version
      if (ImGuiKnobs::KnobInt("Throttle", &m_enginedefs[m_selected_engine].legacy_speed, 0, 200, 1, "%d MPH", ImGuiKnobVariant_Wiper, 256)) {
        // value was changed
        
        //m_enginedefs[m_selected_engine].SetSpeed(m_enginedefs[m_selected_engine].legacy_speed);
        TMCCInterface::EngineSetAbsoluteSpeed2(engineID, m_enginedefs[m_selected_engine].legacy_speed);
      }
    }
    else if(!m_enginedefs[m_selected_engine].legacyEngine && m_enginedefs[m_selected_engine].engineType == EngineTypeTMCC::ENGINE_TYPE_TMCC_CRANE)
    {
      // No Throttle for TMCC Crane
    }
    else
    {
      // TMCC Version
      if (ImGuiKnobs::KnobInt("Throttle", &m_enginedefs[m_selected_engine].tmcc_speed, 0, 32, 1, "%d MPH", ImGuiKnobVariant_Wiper, 256)) {
        // value was changed
        TMCCInterface::EngineSetAbsoluteSpeed(engineID, m_enginedefs[m_selected_engine].tmcc_speed);
      }
    }

    
    ImGui::PushButtonRepeat(true);
    if (ImGui::Button("-", ImVec2(78, 64)))
    {
      printf("Speed -1\n");
      if (m_enginedefs[m_selected_engine].legacyEngine)
        m_enginedefs[m_selected_engine].legacy_speed--;
      else
        m_enginedefs[m_selected_engine].tmcc_speed--;
    };
    ImGui::PopButtonRepeat();

    ImGui::SameLine();
    if (ImGui::Button("STOP", ImVec2(78, 64)))
    {
      printf("STOP\n");
      if (m_enginedefs[m_selected_engine].legacyEngine)
        m_enginedefs[m_selected_engine].legacy_speed = 0;
      else
        m_enginedefs[m_selected_engine].tmcc_speed = 0;
    };
    ImGui::SameLine();

    ImGui::PushButtonRepeat(true);
    if (ImGui::Button("+", ImVec2(78, 64)))
    {
      printf("Speed +1\n");
      if (m_enginedefs[m_selected_engine].legacyEngine)
        m_enginedefs[m_selected_engine].legacy_speed++;
      else
        m_enginedefs[m_selected_engine].tmcc_speed++;
    };
    ImGui::PopButtonRepeat();




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
    
    
        if (ImGui::ImageButton((void*)(intptr_t)reverseIcon->GetGLHandle(), ImVec2(70, 70)))
        {
          printf("Reverse\n");
          TMCCInterface::EngineSetDirection(engineID, TMCC_REVERSE);
        };

    if (m_enginedefs[m_selected_engine].legacyEngine)
    {
      if (ImGui::ImageButton((void*)(intptr_t)bellIcon->GetGLHandle(), ImVec2(70, 70)))
      {
        if (!m_enginedefs[m_selected_engine].oneShotBellEnabled)
        {
          printf("Bell Toggle\n");
          m_enginedefs[m_selected_engine].bellOn = !m_enginedefs[m_selected_engine].bellOn;
          if (m_enginedefs[m_selected_engine].bellOn)
            TMCCInterface::EngineSetBell(engineID, TMCC_ON);
          else
            TMCCInterface::EngineSetBell(engineID, TMCC_OFF);
        }
        else
        {
          printf("One Shot Bell Ding: %d\n", m_enginedefs[m_selected_engine].bellDingCount);
          TMCCInterface::EngineBellOneShotDing(engineID, m_enginedefs[m_selected_engine].bellDingCount);
        }
      }
    }
    else
    {

      if (ImGui::ImageButton((void*)(intptr_t)bellIcon->GetGLHandle(), ImVec2(70, 70)))
      {
          printf("Bell Toggle\n");
          m_enginedefs[m_selected_engine].bellOn = !m_enginedefs[m_selected_engine].bellOn;
          if (m_enginedefs[m_selected_engine].bellOn)
            TMCCInterface::EngineSetBell(engineID, TMCC_ON);
          else
            TMCCInterface::EngineSetBell(engineID, TMCC_OFF);
        }
    }
    

    if (ImGui::ImageButton((void*)(intptr_t)rear_couplerIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      printf("Fire Rear coupler\n");
      TMCCInterface::EngineOpenRearCoupler(engineID);
    };

    if (m_enginedefs[m_selected_engine].legacyEngine)
    {
      // value was changed
          // if steam, use labour
          // else, use diesel run level
      if (m_enginedefs[m_selected_engine].engineType == EngineTypeLegacy::ENGINE_TYPE_STEAM)
      {
        if (ImGuiKnobs::KnobInt("Labour", &m_enginedefs[m_selected_engine].steam_labour_intensity, 0, 31, 1, "%f Labour", ImGuiKnobVariant_Wiper, 100)) {
          
          TMCCInterface::EngineSetLabor(engineID, m_enginedefs[m_selected_engine].steam_labour_intensity);
        };
      }
      else
      {
        if (ImGuiKnobs::KnobInt("Diesel Run Lvl.", &m_enginedefs[m_selected_engine].diesel_electric_rev_lvl, 0, 8, 1, "%f Diesel Run Level", ImGuiKnobVariant_Wiper, 100)) {

          if (m_enginedefs[m_selected_engine].engineType == EngineTypeLegacy::ENGINE_TYPE_DIESEL)
            TMCCInterface::EngineSetDieselRunLevel(engineID, m_enginedefs[m_selected_engine].diesel_electric_rev_lvl);
          else if (m_enginedefs[m_selected_engine].engineType == EngineTypeLegacy::ENGINE_TYPE_ELECTRIC)
            TMCCInterface::EngineSetDieselRunLevel(engineID, m_enginedefs[m_selected_engine].diesel_electric_rev_lvl);
          else if (m_enginedefs[m_selected_engine].engineType == EngineTypeLegacy::ENGINE_TYPE_SUBWAY)
            TMCCInterface::EngineSetDieselRunLevel(engineID, m_enginedefs[m_selected_engine].diesel_electric_rev_lvl);
        };
      }

      ImGui::PushButtonRepeat(true);
      if (ImGui::Button("EFX\n-", ImVec2(38, 64)))
      {
        if (m_enginedefs[m_selected_engine].engineType == EngineTypeLegacy::ENGINE_TYPE_STEAM)
          m_enginedefs[m_selected_engine].steam_labour_intensity--;
        else
          m_enginedefs[m_selected_engine].diesel_electric_rev_lvl--;
      };
      ImGui::PopButtonRepeat();

      ImGui::SameLine();
      if (ImGui::Button("Zero", ImVec2(38, 64)))
      {
        if (m_enginedefs[m_selected_engine].engineType == EngineTypeLegacy::ENGINE_TYPE_STEAM)
          m_enginedefs[m_selected_engine].steam_labour_intensity = 0;
        else
          m_enginedefs[m_selected_engine].diesel_electric_rev_lvl = 0;
      };
      ImGui::SameLine();

      ImGui::PushButtonRepeat(true);
      if (ImGui::Button("EFX\n+", ImVec2(38, 64)))
      {
        if (m_enginedefs[m_selected_engine].engineType == EngineTypeLegacy::ENGINE_TYPE_STEAM)
          m_enginedefs[m_selected_engine].steam_labour_intensity++;
        else
          m_enginedefs[m_selected_engine].diesel_electric_rev_lvl++;
      };
      ImGui::PopButtonRepeat();
    }

    

    //ImGui::Image();

    ImGui::EndTable();
  }
};

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
      int value = (int)(currentQuill * 8.0f);
      ThrottleMenu::lastQuillTime = curTime;
      printf("Quilling: %d\n", value);
      TMCCInterface::EngineSetQuillingHornIntensity(engineID, value);
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

void ThrottleMenu::DrawCAB1Keypad()
{
  ImGui::ImageButton((void*)(intptr_t)num1Icon->GetGLHandle(), ImVec2(70, 70));
  if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
  {
    printf("Num 1 key\n");
    TMCCInterface::EngineNumericCommand(engineID, 1);
  };
  ImGui::SameLine();
  ImGui::ImageButton((void*)(intptr_t)num2Icon->GetGLHandle(), ImVec2(70, 70));
  if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
  {
    printf("Num 2 key\n");
    TMCCInterface::EngineNumericCommand(engineID, 2);
  };
  ImGui::SameLine();
  ImGui::ImageButton((void*)(intptr_t)num3Icon->GetGLHandle(), ImVec2(70, 70));
  if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
  {
    printf("Num 3 key\n");
    TMCCInterface::EngineNumericCommand(engineID, 3);
  };


  ImGui::ImageButton((void*)(intptr_t)num4Icon->GetGLHandle(), ImVec2(70, 70));
  if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
  {
    printf("Num 4 key\n");
    TMCCInterface::EngineNumericCommand(engineID, 4);
  };
  ImGui::SameLine();
  ImGui::ImageButton((void*)(intptr_t)num5Icon->GetGLHandle(), ImVec2(70, 70));
  if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
  {
    printf("Num 5 key\n");
    TMCCInterface::EngineNumericCommand(engineID, 5);
  };
  ImGui::SameLine();
  ImGui::ImageButton((void*)(intptr_t)num6Icon->GetGLHandle(), ImVec2(70, 70));
  if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
  {
    printf("Num 6 key\n");
    TMCCInterface::EngineNumericCommand(engineID, 6);
  };

  ImGui::ImageButton((void*)(intptr_t)num7Icon->GetGLHandle(), ImVec2(70, 70));
  if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
  {
    printf("Num 7 key\n");
    TMCCInterface::EngineNumericCommand(engineID, 7);
  };
  ImGui::SameLine();
  ImGui::ImageButton((void*)(intptr_t)num8Icon->GetGLHandle(), ImVec2(70, 70));
  if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
  {
    printf("Num 8 key\n");
    TMCCInterface::EngineNumericCommand(engineID, 8);
  };
  ImGui::SameLine();
  ImGui::ImageButton((void*)(intptr_t)num9Icon->GetGLHandle(), ImVec2(70, 70));
  if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
  {
    printf("Num 9 key\n");
    TMCCInterface::EngineNumericCommand(engineID, 9);
  };
  if (ImGui::ImageButton((void*)(intptr_t)leftArrowIcon->GetGLHandle(), ImVec2(70, 70)))
  {
    m_enginedefs[m_selected_engine].keyPadPage--;
    if (m_enginedefs[m_selected_engine].keyPadPage < 0)
      m_enginedefs[m_selected_engine].keyPadPage = 2;
  };
  ImGui::SameLine();
  ImGui::ImageButton((void*)(intptr_t)num0Icon->GetGLHandle(), ImVec2(70, 70));
  if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
  {
    printf("Num 0 key\n");
    TMCCInterface::EngineNumericCommand(engineID, 0);
  };
  ImGui::SameLine();
  if (ImGui::ImageButton((void*)(intptr_t)rightArrowIcon->GetGLHandle(), ImVec2(70, 70)))
  {
    m_enginedefs[m_selected_engine].keyPadPage++;
    if (m_enginedefs[m_selected_engine].keyPadPage > 2)
      m_enginedefs[m_selected_engine].keyPadPage = 0;
  };


}

void ThrottleMenu::DrawCAB2SteamKeypad()
{
  if (m_enginedefs[m_selected_engine].keyPadPage == 0)
  {
    if (ImGui::ImageButton((void*)(intptr_t)volUpIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineSetMasterVolume(engineID, TMCC_ON);

    };
    Tooltip("Volume Up");

    ImGui::SameLine();

    ImGui::ImageButton((void*)(intptr_t)crewTalkIcon->GetGLHandle(), ImVec2(70, 70));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("Crewtalk\n");
      TMCCInterface::EngineNumericCommand2(engineID, 2);
    };

    Tooltip("CrewTalk");

    ImGui::SameLine();

    ImGui::ImageButton((void*)(intptr_t)waterIcon->GetGLHandle(), ImVec2(70, 70));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
        printf("Water injector\n");
        TMCCInterface::EngineRSTriggerWaterInjector(engineID);
    };
    
    Tooltip("Water Refill");


    if (ImGui::ImageButton((void*)(intptr_t)volDownIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineSetMasterVolume(engineID, TMCC_OFF);

    };

    Tooltip("Volume Down");

    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)shutDownIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineNumericCommand2(engineID, 5);

    };

    Tooltip("Shut Down");

    ImGui::SameLine();

    ImGui::ImageButton((void*)(intptr_t)blowdownIcon->GetGLHandle(), ImVec2(70, 70));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("Blowdown\n");
      TMCCInterface::EngineRSTriggerLongLetOff(engineID);
    };

    Tooltip("Blowdown");


    ImGui::ImageButton((void*)(intptr_t)towerComIcon->GetGLHandle(), ImVec2(70, 70));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("Tower com\n");
      TMCCInterface::EngineNumericCommand2(engineID, 7);
    };

    Tooltip("Tower Com");

    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)smokeOffIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      m_enginedefs[m_selected_engine].smoke_state_legacy--;
      if (m_enginedefs[m_selected_engine].smoke_state_legacy < 0)
        m_enginedefs[m_selected_engine].smoke_state_legacy = 3;
      printf("Smoke system: %d\n", m_enginedefs[m_selected_engine].smoke_state_legacy);
      TMCCInterface::EngineSetSmokeSystem(engineID, m_enginedefs[m_selected_engine].smoke_state_legacy);
    };

    Tooltip("Smoke Off");

    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)smokeOnIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      m_enginedefs[m_selected_engine].smoke_state_legacy++;
      if (m_enginedefs[m_selected_engine].smoke_state_legacy > 3)
        m_enginedefs[m_selected_engine].smoke_state_legacy = 0;
      printf("Smoke system: %d\n", m_enginedefs[m_selected_engine].smoke_state_legacy);
      TMCCInterface::EngineSetSmokeSystem(engineID, m_enginedefs[m_selected_engine].smoke_state_legacy);
    };
    Tooltip("Smoke On");

    if (ImGui::ImageButton((void*)(intptr_t)tenderMarkerOnIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineSetTenderMarkerLight(engineID, TMCC_ON);
    };
    Tooltip("Tender Marker On");

    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)resetIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineNumericCommand2(engineID, 0);

    };
    Tooltip("Reset (0 key)");

    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)rule17onIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineSetRule17(engineID, TMCC_ON);

    };

    Tooltip("Rule 17 Lighting On");

    if (ImGui::ImageButton((void*)(intptr_t)tenderMarkerOffIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineSetTenderMarkerLight(engineID, TMCC_OFF);

    };

    Tooltip("Tender Marker Off");

    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)extstartUpIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineStartupSequence1(engineID);
      TMCCInterface::EngineStartupSequence2(engineID);
    };
    Tooltip("Start Up Sequence 1 (Delayed Prime Mover)");

    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)rule17offIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineSetRule17(engineID, TMCC_OFF);

    };

    Tooltip("Rule 17 Lighing Off");


    if (ImGui::ImageButton((void*)(intptr_t)leftArrowIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      m_enginedefs[m_selected_engine].keyPadPage--;
      if (m_enginedefs[m_selected_engine].keyPadPage < 0)
        m_enginedefs[m_selected_engine].keyPadPage = 2;
    };
    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)extshutDownIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineShutdownSequence1(engineID);
      TMCCInterface::EngineShutdownSequence2(engineID);
    };
    Tooltip("Shut Down Sequence 1 (Delayed)");
    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)rightArrowIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      m_enginedefs[m_selected_engine].keyPadPage++;
      if (m_enginedefs[m_selected_engine].keyPadPage > 2)
        m_enginedefs[m_selected_engine].keyPadPage = 0;
    };
    
  }

  /// <summary>
  /// PAGE 2
  /// </summary>
  else if(m_enginedefs[m_selected_engine].keyPadPage == 1)
    {
    if (ImGui::ImageButton((void*)(intptr_t)volUpIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineSetMasterVolume(engineID, TMCC_ON);

    };
    Tooltip("Volume Up");

    ImGui::SameLine();

    ImGui::ImageButton((void*)(intptr_t)coalIcon->GetGLHandle(), ImVec2(70, 70));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("Coal refuel\n");
      TMCCInterface::EngineLocoRefuelSound(engineID);
    };

    Tooltip("Coal refuel");

    ImGui::SameLine();
    ImGui::PushButtonRepeat(true);
    if (ImGui::Button("Enable\nCylinder\Cocks", ImVec2(78, 76)))
    {
      TMCCInterface::EngineToggleCylinderCock(engineID, TMCC_ON);
    };
    ImGui::PopButtonRepeat();
    Tooltip("Enable\nCylinder\Cocks");


    if (ImGui::ImageButton((void*)(intptr_t)volDownIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineSetMasterVolume(engineID, TMCC_OFF);

    };

    Tooltip("Volume Down");

    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)blankIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineNumericCommand2(engineID, 5);

    };

    Tooltip("Shut Down");

    ImGui::SameLine();

    ImGui::PushButtonRepeat(true);
    if (ImGui::Button("Disable\nCylinder\Cocks", ImVec2(78, 76)))
    {
      TMCCInterface::EngineToggleCylinderCock(engineID, TMCC_OFF);
    };
    ImGui::PopButtonRepeat();
    Tooltip("Disable\nCylinder\Cocks");


    if (ImGui::ImageButton((void*)(intptr_t)blankIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineNumericCommand2(engineID, 7);

    };


    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)blankIcon->GetGLHandle(), ImVec2(70, 70)))
    {

    };


    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)blankIcon->GetGLHandle(), ImVec2(70, 70)))
    {

    };

    if (ImGui::ImageButton((void*)(intptr_t)blankIcon->GetGLHandle(), ImVec2(70, 70)))
    {
    };

    ImGui::SameLine();

    ImGui::ImageButton((void*)(intptr_t)resetIcon->GetGLHandle(), ImVec2(70, 70));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("Reset\n");
      TMCCInterface::EngineNumericCommand2(engineID, 0);
    };
    Tooltip("Reset (0 key)");

    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)blankIcon->GetGLHandle(), ImVec2(70, 70)))
    {

    };


    if (ImGui::ImageButton((void*)(intptr_t)blankIcon->GetGLHandle(), ImVec2(70, 70)))
    {

    };


    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)extstartUpIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineStartupSequence2(engineID);
    };
    Tooltip("Immediate Start Up");


    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)blankIcon->GetGLHandle(), ImVec2(70, 70)))
    {

    };



    if (ImGui::ImageButton((void*)(intptr_t)leftArrowIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      m_enginedefs[m_selected_engine].keyPadPage--;
      if (m_enginedefs[m_selected_engine].keyPadPage < 0)
        m_enginedefs[m_selected_engine].keyPadPage = 2;
    };
    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)extshutDownIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineShutdownSequence2(engineID);

    };
    Tooltip("Immediate Shut Down");

    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)rightArrowIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      m_enginedefs[m_selected_engine].keyPadPage++;
      if (m_enginedefs[m_selected_engine].keyPadPage > 2)
        m_enginedefs[m_selected_engine].keyPadPage = 0;
    };
    }
  else
  {
    DrawCAB1Keypad();
  }
}

// ELECTRIC

void ThrottleMenu::DrawCAB2ElectricKeypad()
{
  ImGui::PushButtonRepeat(true);
  if (ImGui::ImageButton((void*)(intptr_t)volUpIcon->GetGLHandle(), ImVec2(70, 70)))
  {
    TMCCInterface::EngineSetMasterVolume(engineID, TMCC_ON);

  };
  ImGui::SameLine();
  ImGui::PopButtonRepeat();

  ImGui::PushButtonRepeat(true);
  if (ImGui::ImageButton((void*)(intptr_t)crewTalkIcon->GetGLHandle(), ImVec2(70, 70)))
  {
    TMCCInterface::EngineNumericCommand2(engineID, 2);
  };
  ImGui::PopButtonRepeat();

  ImGui::SameLine();
  ImGui::PushButtonRepeat(true);
  if (ImGui::ImageButton((void*)(intptr_t)waterIcon->GetGLHandle(), ImVec2(70, 70)))
  {
    TMCCInterface::EngineRSTriggerWaterInjector(engineID);
  };
  ImGui::PopButtonRepeat();


  if (ImGui::ImageButton((void*)(intptr_t)volDownIcon->GetGLHandle(), ImVec2(70, 70)))
  {
    TMCCInterface::EngineSetMasterVolume(engineID, TMCC_OFF);

  };
  ImGui::SameLine();
  if (ImGui::ImageButton((void*)(intptr_t)shutDownIcon->GetGLHandle(), ImVec2(70, 70)))
  {
    TMCCInterface::EngineNumericCommand2(engineID, 5);

  };
  ImGui::SameLine();

  if (ImGui::ImageButton((void*)(intptr_t)blowdownIcon->GetGLHandle(), ImVec2(70, 70)))
  {
    TMCCInterface::EngineRSTriggerLongLetOff(engineID);

  };

  Tooltip("Blowdown");

  if (ImGui::ImageButton((void*)(intptr_t)towerComIcon->GetGLHandle(), ImVec2(70, 70)))
  {
    printf("Numerical Key 7 Towercom\n");
    TMCCInterface::EngineNumericCommand2(engineID, 7);

  };



  ImGui::SameLine();
  if (ImGui::ImageButton((void*)(intptr_t)smokeOffIcon->GetGLHandle(), ImVec2(70, 70)))
  {

  };
  ImGui::SameLine();
  if (ImGui::ImageButton((void*)(intptr_t)smokeOnIcon->GetGLHandle(), ImVec2(70, 70)))
  {

  };

  if (ImGui::ImageButton((void*)(intptr_t)panto_front_upIcon->GetGLHandle(), ImVec2(70, 70)))
  {
    printf("Front Panto up\n");

    TMCCInterface::EngineSetFrontPantograph(engineID, TMCC_UP);

  };
  ImGui::SameLine();
  if (ImGui::ImageButton((void*)(intptr_t)resetIcon->GetGLHandle(), ImVec2(70, 70)))
  {
    TMCCInterface::EngineNumericCommand2(engineID, 0);

  };
  ImGui::SameLine();
  if (ImGui::ImageButton((void*)(intptr_t)panto_rear_upIcon->GetGLHandle(), ImVec2(70, 70)))
  {
    printf("Rear Panto up\n");

    TMCCInterface::EngineSetRearPantograph(engineID, TMCC_UP);

  };

  if (ImGui::ImageButton((void*)(intptr_t)panto_front_downIcon->GetGLHandle(), ImVec2(70, 70)))
  {
    printf("Front Panto down\n");

    TMCCInterface::EngineSetFrontPantograph(engineID, TMCC_DOWN);

  };
  ImGui::SameLine();
  if (ImGui::ImageButton((void*)(intptr_t)extstartUpIcon->GetGLHandle(), ImVec2(70, 70)))
  {
    TMCCInterface::EngineStartupSequence1(engineID);
    TMCCInterface::EngineStartupSequence2(engineID);
  };
  Tooltip("Start Up Sequence 1 (Delayed Prime Mover)");
  ImGui::SameLine();
  if (ImGui::ImageButton((void*)(intptr_t)panto_rear_downIcon->GetGLHandle(), ImVec2(70, 70)))
  {
    printf("Rear Panto down\n");

    TMCCInterface::EngineSetRearPantograph(engineID, TMCC_DOWN);
  };

  if (ImGui::ImageButton((void*)(intptr_t)leftArrowIcon->GetGLHandle(), ImVec2(70, 70)))
  {
    currentKeypadStyle--;
    if (currentKeypadStyle < 0)
      currentKeypadStyle = 7;
  };
  ImGui::SameLine();
  if (ImGui::ImageButton((void*)(intptr_t)extshutDownIcon->GetGLHandle(), ImVec2(70, 70)))
  {
    TMCCInterface::EngineShutdownSequence1(engineID);
    TMCCInterface::EngineShutdownSequence2(engineID);
  };
  ImGui::SameLine();
  if (ImGui::ImageButton((void*)(intptr_t)rightArrowIcon->GetGLHandle(), ImVec2(70, 70)))
  {
    currentKeypadStyle++;
    if (currentKeypadStyle > 7)
      currentKeypadStyle = 0;
  };
}


// DINER

void ThrottleMenu::DrawStationDinerKeypad(void)
{
  if (m_enginedefs[m_selected_engine].keyPadPage == 0)
  {
    if (ImGui::ImageButton((void*)(intptr_t)volUpIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineSetMasterVolume(engineID, TMCC_ON);

    };
    Tooltip("Volume Up");

    ImGui::SameLine();

    ImGui::ImageButton((void*)(intptr_t)conductorAnnouncementIcon->GetGLHandle(), ImVec2(70, 70));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("Conductor Announcement\n");
      TMCCInterface::EngineNumericCommand2(engineID, 2);
    };

    Tooltip("CrewTalk");

    ImGui::SameLine();

    ImGui::Button("Enable\nSounds", ImVec2(78, 76));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("Enable Sounds\n");
      TMCCInterface::EngineDialogCommand(engineID, STATION_SOUND_CAR_STARTUP);
    };


    if (ImGui::ImageButton((void*)(intptr_t)volDownIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineSetMasterVolume(engineID, TMCC_OFF);

    };

    Tooltip("Volume Down");

    ImGui::SameLine();
    ImGui::Button("Disable\nSounds", ImVec2(78, 76));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("Disable Sounds\n");
      TMCCInterface::EngineDialogCommand(engineID, STATION_SOUND_CAR_SHUTDOWN);
    };


    ImGui::SameLine();

    ImGui::ImageButton((void*)(intptr_t)nextStopIcon->GetGLHandle(), ImVec2(70, 70));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("Next Stop\n");
      TMCCInterface::EngineDialogCommand(engineID,DialogCommandParams::STATION_SOUND_CAR_CONDUCTOR_NEXT_STOP);
    };

    Tooltip("Blowdown");


    ImGui::ImageButton((void*)(intptr_t)towerComIcon->GetGLHandle(), ImVec2(70, 70));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("Station sounds\n");
      TMCCInterface::EngineNumericCommand2(engineID, 7);
    };

    Tooltip("Station sounds");

    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)lightOffIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      printf("Light off\n");
      TMCCInterface::EngineSetDinerLight(engineID, TMCC_OFF);
    };

    Tooltip("Light Off");

    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)lightOnIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      printf("Light on\n");
      TMCCInterface::EngineSetDinerLight(engineID, TMCC_ON);
    };
    Tooltip("Smoke On");

    if (ImGui::ImageButton((void*)(intptr_t)railClatterOnIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineSetStockCarOption1(engineID, TMCC_ON);
    };
    Tooltip("Rail Clatter ON");

    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)resetIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineNumericCommand2(engineID, 0);

    };
    Tooltip("Reset (0 key)");

    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)curveGrindOnIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineSetStockCarOption2(engineID, TMCC_ON);

    };

    Tooltip("Curve Grinding On");

    if (ImGui::ImageButton((void*)(intptr_t)railClatterOffIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineSetStockCarOption1(engineID, TMCC_OFF);

    };

    Tooltip("Rail Clatter Off");

    ImGui::SameLine();
    ImGui::Button("Premature\nStop", ImVec2(78, 76));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("Premature stop\n");
      TMCCInterface::EngineDialogCommand(engineID, DialogCommandParams::STATION_SOUND_CAR_CONDUCTOR_PREMATURE_STOP);
    };

    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)curveGrindOffIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineSetStockCarOption2(engineID, TMCC_OFF);

    };

    Tooltip("Curve Grinding Off");


    if (ImGui::ImageButton((void*)(intptr_t)leftArrowIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      m_enginedefs[m_selected_engine].keyPadPage--;
      if (m_enginedefs[m_selected_engine].keyPadPage < 0)
        m_enginedefs[m_selected_engine].keyPadPage = 2;
    };
    ImGui::SameLine();
    ImGui::Button("All Aboard", ImVec2(78, 76));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("All Aboard 1\n");
      TMCCInterface::EngineDialogCommand(engineID, DialogCommandParams::STATION_SOUND_CAR_CONDUCTOR_ALL_ABOARD);
    };
    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)rightArrowIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      m_enginedefs[m_selected_engine].keyPadPage++;
      if (m_enginedefs[m_selected_engine].keyPadPage > 2)
        m_enginedefs[m_selected_engine].keyPadPage = 0;
    };

  }

  /// <summary>
  /// PAGE 2
  /// </summary>
  else if (m_enginedefs[m_selected_engine].keyPadPage == 1)
  {
    if (ImGui::ImageButton((void*)(intptr_t)volUpIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineSetMasterVolume(engineID, TMCC_ON);

    };
    Tooltip("Volume Up");

    ImGui::SameLine();

    ImGui::Button("Tickets\nPlease", ImVec2(78, 76));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("Tickets\nPlease");
      TMCCInterface::EngineDialogCommand(engineID, DialogCommandParams::STATION_SOUND_CAR_CONDUCTOR_TICKETS_PLZ);
    };

    ImGui::SameLine();

    ImGui::Button("Watch\nyour Step", ImVec2(78, 76));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("Watch youR Step");
      TMCCInterface::EngineDialogCommand(engineID, DialogCommandParams::STATION_SOUND_CAR_CONDUCTOR_WATCH_STEP);
    };


    if (ImGui::ImageButton((void*)(intptr_t)volDownIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineSetMasterVolume(engineID, TMCC_OFF);

    };

    Tooltip("Volume Down");

    ImGui::SameLine();
    ImGui::Button("PA System:\nTrain\nArrived", ImVec2(78, 76));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("PA System:\nTrain\nArrived");
      TMCCInterface::EngineDialogCommand(engineID, DialogCommandParams::STATION_SOUND_CAR_PA_TRAIN_ARRIVED);
    };

    ImGui::SameLine();

    ImGui::Button("PA System:\nTrain\nArriving", ImVec2(78, 76));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("PA System:\nTrain\nArriving");
      TMCCInterface::EngineDialogCommand(engineID, DialogCommandParams::STATION_SOUND_CAR_PA_TRAIN_ARRIVING);
    };

    ImGui::Button("PA System:\nTrain\nBoarding", ImVec2(78, 76));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("PA System:\nTrain\nBoarding");
      TMCCInterface::EngineDialogCommand(engineID, DialogCommandParams::STATION_SOUND_CAR_PA_TRAIN_BOARDING);
    };

    ImGui::SameLine();
    ImGui::Button("PA System:\nTrain\nDeparting", ImVec2(78, 76));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("PA System:\nTrain\nDeparting");
      TMCCInterface::EngineDialogCommand(engineID, DialogCommandParams::STATION_SOUND_CAR_PA_TRAIN_DEPARTING);
    };

    ImGui::SameLine();
    ImGui::Button("Steward:\nFirst\nSeating", ImVec2(78, 76));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("Steward: First Seating");
      TMCCInterface::EngineDialogCommand(engineID, DialogCommandParams::STATION_SOUND_CAR_STEWARD_FIRST_SEATING);
    };

    ImGui::Button("Steward:\nSecond\nSeating", ImVec2(78, 76));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("Steward:\nSecond\nSeating");
      TMCCInterface::EngineDialogCommand(engineID, DialogCommandParams::STATION_SOUND_CAR_STEWARD_SECOND_SEATING);
    };

    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)resetIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineNumericCommand2(engineID, 0);

    };
    Tooltip("Reset (0 key)");

    ImGui::SameLine();
    ImGui::Button("Steward:\nLounge\nOpen", ImVec2(78, 76));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("Steward:\nLounge\nOpen");
      TMCCInterface::EngineDialogCommand(engineID, DialogCommandParams::STATION_SOUND_CAR_STEWARD_LOUNGE_OPEN);
    };

    ImGui::Button("Steward:\nWelcome", ImVec2(78, 76));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("Steward:\nWelcome");
      TMCCInterface::EngineDialogCommand(engineID, DialogCommandParams::STATION_SOUND_CAR_STEWARD_WELCOME);
    };


    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)blankIcon->GetGLHandle(), ImVec2(70, 70)))
    {
    };


    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)dooropenIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      printf("Aux 3 doors");
      TMCCInterface::EngineAux3Trigger(engineID);

    };



    if (ImGui::ImageButton((void*)(intptr_t)leftArrowIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      m_enginedefs[m_selected_engine].keyPadPage--;
      if (m_enginedefs[m_selected_engine].keyPadPage < 0)
        m_enginedefs[m_selected_engine].keyPadPage = 2;
    };
    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)blankIcon->GetGLHandle(), ImVec2(70, 70)))
    {

    };

    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)rightArrowIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      m_enginedefs[m_selected_engine].keyPadPage++;
      if (m_enginedefs[m_selected_engine].keyPadPage > 2)
        m_enginedefs[m_selected_engine].keyPadPage = 0;
    };
  }
  else
  {
    DrawCAB1Keypad();
  }
};

void ThrottleMenu::DrawTMCCSteamKeypad()
{
  if (m_enginedefs[m_selected_engine].keyPadPage == 0)
  {
    if (ImGui::ImageButton((void*)(intptr_t)volUpIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineNumericCommand(engineID, 1);

    };
    Tooltip("Volume Up");

    ImGui::SameLine();

    ImGui::ImageButton((void*)(intptr_t)crewTalkIcon->GetGLHandle(), ImVec2(70, 70));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("Crewtalk\n");
      TMCCInterface::EngineNumericCommand(engineID, 2);
    };

    Tooltip("CrewTalk");

    ImGui::SameLine();

    ImGui::ImageButton((void*)(intptr_t)waterIcon->GetGLHandle(), ImVec2(70, 70));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("Water injector\n");
      TMCCInterface::EngineNumericCommand(engineID, 3);
    };

    Tooltip("Water Refill");


    if (ImGui::ImageButton((void*)(intptr_t)volDownIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineNumericCommand(engineID, 4);

    };

    Tooltip("Volume Down");

    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)shutDownIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineNumericCommand2(engineID, 5);

    };

    Tooltip("Shut Down");

    ImGui::SameLine();

    ImGui::ImageButton((void*)(intptr_t)blowdownIcon->GetGLHandle(), ImVec2(70, 70));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("Blowdown\n");
      TMCCInterface::EngineNumericCommand(engineID, 6);
    };

    Tooltip("Blowdown");


    ImGui::ImageButton((void*)(intptr_t)towerComIcon->GetGLHandle(), ImVec2(70, 70));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("Tower com\n");
      TMCCInterface::EngineNumericCommand(engineID, 7);
    };

    Tooltip("Tower Com");

    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)smokeOffIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineNumericCommand(engineID, 8);
    };

    Tooltip("Smoke Off");

    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)smokeOnIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineNumericCommand(engineID, 9);
    };
    Tooltip("Smoke On");

    ImGui::PushButtonRepeat(true);
      if (ImGui::Button("Let Off", ImVec2(78, 76)))
      {
        printf("TMCC Let off Pressed\n");
        TMCCInterface::EngineLetOffSound(engineID);
      };
      ImGui::PopButtonRepeat();
    Tooltip("TMCC Let off");

    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)resetIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineNumericCommand(engineID, 0);

    };
    Tooltip("Reset (0 key)");

    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)rule17onIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineSetRule17(engineID, TMCC_ON);

    };

    Tooltip("Rule 17 Lighting On");

    if (ImGui::ImageButton((void*)(intptr_t)tenderMarkerOffIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineSetTenderMarkerLight(engineID, TMCC_OFF);

    };

    Tooltip("Tender Marker Off");

    ImGui::SameLine();
    if (ImGui::Button("Toggle\nHorn 1/2", ImVec2(78, 76)))
    {
      printf("Toggle Horn 1 and 2\n");
      m_enginedefs[m_selected_engine].useHorn2 = !m_enginedefs[m_selected_engine].useHorn2;
    };
    Tooltip("Horn 1 is the normal TMCC horn. Horn 2 is the same sound,\nbut will not enable ditch lights on diesel locos when blown.");

    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)rule17offIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineSetRule17(engineID, TMCC_OFF);

    };

    Tooltip("Rule 17 Lighing Off");


    if (ImGui::ImageButton((void*)(intptr_t)leftArrowIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      m_enginedefs[m_selected_engine].keyPadPage--;
      if (m_enginedefs[m_selected_engine].keyPadPage < 0)
        m_enginedefs[m_selected_engine].keyPadPage = 2;
    };
    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)extshutDownIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineShutdownSequence1(engineID);
      TMCCInterface::EngineShutdownSequence2(engineID);
    };
    Tooltip("Shut Down Sequence 1 (Delayed)");
    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)rightArrowIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      m_enginedefs[m_selected_engine].keyPadPage++;
      if (m_enginedefs[m_selected_engine].keyPadPage > 2)
        m_enginedefs[m_selected_engine].keyPadPage = 0;
    };

  }

  /// <summary>
  /// PAGE 2
  /// </summary>
  else if (m_enginedefs[m_selected_engine].keyPadPage == 1)
  {
    if (ImGui::ImageButton((void*)(intptr_t)volUpIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineSetMasterVolume(engineID, TMCC_ON);

    };
    Tooltip("Volume Up");

    ImGui::SameLine();

    ImGui::ImageButton((void*)(intptr_t)coalIcon->GetGLHandle(), ImVec2(70, 70));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("Coal refuel\n");
      TMCCInterface::EngineLocoRefuelSound(engineID);
    };

    Tooltip("Coal refuel");

    ImGui::SameLine();
    ImGui::PushButtonRepeat(true);
    if (ImGui::Button("Enable\nCylinder\Cocks", ImVec2(78, 76)))
    {
      TMCCInterface::EngineToggleCylinderCock(engineID, TMCC_ON);
    };
    ImGui::PopButtonRepeat();
    Tooltip("Enable\nCylinder\Cocks");


    if (ImGui::ImageButton((void*)(intptr_t)volDownIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineSetMasterVolume(engineID, TMCC_OFF);

    };

    Tooltip("Volume Down");

    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)blankIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineNumericCommand2(engineID, 5);

    };

    Tooltip("Shut Down");

    ImGui::SameLine();

    ImGui::PushButtonRepeat(true);
    if (ImGui::Button("Disable\nCylinder\Cocks", ImVec2(78, 76)))
    {
      TMCCInterface::EngineToggleCylinderCock(engineID, TMCC_OFF);
    };
    ImGui::PopButtonRepeat();
    Tooltip("Disable\nCylinder\Cocks");


    if (ImGui::ImageButton((void*)(intptr_t)blankIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineNumericCommand2(engineID, 7);

    };


    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)blankIcon->GetGLHandle(), ImVec2(70, 70)))
    {

    };


    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)blankIcon->GetGLHandle(), ImVec2(70, 70)))
    {

    };

    if (ImGui::ImageButton((void*)(intptr_t)blankIcon->GetGLHandle(), ImVec2(70, 70)))
    {
    };

    ImGui::SameLine();

    ImGui::ImageButton((void*)(intptr_t)resetIcon->GetGLHandle(), ImVec2(70, 70));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("Reset\n");
      TMCCInterface::EngineNumericCommand(engineID, 0);
    };
    Tooltip("Reset (0 key)");

    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)blankIcon->GetGLHandle(), ImVec2(70, 70)))
    {

    };


    if (ImGui::ImageButton((void*)(intptr_t)blankIcon->GetGLHandle(), ImVec2(70, 70)))
    {

    };


    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)extstartUpIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineStartupSequence2(engineID);
    };
    Tooltip("Immediate Start Up");


    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)blankIcon->GetGLHandle(), ImVec2(70, 70)))
    {

    };



    if (ImGui::ImageButton((void*)(intptr_t)leftArrowIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      m_enginedefs[m_selected_engine].keyPadPage--;
      if (m_enginedefs[m_selected_engine].keyPadPage < 0)
        m_enginedefs[m_selected_engine].keyPadPage = 2;
    };
    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)extshutDownIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineShutdownSequence2(engineID);

    };
    Tooltip("Immediate Shut Down");

    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)rightArrowIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      m_enginedefs[m_selected_engine].keyPadPage++;
      if (m_enginedefs[m_selected_engine].keyPadPage > 2)
        m_enginedefs[m_selected_engine].keyPadPage = 0;
    };
  }
  else
  {
    DrawCAB1Keypad();
  }
}

void ThrottleMenu::DrawTMCCAcelaKeypad(void)
{
  if (m_enginedefs[m_selected_engine].keyPadPage == 0)
  {
    if (ImGui::ImageButton((void*)(intptr_t)volUpIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineNumericCommand(engineID, 1);
    };
    Tooltip("Volume Up");

    ImGui::SameLine();

    ImGui::ImageButton((void*)(intptr_t)conductorAnnouncementIcon->GetGLHandle(), ImVec2(70, 70));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("Acela Announcement\n");
      TMCCInterface::EngineNumericCommand(engineID, 2);
    };

    Tooltip("CrewTalk");

    ImGui::SameLine();

    ImGui::ImageButton((void*)(intptr_t)waterIcon->GetGLHandle(), ImVec2(70, 70));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("Water injector\n");
      TMCCInterface::EngineNumericCommand(engineID, 3);
    };

    Tooltip("Water Refill");


    if (ImGui::ImageButton((void*)(intptr_t)volDownIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineNumericCommand(engineID, 4);

    };

    Tooltip("Volume Down");

    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)shutDownIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineNumericCommand(engineID, 5);

    };

    Tooltip("Shut Down");

    ImGui::SameLine();

    ImGui::ImageButton((void*)(intptr_t)blowdownIcon->GetGLHandle(), ImVec2(70, 70));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("Blowdown\n");
      TMCCInterface::EngineNumericCommand(engineID, 6);
    };

    Tooltip("Blowdown");


    ImGui::ImageButton((void*)(intptr_t)towerComIcon->GetGLHandle(), ImVec2(70, 70));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("Tower com\n");
      TMCCInterface::EngineNumericCommand(engineID, 7);
    };

    Tooltip("Tower Com");

    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)lightOffIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineNumericCommand(engineID, 8);
    };

    Tooltip("Cab Light Off");

    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)lightOnIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineNumericCommand(engineID, 9);
    };
    Tooltip("Cab Light On");

    if (ImGui::ImageButton((void*)(intptr_t)dooropenIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineAux1Option1(engineID);
      TMCCInterface::EngineOpenRearCoupler(engineID);
    };
    Tooltip("Open Right Doors");

    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)resetIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineNumericCommand(engineID, 0);

    };
    Tooltip("Reset (0 key)");

    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)dooropenIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineAux1Option1(engineID);
      TMCCInterface::EngineOpenFrontCoupler(engineID);
    };
    Tooltip("Open Left Doors");

    if (ImGui::ImageButton((void*)(intptr_t)doorcloseIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineAux2Option1(engineID);
      TMCCInterface::EngineOpenRearCoupler(engineID);
    };
    Tooltip("Close Right Doors");

    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)panto_front_upIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineAux1Option1(engineID);
      TMCCInterface::EngineNumericCommand(engineID, 6);
    };
    Tooltip("Cycle Pantograph Position");

    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)doorcloseIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineAux2Option1(engineID);
      TMCCInterface::EngineOpenFrontCoupler(engineID);
    };
    Tooltip("Close Left Doors");


    if (ImGui::ImageButton((void*)(intptr_t)leftArrowIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      m_enginedefs[m_selected_engine].keyPadPage--;
      if (m_enginedefs[m_selected_engine].keyPadPage < 0)
        m_enginedefs[m_selected_engine].keyPadPage = 2;
    };
    ImGui::SameLine();
    if (ImGui::Button("Toggle\nHorn 1/2", ImVec2(78, 76)))
    {
      printf("Toggle Horn 1 and 2\n");
      m_enginedefs[m_selected_engine].useHorn2 = !m_enginedefs[m_selected_engine].useHorn2;
    };
    Tooltip("Horn 1 is the normal TMCC horn. Horn 2 is the same sound,\nbut will not enable ditch lights on diesel locos when blown.");
    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)rightArrowIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      m_enginedefs[m_selected_engine].keyPadPage++;
      if (m_enginedefs[m_selected_engine].keyPadPage > 2)
        m_enginedefs[m_selected_engine].keyPadPage = 0;
    };

  }

  /// <summary>
  /// Page 2
  /// </summary>
  else if (m_enginedefs[m_selected_engine].keyPadPage == 1)
  {
  if (ImGui::ImageButton((void*)(intptr_t)volUpIcon->GetGLHandle(), ImVec2(70, 70)))
  {
    TMCCInterface::EngineNumericCommand(engineID, 1);
  };
  Tooltip("Volume Up");

  ImGui::SameLine();

  if (ImGui::ImageButton((void*)(intptr_t)acelaDepartIcon->GetGLHandle(), ImVec2(70, 70)))
  {
    TMCCInterface::EngineNumericCommand(engineID, 2);
  };

  Tooltip("Acela Departure");

  ImGui::SameLine();

  if (ImGui::ImageButton((void*)(intptr_t)blankIcon->GetGLHandle(), ImVec2(70, 70)))
  {

  };


  if (ImGui::ImageButton((void*)(intptr_t)volDownIcon->GetGLHandle(), ImVec2(70, 70)))
  {
    TMCCInterface::EngineNumericCommand(engineID, 3);

  };

  Tooltip("Volume Down");

  ImGui::SameLine();
  if (ImGui::ImageButton((void*)(intptr_t)acelaArriveIcon->GetGLHandle(), ImVec2(70, 70)))
  {
    TMCCInterface::EngineAux1Option1(engineID);
    TMCCInterface::EngineNumericCommand(engineID, 2);
  };

  Tooltip("Acela Arrival");

  ImGui::SameLine();

  if (ImGui::ImageButton((void*)(intptr_t)blankIcon->GetGLHandle(), ImVec2(70, 70)))
  {

  };


  if (ImGui::ImageButton((void*)(intptr_t)blankIcon->GetGLHandle(), ImVec2(70, 70)))
  {

  };

  ImGui::SameLine();
  if (ImGui::ImageButton((void*)(intptr_t)blankIcon->GetGLHandle(), ImVec2(70, 70)))
  {

  };

  ImGui::SameLine();
  if (ImGui::ImageButton((void*)(intptr_t)blankIcon->GetGLHandle(), ImVec2(70, 70)))
  {

  };

  if (ImGui::ImageButton((void*)(intptr_t)blankIcon->GetGLHandle(), ImVec2(70, 70)))
  {

  };

  ImGui::SameLine();
  if (ImGui::ImageButton((void*)(intptr_t)resetIcon->GetGLHandle(), ImVec2(70, 70)))
  {
    TMCCInterface::EngineNumericCommand(engineID, 0);

  };
  Tooltip("Reset (0 key)");

  ImGui::SameLine();
  if (ImGui::ImageButton((void*)(intptr_t)blankIcon->GetGLHandle(), ImVec2(70, 70)))
  {

  };

  if (ImGui::ImageButton((void*)(intptr_t)blankIcon->GetGLHandle(), ImVec2(70, 70)))
  {

  };

  ImGui::SameLine();
  if (ImGui::ImageButton((void*)(intptr_t)blankIcon->GetGLHandle(), ImVec2(70, 70)))
  {

  };

  ImGui::SameLine();
  if (ImGui::ImageButton((void*)(intptr_t)blankIcon->GetGLHandle(), ImVec2(70, 70)))
  {

  };


  if (ImGui::ImageButton((void*)(intptr_t)leftArrowIcon->GetGLHandle(), ImVec2(70, 70)))
  {
    m_enginedefs[m_selected_engine].keyPadPage--;
    if (m_enginedefs[m_selected_engine].keyPadPage < 0)
      m_enginedefs[m_selected_engine].keyPadPage = 2;
  };
  ImGui::SameLine();
  if (ImGui::ImageButton((void*)(intptr_t)blankIcon->GetGLHandle(), ImVec2(70, 70)))
  {

  };
  ImGui::SameLine();
  if (ImGui::ImageButton((void*)(intptr_t)rightArrowIcon->GetGLHandle(), ImVec2(70, 70)))
  {
    m_enginedefs[m_selected_engine].keyPadPage++;
    if (m_enginedefs[m_selected_engine].keyPadPage > 2)
      m_enginedefs[m_selected_engine].keyPadPage = 0;
  };
  }
  else
  {
    DrawCAB1Keypad();
  }
}

void ThrottleMenu::DrawKeypadType(int currentKeypadType, bool isLegacy, int engineType)
{
  if (isLegacy) // Legacy Locos
  {
    switch (engineType)
    {
    case EngineTypeLegacy::ENGINE_TYPE_STEAM:
    default:
      DrawCAB2SteamKeypad();
      break;
    
    case EngineTypeLegacy::ENGINE_TYPE_DIESEL:
      DrawCAB2SteamKeypad();
      break;

    case EngineTypeLegacy::ENGINE_TYPE_ELECTRIC:
      DrawCAB2ElectricKeypad();
      break;
    case EngineTypeLegacy::ENGINE_TYPE_STATIONSOUND_CAR:
      DrawStationDinerKeypad();
      break;
    case EngineTypeLegacy::ENGINE_TYPE_STOCK_CAR:
      DrawCAB2SteamKeypad();
      break;
    case EngineTypeLegacy::ENGINE_TYPE_SUBWAY:
      DrawCAB2SteamKeypad();
      break;
    }
    
  }

  else // TMCC Locos
  {
    switch (engineType)
    {
    case EngineTypeTMCC::ENGINE_TYPE_TMCC_STEAM:
      DrawTMCCSteamKeypad();
      break;
    case EngineTypeTMCC::ENGINE_TYPE_TMCC_ACELA:
      DrawTMCCAcelaKeypad();
      break;
    default:
      DrawCAB1Keypad();
      break;
    }
  }
}


