#include <iostream>
#include <Muelsyse.hpp>

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