#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"

class Emulator;

class Application
{
public:
	Application();
	~Application();

	bool initialize();
	void run();
	void shutdown();

private:
	void processEvents();
	void update();
	void render();

	void updateGameBoyTexture();

	GLuint gameboyTexture;

	static constexpr uint8_t palette[4][4] =
	{
		{ 155, 188, 15, 255 },
		{ 139, 172, 15, 255 },
		{ 48, 98, 48, 255 },
		{ 15, 56, 15, 255 }
	};

	SDL_Window* window;
	SDL_GLContext glContext;

	bool running;

	Emulator* emulator;

	bool traceOpen;

	int displayScale;
	bool windowSized;

	static void SDLCALL OpenROMCallback(
		void* userdata,
		const char* const* filelist,
		int filter
	);

	void openROM();
	void loadROM(const char* path);

	void renderMenuBar();
	void renderGameScreen();
	void renderCPU();
	void renderTrace();
	void renderMemoryView();
	void renderInput();
	void renderTileViewer();
	void renderPalette();

	bool menuBar;


};