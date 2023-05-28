#ifndef AUDIOCONFIGARDUINOFSP_H
#define AUDIOCONFIGARDUINOFSP_H

#if not IS_FSP()
#error This header should be included for Arduino FSB board (Uno R4/Renesa) family, only
#endif




#define AUDIO_CHANNEL_1_PIN A0
#if (AUDIO_CHANNELS > 1)
#define AUDIO_CHANNEL_2_PIN 10
#endif

#define AUDIO_BITS 12 // random value for now

#define AUDIO_BITS_PER_CHANNEL AUDIO_BITS

#define AUDIO_BIAS ((uint16_t) 1<<(AUDIO_BITS-1))

#define BYPASS_MOZZI_OUTPUT_BUFFER true  // Mozzi initial buffer are not of the good type, so we bypass it and create our own

#endif        //  #ifndef AUDIOCONFIGARDUINOFSP_H

