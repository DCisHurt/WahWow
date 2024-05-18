#pragma once

#include <JuceHeader.h>

//==============================================================================

// template <typename SampleType>
// class SMA {
//   public:

//     SampleType process(SampleType input) {
//         sum -= previousInputs[index];
//         sum += input;
//         previousInputs[index] = input;
//         if (++index == filterLength)
//             index = 0;
//         return sum / (SampleType)filterLength;
//     }
//     SMA(){
//     };

//   private:
//     int index = 0;
//     int filterLength = 1000;
//     SampleType previousInputs[1000]={};
//     SampleType sum = 0;
// };

// template class SMA<float>;
// template class SMA<double>;


template <typename SampleType>
class EnvelopeFollower
{
public:
    //==============================================================================
    /** Constructor. */
    EnvelopeFollower();

    //==============================================================================
    /** Sets the attack time in milliseconds of the compressor.*/
    void setSensitivity (SampleType newSensitivity);

    /** Sets the release time in milliseconds of the compressor.*/
    void setLowPass (SampleType newFreq);

    /** Sets the release time in milliseconds of the compressor.*/
    void seNormalise (bool status);
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
        }
        
        #if JUCE_DSP_ENABLE_SNAP_TO_ZERO
            lowpassFilter.snapToZero();
        #endif
        
    }

    /** Performs the processing operation on a single sample at a time. */
    SampleType processSample (SampleType inputValue);


private:
    //==============================================================================
    void update();

    //==============================================================================
    dsp::BallisticsFilter<SampleType> envelopeFollower;
    dsp::IIR::Filter<SampleType> lowpassFilter;

    double sampleRate = 44100.0;
    bool isNormalised = true;
    SampleType sensitivity = 1.0;
    SampleType attackTime = 40.0, releaseTime = 90.0;
    SampleType lowPassFreq = 15.0;
    SampleType maxEnvlope = (SampleType)0.05;
    // SMA<SampleType> avgFilter;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EnvelopeFollower)
};


