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


void ThrottleMenu::Draw()
{

  // Menu Bar
  if (ImGui::BeginMenuBar())
  {
    if (ImGui::BeginMenu("Menu"))
    {

      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Engine Roster"))
    {

      ImGui::EndMenu();
    }
    //if (ImGui::MenuItem("MenuItem")) {} // You can also use MenuItem() inside a menu bar!
    if (ImGui::BeginMenu("Feature Windows"))
    {
      ImGui::MenuItem("CrewTalk/TowerCom Voice Testing", NULL, &test);
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

  if (ImGui::InputText("ITEM: InputText", &engineIDStr))
  {
    engineID = std::stoi(engineIDStr);
    printf("engineID: %d\n", (int)engineID);
  }

  //static int value = 0;
  static float f = 0.0f;
  static int counter = 0;
  bool show_demo_window = true;
  if (show_demo_window)
    ImGui::ShowDemoWindow(&show_demo_window);

  //ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

  //ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
  if (ImGui::Checkbox("Demo Window", &show_demo_window))
  {

  }      // Edit bools storing our window open/close state
  //ImGui::Checkbox("Another Window", &show_another_window);

  //ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
  //ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color


  //ImGui::Text("counter = %d", counter);
  //ImGui::ProgressBar(speed/200, ImVec2(0.0f, 0.0f));
  if (ImGui::BeginTable("split", 4))
  {
    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 100.0f);
    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 256.0f);
    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 256.0f);
    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 100.0f);

    ImGui::TableNextColumn();
    // Train Brake
    if (ImGui::VSliderInt("##int", ImVec2(70, 250), &currentTrainBrake, 8, 0, "Train\nBrake"))
    {

    };

    ImGui::TableNextColumn();
    ImGui::SetNextItemWidth(50);

    DrawKeypadType(currentKeypadStyle);

    ImGui::TableNextColumn();
    if (ImGuiKnobs::KnobInt("Throttle", &speed, 0, 200, 1, "%d MPH", ImGuiKnobVariant_Wiper, 256)) {
      // value was changed
      TMCCInterface::EngineSetAbsoluteSpeed2(engineID, speed);
    }

    if (ImGui::Button("Forward", ImVec2(80, 60)))
    {
      printf("Front\n");

      TMCCInterface::EngineSetDirection(engineID, TMCC_FORWARD);
    };

    ImGui::SameLine();

    if (ImGui::Button("Bell", ImVec2(80, 60)))
    {
      if (consistentBellDing)
      {
        printf("Bell Toggle\n");
        TMCCInterface::EngineSetBell(engineID, TMCC_ON);
      }
      else
      {
        printf("One Shot Bell Ding: %d\n",bellDingCount);
        TMCCInterface::EngineBellOneShotDing(engineID, bellDingCount);
      }
    };

    ImGui::SameLine();
    if (ImGui::Button("Reverse", ImVec2(80, 60)))
    {
      printf("Reverse\n");
      TMCCInterface::EngineSetDirection(engineID, TMCC_REVERSE);

    };

    if (ImGui::Button("Rear\nCoupler", ImVec2(80, 60)))
    {
      printf("Fire Rear coupler\n");
      TMCCInterface::EngineOpenRearCoupler(engineID);
    };

    ImGui::SameLine();

    ImGui::Checkbox("Bell Type", &consistentBellDing);


    ImGui::SameLine();
    if (ImGui::Button("Front\nCoupler", ImVec2(80, 60)))
    {
      printf("Fire Front coupler\n");

      TMCCInterface::EngineOpenRearCoupler(engineID);

    };

    if (ImGui::ArrowButton("##left", ImGuiDir_Left)) { bellDingCount--; }
    if (bellDingCount < 0)
      bellDingCount = 3;
    ImGui::SameLine();
    ImGui::Text("Times to Ding:%d", bellDingCount);
    ImGui::SameLine();
    if (ImGui::ArrowButton("##right", ImGuiDir_Right)) { bellDingCount++; }
    if (bellDingCount > 3)
      bellDingCount = 0;

    ImGui::TableNextColumn();

    // Whistle
    if (ImGui::VSliderInt("##int2", ImVec2(70, 250), &currentQuill, 8, 0, "Whistle"))
    {
      printf("Quilling: %d\n", currentQuill);
      TMCCInterface::EngineSetQuillingHornIntensity(engineID, currentQuill);
    };
    
    if (ImGuiKnobs::KnobInt("Labour/Rev", &currentEngineLabour, 0, 31, 1, "%f Labour", ImGuiKnobVariant_Wiper, 100)) {
      // value was changed
      // if steam, use labour
      // else, use diesel run level
      TMCCInterface::EngineSetLabor(engineID, currentEngineLabour);
    }
;
    ImGui::EndTable();
  }

  


  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  
  ImGui::PushButtonRepeat(true);
  if (ImGui::Button("Aux 1", ImVec2(80, 60)))
  {
    printf("Aux 1: Pressed\n");
    TMCCInterface::EngineSetQuillingHornIntensity(engineID, currentQuill);
  };
  ImGui::PopButtonRepeat();
  ImGui::SameLine();

  ImGui::PushButtonRepeat(true);
  if (ImGui::Button("Aux 2", ImVec2(80, 60)))
  {
    printf("Aux 2: Pressed\n");
  };
  ImGui::PopButtonRepeat();
  ImGui::SameLine();
  ImGui::PushButtonRepeat(true);

  if (ImGui::Button("Aux 3", ImVec2(80, 60)))
  {
    printf("Aux 3: Pressed\n");
    TMCCInterface::EngineAux3Trigger(engineID);
  };
  ImGui::PopButtonRepeat();

  ImGui::SameLine();
  if (ImGui::Button("TMCC/Legacy Mode", ImVec2(80, 60)))
  {
    printf("Set control mode %s\n", legacyEnabled ? "legacy" : "standard");
    legacyEnabled = !legacyEnabled;
  };


  
  
  ImGui::End();
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
