#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <SDL.h>
#include <SDL_syswm.h>
#include <SDL_opengl.h>
#include <stdio.h>
#include <TMCCInterface.h>
#include "json.hpp"
#include "imgui-knobs.h"
#include "interface/throttlemenu.h"
#include "EngineManagement.h"
//#include "surface/DeviceListener.h"



int main(int argc, char* argv[])
{
  // Setup SDL
  // (Some versions of SDL before <2.0.10 appears to have performance/stalling issues on a minority of Windows systems,
  // depending on whether SDL_INIT_GAMECONTROLLER is enabled or disabled.. updating to latest version of SDL is recommended!)
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
  {
    printf("Error: %s\n", SDL_GetError());
    return -1;
  }

  int currentSpeed = 0;

  //TMCCInterface::Init();
  

  

  // GL 3.0 + GLSL 130
  const char* glsl_version = "#version 130";
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);


  // Create window with graphics context
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
  SDL_Window* window = SDL_CreateWindow("OpenCAB",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 1024, window_flags);
  SDL_GLContext gl_context = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, gl_context);
  SDL_GL_SetSwapInterval(1); // Enable vsync

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  //ImGui::StyleColorsClassic();

  // Setup Platform/Renderer backends
  ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
  ImGui_ImplOpenGL3_Init(glsl_version);

  // Surface Dial
  // 
  // 
  
  SDL_SysWMinfo info;
  SDL_VERSION(&info.version);
  SDL_GetWindowWMInfo(window, &info);


  HWND hwnd = info.info.win.window;

 //DeviceListener::Init(hwnd);

  // Load Fonts
  // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
  // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
  // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
  // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
  // - Read 'docs/FONTS.md' for more instructions and details.
  // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
  //io.Fonts->AddFontDefault();
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
  //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
  //IM_ASSERT(font != NULL);
  

  //int my_image_width = 0;
  //int my_image_height = 0;
  //GLuint my_image_texture = 0;
  //bool ret = LoadTextureFromFile("/graphics/legacy_blowdown.png", &my_image_texture, &my_image_width, &my_image_height);
  //IM_ASSERT(ret);

  //Analog joystick dead zone
  const int JOYSTICK_DEAD_ZONE = 8000;
  //Game Controller 1 handler
  SDL_GameController* gGameController = NULL;
  int rightStickXDeadZone = 0;
  int rightStickYDeadZone = 0;
  int leftStickXDeadZone = 0;
  int leftStickYDeadZone = 0;

  //Check for joysticks
  if (SDL_NumJoysticks() < 1)
  {
    printf("Warning: No joysticks connected!\n");
  }
  else
  {
    //Load joystick
    gGameController = SDL_GameControllerOpen(0);
    if (gGameController == NULL)
    {
      printf("Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError());
    }
    else
    {
      char* mapping;
      printf("Index \'%i\' is a compatible controller, named \'%s\'", 0, SDL_GameControllerNameForIndex(0));
      mapping = SDL_GameControllerMapping(gGameController);
      
      printf("Controller %i is mapped as \"%s\".\n", 0, mapping);
      printf("Controller Vendor:%d, Product:%d, Mapping:%d\n\n", SDL_GameControllerGetVendor(gGameController), SDL_GameControllerGetProduct(gGameController), SDL_GameControllerGetType(gGameController));
      SDL_free(mapping);
    }
  }
  std::string controllerName;
  if (gGameController) // if we have a controller, poll the joystick axis to know the dead zone. 
  {
    controllerName = SDL_GameControllerName(gGameController);

    bool donePollController = false;
    while (!donePollController)
    {
      SDL_Event event;
      while (SDL_PollEvent(&event))
      {
        leftStickXDeadZone = SDL_GameControllerGetAxis(gGameController, SDL_CONTROLLER_AXIS_LEFTX);
        leftStickYDeadZone = SDL_GameControllerGetAxis(gGameController, SDL_CONTROLLER_AXIS_LEFTY);

        rightStickXDeadZone = SDL_GameControllerGetAxis(gGameController, SDL_CONTROLLER_AXIS_RIGHTX);
        rightStickYDeadZone = SDL_GameControllerGetAxis(gGameController, SDL_CONTROLLER_AXIS_RIGHTY);
        printf("leftStickXDead:%d RightStickXDead:%d \nleftStickYDead:%d RightStickYDead:%d\n", leftStickXDeadZone, rightStickXDeadZone, leftStickYDeadZone, rightStickYDeadZone);
        donePollController = true;
        ImGui_ImplSDL2_ProcessEvent(&event);
      }
    }
  }

  json engineRoster;

  ThrottleMenu menu(argv[0],gGameController, controllerName);
  EngineManagement::ReadEngineRoster(engineRoster, menu.m_enginedefs, menu.m_switchdefs, menu.m_accessorydefs, argv[0]);
  menu.LoadRosterLaunch(argv[0]);

  
  

  // Main loop
  bool done = false;
  while (!done)
  {
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      //if (event.type == SDL_CONTROLLERBUTTONDOWN)
      //{
      //  // handle joystick event here
      //  
      //  //printf("Button pressed\n");
      //  //SDL_JoystickGetButton(gGameController,)
      //  // don't let imgui process the event
      //  continue;
      //}



      ImGui_ImplSDL2_ProcessEvent(&event);
      if (event.type == SDL_QUIT)
        done = true;
      if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
        done = true;
    }

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();


    // Set Full Screen Window
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(io.DisplaySize);
    ImGui::Begin("Content", nullptr,
      ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
      ImGuiWindowFlags_NoSavedSettings |
      ImGuiWindowFlags_NoBringToFrontOnFocus);





    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Window Begins here

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    menu.Draw(argv[0], gGameController,leftStickXDeadZone,leftStickYDeadZone,rightStickXDeadZone,rightStickYDeadZone);

        // Rendering
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    //glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
  }
      



  // Cleanup

   //Close game controller
  SDL_GameControllerClose(gGameController);
  gGameController = NULL;

  //TMCCInterface::Shutdown();

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_GL_DeleteContext(gl_context);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}

