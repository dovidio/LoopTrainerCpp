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
    
    addAndMakeVisible(playbackSpeed);
    playbackSpeed.setName("Playback speed");
    playbackSpeed.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxLeft, true, 70.0f, 20.0f);
    playbackSpeed.setTextValueSuffix(" %");
    playbackSpeed.setRange(50, 200);
    playbackSpeed.setValue(100);
    playbackSpeed.addListener(this);

    addAndMakeVisible(playbackSpeedLabel);
    playbackSpeedLabel.setText("Playback speed", juce::dontSendNotification);
    playbackSpeedLabel.attachToComponent(&playbackSpeed, false);
    playbackSpeedLabel.setJustificationType(juce::Justification::Flags::centredBottom);
}

void Toolbar::resized()
{
    juce::FlexBox fb;
    fb.flexDirection = juce::FlexBox::Direction::column;
    fb.alignContent = juce::FlexBox::AlignContent::center;
    fb.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;

    juce::FlexBox buttonFb;
    buttonFb.flexWrap = juce::FlexBox::Wrap::wrap;
    buttonFb.alignContent = juce::FlexBox::AlignContent::center;
    buttonFb.justifyContent = juce::FlexBox::JustifyContent::center;

    buttonFb.items.add(buildButton(openButton));
    buttonFb.items.add(buildButton(playButton));
    buttonFb.items.add(buildButton(pauseButton));
    buttonFb.items.add(buildButton(stopButton));
    buttonFb.items.add(buildButton(loopButton));

    fb.items.add(juce::FlexItem(buttonFb).withMinHeight(50));
    fb.items.add(juce::FlexItem(playbackSpeed).withMinHeight(60).withWidth(430).withAlignSelf(juce::FlexItem::AlignSelf::center));
    fb.performLayout(getLocalBounds().toFloat());
}

juce::FlexItem Toolbar::buildButton(juce::Button& button)
{
    return juce::FlexItem(button).withMinWidth(80.0f).withMinHeight(30.0f).withMargin(5.0f);
}

bool Toolbar::isLoopEnabled()
{
    return loopButton.getToggleState();
}

void Toolbar::sliderValueChanged(juce::Slider* slider)
{
    onSliderValueChange(slider->getValue());
}

void Toolbar::sliderDragStarted(juce::Slider* slider)
{
    onSliderValueChange(slider->getValue());
}

void Toolbar::sliderDragEnded(juce::Slider* slider)
{
    onSliderValueChange(slider->getValue());
}

}
