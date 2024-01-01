#pragma once

extern mul::Application* mul::createApplication();

int main(int argc, char** argv)
{
	mul::Application* app = mul::createApplication();
	app->run();
	delete app;

	return 0;
}