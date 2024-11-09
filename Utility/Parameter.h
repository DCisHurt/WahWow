#pragma once

#include <JuceHeader.h>

namespace Service
{
    class ParameterManager
    {
    public:
        // ParameterManager() = delete;

        struct Parameters
        {
            float outputGain{0}, wahRatio{0.5}, wild{0.5}, qValue{4};
            bool isBypassed{0}, isAuto{0};
        };

        static AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

        Parameters getParameters(AudioProcessorValueTreeState &apvts);
    };
}