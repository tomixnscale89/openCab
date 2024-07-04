#include "cab1menu.h"
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
#include "KeypadTypes.h"

void ThrottleMenu::CAB1Window(bool* p_open, float curTime)
{
  //const auto& engine = m_enginedefs[m_selected_engine];
  ImGui::SetNextWindowSize(ImVec2(512, 900), ImGuiCond_FirstUseEver);
  if (ImGui::Begin("CAB 1 Menu", p_open, ImGuiWindowFlags_MenuBar))
  {
    if (ImGui::BeginTable("split", 4))
    {
      ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 80.0f);
      ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 256.0f);
      ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 256.0f);
      ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 80.0f);

      ImGui::TableNextColumn();

      if (ImGui::Button("SW", ImVec2(78, 76)))
      {
        printf("Switch Starter TMCC1\n");
        objectIDByte = (std::byte)COMMAND_STARTING_BIT_TYPE::SWITCH_COMMAND;
      };

      ImGui::SameLine();

      if (ImGui::Button("ACC", ImVec2(78, 76)))
      {
        printf("ACC Starter TMCC1\n");
        objectIDByte = (std::byte)COMMAND_STARTING_BIT_TYPE::ACCESSORY_COMMAND;
      };

      ImGui::SameLine();

      if (ImGui::Button("RTE", ImVec2(78, 76)))
      {
        printf("Route Starter TMCC1\n");
        objectIDByte = (std::byte)COMMAND_STARTING_BIT_TYPE::ROUTE_COMMAND;
      };

      ImGui::SameLine();

      if (ImGui::Button("TR", ImVec2(78, 76)))
      {
        printf("Train Starter TMCC1\n");
        objectIDByte = (std::byte)COMMAND_STARTING_BIT_TYPE::TRAIN_COMMAND;
      };

      ImGui::SameLine();

      if (ImGui::Button("ENG", ImVec2(78, 76)))
      {
        printf("Train Starter TMCC1\n");
        objectIDByte = (std::byte)COMMAND_STARTING_BIT_TYPE::ENGINE_COMMAND;
      };

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
      //ImGui::PushButtonRepeat(true);
      //if (ImGui::ImageButton((void*)(intptr_t)letOffIcon->GetGLHandle(), ImVec2(70, 70))) {
      //  {
      //  printf("TMCC Let off Pressed\n");
      //  TMCCInterface::EngineLetOffSound(engineID);
      //};
      //ImGui::PopButtonRepeat();

      // Boost  and brake
      ImGui::PushButtonRepeat(true);
      if (ImGui::Button("Boost", ImVec2(78, 76)))
      {
        printf("Boost TMCC1\n");
        TMCCInterface::EngineBoostSpeed(engineID);
      };
      ImGui::PopButtonRepeat();

      ImGui::PushButtonRepeat(true);
      if (ImGui::Button("Brake", ImVec2(78, 76)))
      {
        printf("Brake TMCC1\n");
        TMCCInterface::EngineBrakeSpeed(engineID);
      };
      ImGui::PopButtonRepeat();


      ImGui::TableNextColumn();
      ImGui::SetNextItemWidth(50);


      DrawCAB1Keypad(engineID);
      // IF STATIONSOUND DINER OR STOCK CAR, DON'T SHOW THESE









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

      // IF STATIONSOUND DINER OR STOCK CAR, DON'T SHOW THESE
      if (m_enginedefs[m_selected_engine].engineType != EngineType::ENGINE_STATIONSOUND_CAR && m_enginedefs[m_selected_engine].engineType != EngineType::ENGINE_STOCK_CAR)
      {
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
            {
              m_enginedefs[m_selected_engine].steam_labour_intensity--;
              if (m_enginedefs[m_selected_engine].steam_labour_intensity < 0)
                m_enginedefs[m_selected_engine].steam_labour_intensity = 0;
            }

            else
            {
              m_enginedefs[m_selected_engine].diesel_electric_rev_lvl--;
              if (m_enginedefs[m_selected_engine].diesel_electric_rev_lvl < 0)
                m_enginedefs[m_selected_engine].diesel_electric_rev_lvl = 0;
            }

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
            {
              m_enginedefs[m_selected_engine].steam_labour_intensity++;
              if (m_enginedefs[m_selected_engine].steam_labour_intensity > 31)
                m_enginedefs[m_selected_engine].steam_labour_intensity = 31;
            }

            else
            {
              m_enginedefs[m_selected_engine].diesel_electric_rev_lvl++;
              if (m_enginedefs[m_selected_engine].diesel_electric_rev_lvl > 8)
                m_enginedefs[m_selected_engine].diesel_electric_rev_lvl = 8;
            }
          };
          ImGui::PopButtonRepeat();
        }
      }




      //ImGui::Image();

      ImGui::EndTable();
    }
    ImGui::End();
  }
};