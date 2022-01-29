//
// Created by Umberto D'Ovidio on 17.01.22.
//

#pragma once

#include "CommonHeader.h"

namespace LoopTrainer
{
class Toolbar : public juce::Component
{

public:
    Toolbar();

    void resized() override;
    bool isLoopEnabled();

    std::function<void()> onOpenButtonClicked;
    std::function<void()> onPlayButtonClicked;
    std::function<void()> onPauseButtonClicked;
    std::function<void()> onStopButtonClicked;
    std::function<void()> onLoopButtonClicked;

private:
    juce::TextButton openButton;
    juce::TextButton playButton;
    juce::TextButton pauseButton;
    juce::TextButton stopButton;
    juce::TextButton loopButton;
    juce::FlexItem buildButton(juce::Button& button);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Toolbar)
};
}


