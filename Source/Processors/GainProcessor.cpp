#include "GainProcessor.h"

GainProcessor::GainProcessor()
    : AudioProcessor (BusesProperties()
                      .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      .withOutput ("Output", juce::AudioChannelSet::stereo(), true))
{
}

GainProcessor::~GainProcessor()
{
}

void GainProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void GainProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool GainProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;
}

void GainProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            channelData[sample] *= gain;
        }
    }
}

bool GainProcessor::hasEditor() const
{
    return false; // We won't implement an editor for this test node yet
}

juce::AudioProcessorEditor* GainProcessor::createEditor()
{
    return nullptr;
}

const juce::String GainProcessor::getName() const
{
    return "Gain Processor";
}

bool GainProcessor::acceptsMidi() const
{
    return false;
}

bool GainProcessor::producesMidi() const
{
    return false;
}

bool GainProcessor::isMidiEffect() const
{
    return false;
}

double GainProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GainProcessor::getNumPrograms()
{
    return 1;
}

int GainProcessor::getCurrentProgram()
{
    return 0;
}

void GainProcessor::setCurrentProgram (int index)
{
}

const juce::String GainProcessor::getProgramName (int index)
{
    return {};
}

void GainProcessor::changeProgramName (int index, const juce::String& newName)
{
}

void GainProcessor::getStateInformation (juce::MemoryBlock& destData)
{
}

void GainProcessor::setStateInformation (const void* data, int sizeInBytes)
{
}

void GainProcessor::setGain (float newGain)
{
    gain = newGain;
}
