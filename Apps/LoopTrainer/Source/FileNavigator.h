//
// Created by Umberto D'Ovidio on 17.01.22.
//

#pragma once

#include "CommonHeader.h"

namespace LoopTrainer
{
class FileNavigator : public juce::Component, juce::ChangeListener
{
public:

    FileNavigator(juce::AudioFormatManager&);

    void paint(juce::Graphics&) override;
    void setSource(juce::File& file);

    void changeListenerCallback(juce::ChangeBroadcaster*) override;

private:
    juce::AudioThumbnailCache audioThumbnailCache;
    juce::AudioThumbnail thumbnail;

};
}

