#include "ButtonControl.h"

ButtonControl::ButtonControl()
{
}

bool ButtonControl::WasPressedThisFrame() const
{
    return false;
}

bool ButtonControl::WasReleasedThisFrame() const
{
    return false;
}

bool ButtonControl::IsPressed() const
{
    return false;
}

bool ButtonControl::IsValueConsideredPressed(float value)
{


    return false;
}
