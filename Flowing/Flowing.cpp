#include <Muelsyse.h>
#include <Muelsyse/Core/EntryPoint.h>

class Flowing : public mul::Application
{
public:
	Flowing(const mul::ApplicationSpecification& specification): mul::Application(specification) {};
	~Flowing() {};

private:

};

FLOWING(Flowing, "./");