/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/**
*/
class SummativeAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    SummativeAudioProcessor();
    ~SummativeAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    // gain-dsp
    dsp::Gain<float> gain;
    
    // floats for levels
    SmoothedValue<float>mixLevel;
    SmoothedValue<float>freqLevel;

private:
    
    float currentSampleRate;
    float currentAngle;
    float angleDelta;
    float sinFreq;
    
    void updateAngleDelta();
    
    // variable to store our audio float param
    //float* mixParameter = nullptr;
    
    // create the APVTS
    //AudioProcessorValueTreeState parameters;
    
    Random random;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SummativeAudioProcessor)
};

/*
class Vibe
{
    
public:
    
    Vibe (float * efxoutl_, float * efxoutr_);
    ~Vibe ();
    //note some of these functions not pasted below to improve clarity
    //and to save space
    void out (float * smpsl, float * smpsr);
    void setvolume(int value);
    void setpanning(int value);
    void setpreset (int npreset);
    void changepar (int npar, int value);
    int getpar (int npar);
    void cleanup ();
    
    float outvolume;
    float *efxoutl;
    float *efxoutr;
    
    
private:
    int Pwidth;
    int Pfb;
    int Plrcross;
    int Pdepth;
    int Ppanning;
    int Pvolume;
    //all the ints above are the parameters to modify with a proper function.
    
    float fwidth;
    float fdepth;
    float rpanning, lpanning;
    float flrcross, fcross;
    float fb;
    EffectLFO lfo; //EffectLFO is an object that calculates the next sample from the LFO each time it's called
    
    float Ra, Rb, b, dTC, dRCl, dRCr, lampTC, ilampTC, minTC, alphal, alphar, stepl, stepr, oldstepl, oldstepr;
    float fbr, fbl;
    float dalphal, dalphar;
    float lstep,rstep;
    float cperiod;
    float gl, oldgl;
    float gr, oldgr;
    
    class fparams {
    public:
        float x1;
        float y1;
        //filter coefficients
        float n0;
        float n1;
        float d0;
        float d1;
    } vc[8], vcvo[8], ecvc[8], vevo[8], bootstrap[8];
    
    float vibefilter(float data, fparams *ftype, int stage);
    void init_vibes();
    void modulate(float ldrl, float ldrr);
    float bjt_shape(float data);
    
    float R1;
    float Rv;
    float C2;
    float C1[8];
    float beta;  //transistor forward gain.
    float gain, k;
    float oldcvolt[8] ;
    float en1[8], en0[8], ed1[8], ed0[8];
    float cn1[8], cn0[8], cd1[8], cd0[8];
    float ecn1[8], ecn0[8], ecd1[8], ecd0[8];
    float on1[8], on0[8], od1[8], od0[8];
    
    class FPreset *Fpre;
    
    
};

 
*/

class CFilterButterworth24db
{
public:
    CFilterButterworth24db(void);
    ~CFilterButterworth24db(void);
    void SetSampleRate(float fs);
    void Set(float cutoff, float q);
    float Run(float input);
    
private:
    float t0, t1, t2, t3;
    float coef0, coef1, coef2, coef3;
    float history1, history2, history3, history4;
    float gain;
    float min_cutoff, max_cutoff;
};


