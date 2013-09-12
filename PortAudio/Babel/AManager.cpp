#include	"AManager.h"
#include	<iostream>
#include	<cstdlib>

AManager::AManager(void) : err(paNoError) {}

AManager::~AManager(void) {}

int	AManager::playCallBack(const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo *timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData)
{
	paTestData *data = (paTestData*)userData;
    SAMPLE *rptr = &data->recordedSamples[data->frameIndex * NUM_CHANNELS];
	SAMPLE *wptr = (SAMPLE*)outputBuffer;
    unsigned int i;
    int finished;
	unsigned int framesLeft = data->maxFrameIndex - data->frameIndex;

	(void) inputBuffer;
	(void) timeInfo;
	(void) statusFlags;
	(void) userData;

	if (framesLeft < framesPerBuffer)
	{
		for (i = 0; i < framesLeft; i++)
		{
			*wptr++ = *rptr++;
			if (NUM_CHANNELS == 2)
				*wptr++ = *rptr++;
		}
		for (i = 0; i < framesPerBuffer; i++)
		{
			*wptr++ = 0;
			if (NUM_CHANNELS == 2)
				*wptr++ = 0;
		}
		data->frameIndex += framesLeft;
		finished = paComplete;
	}
	else
	{
		for (i = 0; i < framesPerBuffer; i++)
		{
			*wptr++ = *rptr++;
			if (NUM_CHANNELS == 2)
				*wptr++ = *rptr++;
		}
		data->frameIndex += framesPerBuffer;
		finished = paContinue;
	}
	return finished;
}

int	AManager::recordCallBack(const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData)
{
	paTestData *data = (paTestData*)userData;
	const SAMPLE *rptr = (const SAMPLE*)inputBuffer;
	SAMPLE *wptr = &data->recordedSamples[data->frameIndex * NUM_CHANNELS];
	long framesToCalc;
	long i;
	int finished;
	unsigned long framesLeft = data->maxFrameIndex - data->frameIndex;

	(void) outputBuffer;
	(void) timeInfo;
	(void) statusFlags;
	(void) userData;
	
	if (framesLeft < framesPerBuffer)
	{
		framesToCalc = framesLeft;
		finished = paComplete;
	}
	else
	{
		framesToCalc = framesPerBuffer;
		finished = paContinue;
	}
	if (inputBuffer == NULL)
	{
		for (i = 0; i < framesToCalc; i++)
		{
			*wptr++ = SAMPLE_SILENCE;
			if (NUM_CHANNELS == 2)
				*wptr++ = SAMPLE_SILENCE;
		}
	}
	else
	{
		for (i = 0; i < framesToCalc; i++)
		{
			*wptr++ = *rptr++;
			if (NUM_CHANNELS == 2)
				*wptr++ = *rptr++;
		}
	}
	data->frameIndex += framesToCalc;
	return finished;
}