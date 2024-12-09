#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WahWowAudioProcessorEditor::WahWowAudioProcessorEditor(WahWowAudioProcessor &p)
    : processorRef(p),
      presetPanel(p.getPresetManager()),
      outputGainAth{p.apvts, "Output Level", outputGain},
      wahPositionAth{p.apvts, "Wah Frequency", wahPosition},
      isAutoAth{p.apvts, "Auto", isAuto},
      xyPad{p.apvts, "Wild", "Q"},
      isBypassedAth{p.apvts, "Bypass", isBypassed}
{
    // juce::ignoreUnused (processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    addAndMakeVisible(outputGain);
    addAndMakeVisible(wahPosition);
    addAndMakeVisible(isAuto);
    addAndMakeVisible(isBypassed);
    addAndMakeVisible(xyPad);
    addAndMakeVisible(presetPanel);

    outputGain.setSliderStyle(Slider::SliderStyle::LinearVertical);
    wahPosition.setSliderStyle(Slider::SliderStyle::LinearVertical);
    wahPosition.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    outputGain.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

    startTimerHz(30);
}

WahWowAudioProcessorEditor::~WahWowAudioProcessorEditor()
{
}

//==============================================================================
void WahWowAudioProcessorEditor::timerCallback()
{
    // DBG(processorRef.getRmsValue());
}
//==============================================================================
void WahWowAudioProcessorEditor::paint(juce::Graphics &g)
{
    g.drawImage(ImageCache::getFromMemory(BinaryData::background_png, BinaryData::background_pngSize), getLocalBounds().toFloat());
}

void WahWowAudioProcessorEditor::resized()
{
    isBypassed.setBounds(1280, 850, 125, 68);
    xyPad.setBounds(141, 360, 540, 540);
    wahPosition.setBounds(972, 358, 53, 470);
    isAuto.setBounds(936, 850, 125, 68);
    outputGain.setBounds(1316, 358, 53, 470);
    presetPanel.setBounds(1160, 75, 350, 180);
}

WrappedEditor::WrappedEditor(WahWowAudioProcessor &p) : AudioProcessorEditor(p),
                                                        Component(p)
{
    addAndMakeVisible(Component);

    PropertiesFile::Options options;
    options.applicationName = ProjectInfo::projectName;
    options.commonToAllUsers = true;
    options.filenameSuffix = "settings";
    options.osxLibrarySubFolder = "Application Support";
    applicationProperties.setStorageParameters(options);

    getConstrainer()->setFixedAspectRatio(ratio);
    getConstrainer()->setSizeLimits(int(originalWidth / 2.5), int(originalHeight / 2.5),
                                    int(originalWidth * 1.5), int(originalHeight * 1.5));

    auto sizeRatio{1.0};

    sizeRatio = applicationProperties.getCommonSettings(true)->getDoubleValue("sizeRatio", 1.0);

    setResizable(true, true);
    setSize(int(originalWidth * sizeRatio), int(originalHeight * sizeRatio));
}

void WrappedEditor::resized()
{
    const auto scaleFactor = float(getWidth()) / originalWidth;

    applicationProperties.getCommonSettings(true)->setValue("sizeRatio", scaleFactor);

    Component.setTransform(AffineTransform::scale(scaleFactor));
    Component.setBounds(0, 0, originalWidth, originalHeight);
}