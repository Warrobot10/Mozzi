/*  Example using clipping to modify the spectrum of an audio signal
    and emphasise a tone generated by the difference in frequency of 2 waves,
    using Mozzi sonification library.

    Adaptation of the Difference_Tone example using FixMath.

    Demonstrates the use of EventDelay(), rand() and fixed-point numbers.

    Demonstrates Oscil::phMod() for phase modulation,
    Smooth() for smoothing control signals,
    and FixMath fixed point number types for fractional frequencies.
    This is the same technique than the FMsynth example but
    using FixMath instead of mozzi_fixmath.

		Mozzi documentation/API
		https://sensorium.github.io/Mozzi/doc/html/index.html

		Mozzi help/discussion/announcements:
    https://groups.google.com/forum/#!forum/mozzi-users

    Tim Barras, Thomas Combriat and the Mozzi team 2023, CC by-nc-sa.
*/


#include <Mozzi.h>
#include <Oscil.h>
#include <EventDelay.h>
#include <FixMath.h>
#include <mozzi_rand.h>
#include <mozzi_midi.h>
#include <tables/sin2048_int8.h>


// use: Oscil <table_size, update_rate> oscilName (wavetable), look in .h file of table #included above
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> aSin1(SIN2048_DATA);  // sine wave sound source
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> aSin2(SIN2048_DATA);  // sine wave sound source
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> aGain(SIN2048_DATA);  // to fade audio signal in and out before waveshaping

// for scheduling note changes
EventDelay kChangeNoteDelay;

const UFix<8, 0> freq1 = 184;
const auto harmonic_step = freq1 * UFix<8, 0>(12).invAccurate();  // harmonic_step = freq1/12;

void setup() {
  Serial.begin(115200);
  aSin1.setFreq(freq1);
  aGain.setFreq(0.2f);         // use a float for low frequencies, in setup it doesn't need to be fast
  kChangeNoteDelay.set(2000);  // note duration ms, within resolution of CONTROL_RATE
  startMozzi();                // :)
}

void updateControl() {
  if (kChangeNoteDelay.ready()) {
    UFix<4, 0> harmonic = rand((byte)12);
    auto shimmer = toUFraction(rand((byte)255));  // Creates a UFix<0,8>
    auto freq2difference = (harmonic * harmonic_step) + (harmonic_step * shimmer).sR<3>(); // the shimmering is divided by 8 here
    auto freq2 = (freq1 - freq2difference).asUFix();
    aSin2.setFreq((freq2));
    kChangeNoteDelay.start();
  }
}

AudioOutput_t updateAudio() {
  auto asig = (toSInt(aSin1.next()) + toSInt(aSin2.next())) * (toSFraction(aGain.next()) + UFix<1, 7>(1.2));  // this is a SFix<9,9> in the end
  return MonoOutput::fromAlmostNBit(11, asig.asRaw()).clip();                                                 // TODO, implement smart MonoOutput
}

void loop() {
  audioHook();  // required here
}