#pragma once

#include <JuceHeader.h>

class MyTextButton : public TextButton
{
public:
    enum ColourIds
    {
        switchColour = 0x101000,
        switchOnBackgroundColour,
        switchOffBackgroundColour
    };

    MyTextButton(String name)
        : TextButton(name)
    {
        this->setClickingTogglesState(true);
    }
};