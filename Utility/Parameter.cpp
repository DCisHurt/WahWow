#include "Parameter.h"

namespace Service

{

    AudioProcessorValueTreeState::ParameterLayout ParameterManager::createParameterLayout()
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

    ParameterManager::Parameters ParameterManager::getParameters(AudioProcessorValueTreeState &apvts)
    {
        Parameters settings;

        settings.outputGain = apvts.getRawParameterValue("Output Level")->load();
        settings.wild = apvts.getRawParameterValue("Wild")->load();
        settings.qValue = apvts.getRawParameterValue("Q")->load();
        settings.wahRatio = apvts.getRawParameterValue("Wah Frequency")->load();
        settings.isAuto = apvts.getParameter("Auto")->getValue();
        settings.isBypassed = apvts.getParameter("Bypass")->getValue();

        return settings;
    }
}
