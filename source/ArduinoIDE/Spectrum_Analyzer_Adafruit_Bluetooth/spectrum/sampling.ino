////////////////////////////////////////////////////////////////////////////////
// SAMPLING AND FFT FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

IntervalTimer samplingTimer;
int sampleCounter = 0;

void samplingSetup() {
  // Set up ADC and audio input.
  pinMode(AUDIO_INPUT_MIC_PIN, INPUT);
  analogReadResolution(ANALOG_READ_RESOLUTION);
  analogReadAveraging(ANALOG_READ_AVERAGING);
}

////////////////////////////////////////////////////////////////////////////////
// SAMPLING FUNCTIONS
////////////////////////////////////////////////////////////////////////////////
void samplingCallback() {
  // Read from the ADC and store the sample data
  samples[sampleCounter] = (float32_t)analogRead(AUDIO_INPUT_MIC_PIN);
  // Complex FFT functions require a coefficient for the imaginary part of the input.
  // Since we only have real data, set this coefficient to zero.
  samples[sampleCounter+1] = 0.0;
  // Update sample buffer position and stop after the buffer is filled
  sampleCounter += 2;
  if (sampleCounter >= FFT_SIZE*2) {
    samplingTimer.end();
  }
}

void samplingBegin() {
  // Reset sample buffer position and start callback at necessary rate.
  sampleCounter = 0;
  samplingTimer.begin(samplingCallback, 1000000/SAMPLE_RATE_HZ);
}

boolean samplingIsDone() {
  return sampleCounter >= FFT_SIZE*2;
}

void performFFT() {
  // Run FFT on sample data.
  arm_cfft_radix4_instance_f32 fft_inst;
  arm_cfft_radix4_init_f32(&fft_inst, FFT_SIZE, 0, 1);
  arm_cfft_radix4_f32(&fft_inst, samples);
  // Calculate magnitude of complex numbers output by the FFT.
  arm_cmplx_mag_f32(samples, magnitudes, FFT_SIZE);
}

