//
// Created by Umberto D'Ovidio on 17.01.22.
//

#pragma once

#include "CommonHeader.h"

namespace LoopTrainer
{
// Class responsible for drawing the thumbnail of the audio source
class ThumbnailComponent: public juce::Component, juce::ChangeListener
{
public:
    ThumbnailComponent(juce::AudioFormatManager&);
    
    void paint(juce::Graphics&) override;
    void changeListenerCallback(juce::ChangeBroadcaster*) override;
    void setSource(juce::File& file);
private:
    juce::AudioThumbnailCache thumbnailCache;
    juce::AudioThumbnail thumbnail;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ThumbnailComponent)
};

// Class responsible for drawing a marker at the correct file position
class Marker : public juce::Component
{
public:

    Marker();
    void paint(juce::Graphics&) override;
    void setPosition(double);
private:
    // position ranges from 0 (beginning of file) to 1 (end of file)
    double position = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Marker)
};

// Parent class, holding multiple components and putting them together
class FileNavigator : public juce::Component
{
public:

    FileNavigator(juce::AudioFormatManager&);

    void setSource(juce::File& file);
    void resized() override;
    void setPosition(double);

private:
    int sliderPadding = 11;
    int sliderHeight = 40;
    
    ThumbnailComponent thumbnailComponent;
    Marker marker;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FileNavigator)
};
}

