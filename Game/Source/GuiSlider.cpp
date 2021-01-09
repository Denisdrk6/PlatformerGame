#include "GuiSlider.h"

GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::SLIDER, id)
{
    this->bounds = bounds;
    this->text = text;
    this->maxValue = 128;
    this->minValue = 0;
    innerRect = bounds;
    innerRect.h = 40;
    innerRect.w = 40;
    innerRect.y += bounds.h/4;
}

GuiSlider::~GuiSlider()
{
}

bool GuiSlider::Update(Input* input, float dt, bool camera, Render* render)
{
    bool ret = true;

    if (state != GuiControlState::DISABLED)
    {
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
            state = GuiControlState::FOCUSED;

            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
            {
                state = GuiControlState::PRESSED;
            }

            // If mouse button pressed -> Generate event!
            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
            {
               ret = NotifyObserver();
            }
        }
        else state = GuiControlState::NORMAL;
    }


    return ret;
}

bool GuiSlider::Draw(bool camera, Render* render, Input* input)
{
    input->GetMousePosition(mouseX, mouseY);

    if (camera == true)
    {
        mouseX = mouseX + render->camera.x * -1;
        mouseY = mouseY + render->camera.y * -1;
    }

    // Draw the right button depending on state
    if (!debugDraw)
    {
        switch (state)
        {
        case GuiControlState::DISABLED: render->DrawRectangle(bounds, { 0, 0, 0, 100 });
            break;
        case GuiControlState::NORMAL: render->DrawRectangle(bounds, { 0, 255, 0, 0 });
            break;
        case GuiControlState::FOCUSED: render->DrawRectangle(bounds, { 0, 0, 255, 50 });
            break;
        case GuiControlState::PRESSED:
            render->DrawRectangle(bounds, { 0, 0, 255, 100 });
            sliderRect = { bounds.x, bounds.y, mouseX - bounds.x, bounds.h };
            innerRect.x = sliderRect.w + sliderRect.x;
            innerRect.x -= innerRect.w / 2;
            innerRect.y = sliderRect.y + sliderRect.h / 4;
            innerRect.h = sliderRect.h - sliderRect.h / 2;
            innerRect.w = innerRect.h;
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
        case GuiControlState::DISABLED: render->DrawRectangle(bounds, { 0, 0, 0, 255 });
            break;
        case GuiControlState::NORMAL: render->DrawRectangle(bounds, { 0, 255, 0, 255 });
            break;
        case GuiControlState::FOCUSED: render->DrawRectangle(bounds, { 0, 0, 255, 255 });
            break;
        case GuiControlState::PRESSED:
            render->DrawRectangle(bounds, { 255, 0, 255, 255 });
            sliderRect = { bounds.x, bounds.y, mouseX - bounds.x, bounds.h };
            innerRect.x = sliderRect.w + sliderRect.x;
            innerRect.x -= innerRect.w / 2;
            innerRect.y = sliderRect.y + sliderRect.h / 4;
            innerRect.h = sliderRect.h - sliderRect.h / 2;
            innerRect.w = innerRect.h;
            break;
        case GuiControlState::SELECTED: render->DrawRectangle(bounds, { 0, 255, 0, 255 });
            break;
        default:
            break;
        }
    }

    if (state == GuiControlState::PRESSED)
        value = (int)((float)sliderRect.w / (float)((float)bounds.w / (float)maxValue));

    if (!debugDraw)
    {
        render->DrawRectangle(sliderRect, { 0, 0, 255, 155 });
        if (sliderRect.x == bounds.x) render->DrawRectangle(innerRect, { 0, 0, 255, 200 });
    }

    else
    {
        render->DrawRectangle(sliderRect, { 0, 255, 255, 255 });
        if (sliderRect.x == bounds.x) render->DrawRectangle(innerRect, { 255, 0, 255, 255 });
    }

    return false;
}

int GuiSlider::GetValue()
{
    return value;
}