#pragma once

extern mul::Application* mul::createApplication();

int main(int argc, char** argv)
{
	mul::Logger::init();
	MUL_CORE_INFO("Initialized Logger!");

	mul::Application* app = mul::createApplication();
	app->run();
	delete app;

	MUL_CORE_INFO("Muelsyse exit.");
	return 0;
}

#define FLOWING(X)	mul::Application* mul::createApplication() { return new X(); }