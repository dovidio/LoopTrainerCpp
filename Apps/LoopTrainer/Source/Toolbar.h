//
// Created by Umberto D'Ovidio on 17.01.22.
//

#pragma once

#include "CommonHeader.h"

namespace LoopTrainer
{
class Toolbar : public juce::Component, juce::Slider::Listener
{

public:
    Toolbar();

    void resized() override;
    bool isLoopEnabled();
    bool isPlaying();
    void sliderValueChanged(juce::Slider* slider) override;
    void sliderDragStarted(juce::Slider* slider) override;
    void sliderDragEnded(juce::Slider* slider) override;
    void setFileName(juce::String);
    void setTotalLength(juce::int64);

    std::function<void()> onOpenButtonClicked;
    std::function<void()> onPlayButtonClicked;
    std::function<void()> onStopButtonClicked;
    std::function<void()> onLoopButtonClicked;
    std::function<void(double)> onSliderValueChange;
private:
    juce::String fileName = "System of a Down";
    
    juce::TextButton openButton;
    juce::TextButton playButton;
    juce::TextButton stopButton;
    juce::TextButton loopButton;
    juce::Slider playbackSpeed;
    juce::Label playbackSpeedLabel;
    juce::Label title;
    
    juce::FlexItem buildButton(juce::Button& button);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Toolbar)
};
}


