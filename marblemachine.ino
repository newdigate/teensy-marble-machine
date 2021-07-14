#include <Arduino.h>
#include <MIDI.h>
#include <Audio.h>
#include "playsdwavresmp.h"
#include "sampler.h"
#include "flashloader.h"
#include "midireader.h"
#include "midisequenceadapter.h"
#include "polyphonicsampler.h"
#include "tempo.h"
#include "TeensyVariablePlayback.h"

MIDI_CREATE_DEFAULT_INSTANCE();
#define KEY_NOTENUMBER_C5 84
#define KEY_NOTENUMBER_C4 72
#define KEY_NOTENUMBER_C3 60
#define KEY_NOTENUMBER_C2 48
#define KEY_NOTENUMBER_C1 36

#pragma region audio
// GUItool: begin automatically generated code
AudioPlayArrayResmp      voice2;         //xy=364,311
AudioPlayArrayResmp      voice1;         //xy=368,214
AudioPlayArrayResmp      voice5;         //xy=378,609
AudioPlayArrayResmp      voice6;         //xy=381,676
AudioPlayArrayResmp      voice7;         //xy=383,798
AudioPlayArrayResmp      voice8;         //xy=387,907
AudioPlayArrayResmp      voice3;         //xy=396,418
AudioPlayArrayResmp      voice4;         //xy=398,490
AudioEffectEnvelope      envelope3;      //xy=541,313
AudioEffectEnvelope      envelope1;      //xy=542,215
AudioEffectEnvelope      envelope2;      //xy=544,255
AudioEffectEnvelope      envelope4;      //xy=544,355
AudioEffectEnvelope      envelope5;      //xy=544,407
AudioEffectEnvelope      envelope6;      //xy=548,451
AudioEffectEnvelope      envelope7;      //xy=548,489
AudioEffectEnvelope      envelope8;      //xy=548,527
AudioEffectEnvelope      envelope12;     //xy=552,718
AudioEffectEnvelope      envelope11;     //xy=555,674
AudioEffectEnvelope      envelope10;     //xy=558,621
AudioEffectEnvelope      envelope9;      //xy=561,584
AudioEffectEnvelope      envelope16;     //xy=570,941
AudioEffectEnvelope      envelope15;     //xy=572,900
AudioEffectEnvelope      envelope13;     //xy=574,793
AudioEffectEnvelope      envelope14;     //xy=574,836
AudioMixer4              mixer3;         //xy=741,624
AudioMixer4              mixer2;         //xy=757,370
AudioMixer4              mixer1;         //xy=758,280
AudioMixer4              mixer4;         //xy=772,791
AudioMixer4              mixer5;         //xy=924,256
AudioMixer4              mixer6;         //xy=934,422
AudioEffectReverb        reverb1;        //xy=1057,300
AudioEffectReverb        reverb2;        //xy=1063,455
AudioMixer4              mixer7;         //xy=1194,226
AudioMixer4              mixer8;         //xy=1209,415
AudioOutputTDM           tdm1;           //xy=1377,352
AudioConnection          patchCord1(voice2, 0, envelope3, 0);
AudioConnection          patchCord2(voice2, 1, envelope4, 0);
AudioConnection          patchCord3(voice1, 0, envelope1, 0);
AudioConnection          patchCord4(voice1, 1, envelope2, 0);
AudioConnection          patchCord5(voice5, 0, envelope9, 0);
AudioConnection          patchCord6(voice5, 1, envelope10, 0);
AudioConnection          patchCord7(voice6, 0, envelope11, 0);
AudioConnection          patchCord8(voice6, 1, envelope12, 0);
AudioConnection          patchCord9(voice7, 0, envelope13, 0);
AudioConnection          patchCord10(voice7, 1, envelope14, 0);
AudioConnection          patchCord11(voice8, 0, envelope15, 0);
AudioConnection          patchCord12(voice8, 1, envelope16, 0);
AudioConnection          patchCord13(voice3, 0, envelope5, 0);
AudioConnection          patchCord14(voice3, 1, envelope6, 0);
AudioConnection          patchCord15(voice4, 0, envelope7, 0);
AudioConnection          patchCord16(voice4, 1, envelope8, 0);
AudioConnection          patchCord17(envelope3, 0, mixer1, 1);
AudioConnection          patchCord18(envelope1, 0, mixer1, 0);
AudioConnection          patchCord19(envelope2, 0, mixer2, 0);
AudioConnection          patchCord20(envelope4, 0, mixer2, 1);
AudioConnection          patchCord21(envelope5, 0, mixer1, 2);
AudioConnection          patchCord22(envelope6, 0, mixer2, 2);
AudioConnection          patchCord23(envelope7, 0, mixer1, 3);
AudioConnection          patchCord24(envelope8, 0, mixer2, 3);
AudioConnection          patchCord25(envelope12, 0, mixer4, 1);
AudioConnection          patchCord26(envelope11, 0, mixer3, 1);
AudioConnection          patchCord27(envelope10, 0, mixer4, 0);
AudioConnection          patchCord28(envelope9, 0, mixer3, 0);
AudioConnection          patchCord29(envelope16, 0, mixer4, 3);
AudioConnection          patchCord30(envelope15, 0, mixer3, 3);
AudioConnection          patchCord31(envelope13, 0, mixer3, 2);
AudioConnection          patchCord32(envelope14, 0, mixer4, 2);
AudioConnection          patchCord33(mixer3, 0, mixer5, 1);
AudioConnection          patchCord34(mixer2, 0, mixer6, 0);
AudioConnection          patchCord35(mixer1, 0, mixer5, 0);
AudioConnection          patchCord36(mixer4, 0, mixer6, 1);
AudioConnection          patchCord37(mixer5, reverb1);
AudioConnection          patchCord38(mixer5, 0, mixer7, 0);
AudioConnection          patchCord39(mixer6, reverb2);
AudioConnection          patchCord40(mixer6, 0, mixer8, 0);
AudioConnection          patchCord41(reverb1, 0, mixer7, 1);
AudioConnection          patchCord42(reverb2, 0, mixer8, 1);
AudioConnection          patchCord43(mixer7, 0, tdm1, 0);
AudioConnection          patchCord44(mixer7, 0, tdm1, 4);
AudioConnection          patchCord45(mixer8, 0, tdm1, 2);
AudioConnection          patchCord46(mixer8, 0, tdm1, 6);
AudioControlCS42448      cs42448;      //xy=821,899
// GUItool: end automatically generated code
#pragma endregion

sampler                 _sampler;
newdigate::audiosample  *sample1, *sample2, *sample3, *sample4, *sample5;

void handleNoteOn(uint8_t channel, uint8_t pitch, uint8_t velocity)
{
    _sampler.noteEvent(pitch, velocity, true, false);
    Serial.printf("on %d %d\n", channel, pitch);
}

void handleNoteOff(uint8_t channel, uint8_t pitch, uint8_t velocity)
{
    _sampler.noteEvent(pitch, velocity, false, false);
    Serial.printf("off %d %d\n", channel, pitch);
}

tempo _tempo(120.0f);
multisequencer _multisequencer(_tempo);
midisequenceadapter _adapter(_multisequencer); // to read midi files into the a sequencer pattern
sequencer *_sequencer;

AudioPlayArrayResmp *voices[] = {&voice1, &voice2, &voice3, &voice4, &voice5, &voice6, &voice7, &voice8};

void setup() {
    Serial.begin(9600);
    //while(!Serial) {
    // delay(100);
    //}
    delay(100);
    Serial.print("starting...\n");
    Serial.flush();
    
    delay(200);

    Serial.print("Initializing SD card...");
    Serial.flush();
    
    while (!SD.begin(BUILTIN_SDCARD)) {
      Serial.println("initialization failed!");
      delay(1000);
    }
    Serial.println("initialization done.");
    Serial.flush();
    
    for (unsigned int i=0; i<8; i++) {
        voices[i]->setNumChannels(2);
        voices[i]->enableInterpolation(true);
        voices[i]->begin();
    }

    reverb1.reverbTime(0.01);
    reverb2.reverbTime(0.01);

    mixer7.gain(1, 0.25);
    mixer8.gain(1, 0.25);
    
    envelope1.attack(0);
    envelope2.attack(0);
    envelope3.attack(0);
    envelope4.attack(0);
    envelope5.attack(0);
    envelope6.attack(0);
    envelope7.attack(0);
    envelope8.attack(0);
    
    _sampler.addVoice( voice1, mixer1, 0, envelope1, mixer2, 0, envelope2);
    _sampler.addVoice( voice2, mixer1, 1, envelope3, mixer2, 1, envelope4);
    _sampler.addVoice( voice3, mixer1, 2, envelope5, mixer2, 2, envelope6);
    _sampler.addVoice( voice4, mixer1, 3, envelope7, mixer2, 3, envelope8);
    _sampler.addVoice( voice5, mixer3, 0, envelope9,  mixer4, 0, envelope10);
    _sampler.addVoice( voice6, mixer3, 1, envelope11, mixer4, 1, envelope12);
    _sampler.addVoice( voice7, mixer3, 2, envelope13, mixer4, 2, envelope14);
    _sampler.addVoice( voice8, mixer3, 3, envelope15, mixer4, 3, envelope16);

    newdigate::flashloader loader;
    sample1 = loader.loadSample("PNO1C1.RAW");
    sample2 = loader.loadSample("PNO1C2.RAW");
    sample3 = loader.loadSample("PNO1C3.RAW");
    sample4 = loader.loadSample("PNO1C4.RAW");
    sample5 = loader.loadSample("PNO1C5.RAW");
    
    _sampler.addSample(KEY_NOTENUMBER_C1, sample1->sampledata, sample1->samplesize / 2);
    _sampler.addSample(KEY_NOTENUMBER_C2, sample2->sampledata, sample2->samplesize / 2);
    _sampler.addSample(KEY_NOTENUMBER_C3, sample3->sampledata, sample3->samplesize / 2);
    _sampler.addSample(KEY_NOTENUMBER_C4, sample4->sampledata, sample4->samplesize / 2);
    _sampler.addSample(KEY_NOTENUMBER_C5, sample5->sampledata, sample5->samplesize / 2);

    MIDI.setHandleNoteOn(handleNoteOn);  
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.begin(MIDI_CHANNEL_OMNI);

    //mixer3.gain(0, 0.25f);
    //mixer3.gain(1, 0.25f);

    AudioMemory(50);
    cs42448.enable();
    cs42448.volume(1.0f);

    delay(10);
    Serial.println("Teensy polyphonic sampling demo... Connect your MIDI input cable ");

    /*
    sequencer *_tempoSequencer = _multisequencer.getTempoSequencer();
    _adapter.loadMidi("marbles.mid");
    _adapter.loadMidiTempoTrack(0, 64, 0); // track 0, 64 * 4 beats, offset by 0 bars
    */
    _sequencer = _multisequencer.newSequencer();

    _adapter.loadMidi("marbles.mid");
    _adapter.loadMidifileToNextChannelPattern(0, 0, 64, 0);  // multicequencer channel number, midi track number, 35 bars long
    _adapter.close();

    _multisequencer.start(micros());
    _sequencer->setNextPattern(0);  

    _sequencer->onevent = [] (sequencerevent *event) { 
        _sampler.noteEvent(event->noteNumber, event->velocity, event->isNoteStartEvent, false);
    };  
}

void loop() {
    _multisequencer.tick(micros());
    MIDI.read();
    //delay(10);
}

namespace std {
    void __throw_bad_function_call() {}
    void __throw_length_error(char const*) {}
}

unsigned __exidx_start;
unsigned __exidx_end;