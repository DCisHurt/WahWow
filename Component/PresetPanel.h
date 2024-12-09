#pragma once

#include <JuceHeader.h>
#include "../Utility/PresetManager.h"
#include "MyArrowButton.h"
namespace Gui
{
	class MyMeueLook : public LookAndFeel_V4
	{
	public:

		void drawPopupMenuBackground(Graphics &g, int w, int h) override
		{
			g.fillAll(juce::Colour(0xFF262626));
			g.setColour(Colours::white.withAlpha(0.6f));
			g.drawRect(0, 0, w, h);
		};

		void positionComboBoxText(ComboBox &, Label &) override {

		};

		void drawComboBox(Graphics &g, int width, int height, bool,
						  int, int, int, int, ComboBox &box) override
		{
			int frame_h = 336;
			int interval = 80;
			int h = interval + frame_h;
			int y = 45;
			y += box.getSelectedItemIndex() * h;

			g.drawImage(myImage, 0, 0, width, height, 0, y, myImage.getWidth(), frame_h);
		};

		void getIdealPopupMenuItemSize(const String &text, const bool,
									   int standardMenuItemHeight, int &idealWidth, int &idealHeight) override
		{
			auto font = getPopupMenuFont();

			if (standardMenuItemHeight > 0 && font.getHeight() > (float)standardMenuItemHeight / 1.3f)
				font.setHeight((float)standardMenuItemHeight / 1.3f);

			idealHeight = 48;
			idealWidth = GlyphArrangement::getStringWidthInt(font, text) + idealHeight * 2;
		};

	private:
		Image myImage = ImageCache::getFromMemory(BinaryData::preset_png, BinaryData::preset_pngSize);
	};
	class PresetPanel : public Component, Button::Listener, ComboBox::Listener
	{
	public:
		PresetPanel(Service::PresetManager &pm) : presetManager(pm),
												  previousPresetButton("<", true),
												  nextPresetButton(">", false)
		{
			configureButton(previousPresetButton);
			configureButton(nextPresetButton);

			presetList.setTextWhenNothingSelected("No Preset Selected");
			presetList.setMouseCursor(MouseCursor::PointingHandCursor);
			addAndMakeVisible(presetList);
			presetList.addListener(this);
			presetList.setLookAndFeel(&newLook);
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
			// bounds.removeFromTop(container.proportionOfHeight(0.05f));
			bounds.removeFromLeft(bounds.proportionOfWidth(0.5f));
			auto buttonBounds = bounds;
			previousPresetButton.setBounds(bounds.removeFromLeft(container.proportionOfWidth(0.25f)).reduced(4));
			nextPresetButton.setBounds(buttonBounds.removeFromRight(container.proportionOfWidth(0.25f)).reduced(4));
		}

	private:
		void buttonClicked(Button *button) override
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
		void comboBoxChanged(ComboBox *comboBoxThatHasChanged) override
		{
			if (comboBoxThatHasChanged == &presetList)
			{
				presetManager.loadPreset(presetList.getSelectedItemIndex());
			}
		}

		void configureButton(MyArrowButton &button)
		{
			// button.setMouseCursor(MouseCursor::PointingHandCursor);
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

		Service::PresetManager &presetManager;
		MyArrowButton previousPresetButton, nextPresetButton;
		ComboBox presetList;
		MyMeueLook newLook;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetPanel)
	};
}
