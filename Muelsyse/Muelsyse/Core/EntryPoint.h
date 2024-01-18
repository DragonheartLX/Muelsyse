#pragma once

extern mul::Application* mul::createApplication(ApplicationCommandLineArgs args);

#ifdef MUL_NO_CONSOLE
	#include <windows.h>

	int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
#else
	int main(int argc, char** argv)
#endif
	{
		mul::Logger::init();
		MUL_CORE_INFO("Initialized Logger!");

		MUL_CORE_INFO("Muelsyse version: {0}", MUL_VERSION_STRING);
		MUL_PROFILE_BEGIN_SESSION("Startup", "MuelsyseProfile-Startup.json");
		mul::Application* app = mul::createApplication({ argc, argv });
		MUL_PROFILE_END_SESSION();

		MUL_PROFILE_BEGIN_SESSION("Runtime", "MuelsyseProfile-Runtime.json");
		app->m_Run();
		MUL_PROFILE_END_SESSION();

		MUL_PROFILE_BEGIN_SESSION("Shutdown", "MuelsyseProfile-Shutdown.json");
		delete app;
		MUL_PROFILE_END_SESSION();

		MUL_CORE_INFO("Muelsyse exit.");
		return 0;
	}

#define FLOWING(X) mul::Application* mul::createApplication(mul::ApplicationCommandLineArgs args) { return new X(args); }