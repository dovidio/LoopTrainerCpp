//
// Created by Umberto D'Ovidio on 17.01.22.
//

#include "FileNavigator.h"

namespace LoopTrainer
{

/*
 * THUMBNAIL IMPLEMENTATION
 */
ThumbnailComponent::ThumbnailComponent(juce::AudioFormatManager& audioFormatManager):
    thumbnailCache(5),
thumbnail(512, audioFormatManager, thumbnailCache)
{
    thumbnail.addChangeListener(this);
}

void ThumbnailComponent::paint(juce::Graphics& g)
{
    g.setColour(juce::Colours::darkgrey);
    g.fillRect(getLocalBounds());
    g.setColour(juce::Colours::white);
    if (thumbnail.getNumChannels() > 0) {
        thumbnail.drawChannels(g, getLocalBounds(), 0, thumbnail.getTotalLength(), 1.0f);
    } else {
        g.drawFittedText("No File Loaded", getLocalBounds(), juce::Justification::centred, 1);
    }
}

void ThumbnailComponent::setSource(juce::File& file)
{
    thumbnail.setSource(new juce::FileInputSource(file));
}

void ThumbnailComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &thumbnail)
    {
        repaint();
    }
}

/*
 * MARKER IMPLEMENTATION
 */
void Marker::paint(juce::Graphics &g)
{
    g.setColour(juce::Colours::green);
    auto initialX = juce::roundToInt(position * getWidth());
    auto line = juce::Line<int>(initialX, 0, initialX + 2, getHeight());
    g.drawLine(line.toFloat());
}

void Marker::setPosition(double pos)
{
    position = pos;
    repaint();
}

/*
 * LOOP RECTANGLE IMPLEMENTATION
 */
void LoopRectangle::paint(Graphics& g)
{
    g.setColour(juce::Colours::yellow);
    auto x = startX * getWidth();
    auto width = endX* getWidth() - x;
    // juce slider positioning doesn't seem to be reliable, therefore we apply this hack to correct the position
    auto x1Correction = 2.0 * x / getWidth();
    auto x2Correction = 2.0 * (1 - endX) * getWidth() / getWidth();
    auto initialX = juce::roundToInt(x - x1Correction);
    auto end = juce::roundToInt(width + x2Correction);
    g.drawRect(juce::Rectangle<int>(initialX, 0, end, getHeight()), 1);
}

void LoopRectangle::setPosition(double start, double end)
{
    startX = start;
    endX = end;
    repaint();
}

/*
 * FILE NAVIGATOR IMPLEMENTATION
 */
FileNavigator::FileNavigator(juce::AudioFormatManager& audioFormatManager):
    thumbnailComponent(audioFormatManager)
{
    addAndMakeVisible(thumbnailComponent);
    addAndMakeVisible(marker);
    addAndMakeVisible(rectangle);
    rectangle.setInterceptsMouseClicks(false, false);
    addAndMakeVisible(slider);
    
    addAndMakeVisible(slider);

    slider.setName("LoopSliderSelector");
    slider.setSliderSnapsToMousePosition(false);
    slider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    slider.setSliderStyle(juce::Slider::SliderStyle::TwoValueHorizontal);
    slider.setColour(juce::Slider::trackColourId, juce::Colours::yellow);
    slider.setColour(juce::Slider::thumbColourId, juce::Colours::darkorange);
    slider.setRange(minimum, maximum);
    slider.setMinValue(minimum);
    slider.setMaxValue(maximum);
    slider.addListener(this);
}

void FileNavigator::setSource(juce::File& file)
{
    thumbnailComponent.setSource(file);
}

void FileNavigator::resized()
{
    auto bounds = juce::Rectangle<int>(sliderPadding, sliderHeight, getWidth() - sliderPadding * 2, getHeight() - sliderHeight);
    thumbnailComponent.setBounds(bounds);
    slider.setBounds(0, 0, getWidth(), sliderHeight);
    // rectangle is a bit taller because must be connected with the slider above
    rectangle.setBounds(sliderPadding, sliderHeight / 2, getWidth() - sliderPadding * 2, getHeight() - sliderHeight / 2);
    marker.setBounds(bounds);
}

void FileNavigator::setPosition(double position) {
    marker.setPosition(position);
}

void FileNavigator::sliderDragStarted(juce::Slider* s)
{
    updateRectangle();
}

void FileNavigator::sliderDragEnded(juce::Slider* s)
{
    updateRectangle();
}

void FileNavigator::sliderValueChanged(juce::Slider* s)
{
    updateRectangle();
}

void FileNavigator::updateRectangle()
{
    slider.setMinValue(slider.getMinValue());
    rectangle.setPosition(slider.getMinValue(), slider.getMaxValue());
}

double FileNavigator::loopEnd()
{
    return slider.getMaxValue();
}

double FileNavigator::loopStart()
{
    return slider.getMinValue();
}

void FileNavigator::setLoopSelectorVisibility(bool visible)
{
    slider.setVisible(visible);
    rectangle.setVisible(visible);
}
}
