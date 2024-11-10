#pragma once

#include <JuceHeader.h>

class MyLook : public LookAndFeel_V4
{
public:
    void drawLinearSlider(Graphics &g, int x, int y, int width, int height,
                          float sliderPos, float , float ,
                          Slider::SliderStyle , Slider &slider) override
    {
        g.fillAll(slider.findColour(Slider::backgroundColourId));

        Path p;

        p.addRectangle((float)x, sliderPos, (float)width, 1.0f + (float)height - sliderPos);

        auto baseColour = slider.findColour(Slider::rotarySliderFillColourId)
                              .withMultipliedSaturation(slider.isEnabled() ? 1.0f : 0.5f)
                              .withMultipliedAlpha(0.8f);

        g.setColour(baseColour);
        g.fillPath(p);

        auto lineThickness = jmin(15.0f, (float)jmin(width, height) * 0.45f) * 0.1f;
        g.drawRect(slider.getLocalBounds().toFloat(), lineThickness);
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
