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

    Marker() {};
    void paint(juce::Graphics&) override;
    void setPosition(double);
private:
    // position ranges from 0 (beginning of file) to 1 (end of file)
    double position = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Marker)
};

// Class responsible for showing the loop selection on top of the thumbail
class LoopRectangle : public Component
{
public:
    LoopRectangle() {};
    void paint(juce::Graphics&) override;
    void setPosition(double, double);
    
    double startX = 0.0;
    double endX = 0.0;

private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LoopRectangle)
};

// Parent class, holding multiple components and putting them together
class FileNavigator : public juce::Component, juce::Slider::Listener
{
public:
    
    const double minimum = 0;
    const double maximum = 1;

    FileNavigator(juce::AudioFormatManager&);

    void setSource(juce::File& file);
    void resized() override;
    void setPosition(double);
    
    void sliderValueChanged(juce::Slider *slider) override;
    void sliderDragStarted(juce::Slider *slider) override;
    void sliderDragEnded(juce::Slider *slider) override;
    
    double loopStart();
    double loopEnd();
    void setLoopSelectorVisibility(bool);

private:
    int sliderPadding = 11;
    int sliderHeight = 40;
    
    ThumbnailComponent thumbnailComponent;
    Marker marker;
    LoopRectangle rectangle;
    juce::Slider slider;
    
    void updateRectangle();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FileNavigator)
};
}

