#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    audioEngine.prepareToPlay (sampleRate, samplesPerBlockExpected);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // If the device is not active, clear the buffer
    if (bufferToFill.buffer == nullptr)
        return;

    // Process the audio through our engine
    juce::MidiBuffer incomingMidi; // We don't have MIDI input yet

    // We need to pass the buffer from AudioSourceChannelInfo.
    // AudioEngine::processBlock expects a reference to the whole buffer,
    // but we should technically only process the relevant samples.
    // However, AudioProcessor::processBlock usually takes the whole buffer
    // and we assume the host sets it up correctly.
    // AudioSourceChannelInfo contains startSample and numSamples.
    // We should ideally create a proxy buffer or ensure AudioEngine handles sub-blocks.

    // For simplicity with AudioProcessorGraph (which expects full blocks usually),
    // we assume the bufferToFill represents the block we want to process.
    // But wait, bufferToFill.buffer might be larger than numSamples.

    // JUCE AudioApp component typically gives us a window into the buffer.
    // Let's check if we need to create a subset buffer.

    if (bufferToFill.startSample == 0 && bufferToFill.numSamples == bufferToFill.buffer->getNumSamples())
    {
        audioEngine.processBlock (*bufferToFill.buffer, incomingMidi);
    }
    else
    {
        // Wrap the part of the buffer we need
        // Note: Creating a temporary AudioBuffer from pointers is lightweight
        // But AudioBuffer doesn't own the data in this constructor
        /*
           AudioBuffer (Type* const* dataToReferTo,
                        int numChannelsToUse,
                        int numSamplesToUse)
        */

        juce::AudioBuffer<float> proxyBuffer (bufferToFill.buffer->getArrayOfWritePointers(),
                                              bufferToFill.buffer->getNumChannels(),
                                              bufferToFill.startSample + bufferToFill.numSamples);

        // This proxy is tricky because we need to offset the pointers.
        // It's safer to rely on the fact that AudioAppComponent usually gives us a full block
        // or we just process the region.
        // Actually, AudioProcessorGraph::processBlock processes the *entire* buffer passed to it.
        // So we really should pass a buffer that only contains the samples we want to process.

        // Correct way to make a proxy buffer for a region:
        // We can't easily make a proxy "AudioBuffer" that starts at an offset index
        // without manually offsetting the pointers passed to the constructor.

        std::vector<float*> pointers;
        for (int i = 0; i < bufferToFill.buffer->getNumChannels(); ++i)
            pointers.push_back (bufferToFill.buffer->getWritePointer (i, bufferToFill.startSample));

        juce::AudioBuffer<float> subBuffer (pointers.data(),
                                            bufferToFill.buffer->getNumChannels(),
                                            bufferToFill.numSamples);

        audioEngine.processBlock (subBuffer, incomingMidi);
    }
}

void MainComponent::releaseResources()
{
    audioEngine.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setFont (juce::FontOptions (16.0f));
    g.setColour (juce::Colours::white);
    g.drawText ("AI-DAW - Audio Engine Running\n(Input -> Gain -> Output)", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}
