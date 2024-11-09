#pragma once

#include "PluginProcessor.h"
#include "Component/XYPad.h"
#include "Component/MyTextButton.h"
#include "Component/MyPowerButton.h"
#include "Component/MyArrowButton.h"
#include "Component/PresetPanel.h"


//==============================================================================
class WahWowAudioProcessorEditor : public Component, public Timer
{
public:
    explicit WahWowAudioProcessorEditor(WahWowAudioProcessor &);
    ~WahWowAudioProcessorEditor() override;

    //==============================================================================
    void timerCallback() override;
    void paint(Graphics &) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    WahWowAudioProcessor &processorRef;
    Gui::PresetPanel presetPanel;
    Image background;

    Slider outputGain = Slider(" dB");
    Slider wahPosition = Slider();

    XYPad xyPad;

    MyTextButton isAuto = MyTextButton("Auto");
    MyPowerButton isBypassed = MyPowerButton();

    const String title = "Cab Impulse";

    Label MyLabel = Label(title, title);

    AudioProcessorValueTreeState::SliderAttachment outputGainAth, wahPositionAth;

    AudioProcessorValueTreeState::ButtonAttachment isAutoAth, isBypassedAth;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WahWowAudioProcessorEditor);
};

// Root Wrapper

class WrappedEditor : public AudioProcessorEditor
{
public:
    WrappedEditor(WahWowAudioProcessor &);
    void resized() override;

private:
    static constexpr double ratio = (1600.0 / 1000.0);
    static constexpr int originalWidth{1600}, originalHeight{1000};

    WahWowAudioProcessorEditor Component;
    ApplicationProperties applicationProperties;
};
