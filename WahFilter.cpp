
#include "WahFilter.h"

//==============================================================================
template <typename SampleType>
WahFilter<SampleType>::WahFilter()
{

    update();
    viewer.setRepaintRate(30);
    viewer.setBufferSize(256);
}

//==============================================================================
template <typename SampleType>
void WahFilter<SampleType>::setAuto (bool status){
    isAuto = status;
}

template <typename SampleType>
void WahFilter<SampleType>::setMinFreq (SampleType newMinFreq)
{
    jassert (newMinFreq > 0 && newMinFreq <= static_cast<SampleType> (sampleRate * 0.5));
    minFreq = newMinFreq;
}

template <typename SampleType>
void WahFilter<SampleType>::setMaxFreq(SampleType newMaxFreq)
{
    jassert (newMaxFreq > 0 && newMaxFreq <= static_cast<SampleType> (sampleRate * 0.5));
    maxFreq = newMaxFreq;
}

template <typename SampleType>
void WahFilter<SampleType>::setQvalue(SampleType newQvalue)
{
    jassert (newQvalue >= 1.0);
    qValue = newQvalue;
}

template <typename SampleType>
void WahFilter<SampleType>::setWahFreqRatio(SampleType newWahFreqRatio)
{
    jassert (newWahFreqRatio >= 0.0 && newWahFreqRatio <= 1.0);
    wahFreqRatio = newWahFreqRatio;
}

template <typename SampleType>
void WahFilter<SampleType>::setSensitivity (SampleType newSensitivity)
{
    jassert (newSensitivity >= 0.0 && newSensitivity <= 1.0);
    sensitivity = newSensitivity;
    update();
}

template <typename SampleType>
SampleType WahFilter<SampleType>::getWahPosition ()
{
    return wahFreqRatio;
}
//==============================================================================
template <typename SampleType>
void WahFilter<SampleType>::prepare (const dsp::ProcessSpec& spec)
{
    jassert (spec.sampleRate > 0);
    jassert (spec.numChannels > 0);

    sampleRate = spec.sampleRate;

    wah.prepare (spec);
    envelopeFilter.prepare (spec);

    update();
    reset();
}

template <typename SampleType>
void WahFilter<SampleType>::reset()
{
    wah.reset();
    envelopeFilter.reset();
}

//==============================================================================
template <typename SampleType>
SampleType WahFilter<SampleType>::processSample (SampleType inputValue)
{
    if (isAuto){
        wahFreqRatio = envelopeFilter.processSample (inputValue);
    }
    d = static_cast<float>((wahFreqRatio-0.5)*2.0);

    wahFreq = static_cast<SampleType>(std::pow(10,std::log10(minFreq) + std::log10(maxFreq/minFreq)*wahFreqRatio));

    // makeup
    auto gain = Decibels::decibelsToGain(qValue-(SampleType)1.0 + makeupBoost);

    wah.coefficients = dsp::IIR::Coefficients<SampleType>::makeBandPass(sampleRate, wahFreq, qValue);

    return wah.processSample(inputValue)*gain;
}

template <typename SampleType>
void WahFilter<SampleType>::update()
{
    envelopeFilter.setSensitivity (sensitivity);
}

//==============================================================================
template class WahFilter<float>;
template class WahFilter<double>;

