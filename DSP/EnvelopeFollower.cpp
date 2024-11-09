
#include "EnvelopeFollower.h"

//==============================================================================
template <typename SampleType>
EnvelopeFollower<SampleType>::EnvelopeFollower()
{
    update();
}

//==============================================================================

template <typename SampleType>
void EnvelopeFollower<SampleType>::setSensitivity(SampleType newSensitivity)
{
    jassert(newSensitivity >= 0.0 && newSensitivity <= 1.0);
    sensitivity = newSensitivity;
    update();
}

template <typename SampleType>
void EnvelopeFollower<SampleType>::setLowPass(SampleType newFreq)
{
    lowPassFreq = newFreq;
    update();
}

template <typename SampleType>
void EnvelopeFollower<SampleType>::seNormalise(bool status)
{
    isNormalised = status;
}
//==============================================================================
template <typename SampleType>
void EnvelopeFollower<SampleType>::prepare(const dsp::ProcessSpec &spec)
{
    jassert(spec.sampleRate > 0);
    jassert(spec.numChannels > 0);

    sampleRate = spec.sampleRate;

    lowpassFilter.prepare(spec);
    envelopeFollower.prepare(spec);

    update();
    reset();
}

template <typename SampleType>
void EnvelopeFollower<SampleType>::reset()
{
    lowpassFilter.reset();
    envelopeFollower.reset();
}

//==============================================================================
template <typename SampleType>
SampleType EnvelopeFollower<SampleType>::processSample(SampleType inputValue)
{
    // Ballistics filter with peak rectifier
    auto env = envelopeFollower.processSample(0, inputValue);

    maxEnvlope = env > maxEnvlope ? env : maxEnvlope;

    if (isNormalised)
    {
        // env = env/maxEnvlope;
        env = (SampleType)1.0 + std::log10(env / maxEnvlope);
        env = env > (SampleType)1.0 ? (SampleType)1.0 : env;
        env = env < (SampleType)0.0 ? (SampleType)0.0 : env;
    }
    env = lowpassFilter.processSample(env);

    env = env > (SampleType)1.0 ? (SampleType)1.0 : env;
    env = env < (SampleType)0.0 ? (SampleType)0.0 : env;
    return env;
    // return avgFilter.process(env);
}

template <typename SampleType>
void EnvelopeFollower<SampleType>::update()
{
    envelopeFollower.setAttackTime(attackTime / sensitivity);
    envelopeFollower.setReleaseTime(releaseTime / sensitivity);
    lowpassFilter.coefficients = dsp::IIR::Coefficients<SampleType>::makeLowPass(sampleRate, lowPassFreq);
}

//==============================================================================
template class EnvelopeFollower<float>;
template class EnvelopeFollower<double>;
