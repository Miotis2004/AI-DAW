#include "AudioEngine.h"

AudioEngine::AudioEngine()
    : AudioProcessor (BusesProperties()
                      .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      .withOutput ("Output", juce::AudioChannelSet::stereo(), true))
{
    mainProcessorGraph = std::make_unique<juce::AudioProcessorGraph>();
}

AudioEngine::~AudioEngine()
{
}

void AudioEngine::initialiseGraph()
{
    mainProcessorGraph->clear();

    // Create IO Nodes
    audioInputNode  = mainProcessorGraph->addNode (std::make_unique<juce::AudioGraphIOProcessor> (juce::AudioGraphIOProcessor::audioInputNode));
    audioOutputNode = mainProcessorGraph->addNode (std::make_unique<juce::AudioGraphIOProcessor> (juce::AudioGraphIOProcessor::audioOutputNode));
    midiInputNode   = mainProcessorGraph->addNode (std::make_unique<juce::AudioGraphIOProcessor> (juce::AudioGraphIOProcessor::midiInputNode));
    midiOutputNode  = mainProcessorGraph->addNode (std::make_unique<juce::AudioGraphIOProcessor> (juce::AudioGraphIOProcessor::midiOutputNode));

    // Create Test Node (GainProcessor)
    auto gainNode = mainProcessorGraph->addNode (std::make_unique<GainProcessor>());

    // Connect: Input -> Gain -> Output
    if (audioInputNode && audioOutputNode && gainNode)
    {
        // Stereo connection
        for (int channel = 0; channel < 2; ++channel)
        {
            mainProcessorGraph->addConnection ({ { audioInputNode->nodeID,  channel },
                                                 { gainNode->nodeID,        channel } });

            mainProcessorGraph->addConnection ({ { gainNode->nodeID,        channel },
                                                 { audioOutputNode->nodeID, channel } });
        }
    }
}

void AudioEngine::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mainProcessorGraph->setPlayConfigDetails (getTotalNumInputChannels(),
                                              getTotalNumOutputChannels(),
                                              sampleRate, samplesPerBlock);

    mainProcessorGraph->prepareToPlay (sampleRate, samplesPerBlock);

    initialiseGraph();
}

void AudioEngine::releaseResources()
{
    mainProcessorGraph->releaseResources();
}

void AudioEngine::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // Pass the audio buffer to the graph
    // The graph handles the processing of all connected nodes
    mainProcessorGraph->processBlock (buffer, midiMessages);
}
