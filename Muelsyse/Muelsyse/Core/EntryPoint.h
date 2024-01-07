#pragma once

extern mul::Application* mul::createApplication();

#ifdef MUL_NO_CONSOLE
	#include <windows.h>

	int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
	{
		mul::Logger::init();
		MUL_CORE_INFO("Initialized Logger!");

		MUL_PROFILE_BEGIN_SESSION("Startup", "MuelsyseProfile-Startup.json");
		mul::Application* app = mul::createApplication();
		MUL_PROFILE_END_SESSION();

		MUL_PROFILE_BEGIN_SESSION("Runtime", "MuelsyseProfile-Runtime.json");
		app->m_Run();
		MUL_PROFILE_END_SESSION();

		MUL_PROFILE_BEGIN_SESSION("Startup", "MuelsyseProfile-Shutdown.json");
		delete app;
		MUL_PROFILE_END_SESSION();

		MUL_CORE_INFO("Muelsyse exit.");
		return 0;
	}
#else
	int main(int argc, char** argv)
	{
		mul::Logger::init();
		MUL_CORE_INFO("Initialized Logger!");

		MUL_PROFILE_BEGIN_SESSION("Startup", "MuelsyseProfile-Startup.json");
		mul::Application* app = mul::createApplication();
		MUL_PROFILE_END_SESSION();

		MUL_PROFILE_BEGIN_SESSION("Runtime", "MuelsyseProfile-Runtime.json");
		app->m_Run();
		MUL_PROFILE_END_SESSION();

		MUL_PROFILE_BEGIN_SESSION("Startup", "MuelsyseProfile-Shutdown.json");
		delete app;
		MUL_PROFILE_END_SESSION();

		MUL_CORE_INFO("Muelsyse exit.");
		return 0;
	}
#endif

#define FLOWING(X) mul::Application* mul::createApplication() { return new X(); }