#pragma once

#include <JuceHeader.h>

class MyLook : public LookAndFeel_V4
{
public:
    void drawLinearSlider(Graphics &g, int x, int y, int width, int height,
                          float sliderPos, float, float,
                          Slider::SliderStyle, Slider &) override
    {
        // g.fillAll(slider.findColour(Slider::backgroundColourId));
        g.setColour(Colour(0xD9, 0xD9, 0xD9));
        // g.fillRect(x,y-12,width,height+24);

        g.fillRoundedRectangle((float)x, (float)y - 12.0f, (float)width, (float)height + 24.0f, 12.0f);
        Path p;

        float sliderHolderHeight = (float)width * 0.2f;

        float holderY = jmap((sliderPos - 12.0f) / ((float)height - 24.0f), 6.0f, (float)height - sliderHolderHeight - 6.0f);

        p.addRectangle((float)x + (float)width * 0.075f, holderY, (float)width * 0.85f, sliderHolderHeight);

        g.setColour(Colour(0x53, 0x55, 0x4E));
        g.fillPath(p);

        // auto lineThickness = jmin(15.0f, (float)jmin(width, height) * 0.45f) * 0.1f;
        // g.drawRect(slider.getLocalBounds().toFloat(), lineThickness);
    }
};

class MySlider : public Slider
{
public:
    MySlider(String name)
        : Slider(name)
    {
        this->setLookAndFeel(&newLook);
    }

private:
    MyLook newLook;
};
