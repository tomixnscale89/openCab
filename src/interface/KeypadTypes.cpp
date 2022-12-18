#include "KeypadTypes.h"
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
// DIESEL

void ThrottleMenu::DrawCAB2DieselKeypad()
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

    if(ImGui::ImageButton((void*)(intptr_t)rpmUPIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      m_enginedefs[m_selected_engine].diesel_electric_rev_lvl++;
      if (m_enginedefs[m_selected_engine].diesel_electric_rev_lvl > 8)
        m_enginedefs[m_selected_engine].diesel_electric_rev_lvl = 8;
      printf("RPM UP, value at:%d\n", m_enginedefs[m_selected_engine].diesel_electric_rev_lvl);
      TMCCInterface::EngineSetDieselRunLevel(engineID, m_enginedefs[m_selected_engine].diesel_electric_rev_lvl);
    };

    Tooltip("RPM Up");


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

    if(ImGui::ImageButton((void*)(intptr_t)rpmDownIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      m_enginedefs[m_selected_engine].diesel_electric_rev_lvl--;
      if (m_enginedefs[m_selected_engine].diesel_electric_rev_lvl < 0)
        m_enginedefs[m_selected_engine].diesel_electric_rev_lvl = 0;
      printf("RPM Down, value at:%d\n", m_enginedefs[m_selected_engine].diesel_electric_rev_lvl);
      TMCCInterface::EngineSetDieselRunLevel(engineID, m_enginedefs[m_selected_engine].diesel_electric_rev_lvl);
    };

    Tooltip("RPM Down");


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

    if (ImGui::ImageButton((void*)(intptr_t)ditchlightonIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      printf("Ditch Lights on, Pulse w/ Horn on\n");
      TMCCInterface::EngineSetDitchLights(engineID, TMCC_ON, (TMCCActiveState)LightingCommandParams::LT_DITCH_LIGHTS_ON);
    };
    Tooltip("Ditch On w/Horn Pulse");

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

    if (ImGui::ImageButton((void*)(intptr_t)ditchlightoffIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      printf("Ditch Lights off, Pulse w/ Horn on\n");
      TMCCInterface::EngineSetDitchLights(engineID, TMCC_OFF,(TMCCActiveState)LightingCommandParams::LT_DITCH_LIGHTS_OFF_PULSE_ON);
    };

    Tooltip("Ditch Off w/Horn Pulse");

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
      TMCCInterface::EngineDialogCommand(engineID, DialogCommandParams::DC_TOWER_SHUT_DOWN);
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

    if (ImGui::ImageButton((void*)(intptr_t)marsonIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      printf("Mars on\n");
      TMCCInterface::EngineSetMarsLight(engineID, TMCC_ON);

    };

    Tooltip("Mars Light On");

    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)doglightonIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      printf("Strobe light on\n");
      TMCCInterface::EngineSetStrobeLight(engineID, TMCC_ON);

    };

    Tooltip("Strobe Light On");


    if (ImGui::ImageButton((void*)(intptr_t)volDownIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineSetMasterVolume(engineID, TMCC_OFF);

    };

    Tooltip("Volume Down");

    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)marsoffIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      printf("Mars off\n");
      TMCCInterface::EngineSetMarsLight(engineID, TMCC_OFF);

    };

    Tooltip("Mars Light Off");

    ImGui::SameLine();

    if (ImGui::ImageButton((void*)(intptr_t)doglightoffIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      printf("Strobe light off\n");
      TMCCInterface::EngineSetStrobeLight(engineID, TMCC_OFF);

    };

    Tooltip("Strobe Light Off");


    if (ImGui::ImageButton((void*)(intptr_t)ditchlightonIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      printf("Hazard Lights on\n");
      TMCCInterface::EngineSetHazardLight(engineID, TMCC_ON);
    };

    Tooltip("Hazard Lights on");


    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)ditchlightoffIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      printf("Hazard Lights off\n");
      TMCCInterface::EngineSetHazardLight(engineID, TMCC_OFF);
    };

    Tooltip("Hazard Lights off");


    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)groundlightonIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      printf("Ground light on\n");
      TMCCInterface::EngineSetGroundLights(engineID, TMCC_ON);
    };
    Tooltip("Ground lights on");

    if (ImGui::ImageButton((void*)(intptr_t)ditchlightonIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      printf("Ditch Lights on, Pulse w/ Horn off\n");
      TMCCInterface::EngineSetDitchLights(engineID, TMCC_ON, (TMCCActiveState)LightingCommandParams::LT_DITCH_LIGHTS_ON);
    };

    Tooltip("Ditch Lights entirely on");


    ImGui::SameLine();

    ImGui::ImageButton((void*)(intptr_t)resetIcon->GetGLHandle(), ImVec2(70, 70));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("Reset\n");
      TMCCInterface::EngineNumericCommand2(engineID, 0);
    };
    Tooltip("Reset (0 key)");

    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)groundlightoffIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      printf("Ground light off\n");
      TMCCInterface::EngineSetGroundLights(engineID, TMCC_OFF);
    };
    Tooltip("Ground lights off");


    if (ImGui::ImageButton((void*)(intptr_t)ditchlightoffIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      printf("Ditch Lights off, Pulse w/ Horn off\n");
      TMCCInterface::EngineSetDitchLights(engineID, TMCC_OFF, (TMCCActiveState)LightingCommandParams::LT_DITCH_LIGHTS_OFF);
    };

    Tooltip("Ditch Lights entirely off");


    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)extstartUpIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineStartupSequence2(engineID);
    };
    Tooltip("Immediate Start Up");


    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)groundlightoffIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      printf("Ground light auto\n");
      TMCCInterface::EngineSetGroundLightsAuto(engineID);
    };
    Tooltip("Ground lights auto");



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

  if(ImGui::ImageButton((void*)(intptr_t)rpmUPIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      m_enginedefs[m_selected_engine].diesel_electric_rev_lvl++;
      if (m_enginedefs[m_selected_engine].diesel_electric_rev_lvl > 8)
        m_enginedefs[m_selected_engine].diesel_electric_rev_lvl = 8;
      printf("RPM UP, value at:%d\n", m_enginedefs[m_selected_engine].diesel_electric_rev_lvl);
      TMCCInterface::EngineSetDieselRunLevel(engineID, m_enginedefs[m_selected_engine].diesel_electric_rev_lvl);
    };


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

  if(ImGui::ImageButton((void*)(intptr_t)rpmDownIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      m_enginedefs[m_selected_engine].diesel_electric_rev_lvl--;
      if (m_enginedefs[m_selected_engine].diesel_electric_rev_lvl < 0)
        m_enginedefs[m_selected_engine].diesel_electric_rev_lvl = 0;
      printf("RPM Down, value at:%d\n", m_enginedefs[m_selected_engine].diesel_electric_rev_lvl);
      TMCCInterface::EngineSetDieselRunLevel(engineID, m_enginedefs[m_selected_engine].diesel_electric_rev_lvl);
    };

    Tooltip("RPM Down");

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
    Tooltip("Light On");

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

void ThrottleMenu::DrawFreightCarKeypad(void)
{
  if (m_enginedefs[m_selected_engine].keyPadPage == 0)
  {
    if (ImGui::ImageButton((void*)(intptr_t)volUpIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineSetMasterVolume(engineID, TMCC_ON);

    };
    Tooltip("Volume Up");

    ImGui::SameLine();

    ImGui::Button("Flat Wheel", ImVec2(78, 76));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("Flat Wheel on");
      TMCCInterface::EngineStockCarSetFlatWheel(engineID,TMCC_ON);
    };

    Tooltip("Flat Wheel");

    ImGui::SameLine();

    ImGui::Button("Load\nSequence", ImVec2(78, 76));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("Load sequence with voice");
      TMCCInterface::EngineNumericCommand2(engineID, 3);
    };


    if (ImGui::ImageButton((void*)(intptr_t)volDownIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineSetMasterVolume(engineID, TMCC_OFF);

    };

    Tooltip("Volume Down");

    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)extshutDownIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineShutdownSequence1(engineID);
      TMCCInterface::EngineShutdownSequence2(engineID);
    };


    ImGui::SameLine();

    ImGui::Button("Unload\nSequence", ImVec2(78, 76));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("Unload sequence with voice\n");
      TMCCInterface::EngineNumericCommand2(engineID, 3);
    };

    ImGui::Button("Moo Game", ImVec2(78, 76));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("Moo game (numerical 7 key only)\n");
      TMCCInterface::EngineNumericCommand2(engineID, 7);
    };

    Tooltip("Play Moo Game (only works on Gen. 1 cattle cars)");

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
    if (ImGui::ImageButton((void*)(intptr_t)extstartUpIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineStartupSequence1(engineID);
      TMCCInterface::EngineStartupSequence2(engineID);
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
    ImGui::Button("Random\nVoiceChatter", ImVec2(78, 76));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("Random voice chatter\n");
      TMCCInterface::EngineDialogCommand(engineID, DialogCommandParams::DC_CONVENTIONAL_SHORT_HORN_TRIGGER);
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

    ImGui::Button("Start Up\nVoice\nClips", ImVec2(78, 76));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("Start Up Voice Clips (random)\n");
      TMCCInterface::EngineDialogCommand(engineID, DialogCommandParams::DC_TOWER_INITIAL_ENGINE_STARTUP);
    };

    ImGui::SameLine();

    ImGui::Button("Load\nSequence\n(no voice)", ImVec2(78, 76));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("EngineStockCarLoad\n");
      TMCCInterface::EngineStockCarLoad(engineID);
    };


    if (ImGui::ImageButton((void*)(intptr_t)volDownIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineSetMasterVolume(engineID, TMCC_OFF);

    };

    Tooltip("Volume Down");

    ImGui::SameLine();
    ImGui::Button("Unload\nSequence\n(no voice)", ImVec2(78, 76));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("EngineStockCarUnload\n");
      TMCCInterface::EngineStockCarUnload(engineID);
    };

    ImGui::SameLine();

    ImGui::Button("Cow Game:\nON", ImVec2(78, 76));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("EngineStockCarSetGame:ON\n");
      TMCCInterface::EngineStockCarSetGame(engineID,TMCC_ON);
    };

    ImGui::Button("FRED:\nON", ImVec2(78, 76));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("EngineStockCarSetFRED:ON\n");
      TMCCInterface::EngineStockCarSetFRED(engineID,TMCC_ON);
    };

    ImGui::SameLine();
    ImGui::Button("FRED:\nOFF", ImVec2(78, 76));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("EngineStockCarSetFRED:OFF\n");
      TMCCInterface::EngineStockCarSetFRED(engineID, TMCC_OFF);
    };

    ImGui::SameLine();
    ImGui::Button("Cow Game:\nOFF", ImVec2(78, 76));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("EngineStockCarSetGame:OFF\n");
      TMCCInterface::EngineStockCarSetGame(engineID, TMCC_OFF);
    };

    ImGui::Button("Flatwheel:\nON", ImVec2(78, 76));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("EngineStockCarSetFlatWheel:ON\n");
      TMCCInterface::EngineStockCarSetFlatWheel(engineID, TMCC_ON);
    };

    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)resetIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      TMCCInterface::EngineNumericCommand2(engineID, 0);

    };
    Tooltip("Reset (0 key)");

    ImGui::SameLine();
    ImGui::Button("Flatwheel:\nOFF", ImVec2(78, 76));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
    {
      printf("EngineStockCarSetFlatWheel:OFF\n");
      TMCCInterface::EngineStockCarSetFlatWheel(engineID, TMCC_OFF);
    };

    if (ImGui::ImageButton((void*)(intptr_t)blankIcon->GetGLHandle(), ImVec2(70, 70)))
    {
    };


    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)aux3arrowIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      printf("Aux 3");
      TMCCInterface::EngineAux3Trigger(engineID);

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
    if (ImGui::ImageButton((void*)(intptr_t)blankIcon->GetGLHandle(), ImVec2(70, 70)))
    {

    };


    if (ImGui::ImageButton((void*)(intptr_t)blankIcon->GetGLHandle(), ImVec2(70, 70)))
    {

    };


    ImGui::SameLine();
    if (ImGui::Button("Toggle\nHorn 1/2", ImVec2(78, 76)))
    {
      printf("Toggle Horn 1 and 2\n");
      m_enginedefs[m_selected_engine].useHorn2 = !m_enginedefs[m_selected_engine].useHorn2;
    };
    Tooltip("Horn 1 is the normal TMCC horn. Horn 2 is the same sound,\nbut will not enable ditch lights on diesel locos when blown.");

    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)blankIcon->GetGLHandle(), ImVec2(70, 70)))
    {

    };



    if (ImGui::ImageButton((void*)(intptr_t)leftArrowIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      m_enginedefs[m_selected_engine].keyPadPage--;
      if (m_enginedefs[m_selected_engine].keyPadPage < 0)
        m_enginedefs[m_selected_engine].keyPadPage = 1;
    };
    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)blankIcon->GetGLHandle(), ImVec2(70, 70)))
    {

    };
    ImGui::SameLine();
    if (ImGui::ImageButton((void*)(intptr_t)rightArrowIcon->GetGLHandle(), ImVec2(70, 70)))
    {
      m_enginedefs[m_selected_engine].keyPadPage++;
      if (m_enginedefs[m_selected_engine].keyPadPage > 1)
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