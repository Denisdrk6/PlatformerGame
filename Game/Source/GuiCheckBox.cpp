#include "GuiCheckBox.h"

GuiCheckBox::GuiCheckBox(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::CHECKBOX, id)
{
    this->bounds = bounds;
    this->text = text;
}

GuiCheckBox::~GuiCheckBox()
{
}

bool GuiCheckBox::Update(Input* input, float dt, bool camera, Render* render, Audio* audio, int hover, int click)
{
    bool ret = true;

    if (state != GuiControlState::DISABLED)
    {
        int mouseX, mouseY;
        input->GetMousePosition(mouseX, mouseY);

        if (camera == true)
        {
            mouseX = mouseX + render->camera.x * -1;
            mouseY = mouseY + render->camera.y * -1;
        }

        // Check collision between mouse and button bounds
        if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) && 
            (mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
        {
            if (state != GuiControlState::FOCUSED && state != GuiControlState::PRESSED) audio->PlayFx(hover);

            state = GuiControlState::FOCUSED;

            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
            {
                state = GuiControlState::PRESSED;
            }

            // If mouse button pressed -> Generate event!
            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
            {
                checked = !checked;
                ret = NotifyObserver();
                audio->PlayFx(click);
            }
        }
        else state = GuiControlState::NORMAL;
    }

    return ret;
}

bool GuiCheckBox::Draw(Render* render)
{
    // Draw the right button depending on state
    if (!debugDraw)
    {
        switch (state)
        {
        case GuiControlState::DISABLED:
        {
            if (checked) render->DrawRectangle(bounds, { 100, 0, 100, 100 });
            else render->DrawRectangle(bounds, { 0, 0, 0, 100 });
        } break;
        case GuiControlState::NORMAL:
        {
            if (checked) render->DrawRectangle(bounds, { 255, 255, 255, 125 });
            else render->DrawRectangle(bounds, { 0, 255, 0, 0 });
        } break;
        case GuiControlState::FOCUSED: render->DrawRectangle(bounds, { 255, 255, 255, 50 });
            break;
        case GuiControlState::PRESSED: render->DrawRectangle(bounds, { 0, 0, 255, 100 });
            break;
        case GuiControlState::SELECTED: render->DrawRectangle(bounds, { 0, 255, 0, 255 });
            break;
        default:
            break;
        }
    }

    else
    {
        switch (state)
        {
        case GuiControlState::DISABLED:
        {
            if (checked) render->DrawRectangle(bounds, { 100, 0, 100, 255 });
            else render->DrawRectangle(bounds, { 0, 0, 0, 255 });
        } break;
        case GuiControlState::NORMAL:
        {
            if (checked) render->DrawRectangle(bounds, { 255, 255, 255, 255 });
            else render->DrawRectangle(bounds, { 0, 255, 0, 255 });
        } break;
        case GuiControlState::FOCUSED: render->DrawRectangle(bounds, { 255, 255, 0, 255 });
            break;
        case GuiControlState::PRESSED: render->DrawRectangle(bounds, { 0, 0, 255, 255 });
            break;
        case GuiControlState::SELECTED: render->DrawRectangle(bounds, { 0, 255, 0, 255 });
            break;
        default:
            break;
        }
    }

    return false;
}
