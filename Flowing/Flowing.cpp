#include <iostream>
#include <Muelsyse.hpp>

class Flowing : public mul::Application
{
public:
	Flowing() {};
	~Flowing() {};

private:

};

mul::Application* mul::createApplication()
{
	return new Flowing();
}