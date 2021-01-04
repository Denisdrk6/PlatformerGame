#include "GuiSlider.h"

GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::SLIDER, id)
{
    this->bounds = bounds;
    this->text = text;
    this->maxValue = 100;
    this->minValue = 0;
    innerRect = bounds;
    innerRect.h = 40;
    innerRect.w = 40;
    innerRect.y += bounds.h/4;
}

GuiSlider::~GuiSlider()
{
}

bool GuiSlider::Update(Input* input, float dt)
{
    if (state != GuiControlState::DISABLED)
    {
        input->GetMousePosition(mouseX, mouseY);

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
                NotifyObserver();
            }
        }
        else state = GuiControlState::NORMAL;
    }


    return false;
}

bool GuiSlider::Draw(Render* render)
{
    // Draw the right button depending on state
  
    switch (state)
    {
    case GuiControlState::DISABLED: render->DrawRectangle(bounds, { 100, 100, 100, 255 });
        break;
    case GuiControlState::NORMAL: render->DrawRectangle(bounds, { 0, 255, 0, 255 });
        break;
    case GuiControlState::FOCUSED: render->DrawRectangle(bounds, { 255, 255, 0, 255 });
        break;
    case GuiControlState::PRESSED:
        render->DrawRectangle(bounds, { 0, 255, 255, 255 });
        sliderRect = { bounds.x, bounds.y, mouseX - bounds.x, bounds.h };
        innerRect.x = sliderRect.w + sliderRect.x;
        innerRect.x -= innerRect.w/2;
        break;
    case GuiControlState::SELECTED: render->DrawRectangle(bounds, { 0, 255, 0, 255 });
        break;
    default:
        break;
    }

    if (state == GuiControlState::PRESSED)
        value = (int)(sliderRect.w / (bounds.w / maxValue));

    render->DrawRectangle(sliderRect, { 0, 0, 255, 255 });
    if (sliderRect.x == bounds.x) render->DrawRectangle(innerRect, { 255, 0, 0, 255 });

    return false;
}
