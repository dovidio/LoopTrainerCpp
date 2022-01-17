#pragma once

#include "CommonHeader.h"

namespace LoopTrainer
{
class MainComponent : public juce::AudioAppComponent
{
public:
    MainComponent();
    ~MainComponent();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

    void paint(Graphics&) override;
    void resized() override;

private:

    juce::TextButton openButton;
    juce::TextButton playButton;
    juce::TextButton pauseButton;

    std::unique_ptr<juce::FileChooser> fileChooser;
    juce::AudioFormatManager formatManager;
    juce::AudioTransportSource audioTransportSource;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
    juce::FlexItem buildButton(juce::Button& button);
    void openFileChooser();
};

} // namespace LoopTrainer
