#pragma once

#include <JuceHeader.h>
#include "DSP/WahFilter.h"
#include "Utility/Parameter.h"
#include "Utility/PresetManager.h"
//==============================================================================

class WahWowAudioProcessor : public AudioProcessor
{
public:
    //==============================================================================
    WahWowAudioProcessor();
    ~WahWowAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;

    void processBlock(AudioBuffer<float> &, MidiBuffer &) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    AudioProcessorEditor *createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const String getProgramName(int index) override;
    void changeProgramName(int index, const String &newName) override;

    //==============================================================================
    void getStateInformation(MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;
    //==============================================================================
    float getRmsValue();
    //==============================================================================

    using PM = Service::ParameterManager;

    AudioProcessorValueTreeState apvts{*this, nullptr, "Parameters", PM::createParameterLayout()};

    using Filter = WahFilter<float>;
    using Gain = dsp::Gain<float>;
    using MonoChain = dsp::ProcessorChain<Filter, Gain>;

    MonoChain effectChain;

    enum ChainPositions
    {
        wahFilter,
        outputGain
    };

    Service::PresetManager &getPresetManager() { return *presetManager; }

private:
    std::unique_ptr<Service::PresetManager> presetManager;
    
    void updateWah(const PM::Parameters &settings);
    void updateGain(const PM::Parameters &settings);
    void updateParameters();
    LinearSmoothedValue<float> rmsdB;

    float min_lowfreq = 150.0f, max_lowfreq = 400.0f, min_highfreq = 1500.0f, max_highfreq = 2700.0f;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WahWowAudioProcessor)
};
