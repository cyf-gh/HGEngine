#include <SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

#include "../GameObject.h"
#include "../Transform.hpp"

#include "../EngineImpl.h"
#include "Editor.h"
#include "../../EngineImpl/Log.hpp"

using namespace HG;

HGEngine::V1SDL::Editor::Editor( SDL_Window* pSDLWindow, SDL_Renderer* pSDL2Renderer ) :
    m_pSDL2Renderer( pSDL2Renderer ), IsHiden( false ) {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer( pSDLWindow, m_pSDL2Renderer );
    ImGui_ImplSDLRenderer2_Init( pSDL2Renderer );
}

HGEngine::V1SDL::Editor::~Editor() { 
    // Cleanup
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

int HGEngine::V1SDL::Editor::Render() {
    if( IsHiden ) {
        return 0;
    }
    ImGuiIO& io = ImGui::GetIO();

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);

    // Our state
    bool show_demo_window = false;
    bool gameObjectTranformWindow = false;
    bool show_log_window = true;
    ImVec4 clear_color = ImVec4( 0.45f, 0.55f, 0.60f, 1.00f );

        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.


        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if( show_demo_window )
            ImGui::ShowDemoWindow( &show_demo_window );

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin( "Hello, world!" );                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text( "This is some useful text." );               // Display some text (you can use a format strings too)
            ImGui::Checkbox( "Demo Window", &show_demo_window );      // Edit bools storing our window open/close state
            ImGui::Checkbox( "GameObject Transform Window", &gameObjectTranformWindow );

            ImGui::SliderFloat( "float", &f, 0.0f, 1.0f );            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3( "clear color", ( float* ) &clear_color ); // Edit 3 floats representing a color

            if( ImGui::Button( "Button" ) )                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text( "counter = %d", counter );

            ImGui::Text( "Application average ( FPS)"  );
            ImGui::End();
        }

        // 3. Show another simple window.
        ImGui::Begin( "Another Window", &gameObjectTranformWindow );   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        for( std::pair<std::string,GameObject*> one: GameObject::umTheseOnes ) {
            auto go = one.second;
            auto tr = go->GetComponent<Transform>();
            ImGui::Text( "[%s]\txy: %f,%f | wh: %u,%u | angle %d", one.first.c_str(), tr->tPosition.X, tr->tPosition.Y, tr->tRect.W, tr->tRect.H, tr->f64Angle );
        }
        ImGui::End();
        
        if ( show_log_window ) {
            ImGui::Begin( "Log Window", &show_log_window );   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::InputTextMultiline( "Log", HG::HGLog::HGLogStr.data(), HG::HGLog::HGLogStr.size() );
            //static char input[100];
            //ImGui::InputText( "123", input, sizeof( input ) );
            ImGui::End();   
        }
        
        // Rendering
        ImGui::Render();
        SDL_RenderSetScale( m_pSDL2Renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y );
        // SDL_SetRenderDrawColor( m_pSDL2Renderer, ( Uint8 ) ( clear_color.x * 255 ), ( Uint8 ) ( clear_color.y * 255 ), ( Uint8 ) ( clear_color.z * 255 ), ( Uint8 ) ( clear_color.w * 255 ) );
        // SDL_RenderClear( m_pSDL2Renderer );
        ImGui_ImplSDLRenderer2_RenderDrawData( ImGui::GetDrawData() );
        SDL_RenderPresent( m_pSDL2Renderer );
    
	return 0;
}
