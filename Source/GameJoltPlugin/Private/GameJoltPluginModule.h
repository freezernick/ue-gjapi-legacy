#pragma once


class GameJoltPlugin : public IModuleInterface
{
private:

public:
	GameJoltPlugin();

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};