#ifndef __GUICHECKBOX_H__
#define __GUICHECKBOX_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiCheckBox : public GuiControl
{
public:

    GuiCheckBox(uint32 id, SDL_Rect bounds, const char *text);
    virtual ~GuiCheckBox();

    bool Update(Input* input, float dt, bool camera, Render* render);
    bool Draw(Render* render);

    bool checked;

private:

    // GuiCheckBox specific properties
    // Maybe some animation properties for state change?
    
};

#endif // __GUICHECKBOX_H__
