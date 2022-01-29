//
// Created by Umberto D'Ovidio on 17.01.22.
//

#include "Toolbar.h"

namespace LoopTrainer
{

Toolbar::Toolbar()
{
    addAndMakeVisible(openButton);
    openButton.setButtonText("Open");
    openButton.onClick = [this] { onOpenButtonClicked(); };

    addAndMakeVisible(playButton);
    playButton.setButtonText("Play");
    playButton.onClick = [this] { onPlayButtonClicked(); };

    addAndMakeVisible(pauseButton);
    pauseButton.setButtonText("Pause");
    pauseButton.onClick = [this] { onPauseButtonClicked(); };
    
    addAndMakeVisible(stopButton);
    stopButton.setButtonText("Stop");
    stopButton.onClick = [this] { onStopButtonClicked(); };
    
    addAndMakeVisible(loopButton);
    loopButton.setButtonText("Loop");
    loopButton.setClickingTogglesState(true);
    loopButton.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::darkorange);
    loopButton.onClick = [this] { onLoopButtonClicked(); };
}

void Toolbar::resized()
{
    juce::FlexBox flexBox;
    flexBox.justifyContent = juce::FlexBox::JustifyContent::center;
    flexBox.alignContent = juce::FlexBox::AlignContent::flexStart;

    flexBox.items.add(buildButton(openButton));
    flexBox.items.add(buildButton(playButton));
    flexBox.items.add(buildButton(pauseButton));
    flexBox.items.add(buildButton(stopButton));
    flexBox.items.add(buildButton(loopButton));

    flexBox.performLayout(getLocalBounds());
}

juce::FlexItem Toolbar::buildButton(juce::Button& button)
{
    return juce::FlexItem(button).withMinWidth(80.0f).withMinHeight(30.0f).withMargin(5.0f);
}

bool Toolbar::isLoopEnabled()
{
    return loopButton.getToggleState();
}

}
