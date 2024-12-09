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

    MyArrowButton(String name, bool isFlipped = false)
        : ImageButton(name)
    {
        Image image = isFlipped ? imageL : imageR;
        int w = image.getWidth();
        int h = int(image.getHeight() / 3);

        Image normalButton = Image(image.getClippedImage(Rectangle(0, 0, w, h)).getPixelData()->clone());
        Image overButton = Image(image.getClippedImage(Rectangle(0, h, w, h)).getPixelData()->clone());
        Image downButton = Image(image.getClippedImage(Rectangle(0, h * 2, w, h)).getPixelData()->clone());

        this->setImages(false, true, true, normalButton, 1.0f, {}, overButton, 1.0f, {}, downButton, 1.0f, {});
    }

private:
    Image imageL = ImageCache::getFromMemory(BinaryData::arrow_l_png, BinaryData::arrow_l_pngSize);
    Image imageR = ImageCache::getFromMemory(BinaryData::arrow_r_png, BinaryData::arrow_r_pngSize);
};