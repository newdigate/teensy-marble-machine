#include <Arduino.h>
#include <MIDI.h>
#include <Audio.h>
#include "sampler.h"
#include "flashloader.h"
#include "midireader.h"
#include "midisequenceadapter.h"
#include "polyphonicsampler.h"
#include "tempo.h"
#include "TeensyVariablePlayback.h"
#include <ST7735_t3.h> // Hardware-specific library
#include "TFTPianoDisplay.h"

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
AudioConnection          patchCord37(mixer5, 0, mixer7, 0);
AudioConnection          patchCord38(mixer6, 0, mixer8, 0);
AudioConnection          patchCord39(mixer7, 0, tdm1, 0);
AudioConnection          patchCord40(mixer7, 0, tdm1, 4);
AudioConnection          patchCord41(mixer8, 0, tdm1, 2);
AudioConnection          patchCord42(mixer8, 0, tdm1, 6);
AudioControlCS42448      cs42448;      //xy=821,899
// GUItool: end automatically generated code
#pragma endregion

#pragma region tft
#define TFT_CS   6  // CS & DC can use pins 2, 6, 9, 10, 15, 20, 21, 22, 23
#define TFT_DC    2  //  but certain pairs must NOT be used: 2+10, 6+9, 20+23, 21+22
#define TFT_RST   255 // RST can use any pin
ST7735_t3 tft = ST7735_t3(TFT_CS, TFT_DC, TFT_RST);
#pragma endregion

TFTPianoDisplay pianoDisplay1(tft, 3, 4, 4, 0); //tft, byte octaves, byte startOctave, byte x, byte y
TFTPianoDisplay pianoDisplay2(tft, 3, 7, 4, 13); //tft, byte octaves, byte startOctave, byte x, byte y

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

void bmpDraw(const char *filename, uint8_t x, uint16_t y);
uint16_t read16();
uint32_t read32();

tempo _tempo(140.0f);
multisequencer _multisequencer(_tempo);
midisequenceadapter _adapter(_multisequencer); // to read midi files into the a sequencer pattern
sequencer *_sequencer;

AudioPlayArrayResmp *voices[] = {&voice1, &voice2, &voice3, &voice4, &voice5, &voice6, &voice7, &voice8};

void setup() {
    Serial.begin(9600);

    tft.initR(INITR_GREENTAB);
    tft.setRotation(3);
    tft.fillScreen(ST7735_BLACK);
    tft.println("loading...");
    
    while (!SD.begin(BUILTIN_SDCARD)) {
      tft.println("SD initialization failed!");
      delay(1000);
    }
    tft.println("SD initialised.");

    for (unsigned int i=0; i<8; i++) {
        voices[i]->setNumChannels(2);
        voices[i]->enableInterpolation(true);
        voices[i]->begin();
    }


    mixer7.gain(0, 0.35);
    mixer8.gain(0, 0.35);
    
    
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
    tft.println("C1");

    sample2 = loader.loadSample("PNO1C2.RAW");
    tft.println("C2");

    sample3 = loader.loadSample("PNO1C3.RAW");
    tft.println("C3");

    sample4 = loader.loadSample("PNO1C4.RAW");
    tft.println("C4");

    sample5 = loader.loadSample("PNO1C5.RAW");
    tft.println("C5");
    
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

    tft.fillScreen(ST7735_BLACK);
    tft.setCursor(0, 26);
    tft.println("  Wintergatan\n  Marble machine....");

    _sequencer = _multisequencer.newSequencer();

    _adapter.loadMidi("marbles.mid");
    _adapter.loadMidifileToNextChannelPattern(0, 0, 64, 0);  // multicequencer channel number, midi track number, 35 bars long
    _adapter.close();

    _multisequencer.start(micros());
    _sequencer->setNextPattern(0);  

    _sequencer->onevent = [] (sequencerevent *event) { 

        _sampler.noteEvent(event->noteNumber, event->velocity, event->isNoteStartEvent, false);
        if (event->isNoteStartEvent) {
            pianoDisplay1.keyDown(event->noteNumber);
            pianoDisplay2.keyDown(event->noteNumber);
        } else {
            pianoDisplay1.keyUp(event->noteNumber);
            pianoDisplay2.keyUp(event->noteNumber);
        } 
    };  


    if (SD.exists("wilson.jpg")) {
        Serial.println("drawing wilson...");
        File wilson = SD.open("wilson.jpg");
        bmpDraw("wilson.bmp", 0, 45);
    } else 
      Serial.println("wilson not found...");
  
}

void loop() {
  
    _multisequencer.tick(micros());
    MIDI.read();
    if (pianoDisplay1.displayNeedsUpdating())
        pianoDisplay1.drawPiano();
    if (pianoDisplay2.displayNeedsUpdating())
        pianoDisplay2.drawPiano();
        
}

namespace std {
    void __throw_bad_function_call() {}
    void __throw_length_error(char const*) {}
}

unsigned __exidx_start;
unsigned __exidx_end;

File bmpFile;
#define BUFFPIXEL 20
void bmpDraw(const char *filename, uint8_t x, uint16_t y) {


    int      bmpWidth, bmpHeight;   // W+H in pixels
    uint8_t  bmpDepth;              // Bit depth (currently must be 24)
    uint32_t bmpImageoffset;        // Start of image data in file
    uint32_t rowSize;               // Not always = bmpWidth; may have padding
    uint8_t  sdbuffer[3 * BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
    uint16_t buffidx = sizeof(sdbuffer); // Current position in sdbuffer
    boolean  goodBmp = false;       // Set to true on valid header parse
    boolean  flip    = true;        // BMP is stored bottom-to-top
    int      w, h, row, col;
    uint8_t  r, g, b;
    uint32_t pos = 0, startTime = millis();

    uint16_t awColors[320];  // hold colors for one row at a time...

    if ((x >= tft.width()) || (y >= tft.height())) return;

    Serial.println();
    Serial.print("Loading image '");
    Serial.print((char *) filename);
    Serial.print("\'\n");

    // Open requested file on SD card
    bmpFile = SD.open(filename);
    if (!bmpFile) {
        Serial.print("File not found");
        tft.fillScreen(ST7735_BLACK);
        tft.setCursor(12, tft.height() / 2 - 12);
        tft.print("Unable to");
        tft.setCursor(12, tft.height() / 2 - 0);
        tft.print("read file: ");
        tft.setCursor(12, tft.height() / 2 + 12);
        tft.setTextColor(ST7735_YELLOW);
        tft.print(filename);
        return;
    }

    // Parse BMP header
    if (read16() == 0x4D42) { // BMP signature
        Serial.print("File size: ");
        uint fileSize = read32();
        Serial.printf("%u \n", fileSize);
        (void)read32(); // Read & ignore creator bytes
        bmpImageoffset = read32(); // Start of image data
        Serial.printf("Image Offset: ");
        Serial.printf("%u \n", bmpImageoffset);
        // Read DIB header
        Serial.printf("Header size: %u\n", read32());
        bmpWidth  = read32();
        bmpHeight = read32();
        if (read16() == 1) { // # planes -- must be '1'
            bmpDepth = read16(); // bits per pixel
            Serial.print("Bit Depth: "); Serial.print(bmpDepth);
            if ((bmpDepth == 24) && (read32() == 0)) { // 0 = uncompressed

                goodBmp = true; // Supported BMP format -- proceed!
                Serial.printf("Image size: %u x %u\n", bmpWidth, bmpHeight);

                // BMP rows are padded (if needed) to 4-byte boundary
                rowSize = (bmpWidth * 3 + 3) & ~3;

                // If bmpHeight is negative, image is in top-down order.
                // This is not canon but has been observed in the wild.
                if (bmpHeight < 0) {
                    bmpHeight = -bmpHeight;
                    flip      = false;
                }

                // Crop area to be loaded
                w = bmpWidth;
                h = bmpHeight;
                if ((x + w - 1) >= tft.width())  w = tft.width()  - x;
                if ((y + h - 1) >= tft.height()) h = tft.height() - y;

                for (row = 0; row < h; row++) { // For each scanline...

                    // Seek to start of scan line.  It might seem labor-
                    // intensive to be doing this on every line, but this
                    // method covers a lot of gritty details like cropping
                    // and scanline padding.  Also, the seek only takes
                    // place if the file position actually needs to change
                    // (avoids a lot of cluster math in SD library).
                    if (flip) // Bitmap is stored bottom-to-top order (normal BMP)
                        pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
                    else     // Bitmap is stored top-to-bottom
                        pos = bmpImageoffset + row * rowSize;
                    if (bmpFile.position() != pos) { // Need seek?
                        bmpFile.seek(pos);
                        buffidx = sizeof(sdbuffer); // Force buffer reload
                    }

                    for (col = 0; col < w; col++) { // For each pixel...
                        // Time to read more pixel data?
                        if (buffidx >= sizeof(sdbuffer)) { // Indeed
                            bmpFile.read(sdbuffer, sizeof(sdbuffer));
                            buffidx = 0; // Set index to beginning
                        }

                        // Convert pixel from BMP to TFT format, push to display
#ifdef IMAGE_COLORS_BGR
                        b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
#else
                        r = sdbuffer[buffidx++];
                        g = sdbuffer[buffidx++];
                        b = sdbuffer[buffidx++];
#endif
                        awColors[col] = tft.Color565(r, g, b);
                    } // end pixel
                    tft.writeRect(0, row + y, w, 1, awColors);
                } // end scanline
                Serial.print("Loaded in ");
                Serial.print(millis() - startTime);
                Serial.print(" ms\n");
            } // end goodBmp
        }
    }

    bmpFile.close();
    if (!goodBmp) Serial.print("BMP format not recognized.\n");
}

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16() {
    uint16_t result = 0;
    ((uint8_t *)&result)[0] = bmpFile.read(); // LSB
    ((uint8_t *)&result)[1] = bmpFile.read(); // MSB
    return result;
}

uint32_t read32() {
    uint32_t result = 0;
    uint8_t x1 = (uint8_t)bmpFile.read();
    uint8_t x2 = (uint8_t)bmpFile.read();
    uint8_t x3 = (uint8_t)bmpFile.read();
    uint8_t x4 = (uint8_t)bmpFile.read();
    ((uint8_t *)&result)[0] = x1; // LSB
    ((uint8_t *)&result)[1] = x2;
    ((uint8_t *)&result)[2] = x3;
    ((uint8_t *)&result)[3] = x4; // MSB
    return result;
}
