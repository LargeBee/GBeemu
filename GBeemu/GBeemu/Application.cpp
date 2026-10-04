#include "Application.h"
#include "Emulator.h"


Application::Application()
{
	window = nullptr;
	glContext = nullptr;

	running = false;

	emulator = nullptr;

	gameboyTexture = 0;

	menuBar = true;

	displayScale = 5;
	windowSized = false;

	traceOpen = false;
}

Application::~Application()
{
	shutdown();
}



bool Application::initialize()
{
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_Log(
			"SDL_Init failed: %s",
			SDL_GetError()
		);

		return false;
	}

	window = SDL_CreateWindow(
		"GBeemu",
		800,
		600,
		SDL_WINDOW_OPENGL
	);

	if (!window)
	{
		SDL_Log(
			"SDL_CreateWindow failed: %s",
			SDL_GetError()
		);

		SDL_Quit();

		return false;
	}

	glContext = SDL_GL_CreateContext(window);

	if (!glContext)
	{
		SDL_Log(
			"SDL_GL_CreateContext failed: %s",
			SDL_GetError()
		);

		SDL_DestroyWindow(window);
		SDL_Quit();

		return false;
	}

	if (!SDL_GL_MakeCurrent(window, glContext))
	{
		SDL_Log(
			"SDL_GL_MakeCurrent failed: %s",
			SDL_GetError()
		);
	}

	// ImGui initialisation
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	if (!ImGui_ImplSDL3_InitForOpenGL(window, glContext))
	{
		SDL_Log(
			"ImGui SDL3 backend initialization failed"
		);

		return false;
	}

	if (!ImGui_ImplOpenGL3_Init("#version 330"))
	{
		SDL_Log(
			"ImGui OpenGL3 backend initialization failed"
		);

		return false;
	}

	glGenTextures(1, &gameboyTexture);

	glBindTexture(
		GL_TEXTURE_2D,
		gameboyTexture
	);

	glTexParameteri(
		GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER,
		GL_NEAREST
	);

	glTexParameteri(
		GL_TEXTURE_2D,
		GL_TEXTURE_MAG_FILTER,
		GL_NEAREST
	);

	glTexParameteri(
		GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_S,
		GL_CLAMP_TO_EDGE
	);

	glTexParameteri(
		GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_T,
		GL_CLAMP_TO_EDGE
	);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA8,
		160,
		144,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		nullptr
	);

	glBindTexture(
		GL_TEXTURE_2D,
		0
	);

	// Create emulator
	emulator = new Emulator();

	running = true;

	return true;
}

void Application::run()
{
	while (running)
	{
		processEvents();

		update();

		updateGameBoyTexture();

		render();
	}
}

void Application::shutdown()
{
	if (emulator)
	{
		delete emulator;
		emulator = nullptr;
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();

	ImGui::DestroyContext();

	if (glContext)
	{
		SDL_GL_DestroyContext(glContext);
		glContext = nullptr;
	}

	if (window)
	{
		SDL_DestroyWindow(window);
		window = nullptr;
	}

	SDL_Quit();
}

void Application::processEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL3_ProcessEvent(&event);

		if (event.type == SDL_EVENT_QUIT)
		{
			running = false;
		}
	}
}

void Application::update()
{
	if (emulator)
	{
		emulator->update();
	}
}

void Application::render()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();

	ImGui::NewFrame();

	renderMenuBar();
	renderGameScreen();
	//renderCPU();
	renderTrace();
	//renderMemoryView();
	//renderInput();
	//renderTileViewer();
	//renderPalette();

	ImGui::Render();

	int width;
	int height;

	SDL_GetWindowSize(
		window,
		&width,
		&height
	);

	glViewport(
		0,
		0,
		width,
		height
	);

	glClear(GL_COLOR_BUFFER_BIT);

	ImGui_ImplOpenGL3_RenderDrawData(
		ImGui::GetDrawData()
	);

	SDL_GL_SwapWindow(window);
}

void Application::renderMenuBar()
{
	if (!menuBar)
		return;

	//ImGui menu bar rendering
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open ROM"))
			{
				openROM();
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Load State"))
			{
			}
			if (ImGui::MenuItem("Save State"))
			{
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Exit"))
			{
				running = false;
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Emulation"))
		{
			if (ImGui::MenuItem("Pause"))
			{
				if (emulator->isRunning())
					emulator->pause();
				else
					emulator->run();
			}

			if (ImGui::MenuItem("Reset"))
			{
				emulator->reset();
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::BeginMenu("Scale"))
			{
				if (ImGui::RadioButton("1x", displayScale == 1))
				{
					displayScale = 1;
					windowSized = false;
				}

				if (ImGui::RadioButton("2x", displayScale == 2))
				{
					displayScale = 2;
					windowSized = false;
				}

				if (ImGui::RadioButton("3x", displayScale == 3))
				{
					displayScale = 3;
					windowSized = false;
				}

				if (ImGui::RadioButton("4x", displayScale == 4))
				{
					displayScale = 4;
					windowSized = false;
				}

				if (ImGui::RadioButton("5x", displayScale == 5))
				{
					displayScale = 5;
					windowSized = false;
				}

				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Palette Editor"))
			{
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Input"))
		{
			if (ImGui::MenuItem("Input Bindings"))
			{
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Debug"))
		{
			if (ImGui::MenuItem("Trace Viewer"))
			{
				if (!traceOpen)
				{
					traceOpen = true;
				}
			}
			if (ImGui::MenuItem("Memory Viewer"))
			{
			}
			if (ImGui::MenuItem("Tile Viewer"))
			{
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	if (!windowSized)
	{
		int displayWidth = 160 * displayScale;
		int displayHeight = 144 * displayScale;

		float menuHeight = ImGui::GetFrameHeight();

		SDL_SetWindowSize(
			window,
			displayWidth,
			displayHeight + (int)menuHeight
		);

		windowSized = true;
	}
}

void Application::renderGameScreen()
{
	int displayWidth = 160 * displayScale;
	int displayHeight = 144 * displayScale;

	ImGui::SetNextWindowPos(
		ImVec2(0, ImGui::GetFrameHeight())
	);

	ImGui::SetNextWindowSize(
		ImVec2(
			(float)displayWidth,
			(float)displayHeight
		)
	);

	ImGuiWindowFlags flags =
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoScrollWithMouse;

	ImGui::PushStyleVar(
		ImGuiStyleVar_WindowPadding,
		ImVec2(0, 0)
	);

	ImGui::Begin(
		"GBeemu",
		nullptr,
		flags
	);

	// Game Boy texture goes here

	ImGui::End();

	ImGui::PopStyleVar();
}

void Application::loadROM(const char* path)
{
	if (!emulator->loadROM(path))
	{
		SDL_Log(
			"Failed to load ROM: %s",
			path
		);

		return;
	}

	SDL_Log(
		"Loaded ROM: %s",
		path
	);
}

void Application::openROM()
{
	SDL_DialogFileFilter filters[] =
	{
		{ "Game Boy ROM", "gb" }
	};

	SDL_ShowOpenFileDialog(
		Application::OpenROMCallback,
		this,
		window,
		filters,
		1,
		nullptr,
		false
	);
}


void SDLCALL Application::OpenROMCallback(
	void* userdata,
	const char* const* filelist,
	int filter
)
{
	if (filelist == nullptr)
		return;

	if (filelist[0] == nullptr)
		return;

	Application* application =
		static_cast<Application*>(userdata);

	application->loadROM(filelist[0]);
}

void Application::updateGameBoyTexture()
{
	const uint8_t* framebuffer =
		emulator->getFramebuffer();

	static uint8_t rgbaFramebuffer[
		160 * 144 * 4
	];

	for (int i = 0; i < 160 * 144; i++)
	{
		uint8_t colour =
			framebuffer[i] & 0x03;

		rgbaFramebuffer[i * 4 + 0] =
			palette[colour][0];

		rgbaFramebuffer[i * 4 + 1] =
			palette[colour][1];

		rgbaFramebuffer[i * 4 + 2] =
			palette[colour][2];

		rgbaFramebuffer[i * 4 + 3] =
			palette[colour][3];
	}

	glBindTexture(
		GL_TEXTURE_2D,
		gameboyTexture
	);

	glTexSubImage2D(
		GL_TEXTURE_2D,
		0,
		0,
		0,
		160,
		144,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		rgbaFramebuffer
	);

	glBindTexture(
		GL_TEXTURE_2D,
		0
	);
}

void Application::renderTrace()
{
	if (!traceOpen)
		return;

	ImGui::Begin("Trace Viewer");

	if (ImGui::Button("Step"))
	{
		emulator->stepInstruction();
	}

	ImGui::SameLine();

	if (ImGui::Button("Clear"))
	{
		emulator->clearTrace();
	}

	ImGui::Separator();

	const std::vector<TraceEntry>& trace =
		emulator->getTrace();

	bool atBottom = ImGui::GetScrollY() >= ImGui::GetScrollMaxY() - 10.0f;

	for (const TraceEntry& entry : trace)
	{
		ImGui::Text(
			"%04X  %02X  %-12s  "
			"A:%02X F:%02X "
			"B:%02X C:%02X "
			"D:%02X E:%02X "
			"H:%02X L:%02X "
			"SP:%04X",
			entry.pc,
			entry.opcode,
			entry.instruction.c_str(),
			entry.a,
			entry.f,
			entry.b,
			entry.c,
			entry.d,
			entry.e,
			entry.h,
			entry.l,
			entry.sp
		);
	}

	if (atBottom)
		ImGui::SetScrollHereY(1.0f);

	ImGui::End();
}