#include	"AManager.h"
#include	"portaudio.h"
#include	<iostream>
#include	<cstdlib>

int main(void)
{
	PaStreamParameters  inputParameters, outputParameters;
	PaStream*           stream;
	PaError             err = paNoError;
	paTestData          data;
	int                 i;
	int                 totalFrames;
	int                 numSamples;
	int                 numBytes;
	/* Initialise Input */
	std::cout << "patest_record.c" << std::endl;
	fflush(stdout);
	data.maxFrameIndex = totalFrames = NUM_SECONDS * SAMPLE_RATE;
	data.frameIndex = 0;
	numSamples = totalFrames * NUM_CHANNELS;
	numBytes = numSamples * sizeof(SAMPLE);
	data.recordedSamples = (SAMPLE *) malloc(numBytes);
	if (data.recordedSamples == NULL)
	{
		std::cerr << "Could not allocate record array." << std::endl;
		goto done;
	}
	for (i = 0; i < numSamples; i++)
		data.recordedSamples[i] = 0;
	err = Pa_Initialize();
	if (err != paNoError)
		goto done;
	inputParameters.device = Pa_GetDefaultInputDevice();
	if (inputParameters.device == paNoDevice)
	{
		std::cerr << "Error: No default input device." << std::endl;
		goto done;
	}
	inputParameters.channelCount = 1;
	inputParameters.sampleFormat = PA_SAMPLE_TYPE;
	inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
	inputParameters.hostApiSpecificStreamInfo = NULL;
	/* Open Input Stream */
	err = Pa_OpenStream(&stream,
		&inputParameters,
		NULL,
		SAMPLE_RATE,
		FRAMES_PER_BUFFER,
		paClipOff,
		&(AManager::recordCallBack),
		&data);
	if (err != paNoError)
		goto done;
	/* Record Stream */
	err = Pa_StartStream(stream);
	if (err != paNoError)
		goto done;
	std::cout << std::endl << "=== Now recording!! Please speak into the microphone. ===" << std::endl;
	fflush(stdout);
	while ((err = Pa_IsStreamActive(stream)) == 1)
	{
		Pa_Sleep(1000);
		std::cout << "index = " << data.frameIndex << std::endl;
		fflush(stdout);
	}
	if (err < 0)
		goto done;
	err = Pa_CloseStream(stream);
	if (err != paNoError)
		goto done;
	/* Initialise Output Stream */
	data.frameIndex = 0;
	outputParameters.device = Pa_GetDefaultOutputDevice();
	if (outputParameters.device == paNoDevice)
	{
		fprintf(stderr,"Error: No default output device.\n");
		goto done;
	}
	outputParameters.channelCount = 1;
	outputParameters.sampleFormat = PA_SAMPLE_TYPE;
	outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;
	/* Open Output Stream */
	std::cout << std::endl << "=== Now playing back. ===" << std::endl;
	fflush(stdout);
	err = Pa_OpenStream(&stream,
		NULL,
		&outputParameters,
		SAMPLE_RATE,
		FRAMES_PER_BUFFER,
		paClipOff,
		&(AManager::playCallBack),
		&data );
	if (err != paNoError)
		goto done;
	/* Play Back Stream */
	if (stream)
	{
		err = Pa_StartStream(stream);
		if (err != paNoError)
			goto done;
		std::cout << "Waiting for playback to finish." << std::endl;
		fflush(stdout);
		while ((err = Pa_IsStreamActive(stream)) == 1)
			Pa_Sleep(100);
		if (err < 0)
			goto done;
		err = Pa_CloseStream(stream);
		if (err != paNoError)
			goto done;
		std::cout << "Done." << std::endl;
		fflush(stdout);
	}
	/* Terminate Audio */
done:
	Pa_Terminate();
	if (data.recordedSamples)
		free(data.recordedSamples);
	if (err != paNoError)
	{
		std::cerr << "An error occured while using the portaudio stream" <<std::endl;
		std::cerr << "Error number: " << err << std::endl;
		std::cerr << "Error message: " << Pa_GetErrorText(err) << std::endl;
		err = 1;
	}
	return err;
}