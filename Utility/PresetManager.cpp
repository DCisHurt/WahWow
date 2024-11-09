#include "PresetManager.h"
namespace Service
{
	PresetManager::PresetManager(AudioProcessorValueTreeState &apvts) : valueTreeState(apvts)
	{
		presets.push_back({0.7f, 0.0f, 0.5f, 0.5f, 1.0f, 0.0f});
		presets.push_back({0.5f, 0.0f, 0.7f, 0.7f, 1.0f, 0.0f});
		presets.push_back({0.8f, 0.0f, 0.3f, 0.3f, 1.0f, 0.0f});
	}
	void PresetManager::loadPreset(int index)
	{
		std::vector<float> parametersInPreset = presets[index];

		for (int i = 0; i < parametersInPreset.size(); ++i)
		{
			valueTreeState.getParameter(parameters[i])->setValueNotifyingHost(parametersInPreset[i]);
		}
	}

	int PresetManager::loadNextPreset()
	{
		int maxIndex = presets.size() - 1;
		int index = currentPresetIndex == maxIndex ? 0 : currentPresetIndex + 1;
		currentPresetIndex = index;
		loadPreset(index);

		return index;
	}

	int PresetManager::loadPreviousPreset()
	{
		int maxIndex = presets.size() - 1;
		int index = currentPresetIndex == 0 ? maxIndex : currentPresetIndex - 1;
		currentPresetIndex = index;
		loadPreset(index);

		return index;
	}

	StringArray PresetManager::getAllPresets() const
	{
		return presetName;
	}

	String PresetManager::getCurrentPreset() const
	{
		return presetName[currentPresetIndex];
	}
}