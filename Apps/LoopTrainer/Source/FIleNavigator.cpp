//
// Created by Umberto D'Ovidio on 17.01.22.
//

#include "FileNavigator.h"

namespace LoopTrainer
{

FileNavigator::FileNavigator(juce::AudioFormatManager& audioFormatManager):
    audioThumbnailCache(5),
    thumbnail(512, audioFormatManager, audioThumbnailCache)
{
    thumbnail.addChangeListener(this);
}

void FileNavigator::paint(juce::Graphics& g)
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

void FileNavigator::setSource(juce::File& file)
{
    thumbnail.setSource(new juce::FileInputSource(file));
}

void FileNavigator::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &thumbnail)
    {
        repaint();
    }
}
}