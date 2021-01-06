#include "GuiManager.h"

#include "GuiButton.h"

GuiControl* GuiManager::CreateGuiControl(GuiControlType type)
{
	GuiControl* control = nullptr;

	SDL_Rect bounds = { 0, 0, 0, 0 };

	switch (type)
	{
		// Create the corresponding GuiControl type
		//case GuiControlType::BUTTON: control = new GuiButton((uint32)1, bounds, (const char*)' ');  break;
		default: break;
	}

	// Created entities are added to the list
	if (control != nullptr) controls.Add(control);

	return control;
}

bool GuiManager::Update(float dt)
{
	accumulatedTime += dt;
	if (accumulatedTime >= updateMsCycle) doLogic = true;

	//UpdateAll(dt, doLogic);

	if (doLogic == true)
	{
		accumulatedTime = 0.0f;
		doLogic = false;
	}

	return true;
}

