#pragma once

#include <JuceHeader.h>



class MyPowerButton : public TextButton
{
public:
    enum ColourIds
    {
        switchColour = 0x101000,
        switchOnBackgroundColour,
        switchOffBackgroundColour
    };

    MyPowerButton()
    {
        this->setClickingTogglesState(true);
    }

    void paintButton(Graphics &g,
                     bool shouldDrawButtonAsHighlighted,
                     bool shouldDrawButtonAsDown
                     ) override
    {
        ignoreUnused(shouldDrawButtonAsHighlighted);    //Do nothing
        ignoreUnused(shouldDrawButtonAsDown);           //Do nothing
        

        float rotaryStartAngle = 30.0f * MathConstants<float>::pi / 180.0f, rotaryEndAngle = 330.0f * MathConstants<float>::pi / 180.0f;
        auto arcBounds = getLocalBounds().toFloat().reduced(10);

        auto radius = jmin (arcBounds.getWidth(), arcBounds.getHeight()) / 2.0f;
        auto lineW = jmin (4.0f, radius * 0.25f);
        auto arcRadius = radius - lineW * 0.25f;

        Path backgroundArc;
        backgroundArc.addCentredArc (arcBounds.getCentreX(),
                                    arcBounds.getCentreY(),
                                    arcRadius,
                                    arcRadius,
                                    0.0f,
                                    rotaryStartAngle,
                                    rotaryEndAngle,
                                    true);


        g.setColour (findColour (getToggleState() ? buttonOnColourId : buttonColourId));
        g.strokePath (backgroundArc, PathStrokeType (lineW, PathStrokeType::curved, PathStrokeType::rounded));

        auto lineBounds = getLocalBounds().toFloat();
        auto halfW = lineW / 2.0f;

        g.fillRoundedRectangle(lineBounds.getCentreX()-halfW, 7.5f, lineW, lineBounds.getCentreY()-7.5f, halfW);

        auto roundW = lineW*0.75f;
        auto roundBounds = arcBounds.expanded(lineW+roundW);

        radius = jmin (roundBounds.getWidth(), roundBounds.getHeight()) / 2.0f;

        g.drawEllipse(roundBounds.getCentreX()-radius, roundBounds.getCentreY()-radius, radius*2.0f, radius*2.0f, roundW);

    }

};