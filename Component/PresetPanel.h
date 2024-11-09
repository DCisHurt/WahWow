#pragma once

#include <JuceHeader.h>
#include "../Utility/PresetManager.h"

namespace Gui
{
	class PresetPanel : public Component, Button::Listener, ComboBox::Listener
	{
	public:
		PresetPanel(Service::PresetManager& pm) : presetManager(pm)
		{
			configureButton(previousPresetButton, "<");
			configureButton(nextPresetButton, ">");

			presetList.setTextWhenNothingSelected("No Preset Selected");
			presetList.setMouseCursor(MouseCursor::PointingHandCursor);
			addAndMakeVisible(presetList);
			presetList.addListener(this);
			
			loadPresetList();
		}

		~PresetPanel()
		{
			previousPresetButton.removeListener(this);
			nextPresetButton.removeListener(this);
			presetList.removeListener(this);
		}

		void resized() override
		{
			const auto container = getLocalBounds().reduced(4);
			auto bounds = container;
			presetList.setBounds(bounds.removeFromTop(container.proportionOfHeight(0.6f)).reduced(4));
			bounds.removeFromTop(container.proportionOfHeight(0.05f));
			bounds.removeFromLeft(bounds.proportionOfWidth(0.65f));
			auto buttonBounds = bounds;
			previousPresetButton.setBounds(bounds.removeFromLeft(container.proportionOfWidth(0.16f)).reduced(4));
			nextPresetButton.setBounds(buttonBounds.removeFromRight(container.proportionOfWidth(0.16f)).reduced(4));
		}
	private:
		void buttonClicked(Button* button) override
		{
			if (button == &previousPresetButton)
			{
				const auto index = presetManager.loadPreviousPreset();
				presetList.setSelectedItemIndex(index, dontSendNotification);
			}
			if (button == &nextPresetButton)
			{
				const auto index = presetManager.loadNextPreset();
				presetList.setSelectedItemIndex(index, dontSendNotification);
			}
		}
		void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override
		{
			if (comboBoxThatHasChanged == &presetList)
			{
				presetManager.loadPreset(presetList.getSelectedItemIndex());
			}
		}

		void configureButton(Button& button, const String& buttonText) 
		{
			button.setButtonText(buttonText);
			button.setMouseCursor(MouseCursor::PointingHandCursor);
			addAndMakeVisible(button);
			button.addListener(this);
		}

		void loadPresetList()
		{
			presetList.clear(dontSendNotification);
			const auto allPresets = presetManager.getAllPresets();
			const auto currentPreset = presetManager.getCurrentPreset();
			presetList.addItemList(allPresets, 1);
			presetList.setSelectedItemIndex(allPresets.indexOf(currentPreset), dontSendNotification);
		}

		Service::PresetManager& presetManager;
		TextButton previousPresetButton, nextPresetButton;
		ComboBox presetList;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetPanel)
	};
}
