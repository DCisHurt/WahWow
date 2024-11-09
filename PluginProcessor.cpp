#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WahWowAudioProcessor::WahWowAudioProcessor()
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                         .withInput("Input", AudioChannelSet::stereo(), true)
#endif
                         .withOutput("Output", AudioChannelSet::stereo(), true)
#endif
      )
{
}

WahWowAudioProcessor::~WahWowAudioProcessor()
{
}

//==============================================================================
const String WahWowAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WahWowAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool WahWowAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool WahWowAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double WahWowAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int WahWowAudioProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
              // so this should be at least 1, even if you're not really implementing programs.
}

int WahWowAudioProcessor::getCurrentProgram()
{
    return 0;
}

void WahWowAudioProcessor::setCurrentProgram(int index)
{
    ignoreUnused(index);
}

const String WahWowAudioProcessor::getProgramName(int index)
{
    ignoreUnused(index);
    return {};
}

void WahWowAudioProcessor::changeProgramName(int index, const String &newName)
{
    ignoreUnused(index, newName);
}

//==============================================================================
void WahWowAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 1;

    updateParameters();

    effectChain.prepare(spec);

    rmsdB.reset(sampleRate, 0.5);
    rmsdB.setCurrentAndTargetValue(-100.f);
}

void WahWowAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool WahWowAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
#if JucePlugin_IsMidiEffect
    ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono() && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

        // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}

void WahWowAudioProcessor::processBlock(AudioBuffer<float> &buffer,
                                        MidiBuffer &midiMessages)
{
    ignoreUnused(midiMessages);
    ScopedNoDenormals noDenormals;

    updateParameters();

    auto nSamples = buffer.getNumSamples();

    AudioBuffer<float> monnoBuffer;

    monnoBuffer.setSize(1, nSamples);
    monnoBuffer.clear();
    monnoBuffer.addFrom(0, 0, buffer, 1, 0, nSamples, 0.5f);
    monnoBuffer.addFrom(0, 0, buffer, 0, 0, nSamples, 0.5f);

    dsp::AudioBlock<float> block{monnoBuffer};

    effectChain.process(dsp::ProcessContextReplacing<float>(block));

    rmsdB.skip(nSamples);
    {
        const auto value = Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, nSamples));
        if (value < rmsdB.getCurrentValue())
            rmsdB.setTargetValue(value);
        else
            rmsdB.setCurrentAndTargetValue(value);
    }

    buffer.copyFrom(0, 0, monnoBuffer, 0, 0, nSamples);
    buffer.copyFrom(1, 0, monnoBuffer, 0, 0, nSamples);
}

//==============================================================================
bool WahWowAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor *WahWowAudioProcessor::createEditor()
{
    return new WrappedEditor(*this);
    // return new GenericAudioProcessorEditor(*this);
}

//==============================================================================
void WahWowAudioProcessor::getStateInformation(MemoryBlock &destData)
{
    MemoryOutputStream mos(destData, true);
    apvts.state.writeToStream(mos);
}

void WahWowAudioProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    auto tree = ValueTree::readFromData(data, sizeInBytes);
    if (tree.isValid())
    {
        apvts.replaceState(tree);
        updateParameters();
    }
}

ChainSettings getChainSettings(AudioProcessorValueTreeState &apvts)
{
    ChainSettings settings;

    settings.outputGain = apvts.getRawParameterValue("Output Level")->load();
    settings.wild = apvts.getRawParameterValue("Wild")->load();
    settings.qValue = apvts.getRawParameterValue("Q")->load();
    settings.wahRatio = apvts.getRawParameterValue("Wah Frequency")->load();
    settings.isAuto = apvts.getParameter("Auto")->getValue();
    settings.isBypassed = apvts.getParameter("Bypass")->getValue();

    return settings;
}

AudioProcessorValueTreeState::ParameterLayout WahWowAudioProcessor::createParameterLayout()
{
    AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<AudioParameterFloat>("Output Level",
                                                     "Output Level",
                                                     NormalisableRange<float>(-48.0f, 24.0f, 0.1f, 1.0f),
                                                     0.0f));

    layout.add(std::make_unique<AudioParameterFloat>("Wah Frequency",
                                                     "Wah Frequency",
                                                     NormalisableRange<float>(0.0f, 1.0f, 0.01f, 1.0f),
                                                     0.0f));

    layout.add(std::make_unique<AudioParameterFloat>("Wild",
                                                     "Wild",
                                                     NormalisableRange<float>(0.0f, 1.0f, 0.01f, 1.0f),
                                                     0.5f));

    layout.add(std::make_unique<AudioParameterFloat>("Q",
                                                     "Q",
                                                     NormalisableRange<float>(3.0f, 9.0f, 0.01f, 1.0f),
                                                     6.0f));

    layout.add(std::make_unique<AudioParameterBool>("Auto",
                                                    "Auto",
                                                    false));

    layout.add(std::make_unique<AudioParameterBool>("Bypass",
                                                    "Bypass",
                                                    false));

    return layout;
}

void WahWowAudioProcessor::updateParameters()
{
    auto chainSettings = getChainSettings(apvts);

    effectChain.setBypassed<0>(chainSettings.isBypassed);
    effectChain.setBypassed<1>(chainSettings.isBypassed);

    updateWah(chainSettings);
    updateGain(chainSettings);
}

void WahWowAudioProcessor::updateGain(const ChainSettings &chainSettings)
{
    auto &gainOut = effectChain.get<ChainPositions::outputGain>();
    gainOut.setGainDecibels(chainSettings.outputGain);
}

void WahWowAudioProcessor::updateWah(const ChainSettings &chainSettings)
{
    auto &wahFilter = effectChain.get<ChainPositions::wahFilter>();

    auto focusRate = 1.0f - chainSettings.wild;
    auto minFreq = (max_lowfreq - min_lowfreq) * focusRate + min_lowfreq;
    auto maxFreq = (max_highfreq - min_highfreq) * (1.0f - focusRate) + min_highfreq;

    wahFilter.setAuto(chainSettings.isAuto);
    wahFilter.setMaxFreq(maxFreq);
    wahFilter.setMinFreq(minFreq);
    wahFilter.setQvalue(chainSettings.qValue);
    wahFilter.setSensitivity(chainSettings.qValue / 20.0f);

    if (chainSettings.isAuto)
        apvts.getParameter("Wah Frequency")->setValueNotifyingHost(wahFilter.getWahPosition());
    else
        wahFilter.setWahFreqRatio(chainSettings.wahRatio);
}

float WahWowAudioProcessor::getRmsValue()
{
    return rmsdB.getCurrentValue();
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new WahWowAudioProcessor();
}
