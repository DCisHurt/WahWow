#include "XYPad.h"

/*
 * XY Pad Thumb section
 */
XYPad::Thumb::Thumb()
{
    constrainer.setMinimumOnscreenAmounts(thumbSize, thumbSize, thumbSize, thumbSize);
}

void XYPad::Thumb::paint(Graphics &g)
{
    // g.setColour(Colours::white);
    // g.fillEllipse(getLocalBounds().toFloat());
    g.drawImage(ImageCache::getFromMemory(BinaryData::drager_png, BinaryData::drager_pngSize), getLocalBounds().toFloat());
}

void XYPad::Thumb::mouseDown(const MouseEvent &event)
{
    dragger.startDraggingComponent(this, event);
}

void XYPad::Thumb::mouseDrag(const MouseEvent &event)
{
    dragger.dragComponent(this, event, &constrainer);
    if (moveCallback)
        moveCallback(getPosition().toDouble());
}

/*
 * XY Pad section
 */
XYPad::XYPad(AudioProcessorValueTreeState &stateToUse, const String &xID, const String &yID) : xAth{stateToUse, xID, xVal},
                                                                                               yAth{stateToUse, yID, yVal}
{
    addAndMakeVisible(thumb);
    thumb.moveCallback = [&](Point<double> position)
    {
        const auto bounds = getLocalBounds().toDouble();
        const auto w = static_cast<double>(thumbSize);

        xVal.setValue(jmap(position.getX(), 0.0, bounds.getWidth() - w, xVal.getMinimum(), xVal.getMaximum()));
        yVal.setValue(jmap(position.getY(), bounds.getHeight() - w, 0.0, yVal.getMinimum(), yVal.getMaximum()));
    };
}

void XYPad::resized()
{
    thumb.setBounds(getLocalBounds().withSize(thumbSize, 52));
    valueChanged();
}

void XYPad::paint(Graphics &g)
{
    // g.setColour(Colours::black);
    // g.fillRoundedRectangle(getLocalBounds().toFloat(), 10.f);
    // Path p;
    // p.addRoundedRectangle(getLocalBounds().toFloat(), 20.f);
    // g.reduceClipRegion(p);
    // g.drawImage(ImageCache::getFromMemory(BinaryData::Pad_png, BinaryData::Pad_pngSize), getLocalBounds().toFloat());
}

void XYPad::valueChanged()
{
    // Avoid loopback
    if (thumb.isMouseOverOrDragging(false))
        return;

    // Figure out if the slider belongs to xVals or yVals
    const auto bounds = getLocalBounds().toDouble();
    const auto w = static_cast<double>(thumbSize);

    thumb.setTopLeftPosition(
        static_cast<int>(jmap(xVal.getValue(), xVal.getMinimum(), xVal.getMaximum(), 0.0, bounds.getWidth() - w)),
        static_cast<int>(jmap(yVal.getValue(), yVal.getMinimum(), yVal.getMaximum(), bounds.getHeight() - w, 0.0)));

    repaint();
}