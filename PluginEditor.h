#pragma once

#include "PluginProcessor.h"
#include "XYPad.h"
#include "MyTextButton.h"
#include "MyPowerButton.h"

//==============================================================================
class WahWowAudioProcessorEditor  : public Component, public Timer
{
public:
    explicit WahWowAudioProcessorEditor (WahWowAudioProcessor&);
    ~WahWowAudioProcessorEditor() override;

    //==============================================================================
    void timerCallback() override;
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    WahWowAudioProcessor& processorRef;

    Slider outputGain = Slider(" dB");
    Slider wahPosition = Slider();
    
    XYPad xyPad;

    MyTextButton isAuto = MyTextButton("Auto");
    MyPowerButton isBypassed = MyPowerButton();

    const String title = "Cab Impulse";

    Label MyLabel = Label(title, title);

    AudioProcessorValueTreeState::SliderAttachment outputGainAth, wahPositionAth;

    AudioProcessorValueTreeState::ButtonAttachment isAutoAth, isBypassedAth;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WahWowAudioProcessorEditor)
};


// Root Wrapper

class WrappedEditor : public AudioProcessorEditor
{
public:
    WrappedEditor(WahWowAudioProcessor&);
    void resized() override;

private:
    static constexpr double ratio = 1.618;
    static constexpr int originalWidth{ 600 }, originalHeight {int(originalWidth/ratio)};
    
    WahWowAudioProcessorEditor Component;
    ApplicationProperties applicationProperties;
};
