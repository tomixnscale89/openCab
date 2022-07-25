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
}

ThrottleMenu::~ThrottleMenu()
{

}


void ThrottleMenu::Draw()
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


  DrawKeypad();

  if (ImGui::InputText("ITEM: InputText", &engineIDStr))
  {
    engineID = std::stoi(engineIDStr);
    printf("engineID: %d\n", (int)engineID);
  }

  if (ImGui::Button("Whistle", ImVec2(80, 60)))
  {

  };
  ImGui::SameLine();
  if (ImGui::Button("Bell", ImVec2(80, 60)))
  {

  };
  ImGui::SameLine();
  if (ImGui::Button("Aux 1", ImVec2(80, 60)))
  {

  };
  ImGui::SameLine();
  if (ImGui::Button("Aux 2", ImVec2(80, 60)))
  {

  };
  ImGui::SameLine();
  if (ImGui::Button("Aux 3", ImVec2(80, 60)))
  {

  };
  ImGui::SameLine();
  if (ImGui::Button("TMCC/Legacy Mode", ImVec2(80, 60)))
  {
    legacyEnabled = !legacyEnabled;
  };


  if (ImGuiKnobs::KnobInt("Throttle", &speed, 0, 200, 1, "%dMPH", ImGuiKnobVariant_Wiper, 128, 0, 1)) {
    // value was changed
  }


  if(ImGui::VSliderInt("##v", ImVec2(40, 160), &currentTrainBrake, 0, 8, "%.2f\nsec"));


  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  ImGui::End();
}

void ThrottleMenu::DrawKeypad()
{
  if (ImGui::BeginTable("split", 3))
  {
    ImGui::TableNextColumn();
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

    ImGui::TableNextRow();


    ImGui::TableNextColumn();
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
    ImGui::TableNextRow();

    ImGui::TableNextColumn();
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
    ImGui::TableNextRow();

    ImGui::TableNextColumn();
    if (ImGui::InvisibleButton("NULL", ImVec2(80, 80)))
    {

    };
    ImGui::SameLine();
    if (ImGui::Button("0", ImVec2(80, 80)))
    {

    };
    ImGui::SameLine();
    if (ImGui::InvisibleButton("NULL", ImVec2(80, 80)))
    {

    };

    ImGui::EndTable();
  }


}