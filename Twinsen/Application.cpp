#include "Application.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	audio = new ModuleAudio(this, true);
	fbx_loader = new ModuleFBXLoader(this);
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	ui = new ModuleInterface(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	
	// Scenes
	AddModule(scene_intro);
	AddModule(fbx_loader);
	
	// Renderer last!
	AddModule(renderer3D);

	// Interface
	AddModule(ui);
}

Application::~Application()
{
	std::list<Module*>::iterator item = list_modules.begin();

	while(item != list_modules.end())
	{
		delete (item._Ptr->_Myval);
		item++;
	}
}

bool Application::Init()
{
	bool ret = true;

	config = json_parse_file("config.json");
	
	if (config != NULL)
	{
		modules_object = json_value_get_object(config);

		LOG("Document loaded successfully.");
	}
	else
	{
		LOG("Document could not be loaded.");
	}

	// Call Init() in all modules
	std::list<Module*>::iterator item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		ret = item._Ptr->_Myval->Init();
		item++;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		ret = item._Ptr->_Myval->Start();
		item++;
	}
	
	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	
	std::list<Module*>::iterator item = list_modules.begin();
	
	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = item._Ptr->_Myval->PreUpdate(dt);
		item++;
	}

	item = list_modules.begin();

	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = item._Ptr->_Myval->Update(dt);
		item++;
	}

	item = list_modules.begin();

	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = item._Ptr->_Myval->PostUpdate(dt);
		item++;
	}
	
	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	
	std::list<Module*>::iterator item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		ret = item._Ptr->_Myval->CleanUp();
		item++;
	}
	
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

// --------------------------------------------------------------

void Application::RequestBrowser(const char* website_url)
{
	ShellExecuteA(NULL, "open", website_url, NULL, NULL, 0);
}

// Save & Load ---------------------------
void Application::CallSave()
{
	std::list<Module*>::iterator iterator = list_modules.begin();

	while (iterator != list_modules.end())
	{
		iterator._Ptr->_Myval->Save();
		iterator++;
	}

	LOG("Saving engine's configuration...");
}

void Application::CallLoad()
{
	std::list<Module*>::iterator iterator = list_modules.begin();

	while (iterator != list_modules.end())
	{
		iterator._Ptr->_Myval->Load();
		iterator++;
	}

	LOG("Loading engine's configuration...");
}