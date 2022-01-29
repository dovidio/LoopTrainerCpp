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
Marker::Marker() {}
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
 * FILE NAVIGATOR IMPLEMENTATION
 */
FileNavigator::FileNavigator(juce::AudioFormatManager& audioFormatManager):
    thumbnailComponent(audioFormatManager)
{
    addAndMakeVisible(thumbnailComponent);
    addAndMakeVisible(marker);
}

void FileNavigator::setSource(juce::File& file)
{
    thumbnailComponent.setSource(file);
}

void FileNavigator::resized()
{
    auto bounds = juce::Rectangle<int>(sliderPadding, sliderHeight, getWidth() - sliderPadding * 2, getHeight() - sliderHeight);
    thumbnailComponent.setBounds(bounds);
    marker.setBounds(bounds);
}

void FileNavigator::setPosition(double position) {
    marker.setPosition(position);
}
}
