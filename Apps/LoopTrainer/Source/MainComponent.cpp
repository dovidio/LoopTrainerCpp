#include "MainComponent.h"

namespace LoopTrainer
{
MainComponent::MainComponent()
: fileNavigator(formatManager)
{
    setAudioChannels(0,2);
    setSize(600, 400);

    addAndMakeVisible(toolbar);
    toolbar.onOpenButtonClicked = [this] { openFileChooser(); };
    toolbar.onPlayButtonClicked = [this] { audioTransportSource.start(); };
    toolbar.onPauseButtonClicked = [this] { audioTransportSource.stop(); };
    toolbar.onStopButtonClicked = [this] {
        audioTransportSource.stop();
        audioTransportSource.setPosition(0);
        fileNavigator.setPosition(0);
    };
    toolbar.onLoopButtonClicked = [this] {
        fileNavigator.setLoopSelectorVisibility(toolbar.isLoopEnabled());
    };
    toolbar.onSliderValueChange = [this](double value) {
        updatePlaybackSpeed(value);
    };

    addAndMakeVisible(fileNavigator);
    fileNavigator.addMouseListener(this, true);
    fileNavigator.setLoopSelectorVisibility(false);

    formatManager.registerBasicFormats();
    
    startTimer(callbackInterval);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
    audioTransportSource.setSource(nullptr);
}

void MainComponent::paint(Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
   toolbar.setBounds(juce::Rectangle<int>(0, 0, getWidth(), 160.0f));
   fileNavigator.setBounds(juce::Rectangle<int>(0, toolbar.getBottom() + 20, getWidth(), 160.0f));
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    audioTransportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    stretcher = std::make_unique<RubberBand::RubberBandStretcher>(sampleRate, 2, RubberBand::RubberBandStretcher::Option::OptionProcessRealTime, 2.0, 1.0);
}

void MainComponent::releaseResources()
{
    audioTransportSource.releaseResources();
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (readerSource.get() == nullptr) {
        bufferToFill.clearActiveBufferRegion();
        return;
    }

    auto samplesNeeded = bufferToFill.numSamples;
    auto samplesAvailable = stretcher->available();
    while (samplesAvailable < samplesNeeded)
    {
        audioTransportSource.getNextAudioBlock(bufferToFill);
        stretcher->process(bufferToFill.buffer->getArrayOfReadPointers(), bufferToFill.numSamples, false);
        samplesAvailable = stretcher->available();
    }
    stretcher->retrieve(bufferToFill.buffer->getArrayOfWritePointers(), bufferToFill.numSamples);
}

void MainComponent::openFileChooser()
{

    fileChooser = std::make_unique<juce::FileChooser> ("Please select the file you want to load...",
                                              juce::File{},
                                              "*.wav,*.ogg,*.mp3");

    auto fileChooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

    fileChooser->launchAsync(fileChooserFlags, [this] (const juce::FileChooser& chooser)
                             {
                                 auto file = chooser.getResult();

                                 if (file != juce::File{}) {
                                     auto* reader = formatManager.createReaderFor(file);

                                     if (reader != nullptr) {
                                         auto source = std::make_unique<juce::AudioFormatReaderSource>(reader, true);

                                         audioTransportSource.setSource(source.get(), 0, nullptr, reader->sampleRate);

                                         fileNavigator.setSource(file);
                                         readerSource.reset(source.release());
                                     }
                                 }
                             });

}


void MainComponent::timerCallback()
{
    if (audioTransportSource.getTotalLength() > 0)
    {
        auto audioLength = audioTransportSource.getLengthInSeconds();
        auto audioPosition = audioTransportSource.getCurrentPosition();
        auto progress = audioPosition / audioLength;

        if (progress >= fileNavigator.loopEnd()) {
            auto loopStart = fileNavigator.loopStart();
            audioTransportSource.setPosition(loopStart * audioLength);
            fileNavigator.setPosition(loopStart);
        } else {
            fileNavigator.setPosition(progress);
        }
    }
}

void MainComponent::mouseUp(const juce::MouseEvent& event)
{
    if (auto c = dynamic_cast<Marker*>(event.originalComponent))
    {
        auto normalisedProgress = ((double) event.x) / (double) c->getBounds().getWidth();
        auto audioPosition = normalisedProgress * audioTransportSource.getLengthInSeconds();
        audioTransportSource.setPosition(audioPosition);
        fileNavigator.setPosition(normalisedProgress);
    }
}

void MainComponent::updatePlaybackSpeed(double playbackSpeed)
{
    auto timeRatio = 1.0 / (playbackSpeed / 100.0);
    stretcher->setTimeRatio(timeRatio);
}

}
