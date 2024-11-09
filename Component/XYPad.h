#pragma once

#include <JuceHeader.h>

class XYPad : public Component
{
public:
    class Thumb : public Component
    {
    public:
        Thumb();
        void paint(Graphics &g) override;
        void mouseDown(const MouseEvent &event) override;
        void mouseDrag(const MouseEvent &event) override;
        std::function<void(Point<double>)> moveCallback;

    private:
        ComponentDragger dragger;
        ComponentBoundsConstrainer constrainer;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Thumb)
    };

    XYPad(AudioProcessorValueTreeState &stateToUse, const String &xID, const String &yID);
    void resized() override;
    void paint(Graphics &g) override;

private:
    void valueChanged();

    Slider xVal, yVal;
    AudioProcessorValueTreeState::SliderAttachment xAth, yAth;
    
    Thumb thumb;
    Image background;
    static constexpr int thumbSize = 61;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(XYPad)
};
