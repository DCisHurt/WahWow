#include "XYPad.h"


/*
    * XY Pad Thumb section
    */
XYPad::Thumb::Thumb()
{
    constrainer.setMinimumOnscreenAmounts(thumbSize, thumbSize, thumbSize, thumbSize);
}

void XYPad::Thumb::paint(Graphics& g)
{
    g.setColour(Colours::white);
    g.fillEllipse(getLocalBounds().toFloat());
}

void XYPad::Thumb::mouseDown(const MouseEvent& event)
{
    dragger.startDraggingComponent(this, event);
}

void XYPad::Thumb::mouseDrag(const MouseEvent& event)
{
    dragger.dragComponent(this, event, &constrainer);
    if (moveCallback)
        moveCallback(getPosition().toDouble());
}


/*
    * XY Pad section
    */
XYPad::XYPad(AudioProcessorValueTreeState& stateToUse, const String& xID, const String& yID): 
            xAth{stateToUse, xID, xVal},
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
    thumb.setBounds(getLocalBounds().withSizeKeepingCentre(thumbSize, thumbSize));
    valueChanged();
}

void XYPad::paint(Graphics& g)
{
    g.setColour(Colours::black);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 10.f);
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