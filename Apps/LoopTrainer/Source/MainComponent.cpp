#include "MainComponent.h"

namespace LoopTrainer
{
MainComponent::MainComponent()
{
    setAudioChannels(0,2);
    setSize(600, 400);

    addAndMakeVisible(openButton);
    openButton.setButtonText("Open");
    openButton.onClick = [this] { openFileChooser(); };

    addAndMakeVisible(playButton);
    playButton.setButtonText("Play");
    playButton.onClick = [this] { audioTransportSource.start(); };

    addAndMakeVisible(pauseButton);
    pauseButton.setButtonText("Pause");
    pauseButton.onClick = [this] { audioTransportSource.stop(); };

    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent() noexcept
{
    shutdownAudio();
}

void MainComponent::paint(Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    juce::FlexBox flexBox;
    flexBox.justifyContent = juce::FlexBox::JustifyContent::center;
    flexBox.alignContent = juce::FlexBox::AlignContent::flexStart;

    flexBox.items.add(buildButton(openButton));
    flexBox.items.add(buildButton(playButton));
    flexBox.items.add(buildButton(pauseButton));

    flexBox.performLayout(juce::Rectangle<int>(0, 0, getWidth(), 40.0f));
}

juce::FlexItem MainComponent::buildButton(juce::Button& button)
{
    return juce::FlexItem(button).withMinWidth(80.0f).withMinHeight(30.0f).withMargin(5.0f);
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    audioTransportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
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

    audioTransportSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::openFileChooser()
{

    fileChooser = std::make_unique<juce::FileChooser> ("Please select the moose you want to load...",
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

                                         readerSource.reset(source.release());
                                     }
                                 }

                             });

}
} // namespace GuiApp
