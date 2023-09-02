#include <iostream>
#include <memory>

#include <Application.hpp>


class MyApp : public GameEngine::Application
{
	virtual void on_update() override
	{
		//std::cout << "on_update()" << std::endl;
	}
};

int main() {
	auto MyAppPtr = std::make_unique<MyApp>();
	int returnCode = MyAppPtr->start(1024, 768, "GameEngine");
	return returnCode;
}