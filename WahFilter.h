#pragma once

#include <JuceHeader.h>
#include "EnvelopeFollower.h"
//==============================================================================


template <typename SampleType>
class WahFilter
{
public:
    //==============================================================================
    /** Constructor. */
    WahFilter();

    //==============================================================================
    /** Sets the threshold in dB of the compressor.*/
    void setMinFreq (SampleType newMinFreq);

    /** Sets the ratio of the compressor (must be higher or equal to 1).*/
    void setMaxFreq (SampleType newMaxFreq);

    /** Sets the ratio of the compressor (must be higher or equal to 1).*/
    void setQvalue (SampleType newQvalue);    
    
    /** Sets the ratio of the compressor (must be higher or equal to 1).*/
    void setWahFreqRatio (SampleType newWahFreqRatio);

    void setSensitivity (SampleType newSensitivity);
    /** Sets the release time in milliseconds of the compressor.*/
    void setAuto (bool status);

    SampleType getWahPosition ();

    //==============================================================================
    /** Initialises the processor. */
    void prepare (const dsp::ProcessSpec& spec);

    /** Resets the internal state variables of the processor. */
    void reset();

    //==============================================================================
    /** Processes the input and output samples supplied in the processing context. */
    template <typename ProcessContext>
    void process (const ProcessContext& context) noexcept
    {
        auto&& inputBlock  = context.getInputBlock();
        auto&& outputBlock = context.getOutputBlock();

        // This class can only process mono signals. Use the ProcessorDuplicator class
        // to apply this filter on a multi-channel audio stream.
        jassert (inputBlock.getNumChannels()  == 1);
        jassert (outputBlock.getNumChannels() == 1);

        auto numSamples = inputBlock.getNumSamples();
        auto* src = inputBlock .getChannelPointer (0);
        auto* dst = outputBlock.getChannelPointer (0);

        if (context.isBypassed)
        {
            outputBlock.copyFrom (inputBlock);
            return;
        }

        for (size_t i = 0; i < numSamples; ++i){
            dst[i] = processSample (src[i]);
            viewer.pushSample(&d, 1);
        }

        #if JUCE_DSP_ENABLE_SNAP_TO_ZERO
            wah.snapToZero();
        #endif
    }

    /** Performs the processing operation on a single sample at a time. */
    SampleType processSample (SampleType inputValue);
    AudioVisualiserComponent viewer = AudioVisualiserComponent(1);
private:
    //==============================================================================
    void update();

    //==============================================================================
    dsp::IIR::Filter<SampleType> wah;
    
    EnvelopeFollower<SampleType> envelopeFilter;
    float d;

    int count = 0;

    double sampleRate = 44100.0;
    bool isAuto = false;
    SampleType minFreq = 200.0, maxFreq = 2000.0, qValue = 5.0, wahFreqRatio = 0.5;
    SampleType sensitivity = 1.0;
    SampleType wahFreq = 1000.0;
    SampleType makeupBoost = (SampleType)5.0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WahFilter)
};
