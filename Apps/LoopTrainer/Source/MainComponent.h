#pragma once

#include "CommonHeader.h"
#include "Toolbar.h"
#include "FileNavigator.h"

namespace LoopTrainer
{
class MainComponent : public juce::AudioAppComponent, juce::Timer
{
public:
    MainComponent();
    ~MainComponent() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

    void paint(Graphics&) override;
    void resized() override;
    
    void timerCallback() override;
    void mouseUp(const juce::MouseEvent&) override;

private:
    
    int callbackInterval = 40;

    Toolbar toolbar;
    FileNavigator fileNavigator;

    std::unique_ptr<juce::FileChooser> fileChooser;
    juce::AudioFormatManager formatManager;
    juce::AudioTransportSource audioTransportSource;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;

    void openFileChooser();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

} // namespace LoopTrainer
