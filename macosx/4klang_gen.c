#include "4klang.inh"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <memory.h>

#include <AudioToolbox/AudioToolbox.h>

#define SAMPLE_RATE 44100	// 1
#define DURATION 5.0	

#define SAMPLE_SIZE sizeof(SAMPLE_TYPE)

const size_t buf_size = MAX_SAMPLES*2;
static float output[buf_size];

int main() {
	_4klang_render(output);
	__asm("pushl %eax");
	
	FILE *outFile = fopen("4klang.pcm", "wb");
	fwrite(output, SAMPLE_SIZE, buf_size, outFile);
	fclose(outFile);
	
	return 0;
}
