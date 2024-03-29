#pragma once

#include <list>
#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleInterface.h"
#include "ModuleFBXLoader.h"
#include "JSON/parson.h"

class EngineConfiguration;

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleSceneIntro* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleInterface* ui;
	ModuleFBXLoader* fbx_loader;

	JSON_Value *config;
	JSON_Object *modules_object;

private:

	Timer	ms_timer;
	float	dt;
	std::list<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	void RequestBrowser(const char* website_url);

	// Call SaveGame -------------------------------------
	void CallSave();

	// Call LoadGame -------------------------------------
	void CallLoad();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};