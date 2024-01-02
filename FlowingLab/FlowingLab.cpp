#include <iostream>
#include <Muelsyse.h>

class FlowingLab: public mul::Application
{
public:
	FlowingLab() {};
	~FlowingLab() {};

private:

};

mul::Application* mul::createApplication()
{
	return new FlowingLab();
}