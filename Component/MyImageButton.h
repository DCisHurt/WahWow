#pragma once

#include <JuceHeader.h>

class MyImageButton : public ToggleButton
{
public:
    MyImageButton(String name, Image image)
        : ToggleButton(name),
          newLook(image)
    {
        this->setLookAndFeel(&newLook);
    }

    ~MyImageButton()
    {
        setLookAndFeel(nullptr);
    }

private:
    class MyLook : public LookAndFeel_V4
    {
    public:
        MyLook(Image image)
        {
            myImage = image;
        }

        void drawToggleButton(Graphics &g, ToggleButton &button,
                              bool shouldDrawButtonAsHighlighted, bool) override
        {

            int frame_h = 192;
            int interval = (myImage.getHeight()-4*frame_h)/5;
            int h = interval + frame_h;
            int y = interval/2;
            y += shouldDrawButtonAsHighlighted ? h : 0;
            y += button.getToggleState() ? 2 * h : 0;
    
            g.drawImage(myImage, 0, 0, button.getWidth(), button.getHeight(), 0, y, myImage.getWidth(), h);
        }

    private:
        Image myImage;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyLook)
    };
    MyLook newLook;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyImageButton)
};