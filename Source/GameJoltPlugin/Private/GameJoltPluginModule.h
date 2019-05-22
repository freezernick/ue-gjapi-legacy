// Copyright (c) 2019 Free2Play-Entertainment

#pragma once

class GAMEJOLTPLUGIN_API GameJoltPlugin : public IModuleInterface
{
private:

public:
	GameJoltPlugin();

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};