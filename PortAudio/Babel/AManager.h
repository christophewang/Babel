#ifndef	__AMANAGER_H__
#define	__AMANAGER_H__

#include	"IPortAudio.h"
#include	"portaudio.h"

#define SAMPLE_RATE  (8000)
#define FRAMES_PER_BUFFER (256)
#define NUM_SECONDS     (5)
#define NUM_CHANNELS    (1)
#define DITHER_FLAG     (0)
#define WRITE_TO_FILE   (0)
#define PA_SAMPLE_TYPE  paInt16
typedef short SAMPLE;
#define SAMPLE_SILENCE  (0)
#define PRINTF_S_FORMAT "%d"

typedef struct
{
	int          frameIndex;
	int          maxFrameIndex;
	SAMPLE      *recordedSamples;
}paTestData;

class	AManager	:	public	IAudio
{
protected:
	PaStreamParameters  inputParameters, outputParameters;
	PaStream*           stream;
	PaError             err;
	paTestData          data;
	int                 i;
	int                 totalFrames;
	int                 numSamples;
	int                 numBytes;
	SAMPLE              max, val;
	double              average;
public:
	AManager(void);
	virtual	~AManager(void);
	static int	playCallBack(const void *, void *, unsigned long, const PaStreamCallbackTimeInfo *, PaStreamCallbackFlags, void *);
	static int	recordCallBack(const void *, void *, unsigned long,	const PaStreamCallbackTimeInfo *, PaStreamCallbackFlags, void *);
};

#endif