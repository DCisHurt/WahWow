#pragma once

#include <JuceHeader.h>

class MyArrowButton : public ImageButton
{
public:
    enum ColourIds
    {
        switchColour = 0x101000,
        switchOnBackgroundColour,
        switchOffBackgroundColour
    };

    MyArrowButton(String name, Image image)
        : ImageButton(name)
    {
        int w = image.getWidth();
        int h = int(image.getHeight() / 3);

        Image normalButton = Image(image.getClippedImage(Rectangle(0, 0, w, w)).getPixelData()->clone());
        Image overButton = Image(image.getClippedImage(Rectangle(0, h, w, w)).getPixelData()->clone());
        Image downButton = Image(image.getClippedImage(Rectangle(0, h * 2, w, w)).getPixelData()->clone());

        this->setImages(false, false, true, normalButton, 1.0f, {}, overButton, 1.0f, {}, downButton, 1.0f, {});
    }
};