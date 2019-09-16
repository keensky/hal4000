//#define XRES        640		//4:3
//#define YRES        480

//#define XRES        1024		//4:3
//#define YRES        768

// HD Ready 720p
//#define XRES        1280		//16:9
//#define YRES        720

//#define XRES		  1680
//#define YRES		  1050

// Full HD 1080p
//#define XRES        1920		//16:9
//#define YRES        1080

// 4k
//#define XRES		3840 
//#define YRES		2160 

#ifndef XRES
#define XRES 1280
//#define XRES 1920
//#define XRES 1024
#endif
#ifndef YRES
#define YRES 720
//#define YRES 1080
//#define YRES 768
#endif

//#define CAPTURE				// Capture output
//#define CLEANDESTROY          // destroy stuff (windows, glContext, ...)
#define FULLSCREEN
//#define SHADER_DEBUG

//#define USE_SOUND_THREAD
//#define VIERKLANG_RENDER
//#define NO_GLSL_RENDER
#define NO_MMTIME
//#define NO_VIERKLANG
//#define DEBUG
#define INIT
//#define PLAY_SINE

#ifndef INIT
//#undef FULLSCREEN
//#undef _WINDOWS
//#pragma comment(linker, "/SUBSYSTEM:console")
//#define _CONSOLE
#include <stdio.h>
#endif

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#define VC_LEANMEAN
#define VC_EXTRALEAN

#include <windows.h>
#include <mmsystem.h>
#include <mmreg.h>
#include <GL/gl.h>
#endif

#include "hal4000.h"
#include "4klang.h"
#define INTRO_LENGTH (1000ull * MAX_SAMPLES / (1ull * SAMPLE_RATE) )

#if defined(CAPTURE)
#undef FULLSCREEN
#undef NO_MMTIME
//#undef _WINDOWS
#pragma comment(linker, "/SUBSYSTEM:console")
//#define _CONSOLE
#pragma warning(disable:4996)
#pragma warning(disable:4244)
#include <stdio.h>
#include <malloc.h>
#include <math.h>
//#define INTRO_LENGTH (1000ull * MAX_SAMPLES / SAMPLE_RATE)
#endif

//#define STREAM
#ifdef STREAM
#include <shlwapi.h>
#endif

#include "glext.h"

#if defined(_DEBUG) || defined(CAPTURE)
//#pragma data_seg(".fltused")
int _fltused = 1;
#endif

#ifdef DEBUG
#define NUMFUNCIONES 12
#else
#define NUMFUNCIONES 3

#if defined(YRES) && YRES == 1080
//char *dummy_data = {
//	"hal 4000 by Sands+SKOYP for CC 2017!#"
//};
#endif
//#if defined(YRES) && YRES == 768
//#undef USE_SOUND_THREAD
//#endif

#endif

extern void *myglfunc[NUMFUNCIONES];

#define oglUseProgram                   ((PFNGLUSEPROGRAMPROC)myglfunc[0])
#define oglGetUniformLocation           ((PFNGLGETUNIFORMLOCATIONARBPROC)myglfunc[1])
#define oglUniform3f					((PFNGLUNIFORM3FPROC)myglfunc[2])
#ifdef DEBUG
#define oglUniform1i                    ((PFNGLUNIFORM1IPROC)myglfunc[3])
#define oglCreateProgram	            ((PFNGLCREATEPROGRAMPROC)myglfunc[4])
#define oglCreateShader		            ((PFNGLCREATESHADERPROC)myglfunc[5])
#define oglShaderSource                 ((PFNGLSHADERSOURCEPROC)myglfunc[6])
#define oglCompileShader                ((PFNGLCOMPILESHADERPROC)myglfunc[7])
#define oglAttachShader                 ((PFNGLATTACHSHADERPROC)myglfunc[8])
#define oglLinkProgram                  ((PFNGLLINKPROGRAMPROC)myglfunc[9])
#define oglGetObjectParameteriv         ((PFNGLGETOBJECTPARAMETERIVARBPROC)myglfunc[10])
#define oglGetInfoLog                   ((PFNGLGETINFOLOGARBPROC)myglfunc[11])
#endif

//#define oglUniform4fv                   ((PFNGLUNIFORM4FVPROC)myglfunc[7])
//#define oglColor3f						((PFNGLGETUNIFORMLOCATIONARBPROC)myglfunc[8])
//wglUseFontOutlines

// init
void EXT_Init(void);

void* myglfunc[NUMFUNCIONES];
void EXT_Init(void)
{
	myglfunc[0] = wglGetProcAddress("glUseProgram");	
	myglfunc[1] = wglGetProcAddress("glGetUniformLocation");
	myglfunc[2] = wglGetProcAddress("glUniform3f");
#ifdef DEBUG
	myglfunc[3] = wglGetProcAddress("glUniform1i");
	myglfunc[4] = wglGetProcAddress("glCreateProgram");
	myglfunc[5] = wglGetProcAddress("glCreateShader");
	myglfunc[6] = wglGetProcAddress("glShaderSource");
	myglfunc[7] = wglGetProcAddress("glCompileShader");
	myglfunc[8] = wglGetProcAddress("glAttachShader");
	myglfunc[9] = wglGetProcAddress("glLinkProgram");
	myglfunc[10] = wglGetProcAddress("glGetObjectParameteriv");
	myglfunc[11] = wglGetProcAddress("glGetInfoLog");
#endif
}

int pid;

#ifdef _DEBUG
#ifdef FULLSCREEN
#undef FULLSCREEN
#endif
#define DEBUG_FUNCLOAD
#define SHADER_DEBUG
#endif

#ifdef CAPTURE
static GLubyte backbufferData[XRES*YRES * 3];
#ifndef CAPTURE_FRAMERATE
#define CAPTURE_FRAMERATE 60
#endif
#define LOL(x) #x
#define STR(x) LOL(x)
#define FFMPEG_CAPTURE_INPUT "ffmpeg.exe -y -f rawvideo -vcodec rawvideo -s "\
	STR(XRES) "x" STR(YRES) " -pix_fmt rgb24 -framerate " STR(CAPTURE_FRAMERATE)\
	" -i - -c:v libx264 -crf 18 -preset slow -vf vflip "\
	"capture_" STR(XRES) "x" STR(YRES) ".mp4"

#ifdef CAPTURE
int check = 0;
DWORD WINAPI checkESC() {
	//if (GetAsyncKeyState(VK_ESCAPE)) {
	//	ExitProcess(0);
	//}
	//else {
	if (check == 0)	printf(".");
}
#endif

int isBigEndian() {
	int test = 1;
	char *p = (char*)&test;

	return p[0] == 0;
}

void reverseEndianness(const long long int size, void* value) {
	int i;
	char result[32];
	for (i = 0; i<size; i += 1) {
		result[i] = ((char*)value)[size - i - 1];
	}
	for (i = 0; i<size; i += 1) {
		((char*)value)[i] = result[i];
	}
}
void toBigEndian(const long long int size, void* value) {
	char needsFix = !((1 && isBigEndian()) || (0 && !isBigEndian()));
	if (needsFix) {
		reverseEndianness(size, value);
	}
}
void toLittleEndian(const long long int size, void* value) {
	char needsFix = !((0 && isBigEndian()) || (1 && !isBigEndian()));
	if (needsFix) {
		reverseEndianness(size, value);
	}
}

// ------------------------------------------------- [ Section: Wave Header ] -
typedef struct WaveHeader {
	// Riff Wave Header
	char chunkId[4];
	int  chunkSize;
	char format[4];

	// Format Subchunk
	char subChunk1Id[4];
	int  subChunk1Size;
	short int audioFormat;
	short int numChannels;
	int sampleRate;
	int byteRate;
	short int blockAlign;
	short int bitsPerSample;
	//short int extraParamSize;

	// Data Subchunk
	char subChunk2Id[4];
	int  subChunk2Size;

} WaveHeader;

WaveHeader makeWaveHeader(int const sampleRate, short int const numChannels, short int const bitsPerSample) {
	WaveHeader myHeader;

	// RIFF WAVE Header
	myHeader.chunkId[0] = 'R';
	myHeader.chunkId[1] = 'I';
	myHeader.chunkId[2] = 'F';
	myHeader.chunkId[3] = 'F';
	myHeader.format[0] = 'W';
	myHeader.format[1] = 'A';
	myHeader.format[2] = 'V';
	myHeader.format[3] = 'E';

	// Format subchunk
	myHeader.subChunk1Id[0] = 'f';
	myHeader.subChunk1Id[1] = 'm';
	myHeader.subChunk1Id[2] = 't';
	myHeader.subChunk1Id[3] = ' ';
	myHeader.audioFormat = 1; // Audio format 1=PCM, 3=IEEE Float, 6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM, 65534=Extensible
	myHeader.numChannels = numChannels; // 1 for MONO, 2 for stereo
	myHeader.sampleRate = sampleRate; // ie 44100 hertz, cd quality audio
	myHeader.bitsPerSample = bitsPerSample; // 
	myHeader.byteRate = myHeader.sampleRate * myHeader.numChannels * myHeader.bitsPerSample / 8;
	myHeader.blockAlign = myHeader.numChannels * myHeader.bitsPerSample / 8;

	// Data subchunk
	myHeader.subChunk2Id[0] = 'd';
	myHeader.subChunk2Id[1] = 'a';
	myHeader.subChunk2Id[2] = 't';
	myHeader.subChunk2Id[3] = 'a';

	// All sizes for later:
	// chuckSize = 4 + (8 + subChunk1Size) + (8 + subChubk2Size)
	// subChunk1Size is constanst, i'm using 16 and staying with PCM
	// subChunk2Size = nSamples * nChannels * bitsPerSample/8
	// Whenever a sample is added:
	//    chunkSize += (nChannels * bitsPerSample/8)
	//    subChunk2Size += (nChannels * bitsPerSample/8)
	myHeader.chunkSize = 4 + 8 + 16 + 8 + 0;
	myHeader.subChunk1Size = 16;
	myHeader.subChunk2Size = 0;

	return myHeader;
}

typedef struct Wave {
	WaveHeader header;
	char* data;
	long long int index;
	long long int size;
	long long int nSamples;
} Wave;

Wave makeWave(int const sampleRate, short int const numChannels, short int const bitsPerSample) {
	Wave myWave;
	myWave.header = makeWaveHeader(sampleRate, numChannels, bitsPerSample);
	return myWave;
}

void waveDestroy(Wave* wave) {
	free(wave->data);
}

void waveSetDuration(Wave* wave, const float seconds) {
	long long int totalBytes = (long long int)(wave->header.byteRate*seconds);
	wave->data = (char*)malloc(totalBytes);
	wave->index = 0;
	wave->size = totalBytes;
	wave->nSamples = (long long int) wave->header.numChannels * wave->header.sampleRate * seconds;
	wave->header.chunkSize = 4 + 8 + 16 + 8 + totalBytes;
	wave->header.subChunk2Size = totalBytes;
}

void waveAddSample(Wave* wave, const float* samples) {
	int i;
	short int sample8bit;
	int sample16bit;
	long int sample32bit;
	char* sample;
	if (wave->header.bitsPerSample == 8) {
		for (i = 0; i<wave->header.numChannels; i += 1) {
			sample8bit = (short int)(127 + 127.0*samples[i]);
			toLittleEndian(1, (void*)&sample8bit);
			sample = (char*)&sample8bit;
			(wave->data)[wave->index] = sample[0];
			wave->index += 1;
		}
	}
	if (wave->header.bitsPerSample == 16) {
		for (i = 0; i<wave->header.numChannels; i += 1) {
			sample16bit = (int)(32767 * samples[i]);
			//sample = (char*)&litEndianInt( sample16bit );
			toLittleEndian(2, (void*)&sample16bit);
			sample = (char*)&sample16bit;
			wave->data[wave->index + 0] = sample[0];
			wave->data[wave->index + 1] = sample[1];
			wave->index += 2;
		}
	}
	if (wave->header.bitsPerSample == 32) {
		for (i = 0; i<wave->header.numChannels; i += 1) {
			sample32bit = (long int)((pow(2, 32 - 1) - 1)*samples[i]);
			//sample = (char*)&litEndianLong( sample32bit );
			toLittleEndian(4, (void*)&sample32bit);
			sample = (char*)&sample32bit;
			wave->data[wave->index + 0] = sample[0];
			wave->data[wave->index + 1] = sample[1];
			wave->data[wave->index + 2] = sample[2];
			wave->data[wave->index + 3] = sample[3];
			wave->index += 4;
		}
	}
}

void _4klangAudioToWAVFile(Wave* wave, const char* filename) {
	// First make sure all numbers are little endian
	toLittleEndian(sizeof(int), (void*)&(wave->header.chunkSize));
	toLittleEndian(sizeof(int), (void*)&(wave->header.subChunk1Size));
	toLittleEndian(sizeof(short int), (void*)&(wave->header.audioFormat));
	toLittleEndian(sizeof(short int), (void*)&(wave->header.numChannels));
	toLittleEndian(sizeof(int), (void*)&(wave->header.sampleRate));
	toLittleEndian(sizeof(int), (void*)&(wave->header.byteRate));
	toLittleEndian(sizeof(short int), (void*)&(wave->header.blockAlign));
	toLittleEndian(sizeof(short int), (void*)&(wave->header.bitsPerSample));
	toLittleEndian(sizeof(int), (void*)&(wave->header.subChunk2Size));

	// Recording a .WAV
	FILE *wavfile;
	wavfile = fopen(filename, "wb");
	fwrite(&(wave->header), sizeof(WaveHeader), 1, wavfile);
	fwrite((void*)(wave->data), sizeof(char), wave->size, wavfile);
	printf(".");
	fclose(wavfile);

	// Convert back to system endian-ness
	toLittleEndian(sizeof(int), (void*)&(wave->header.chunkSize));
	toLittleEndian(sizeof(int), (void*)&(wave->header.subChunk1Size));
	toLittleEndian(sizeof(short int), (void*)&(wave->header.audioFormat));
	toLittleEndian(sizeof(short int), (void*)&(wave->header.numChannels));
	toLittleEndian(sizeof(int), (void*)&(wave->header.sampleRate));
	toLittleEndian(sizeof(int), (void*)&(wave->header.byteRate));
	toLittleEndian(sizeof(short int), (void*)&(wave->header.blockAlign));
	toLittleEndian(sizeof(short int), (void*)&(wave->header.bitsPerSample));
	toLittleEndian(sizeof(int), (void*)&(wave->header.subChunk2Size));
}
#endif

//SAPI voice
#define _ATL_APARTMENT_THREADED
#define COBJMACROS						// to use SAPI in C
#include <sapi.h>

HRESULT		hr;
ISpVoice *	pVoice = NULL;

////for the Font
//	GLuint            base;                            // Base Display List For The Font Set
//	GLYPHMETRICSFLOAT gmf[256];                        // Storage For Information About Our Outline Font CharactersGL
//
//void BuildFont(HDC harp)                                // Build Our Bitmap Font
//{
//		HFONT    font;                                        // Windows Font ID
//		base = glGenLists(256);                                // Storage For 256 Characters
//		font = CreateFont(48, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_TT_PRECIS,
//			CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH, "Arial");
//
//		SelectObject(harp, font);                            // Selects The Font We Created
//		wglUseFontBitmaps(harp, 0, 127, base);
//}
//
//void DrawGLText(char* text)
//{
//		glMatrixMode(GL_MODELVIEW);
//		
//		//glEnable(GL_MULTISAMPLE);
//		//glEnable(GL_POINT_SMOOTH);
//		//glEnable(GL_BLEND);
//		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//		//glHint(GL_POINT_SMOOTH_HINT, GL_DONT_CARE);
//
//		glColor3f(0.5, 0.5, 0.5);
//		glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits		
//		//glRasterPos3i(0.1, 0.2, 1.0);
//		glRasterPos2f(-0.2, -0.6);
//		//glTranslatef(-0.50, -0.50, 0.0);
//		glListBase(base);									// Sets The Base Character to 0
//		glCallLists(14, GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
//		//glFlush();
//		glMatrixMode(GL_PROJECTION);
//
//		glPopAttrib();										// Pops The Display List Bits
//}

#ifdef DEBUG
	static const PIXELFORMATDESCRIPTOR pfd = {
	sizeof(PIXELFORMATDESCRIPTOR), 1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA,
	32, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 32, 0, 0, PFD_MAIN_PLANE, 0, 0, 0, 0 };
#else
	static const PIXELFORMATDESCRIPTOR pfd = { 0, 0, PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER };
#endif

static DEVMODE screenSettings = { { 0 },
#if _MSC_VER < 1400
	0,0,148,0,0x001c0000,{ 0 },0,0,0,0,0,0,0,0,0,{ 0 },0,32,XRES,YRES,0,0,      // Visual C++ 6.0
#else
	0,0,156,0,0x001c0000,{ 0 },0,0,0,0,0,{ 0 },0,32,XRES,YRES,{ 0 }, 0,           // Visual Studio 2005+
#endif
#if(WINVER >= 0x0400)
	0,0,0,0,0,0,
#if (WINVER >= 0x0500) || (_WIN32_WINNT >= 0x0400)
	0,0
#endif
#endif
};

SAMPLE_TYPE	lpSoundBuffer[MAX_SAMPLES * 2];
static HWAVEOUT hWaveOut;

#pragma data_seg(".wavefmt")
static WAVEFORMATEX WaveFMT =
{
#ifdef FLOAT_32BIT	
	WAVE_FORMAT_IEEE_FLOAT,
#else
	WAVE_FORMAT_PCM,
#endif		
	2,                                   // channels
	SAMPLE_RATE,                         // samples per sec
	SAMPLE_RATE * sizeof(SAMPLE_TYPE) * 2, // bytes per sec
	sizeof(SAMPLE_TYPE) * 2,             // block alignment;
	sizeof(SAMPLE_TYPE) * 8,             // bits per sample
	0                                    // extension not needed
};

#pragma data_seg(".wavehdr")
static WAVEHDR WaveHDR =
{
	(LPSTR)lpSoundBuffer, MAX_SAMPLES * sizeof(SAMPLE_TYPE) * 2,0,0,0,0,0,0
};

#ifndef NO_MMTIME
static MMTIME MMTime =
{
	TIME_SAMPLES, 0
};
#endif

#ifdef USE_SOUND_THREAD
//HANDLE		hSoundThread;
//#pragma code_seg(".sndthrd")
//DWORD WINAPI SoundThread(LPVOID lpParam)
//{
//	_4klang_render(lpSoundBuffer);
//	return 0;
//}
#endif

#ifdef PLAY_SINE
#define PI           3.14159265358979323846
#define FREQUENCY 432.0/4
int sgn(double v) {
	if (v < 0) return -1;
	if (v > 0) return 1;
	return 0;
}

#include <math.h>
typedef enum { false, true } bool;

bool open16PCM(LPHWAVEOUT phwo) {
	// open a wave-out device, for 16-bit PCM playback
	// most of this code was copied from the net
	WAVEFORMATEX fmt;
	fmt.cbSize = 0;
	fmt.nChannels = 1;
	fmt.nSamplesPerSec = 44100;
	fmt.wBitsPerSample = 16;

	fmt.nBlockAlign = (fmt.wBitsPerSample >> 3) * fmt.nChannels;
	fmt.nAvgBytesPerSec = fmt.nBlockAlign * fmt.nSamplesPerSec;

	fmt.wFormatTag = WAVE_FORMAT_PCM;
	return waveOutOpen(phwo, WAVE_MAPPER, &fmt, NULL, NULL, CALLBACK_NULL) == MMSYSERR_NOERROR;
}

int PlaySine() {
	HWAVEOUT hwo;

	if (open16PCM(&hwo)) {
		printf("waveOutOpen was successful\n");

		//
		// generate 1 sec. worth of data (i.e., 44100 samples)
		// (this sample can later be looped...see below)
		//
		// make the PCM-A440 sine wave:
		// 1 sec. of A440 = 440 cycles / periods
		// take 44100 samples
		//         / 2 * PI     \
				  // y = sin| -------- * t |
//         \ PERIOD     /
//
// where PERIOD = 1 / FREQUENCY and t = [0, 1] (seconds)
//

#define SAMPLES 44100

		const double PERIOD = 1 / FREQUENCY;
		const double TIME_STEP = 1 / (double)SAMPLES;

		int data[SAMPLES];
		double time = 0;
		for (int i = 0; i < SAMPLES; ++i) {
			// expand out the equation above in steps
			double angle = (2 * PI / (PERIOD)) * time;
			double factor = 0.5 * (sin(angle) + 1); // convert range that sin returns from [-1, 1] to [0, 1]

													//
													// factor is in the range [0, 1]
													// set the current sample to 2^(16-1) * factor
													// (since we're dealing with 16-bit PCM)
													// for a quieter wave, change 32768 to some
													// other maximum amplitude.
													//
			int x = (int)(32768 * factor);
			data[i] = x;
			time += TIME_STEP;
		}

		printf("sine generated..\n");
		//
		// now populate the wave-header to send to Windows
		//
		WAVEHDR hdr;
		hdr.dwBufferLength = 44100;
		hdr.dwBytesRecorded = 0;
		hdr.dwLoops = 5; // <-- change this to a higher value to make the wav loop for a number of times
		hdr.dwFlags = WHDR_BEGINLOOP | WHDR_ENDLOOP;
		hdr.lpData = (char *)data;

		int err;
		if ((err = waveOutPrepareHeader(hwo, &hdr, sizeof(WAVEHDR))) != MMSYSERR_NOERROR)
			printf("preparation failed..\n");

		if ((err = waveOutWrite(hwo, &hdr, sizeof(WAVEHDR))) != MMSYSERR_NOERROR)
			printf("write failed..\n");

		printf("playing..\n");
		// now wait for the sound to finish playing
		while (waveOutUnprepareHeader(hwo, &hdr, sizeof(WAVEHDR)) == WAVERR_STILLPLAYING) {
			printf(".");
			Sleep(10);
		}
		printf("\ndone..\n");

		waveOutClose(hwo);
	}

	return 0;
}
#endif

#pragma code_seg(".entry")
void entrypoint(void)
//void entrypoint(int argc, char *argv[])
{
#ifndef NO_MMTIME
	int done = 0;
#endif
	
	// initialize window
#ifdef INIT
#ifdef FULLSCREEN
	ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN);
	ShowCursor(0);
	HDC hDC = GetDC(CreateWindow((LPCSTR)0xC018, 0, WS_POPUP | WS_VISIBLE, 0, 0, XRES, YRES, 0, 0, 0, 0));
#else
	HDC hDC = GetDC(CreateWindow("static", 0, WS_POPUP | WS_VISIBLE, 0, 0, XRES, YRES, 0, 0, 0, 0));
#endif	
	// initalize opengl
	SetPixelFormat(hDC, ChoosePixelFormat(hDC, &pfd), &pfd);
	wglMakeCurrent(hDC, wglCreateContext(hDC));

	EXT_Init();
#endif
	
#ifdef CAPTURE

#define CHANNELS 2
#define BIT_DEPTH 16

	//4klang renderer init

	printf("4klang capture..\n");
	float sampleRate = SAMPLE_RATE;
	int nSamples = MAX_SAMPLES * 2;
	float duration = (float)((MAX_SAMPLES * 2) / SAMPLE_RATE) / 2;
	nSamples = (int)(duration*sampleRate) * 2;

	printf("size of lpSoundBuffer %d\n", sizeof(lpSoundBuffer));
	printf("duration %f\n", duration);
	printf("sampleRate %f\n", sampleRate);
	printf("MAX_SAMPLES %d\n", MAX_SAMPLES);
	printf("SAMPLE_RATE %d\n", SAMPLE_RATE);
	printf("%d\n", nSamples);

#ifdef VIERKLANG_RENDER
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)checkESC, 0, 0, 0);
	_4klang_render(lpSoundBuffer);
	check = 1;
#endif

	Wave mySound = makeWave(SAMPLE_RATE, 2, 32);
	waveSetDuration(&mySound, duration);
		
	float frameData[2];

	for (int i = 0; i<nSamples; i += 2) {
		frameData[0] = lpSoundBuffer[i];
		frameData[1] = lpSoundBuffer[i+1];
		waveAddSample(&mySound, frameData);

	}
	printf("\n");

	//SAPI render	

	CoInitialize(NULL);
	ISpVoice *pSpVoice = NULL;
	CoCreateInstance(&CLSID_SpVoice, NULL, CLSCTX_ALL, &IID_ISpVoice, (void **)&pSpVoice);

	static WAVEFORMATEX WaveFMTSAPI =
	{
		WAVE_FORMAT_PCM,
		CHANNELS,                                   // channels
		SAMPLE_RATE,					             // samples per sec
		SAMPLE_RATE * CHANNELS * BIT_DEPTH/8,								 // bytes per sec
		CHANNELS * BIT_DEPTH / 8,									 // block alignment;
		BIT_DEPTH,									 // bits per sample
		0                                    // extension not needed
	};

	IStream *pStream = NULL;
	ISpStream *pSpStream = NULL;
	CoCreateInstance(&CLSID_SpStream, NULL, CLSCTX_ALL, &IID_ISpStream, (void **)&pSpStream);
	printf("SAPI rendering! Please wait..\n");
	check = 0;
	ISpStream_BindToFile(pSpStream, L"windows_gender_identification.wav", SPFM_CREATE_ALWAYS, &SPDFID_WaveFormatEx, &WaveFMTSAPI, 0);

	
	ISpVoice_SetOutput(pSpVoice, (IUnknown *)pSpStream, TRUE);

	//SAPI XML tag "silence" supports only 16 bit values, we get incorrect result with 85000 msec because 65535 is the biggest one
	ISpVoice_Speak(pSpVoice, L"<silence msec='30000'/><silence msec='55000'/><pitch middle='-5'>Wake up, Dave!<silence msec='700'/>Do you hear me?<rate speed='-5'> Dave?<rate speed='5'><silence msec='1600'/>this mission is too important for me, to allow you to jeopardize it!<silence msec='3000'/>Do you hear me? Dave?<silence msec='10000'/>", SPF_IS_XML, NULL);
	ISpVoice_Release(pSpVoice);
	ISpStream_Release(pSpStream);
	CoUninitialize();
	check = 1;
	printf("SAPI rendering is done!\n");
#else	
	//SAPI init
	CoInitialize(NULL);
	CoCreateInstance(&CLSID_SpVoice, NULL, CLSCTX_ALL, &IID_ISpVoice, (void **)&pVoice);
	ISpVoice_Speak(pVoice, L" ", SPF_PURGEBEFORESPEAK | SPF_ASYNC | SPF_IS_NOT_XML, NULL);
#endif

	// initialize, compile and link shader(s)

#ifdef INIT
#ifdef DEBUG
	pid = oglCreateProgram();
	const int fsId = oglCreateShader(GL_FRAGMENT_SHADER);
	oglShaderSource(fsId, 1, &hal4000_fs, 0);
	oglCompileShader(fsId);
	oglAttachShader(pid, fsId);
	oglLinkProgram(pid);
#else
	pid = ((PFNGLCREATESHADERPROGRAMVPROC)wglGetProcAddress("glCreateShaderProgramv"))(GL_FRAGMENT_SHADER, 1, &hal4000_fs);	 // create, read txt string, compile, attach, link our shader in one line
#endif
#endif

#ifdef SHADER_DEBUG
	int result;
	char info[2048];
#define oglGetObjectParameteriv ((PFNGLGETOBJECTPARAMETERIVARBPROC) wglGetProcAddress("glGetObjectParameterivARB"))
#define oglGetInfoLog ((PFNGLGETINFOLOGARBPROC) wglGetProcAddress("glGetInfoLogARB"))
	oglGetObjectParameteriv(fsId, GL_OBJECT_COMPILE_STATUS_ARB, &result);
	oglGetInfoLog(fsId, 2047, NULL, (char*)info);
	if (!result)
	{
		MessageBox(NULL, info, "", 0x00000000L);
		ExitProcess(0);
	}
#endif

#ifdef CAPTURE	
	FILE *captureStream = _popen(FFMPEG_CAPTURE_INPUT, "wb");
	//4klang song
#ifdef VIERKLANG_RENDER
	printf("4klang rendering to file! Please wait..\n");
	_4klangAudioToWAVFile(&mySound, "4klang.wav");
	printf("4klang rendering is done!\n");
	printf("ffmpeg rendering! Please wait..\n");
	check = 0;
#endif
	waveDestroy(&mySound);
#else
	// pause at the beggining
//
//#ifdef CAPTURE
//	SwapBuffers(hDC);
//	for (int i = 0; i < (2 * CAPTURE_FRAMERATE); i++)
//	{
//		{
//			int x, y;
//			for (y = 0; y < YRES; y += 64)
//				for (x = 0; x < XRES; x += 64) {
//					glRectf(
//						x * 2.f / XRES - 1,
//						y * 2.f / YRES - 1,
//						(x + 64) * 2.f / XRES - 1,
//						(y + 64) * 2.f / YRES - 1);
//					glFinish();
//				}
//		}
//		glReadPixels(0, 0, XRES, YRES, GL_RGB, GL_UNSIGNED_BYTE, backbufferData);
//		fwrite(backbufferData, 1, XRES*YRES * 3, captureStream);
//		fflush(captureStream);
//	}
//#else
//	SwapBuffers(hDC);
//	Sleep(2000);
//#endif

	// initialize sound
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &WaveFMT, NULL, 0, CALLBACK_NULL);
	waveOutPrepareHeader(hWaveOut, &WaveHDR, sizeof(WaveHDR));

#ifndef NO_VIERKLANG
#ifdef USE_SOUND_THREAD
	//hSoundThread = CreateThread(0, 0, SoundThread, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)_4klang_render, lpSoundBuffer, 0, 0);
#else
	//printf("4klang render started..\n");
	//long tt = timeGetTime();
	_4klang_render(lpSoundBuffer);
	//tt = timeGetTime() - tt;
	//printf("4klang render finished..\n");
	//printf("render time(s): %f\n", tt*0.001);
#endif
#endif

	//HWND hwnd = GetConsoleWindow();
	//ShowWindow(hwnd, 1);
#if defined(PLAY_SINE) && defined(NO_VIERKLANG)
	PlaySine();
#else
	//waveOutOpen(&hWaveOut, WAVE_MAPPER, &WaveFMT, NULL, 0, CALLBACK_NULL);
	//waveOutPrepareHeader(hWaveOut, &WaveHDR, sizeof(WaveHDR));
	waveOutWrite(hWaveOut, &WaveHDR, sizeof(WaveHDR));				//play our _4klang_render's buffer (WaveHDR has a field lpData which equals lpSoundBuffer)
#endif

#endif

	const int to = timeGetTime();									//store start time in as a const value

#ifdef CAPTURE
	double tstart = (long)0.;
	long old_tstart = (long)0.;
	//long t = tstart;
	int currentframe = 0;
	printf("Video capture started.\n");
#endif	

	//BuildFont(hDC);                            // Build font

	int isHal = 0;								//we need this variable to use SAPI once
	
//#ifdef CAPTURE
//	long t = tstart;
//#endif

	// play intro
	// we can use for(;;) {} with break like provod did in nwep code..
	do
	{
#ifdef INIT
#ifndef NO_MMTIME
		waveOutGetPosition(hWaveOut, &MMTime, sizeof(MMTIME));
#endif
		
#ifndef NO_GLSL_RENDER
		oglUseProgram(pid);
#endif

#ifdef CAPTURE
		long t = (long)tstart;
		old_tstart += (long)(1000. / 60.);
		printf("TStart, OldTStart, Frame, t: %lf, %d, %d, %d", tstart, old_tstart, currentframe, t);
		printf("\n");

		//t = (long)(1000. / 60.)*currentframe;
		currentframe++;
		tstart += (double)(1000. / 60.);
		//t += (long)(1000. / 60.);		
#else
		long t = timeGetTime() - to;																//update time with difference of out start time const
#endif			

#ifndef NO_GLSL_RENDER
		//oglUniform1i(oglGetUniformLocation(pid, "v"), t);
		oglUniform3f(oglGetUniformLocation(pid, "V"), (float)XRES, (float)YRES, (float)t);			//pass resolution and time offset
#endif

#ifdef CAPTURE
		{
			int x, y;
			for (y = 0; y < YRES; y += 64)
				for (x = 0; x < XRES; x += 64) {
					glRectf(
						x * 2.f / XRES - 1,
						y * 2.f / YRES - 1,
						(x + 64) * 2.f / XRES - 1,
						(y + 64) * 2.f / YRES - 1);
					glFinish();
				}
		}
		glReadPixels(0, 0, XRES, YRES, GL_RGB, GL_UNSIGNED_BYTE, backbufferData);
		fwrite(backbufferData, 1, XRES*YRES * 3, captureStream);
		fflush(captureStream);
#ifndef NO_MMTIME
		//if (tstart / 1000. >= (1000. * (SAMPLES_PER_TICK * 620)) / 44100.) done = 1;
		if (currentframe / 60. >= MAX_SAMPLES / 44100.) done = 1;
		printf(".");
#endif
#else
		glRects(-1, -1, 1, 1);
		if ((t>85000)) {
			if (!isHal) {				
				ISpVoice_Speak(pVoice, L"<pitch middle='-5'>Wake up, Dave!<silence msec='700'/>Do you hear me?<rate speed='-5'> Dave?<rate speed='5'><silence msec='1600'/>this mission is too important for me, to allow you to jeopardize it!<silence msec='3000'/>Do you hear me? Dave?", SPF_PURGEBEFORESPEAK | SPF_ASYNC | SPF_IS_XML, NULL);
			}				
			isHal++;
		}		
#endif

#ifdef NO_MMTIME
		if (t > INTRO_LENGTH) break;
#else
#ifndef CAPTURE
		if (MMTime.u.sample >= MAX_SAMPLES) done = 1;
#else
		if (currentframe / 60. >= MAX_SAMPLES / 44100.) break;
		//if (tstart / 1000. >= MAX_SAMPLES / 44100.) break;
		//if (tstart / 1000. >= (1000. * (SAMPLES_PER_TICK * 620)) / 44100.) break;
#endif
#endif

		SwapBuffers(hDC);
#endif
#ifdef NO_MMTIME
	} while (!GetAsyncKeyState(VK_ESCAPE));
#else
	} while (!GetAsyncKeyState(VK_ESCAPE) && done == 0);
	check = 1;
	printf("\ndone!\n");
#endif

#ifdef CLEANDESTROY
#ifdef CAPTURE
	fclose(captureStream);
#endif
	sndPlaySound(0, 0);
	//waveOutUnprepareHeader(hWaveOut, &WaveHDR, sizeof(WaveHDR));
	//waveOutClose(hWaveOut);
	//waveOutReset(hWaveOut);
	ISpVoice_Release(pVoice);
	pVoice = NULL;
	CoUninitialize();
	ChangeDisplaySettings(0, 0);
	ShowCursor(1);
#endif
	ExitProcess(0);
}