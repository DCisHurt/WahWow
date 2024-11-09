#pragma once

#include <JuceHeader.h>

namespace Service
{
	class PresetManager
	{
	public:
		PresetManager(AudioProcessorValueTreeState &);

		void loadPreset(int index);
		int loadNextPreset();
		int loadPreviousPreset();
		StringArray getAllPresets() const;
		String getCurrentPreset() const;

	private:
		AudioProcessorValueTreeState &valueTreeState;
		int currentPresetIndex = 0;
		const StringArray presetName = {
			"A",
			"B",
			"Wild"};

		const StringArray parameters = {
			"Output Level",
			"Wah Frequency",
			"Wild",
			"Q",
			"Auto",
			"Bypass"};
		std::vector<std::vector<float>> presets;
	};
}