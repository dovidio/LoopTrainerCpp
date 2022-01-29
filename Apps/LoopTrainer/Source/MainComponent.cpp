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

    addAndMakeVisible(fileNavigator);

    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent() noexcept
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
   toolbar.setBounds(juce::Rectangle<int>(0, 0, getWidth(), 40.0f));
   fileNavigator.setBounds(juce::Rectangle<int>(0, toolbar.getBottom() + 20, getWidth(), 120.0f));
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

                                         fileNavigator.setSource(file);
                                         readerSource.reset(source.release());
                                     }
                                 }
                             });

}
} // namespace GuiApp
