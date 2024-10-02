#include "audio.h"
#include "util.h"

#include <raylib.h>

#define SAMPLES_COUNT 1024
#define SAMPLE_RATE 44100
#define SAMPLE_DURATION (1.0f/SAMPLE_RATE)

float frequency = 1.0f;
float amplitude = 1.0f;
float phase = 0.0f;

AudioStream stream;

float get_frequency() {return frequency;}
void  set_frequency(float new_frequency) {frequency = new_frequency;}
float get_amplitude() {return amplitude;}
void  set_amplitude(float new_amplitude) {amplitude = new_amplitude;}

void update_audio(void* buffer, unsigned int samples_count) {
  float* samples = (float*) buffer;
  for (unsigned int sample_index = 0; sample_index < samples_count; sample_index++) {
    float p = 0.5;
    samples[sample_index] = amplitude * ((1-p)*sinf(phase) + p*sinf(phase/2.0));
    phase += 2*PI * SAMPLE_DURATION * frequency;
    // need this for precision issues; program goes "out of tune" without it
    if (phase > 2*PI) phase -= 2*PI;
  }
}

void setup_audio() {
  SetTraceLogLevel(LOG_ERROR);

  InitAudioDevice();
  assert(IsAudioDeviceReady());

  SetAudioStreamBufferSizeDefault(SAMPLES_COUNT);

  int sample_size = 32;
  int channels = 1;
  stream = LoadAudioStream(SAMPLE_RATE, sample_size, channels); 

  SetAudioStreamCallback(stream, &update_audio);

  PlayAudioStream(stream);
}

void cleanup_audio() {
  UnloadAudioStream(stream);
  CloseAudioDevice();
}


