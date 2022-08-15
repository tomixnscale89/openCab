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


ThrottleMenu::ThrottleMenu()
{
  speed = 0;
  currentTrainBrake = 0;
  currentQuill = 0;
  engineID = 1;
}

ThrottleMenu::~ThrottleMenu()
{

}



void ThrottleMenu::Draw(const std::string& appDir)
{

  float curTime = (float)clock() / CLOCKS_PER_SEC;

  if (voiceClipMenuVisible)              ShowVoiceWindow(&voiceClipMenuVisible);
  if (addEngineMenuVisible)              AddEngineWindow(&addEngineMenuVisible, appDir);

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
      ImGui::MenuItem("CrewTalk/TowerCom Voice Testing", NULL, &voiceClipMenuVisible);
      ImGui::MenuItem("Lighting Menu", NULL, &test);
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


    // second argument is the text hint for the current item
    //if (ImGui::BeginCombo("Engine", m_enginedefs[m_selected_engine].name.c_str()))
    //{
    //  // combobox has been clicked, list the options
    //  for (int i = 0; i < m_enginedefs.size(); i++)
    //  {
    //    // first argument is the option name, second argument is whether to display this item as selected
    //    if (ImGui::Selectable(m_enginedefs[i].name.c_str(), m_selected_engine == i))
    //    {
    //      // combobox item has been clicked, set the new selected engine
    //      m_selected_engine = i;
    //    }
    //  }
    //  ImGui::EndCombo();
    //}

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
      json test;
      if (newEngineisLegacy)
        newEngineType = engineTypesLegacy[type_index].engineType;
      else
        newEngineType = engineTypes[type_index].engineType;
      printf("Engine ID: %d, EngineName: %s, engineType: %d, Legacy: %d\n", newEngineID, newEngineName.c_str(), newEngineType, newEngineisLegacy);
      printf("%s\n", appDir.c_str());
      //EngineManagement::AddEngineToJson(test, newEngineID, newEngineType, newEngineisLegacy, appDir.c_str());
      AddEngineDataToJson(test, newEngineID, newEngineName, newEngineType, newEngineisLegacy, appDir.c_str());
    }


    ImGui::End();

  }
};

void ThrottleMenu::AddEngineDataToJson(json& engineRoster, int engineID, std::string engineName, int currentEngineType, bool isLegacy, const std::string& appDir)
{
  std::vector<json> engines;
  for (const auto& engine : m_enginedefs)
  {
    json enginedata;
    enginedata["engineID"] = engineID;
    enginedata["engineName"] = engineName;
    enginedata["engineType"] = currentEngineType;
    enginedata["isLegacy"] = isLegacy;
    engines.push_back(enginedata);
  }
  engineRoster["engines"] = engines;
  //engineRoster["engines"] = engines;
  EngineManagement::WriteEngineRoster(engineRoster, appDir);

}

void ThrottleMenu::ThrottleWindow(bool* p_open, float curTime)
{
  if (ImGui::BeginTable("split", 4))
  {
    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 80.0f);
    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 256.0f);
    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 256.0f);
    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 80.0f);

    ImGui::TableNextColumn();
    // Train Brake
    if (ImGui::VSliderInt("##int", ImVec2(80, 250), &currentTrainBrake, 8, 0, "Train\nBrake"))
    {

    };

    if (ImGui::Button("Forward", ImVec2(80, 60)))
    {
      printf("Front\n");

      TMCCInterface::EngineSetDirection(engineID, TMCC_FORWARD);
    };

    if (ImGui::Button("Front\nCoupler", ImVec2(80, 60)))
    {
      printf("Fire Front coupler\n");

      TMCCInterface::EngineOpenRearCoupler(engineID);

    };

    ImGui::PushButtonRepeat(true);
    if (ImGui::Button("Aux 1", ImVec2(80, 60)))
    {
      printf("Aux 1: Pressed\n");
      TMCCInterface::EngineAux1Option1(engineID);
    };
    ImGui::PopButtonRepeat();

    ImGui::PushButtonRepeat(true);
    if (ImGui::Button("Aux 2", ImVec2(80, 60)))
    {
      printf("Aux 2: Pressed\n");
      TMCCInterface::EngineAux2Option1(engineID);
    };
    ImGui::PopButtonRepeat();
    ImGui::PushButtonRepeat(true);

    if (ImGui::Button("Aux 3", ImVec2(80, 60)))
    {
      printf("Aux 3: Pressed\n");
      TMCCInterface::EngineAux3Trigger(engineID);
    };
    ImGui::PopButtonRepeat();

    ImGui::TableNextColumn();
    ImGui::SetNextItemWidth(50);

    DrawKeypadType(currentKeypadStyle);
    if (ImGuiKnobs::KnobInt("Throttle", &speed, 0, 200, 1, "%d MPH", ImGuiKnobVariant_Wiper, 256)) {
      // value was changed
      TMCCInterface::EngineSetAbsoluteSpeed2(engineID, speed);
    }

    ImGui::Text("One Shot Bell Enabled:");
    ImGui::SameLine();
    ImGui::Checkbox("##OneShotBellEnabler", &consistentBellDing);

    if (ImGui::ArrowButton("##left", ImGuiDir_Left)) { bellDingCount--; }
    if (bellDingCount < 1)
      bellDingCount = 3;
    ImGui::SameLine();
    ImGui::Text("One Shot Bell Volume:%d", bellDingCount);
    ImGui::SameLine();
    if (ImGui::ArrowButton("##right", ImGuiDir_Right)) { bellDingCount++; }
    if (bellDingCount > 3)
      bellDingCount = 1;

    ImGui::TableNextColumn();
        ImGui::SetNextItemWidth(50);

    // Whistle
    if (ImGui::VSliderInt("##int2", ImVec2(80, 250), &currentQuill, 8, 0, "Whistle"))
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
    
    if (ImGui::Button("Reverse", ImVec2(80, 60)))
    {
      printf("Reverse\n");
      TMCCInterface::EngineSetDirection(engineID, TMCC_REVERSE);

    };


    if (ImGui::Button("Bell", ImVec2(80, 60)))
    {
      if (consistentBellDing)
      {
        printf("Bell Toggle\n");
        bellOn = !bellOn;
        if (bellOn)
          TMCCInterface::EngineSetBell(engineID, TMCC_ON);
        else
          TMCCInterface::EngineSetBell(engineID, TMCC_OFF);
      }
      else
      {
        printf("One Shot Bell Ding: %d\n", bellDingCount);
        TMCCInterface::EngineBellOneShotDing(engineID, bellDingCount);
      }
    };
    

    if (ImGui::Button("Rear\nCoupler", ImVec2(80, 60)))
    {
      printf("Fire Rear coupler\n");
      TMCCInterface::EngineOpenRearCoupler(engineID);
    };

    if (ImGuiKnobs::KnobInt("Labour/Rev", &currentEngineLabour, 0, 31, 1, "%f Labour", ImGuiKnobVariant_Wiper, 100)) {
      // value was changed
      // if steam, use labour
      // else, use diesel run level
      TMCCInterface::EngineSetLabor(engineID, currentEngineLabour);
    };
    




    


    ImGui::EndTable();
  }
}

void ThrottleMenu::ShowVoiceWindow(bool* p_open)
{
  ImGui::SetNextWindowSize(ImVec2(480, 574), ImGuiCond_FirstUseEver);
  if (ImGui::Begin("CrewTalk/TowerCom Voice Testing", p_open, ImGuiWindowFlags_MenuBar))
  {
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
      TMCCInterface::EngineDialogCommand(engineID, dialog_map[m_dialog_index].value);
    };

    ImGui::End();
  }
}

void ThrottleMenu::PlayWhistle(bool enabled , float curTime, int currentQuill, int engineID)
{
  float quillInterval = 0.2f;
  if (enabled)
  {
    if (curTime - ThrottleMenu::lastQuillTime >= quillInterval)
    {
      ThrottleMenu::lastQuillTime = curTime;
      printf("Quilling: %d\n", currentQuill, lastQuillTime);
      TMCCInterface::EngineSetQuillingHornIntensity(engineID, currentQuill);
    }
  }
  
}


void ThrottleMenu::DrawCAB1Keypad()
{
  if (ImGui::Button("1", ImVec2(80, 80)))
  {

  };
  ImGui::SameLine();
  if (ImGui::Button("2", ImVec2(80, 80)))
  {

  };
  ImGui::SameLine();
  if (ImGui::Button("3", ImVec2(80, 80)))
  {

  };


  if (ImGui::Button("4", ImVec2(80, 80)))
  {

  };
  ImGui::SameLine();
  if (ImGui::Button("5", ImVec2(80, 80)))
  {

  };
  ImGui::SameLine();
  if (ImGui::Button("6", ImVec2(80, 80)))
  {

  };

  if (ImGui::Button("7", ImVec2(80, 80)))
  {

  };
  ImGui::SameLine();
  if (ImGui::Button("8", ImVec2(80, 80)))
  {

  };
  ImGui::SameLine();
  if (ImGui::Button("9", ImVec2(80, 80)))
  {

  };

  if (ImGui::Button("<-", ImVec2(80, 80)))
  {
    currentKeypadStyle--;
    if (currentKeypadStyle < 0)
      currentKeypadStyle = 7;
  };
  ImGui::SameLine();
  if (ImGui::Button("0", ImVec2(80, 80)))
  {

  };
  ImGui::SameLine();
  if (ImGui::Button("->", ImVec2(80, 80)))
  {
    currentKeypadStyle++;
    if (currentKeypadStyle > 7)
      currentKeypadStyle = 0;
  };

}

void ThrottleMenu::DrawTMCCKeypad()
{
    if (ImGui::Button("1", ImVec2(80, 80)))
    {

    };
    ImGui::SameLine();
    if (ImGui::Button("2", ImVec2(80, 80)))
    {

    };
    ImGui::SameLine();
    if (ImGui::Button("3", ImVec2(80, 80)))
    {

    };


    if (ImGui::Button("4", ImVec2(80, 80)))
    {

    };
    ImGui::SameLine();
    if (ImGui::Button("5", ImVec2(80, 80)))
    {

    };
    ImGui::SameLine();
    if (ImGui::Button("6", ImVec2(80, 80)))
    {

    };

    if (ImGui::Button("7", ImVec2(80, 80)))
    {

    };
    ImGui::SameLine();
    if (ImGui::Button("8", ImVec2(80, 80)))
    {

    };
    ImGui::SameLine();
    if (ImGui::Button("9", ImVec2(80, 80)))
    {

    };

    if (ImGui::Button("<-", ImVec2(80, 80)))
    {
      currentKeypadStyle--;
      if (currentKeypadStyle < 0)
        currentKeypadStyle = 7;
    };
    ImGui::SameLine();
    if (ImGui::Button("0", ImVec2(80, 80)))
    {

    };
    ImGui::SameLine();
    if (ImGui::Button("->", ImVec2(80, 80)))
    {
      currentKeypadStyle++;
      if (currentKeypadStyle > 7)
        currentKeypadStyle = 0;
    };

  }

void ThrottleMenu::DrawCAB2SteamKeypad()
{
  if (ImGui::Button("1", ImVec2(80, 80)))
  {

  };
  ImGui::SameLine();
  if (ImGui::Button("666", ImVec2(80, 80)))
  {

  };
  ImGui::SameLine();
  if (ImGui::Button("3", ImVec2(80, 80)))
  {

  };


  if (ImGui::Button("4", ImVec2(80, 80)))
  {

  };
  ImGui::SameLine();
  if (ImGui::Button("5", ImVec2(80, 80)))
  {

  };
  ImGui::SameLine();
  if (ImGui::Button("6", ImVec2(80, 80)))
  {

  };

  if (ImGui::Button("7", ImVec2(80, 80)))
  {

  };
  ImGui::SameLine();
  if (ImGui::Button("8", ImVec2(80, 80)))
  {

  };
  ImGui::SameLine();
  if (ImGui::Button("9", ImVec2(80, 80)))
  {

  };

  if (ImGui::Button("", ImVec2(80, 80)))
  {

  };
  ImGui::SameLine();
  if (ImGui::Button("0", ImVec2(80, 80)))
  {

  };
  ImGui::SameLine();
  if (ImGui::Button("", ImVec2(80, 80)))
  {

  };

  if (ImGui::Button("<-", ImVec2(80, 80)))
  {
    currentKeypadStyle--;
    if (currentKeypadStyle < 0)
      currentKeypadStyle = 7;
  };
  ImGui::SameLine();
  if (ImGui::Button("", ImVec2(80, 80)))
  {
    
  };
  ImGui::SameLine();
  if (ImGui::Button("->", ImVec2(80, 80)))
  {
    currentKeypadStyle++;
    if (currentKeypadStyle > 7)
      currentKeypadStyle = 0;
  };
}

void ThrottleMenu::DrawKeypadType(int currentKeypadType)
{
  switch (currentKeypadType)
  {
  case 0:
  default:
    DrawCAB1Keypad();
    break;
  case 1:
    DrawTMCCKeypad();
    break;
  case 2:
    DrawCAB2SteamKeypad();
    break;
  }
}
