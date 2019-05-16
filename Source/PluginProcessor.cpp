/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <math.h>

#define BUDDA_Q_SCALE 6.f


//==============================================================================
SummativeAudioProcessor::SummativeAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    currentSampleRate = 0.0f;
    currentAngle = 0.0f;
    angleDelta = 0.0f;
    sinFreq = 60.0f;
}

SummativeAudioProcessor::~SummativeAudioProcessor()
{
}

CFilterButterworth24db::CFilterButterworth24db(void)
{
    this->history1 = 0.f;
    this->history2 = 0.f;
    this->history3 = 0.f;
    this->history4 = 0.f;
    
    this->SetSampleRate(44100.f);
    this->Set(22050.0f, 0.0f);
}

CFilterButterworth24db::~CFilterButterworth24db(void)
{
}


//==============================================================================
const String SummativeAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SummativeAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SummativeAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SummativeAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SummativeAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SummativeAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SummativeAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SummativeAudioProcessor::setCurrentProgram (int index)
{
}

const String SummativeAudioProcessor::getProgramName (int index)
{
    return {};
}

void SummativeAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SummativeAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    currentSampleRate = sampleRate;
    sinFreq = 60.0f;
    
    updateAngleDelta();
    
    
    // initiate mixLevel
    mixLevel.reset(sampleRate, 0.1f);
    mixLevel.setTargetValue(0.25f);
    
    freqLevel.reset(sampleRate, 0.01f);
    freqLevel.setTargetValue(0.0);
    
    // setting gain to call later, in decibels
    gain.setGainDecibels(12.0f);
    
    
    
    
    String message;
    message << "Preparing to play..." << newLine;
    message << "My sample rate is" << currentSampleRate << newLine;
    message << "Buffer size is" << samplesPerBlock << newLine;
    Logger::getCurrentLogger()->writeToLog(message);
     
    
    
}

void SummativeAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SummativeAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SummativeAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    
    AudioBuffer<float> wetBuffer(getTotalNumInputChannels(), buffer.getNumSamples());
    wetBuffer.makeCopyOf(buffer);
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        // we are getting an audio block of size 512
        auto* channelData = buffer.getWritePointer (channel);
        auto* wetData = wetBuffer.getWritePointer(channel);
        
        
        for(int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            
            
            
            
            sinFreq = freqLevel.getNextValue();
            updateAngleDelta();
            
            
            // This part is the ring mod where it is incorporating the Sin set to 60.0f at the top of the code
            auto currentSinSample = (float) std::sin(currentAngle);
            currentAngle += angleDelta; //currentAngle = currentAngle + angleDelta
            
            // creating/setting the wetData value
            //wetData[sample] = wetData[sample] * currentSinSample;
            
            //wetData[sample] = currentSinSample;
            
            // this makes it so you can process it first and then chuck it through a wave shaper
            
            // hard clipping waveshaper
            // might not be needed for assessment having both, but thought that they sounded cool together so apologies if we only needed one or the other (hopefully its fine to do!)
            // I liked the values of -0.7 and 0.7, specifically when used with a synth sample with a high and low line/part
            // auto shapedSample = jlimit((float) -0.7, (float) 0.7, wetData[sample]);
            
            
            // Finalising the wetData for mix below
            //wetData[sample] = shapedSample;
            
            
            // Running the Butterworth filter, needs fixing
            
            CFilterButterworth24db butter;
            butter.Set(5000.0f, 0.1f);
            wetData[sample] = butter.Run(wetData[sample]);
            
            
            channelData[sample] = butter.Run(channelData[sample]);
            // implementing slider control
            //channelData[sample] = channelData[sample] * (1.0f - (mixLevel.getNextValue()*0.01f)) + ((wetData[sample] * mixLevel.getNextValue()) * 0.01f);
            
            // lowering final gain
            //channelData[sample] = channelData[sample] * 0.4f;
        }
    }
}

//==============================================================================
bool SummativeAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SummativeAudioProcessor::createEditor()
{
    return new SummativeAudioProcessorEditor (*this);
}

//==============================================================================
void SummativeAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    /*
    auto state = parameters.copyState();
    std::unique_ptr<XmlElement> xml (state.createXml());
    copyXmlToBinary(*xml, destData);
     */
}

void SummativeAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    /*
    std::unique_ptr<XmlElement> xmlState (getXmlFromBinary(data, sizeInBytes));
    
    
    if(xmlState.get() != nullptr)
    {
        if(xmlState->hasTagName(parameters.state.getType()))
        {
            parameters.replaceState(ValueTree::fromXml(*xmlState));
        }
    }
     */
}

//==============================================================================
// User functions
    
    void SummativeAudioProcessor::updateAngleDelta()
    {
        // calculate no. of cycles that will need to complete for each output sample
        auto cyclesPerSample = sinFreq / currentSampleRate;
        // multiply by the length of a whole sin wave cycle
        angleDelta = cyclesPerSample * MathConstants<float>::twoPi;
    }

void CFilterButterworth24db::SetSampleRate(float fs)
{
    float pi = 4.f * atanf(1.f);
    
    this->t0 = 4.f * fs * fs;
    this->t1 = 8.f * fs * fs;
    this->t2 = 2.f * fs;
    this->t3 = pi / fs;
    
    this->min_cutoff = fs * 0.01f;
    this->max_cutoff = fs * 0.45f;
}

void CFilterButterworth24db::Set(float cutoff, float q)
{
    if (cutoff < this->min_cutoff)
        cutoff = this->min_cutoff;
    else if(cutoff > this->max_cutoff)
        cutoff = this->max_cutoff;
    
    if(q < 0.f)
        q = 0.f;
    else if(q > 1.f)
        q = 1.f;
    
    float wp = this->t2 * tanf(this->t3 * cutoff);
    float bd, bd_tmp, b1, b2;
    
    q *= BUDDA_Q_SCALE;
    q += 1.f;
    
    b1 = (0.765367f / q) / wp;
    b2 = 1.f / (wp * wp);
    
    bd_tmp = this->t0 * b2 + 1.f;
    
    bd = 1.f / (bd_tmp + this->t2 * b1);
    
    this->gain = bd * 0.5f;
    
    this->coef2 = (2.f - this->t1 * b2);
    
    this->coef0 = this->coef2 * bd;
    this->coef1 = (bd_tmp - this->t2 * b1) * bd;
    
    b1 = (1.847759f / q) / wp;
    
    bd = 1.f / (bd_tmp + this->t2 * b1);
    
    this->gain *= bd;
    this->coef2 *= bd;
    this->coef3 = (bd_tmp - this->t2 * b1) * bd;
}

float CFilterButterworth24db::Run(float input)
{
    float output = input * this->gain;
    float new_hist;
    
    output -= this->history1 * this->coef0;
    new_hist = output - this->history2 * this->coef1;
    
    output = new_hist + this->history1 * 2.f;
    output += this->history2;
    
    this->history2 = this->history1;
    this->history1 = new_hist;
    
    output -= this->history3 * this->coef2;
    new_hist = output - this->history4 * this->coef3;
    
    output = new_hist + this->history3 * 2.f;
    output += this->history4;
    
    this->history4 = this->history3;
    this->history3 = new_hist;
    
    return output;
}



//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SummativeAudioProcessor();
}
