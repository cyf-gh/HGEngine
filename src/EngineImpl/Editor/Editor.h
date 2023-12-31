#pragma once

struct SDL_Window;
struct SDL_Renderer;
struct ImGuiIO;

namespace HGEngine {
namespace V1SDL {
class Editor final {
public:
	bool IsHiden;
	void Hide() { IsHiden = true; }
	Editor( SDL_Window* pSDLWindow, SDL_Renderer* pSDL2Renderer );
	~Editor();
	int Render();
private:
	SDL_Renderer*m_pSDL2Renderer;
};
}}