#pragma once

#include "PluginProcessor.h"
#include "Component/XYPad.h"
#include "Component/MyImageButton.h"
#include "Component/MyPowerButton.h"
#include "Component/MyArrowButton.h"
#include "Component/PresetPanel.h"
#include "Component/MySlider.h"
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

    MySlider outputGain = MySlider("dB");
    MySlider wahPosition = MySlider("wah");

    XYPad xyPad; 

    MyImageButton isAuto = MyImageButton("Auto", ImageCache::getFromMemory(BinaryData::auto_button_png, BinaryData::auto_button_pngSize));
    MyImageButton isBypassed = MyImageButton("Bypass", ImageCache::getFromMemory(BinaryData::bypass_button_png, BinaryData::bypass_button_pngSize));

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
