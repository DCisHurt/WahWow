#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WahWowAudioProcessorEditor::WahWowAudioProcessorEditor (WahWowAudioProcessor& p)
    : processorRef (p),
      outputGainAth{p.apvts, "Output Level", outputGain},
      wahPositionAth{p.apvts, "Wah Frequency", wahPosition},
      isAutoAth{p.apvts, "Auto", isAuto},
      xyPad{p.apvts, "Wild", "Q"},
      isBypassedAth{p.apvts, "Bypass", isBypassed}
{    
    // juce::ignoreUnused (processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    auto &wahFilter = p.effectChain.get<p.ChainPositions::wahFilter>();
    addAndMakeVisible(outputGain);
    addAndMakeVisible(wahPosition);
    addAndMakeVisible(isAuto);
    addAndMakeVisible(isBypassed);
    addAndMakeVisible(xyPad);
    addAndMakeVisible(wahFilter.viewer);

    outputGain.setSliderStyle(Slider::SliderStyle::LinearVertical);
    wahPosition.setSliderStyle(Slider::SliderStyle::LinearVertical);
    wahPosition.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    outputGain.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

    startTimerHz(30);
    wahFilter.viewer.setColours(juce::Colours::black, juce::Colours::whitesmoke);
}

WahWowAudioProcessorEditor::~WahWowAudioProcessorEditor()
{
}

//==============================================================================
void WahWowAudioProcessorEditor::timerCallback()
{
    DBG(processorRef.getRmsValue());
}
//==============================================================================
void WahWowAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colour(0xFF2C2C2C));
    g.setColour(juce::Colour(0xFFC3C3C3));
    g.setFont(12.0f);
}

void WahWowAudioProcessorEditor::resized()
{
    auto &wahFilter = processorRef.effectChain.get<processorRef.ChainPositions::wahFilter>();
    
    isBypassed.setBounds(23, 15, 35, 35);
    xyPad.setBounds(20, 80, 280, 280);
    wahPosition.setBounds(408, 85, 20, 220);
    isAuto.setBounds(385, 318, 65, 25);
    outputGain.setBounds(550, 66, 50, 304);
    wahFilter.viewer.setBounds(327, 350, 185, 18);
}

WrappedEditor::WrappedEditor(WahWowAudioProcessor& p) :
    AudioProcessorEditor(p),
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
    getConstrainer()->setSizeLimits(int(originalWidth/1.5), int(originalHeight/1.5),
                                    int(originalWidth*2.5), int(originalHeight*2.5));

    auto sizeRatio{ 1.0 };

    sizeRatio = applicationProperties.getCommonSettings(true)->getDoubleValue("sizeRatio", 1.0);

    setResizable(true, true);
    setSize(int(originalWidth * sizeRatio), int(originalHeight * sizeRatio));
}

void WrappedEditor::resized()
{
    const auto scaleFactor = float(getWidth())/originalWidth;

    applicationProperties.getCommonSettings(true)->setValue("sizeRatio", scaleFactor);

    Component.setTransform(AffineTransform::scale(scaleFactor));
    Component.setBounds(0, 0, originalWidth, originalHeight);
}