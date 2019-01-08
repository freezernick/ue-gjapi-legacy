// Copyright (c) 2019 Free2Play-Entertainment

#pragma once

class GameJoltPlugin : public IModuleInterface
{
private:

public:
	GameJoltPlugin();

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};