#define SOUND

//#define XRES 1280.0f
//#define YRES 720.0f
//#define XRES 1920.0f
//#define YRES 1080.0f
//#define NOTHREAD

#include <OpenGL/OpenGL.h>
#include <ApplicationServices/ApplicationServices.h>
#include <Carbon/Carbon.h>
#include <OpenGL/glu.h>
#include <OpenGL/CGLTypes.h>
#include <OpenGL/CGLContext.h>

#include <sys/time.h>

//4klang
#ifdef SOUND
#include "4klang.inh"
//#include <stdio.h>
//#include <stdint.h>
//#include <stdlib.h>
//#include <limits.h>
//#include <math.h>
//#include <memory.h>

//audio output
#include <AudioToolbox/AudioQueue.h>
#include <CoreAudio/CoreAudioTypes.h>
#include <CoreFoundation/CFRunLoop.h>
//#include <CoreAudio/CoreAudio.h>
//#include <AudioUnit/AudioUnit.h>
#include <pthread.h>

#define NUM_CHANNELS POLYPHONY
#define NUM_BUFFERS 4
#define BUFFER_SIZE 4096
#define SAMPLE_SIZE sizeof(SAMPLE_TYPE)

static float output[MAX_SAMPLES*2];

unsigned int count;

void callback(void *custom_data, AudioQueueRef queue, AudioQueueBufferRef buffer);
#endif

extern void CGSGetKeys(KeyMap k);

//"#version 120\n"
//"uniform float time;\n"
//"vec2 V=vec2(2560.,1600.);\n" //2560×1600

static const GLchar *fragment_shader=
"uniform vec3 V;\n"
"float m=V.z*.001;\n"
"float s(vec3 f,float x)\n"
"{\n"
"f.z-=4.5;\n"
"vec3 p=vec3(m/2.3,m/1.95,m/2.7),v=sin(p),c=cos(p);\n"
"f*=mat3(c.y,v.z*v.y,-v.y*c.z,-v.x*v.y,c.x*c.z+v.x*v.z*c.y,v.z*c.x-v.x*c.z*c.y,c.x*v.y,v.x*c.z-c.x*v.z*c.y,v.z*v.x+c.x*c.z*c.y);\n"
"float z=clamp((m-33.)/14.,0.,1.);\n"
"x=mix(x,0.,z*z*z);\n"
"float i=8.-6.*x,y=3.8*x-2.;\n"
"i=mix(i,2.,z*z*z);\n"
"f=abs(f);\n"
"return pow(pow(f.x,i)+pow(f.y,i)+pow(f.z,i)+y,2.)-x*8.*(f.x*f.x+f.y*f.y);\n"
"}\n"
"float s(vec3 v)\n"
"{\n"
"return sin(v.x*5.)/15.+cos(v.x+1.5)/3.+sin(v.z+m)/5.-cos(v.x+2.)*cos(v.z+m)/3.;\n"
"}\n"
"float p(vec3 m)\n"
"{\n"
"return length(m-vec3(m.x,s(m)-1.5,m.z));\n"
"}\n"
"float p(float m,float c)\n"
"{\n"
"return(m-m*c+1.)*991.;\n"
"}\n"
"float x(float m)\n"
"{\n"
"return pow(1.-min(1.,abs(m)),4.);\n"
"}\n"
"void main()\n"
"{\n"
"vec2 v=gl_FragCoord.xy/V.xy;\n"
"float f=V.x/V.y;\n"
"vec2 c=(v-vec2(.5))/vec2(1.,f)*1.4;\n"
"float z=0.,i=0.,y=0.,a=0.,l=0.,r=3.1416,n=1.,b,e=.0001,k,o,t=0.,w,h,g;\n"
"vec3 d,u=vec3(0.,0.,0.),q=u,F=vec3(0.,1.,12.+max(0.,(m-47.)/8.));\n"
"if(m<62.)\n"
"{\n"
"float C=clamp(m/4.-5.,0.,1.),Z=cos(m/3.1)/2.5+.4,Y=m-22.,X=1.+max(Y*Y*Y/999.,0.)/9.;\n"
"h=min(m*m*m/451.,17.7)+.3;\n"
"h=mix(h,3.,clamp((m-42.)/5.,0.,1.));\n"
"g=30.-fract(m);\n"
"for(int W=30;W>0;W--)\n"
"d=vec3(c*(1.+g),g),z=max(z,pow(x(-d.y-1.5+C*s(d+vec3(0.,0.,1.))),7.)*-c.y*2.6),g--;\n"
"g=4.5+2.*(h-1.)/(h+1.);\n"
"for(int W=18;W>0;W--)\n"
"d=vec3(c*(2.+g)*X,g),y=max(y,float(s(d,Z)<.1)),i=max(i,x(s(d,Z))/(g-2.6)),g-=4./(h+1.);\n"
"y=1.-y;\n"
"if(m>46.)\n"
"{\n"
"d=normalize(vec3(c,1.));\n"
"for(int W=0;W<40;W++)\n"
"{\n"
"b=p(d*n);\n"
"if(b<.001)\n"
"break;\n"
"n+=b;\n"
"if(n>40.)\n"
"break;\n"
"}\n"
"if(n<40.)\n"
"d*=n,q=normalize(vec3(p(d+vec3(e,0.,0.))-p(d-vec3(e,0.,0.)),p(d+vec3(0.,e,0.))-p(d-vec3(0.,e,0.)),p(d+vec3(0.,0.,e))-p(d-vec3(0.,0.,e)))),a=y*pow((length(q+normalize(F-d))-1.1)*max(1.2-length(F-d)/15.,0.),4.);\n"
"}\n"
"e=2371.;\n"
"for(int W=0;W<40;W++)\n"
"{\n"
"h=m/15.;\n"
"if(m>=47.1)\n"
"h=3.14+(m-47.1)/20.;\n"
"g=fract(h+t);\n"
"o=p(c.y,g);\n"
"if(o>935.)\n"
"{\n"
"k=p(c.x,g);\n"
"b=fract(sin(pow(floor(k),floor(o)/941.))*e);\n"
"if(b>.9985)\n"
"b=distance(vec2(k,o),vec2(floor(k)+.5,floor(o)+.5))*2.,l=max(l,g*min(1.,pow(1.5-b,15.))),l=max(l,g*max(0.,1.-b));\n"
"}\n"
"e-=.9;\n"
"t+=.05;\n"
"}\n"
"z*=y;\n"
"l*=y;\n"
"z*=clamp(m/4.-2.5,0.,1.);\n"
"l*=clamp(m-31.,0.,1.);\n"
"a+=y*max(2.-n/20.,0.)*max(q.z,0.);\n"
"a*=pow(clamp(m*2.-93.2,0.,1.),3.);\n"
"a=max(0.,a);\n"
"u=vec3(l/2.,l*l,sqrt(l))+vec3(0.,a*a,a)+vec3(0.,i*i,i)+vec3(z*z/2.,0.,z);\n"
"u*=min(1.,m/4.);\n"
"b=2.-clamp(m/4.-12.75,0.,2.);\n"
"z=smoothstep(1.,0.,abs(b-length(c)));\n"
"u+=vec3(0.,z*z,z);\n"
"b=2.-clamp(m/4.-13.5,0.,2.2);\n"
"z=smoothstep(b-.1,b+.1,length(c));\n"
"u*=1.-z;\n"
"}\n"
"else\n"
"{\n"
"if(m<104.)\n"
"{\n"
"k=min(m,85.)-62.;\n"
"w=31.-k;\n"
"g=fract(cos(k*3.9)+sin(k)*cos(k*3.7));\n"
"if(m<65.)\n"
"g=3.-k;\n"
"t=length(c)*w;\n"
"z=max(0.,.7-min(1.,t));\n"
"i=pow(t/99.*float(t<.6),.4)+(.8-t)*float(t<.02);\n"
"y=min(1.,pow(max(0.,1.25-t),8.))/(10.-g*3.)+(g*.8+.3)*min(.7,pow(max(0.,.15-t)*9.,12.));\n"
"h=c.y*w;\n"
"o=.3+.6*cos(cos(9.*(c.x-.4)*r+m/3.)+9.*c.y+m/7.)-.6+.6*cos(cos(9.*c.y*r+m/5.)+9.*(c.x-.4)-m/9.);\n"
"z+=clamp(o,0.,1.)/3.*smoothstep(.1,0.,t-.4);\n"
"z=mix(z,floor(z*16.)/16.,max(0.,-h));\n"
"d=vec3(z+y,y,i);\n"
"z=atan(abs(c.x),c.y);\n"
"y=smoothstep(.1,0.,t-.53)*clamp(pow(1.3-abs(z),7.)+pow(1.05-abs(.8-z),7.),0.,1.);\n"
"a=max(0.,h)*pow(1.-sqrt(abs(t-.55)),10.);\n"
"a+=-pow(abs(h),3.)*pow(1.-sqrt(abs(t-.5)),14.);\n"
"a*=.5+y/2.;\n"
"z=smoothstep(.97,.99,1.035-abs(t-.68));\n"
"z-=(1.-g/3.)*.5*float(t>.61)*smoothstep(0.,.01,.685-t);\n"
"t=length(c*vec2(.7,1.))*w;\n"
"o=1.35-fract(k/24.);\n"
"i=0.;\n"
"for(int W=1;W<4;W++)\n"
"i+=min(.8,max(0.,h)*pow(1.2-sqrt(abs(length(t-o/float(W)))),12.))/((float(W)+.5)/1.5);\n"
"i*=y;\n"
"a+=i;\n"
"F=vec3(a*.9,a*.9,a);\n"
"q=.4*sqrt(h+.8)*vec3(z/2.7,z/2.,z/1.5);\n"
"u=float(t<2.)*(d+F+q);\n"
"u*=min(1.,(m-62.)/4.);\n"
"}\n"
"}\n"
"gl_FragColor=vec4(u,1.);\n"
"}\n";

#ifdef SOUND
void callback(void *custom_data, AudioQueueRef queue, AudioQueueBufferRef buffer)
{
    SAMPLE_TYPE *casted_buffer = (SAMPLE_TYPE *)buffer->mAudioData;
    unsigned int i;

    for (i = 0; i < BUFFER_SIZE / sizeof(SAMPLE_TYPE); i += NUM_CHANNELS)
    {
        casted_buffer[i + 0] = output[count];
        casted_buffer[i + 1] = output[count+1];

        count+=2;
    }

    AudioQueueEnqueueBuffer(queue, buffer, 0, NULL);

    if (count > MAX_SAMPLES * 2)
    {
        AudioQueueStop(queue, false);
        AudioQueueDispose(queue, false);
        CFRunLoopStop(CFRunLoopGetCurrent());
    }

}
void *_4klang_run() {
    //4klang
    _4klang_render(output);
    __asm("pushl %eax");
    return NULL;
}

void *_say() {
    //say
    system("osascript -e 'say \"Wake up, Dave!,Do you hear me? Dave?,,,this mission is too important for me, to allow you to jeopardize it!,,,,,,Do you hear me? Dave?\" pitch 38 modulation 20'");
    return NULL;
}

void *audio_play() {
    count = 0;
    unsigned int i;

    AudioStreamBasicDescription format;
    AudioQueueRef queue;
    AudioQueueBufferRef buffers[NUM_BUFFERS];

    format.mSampleRate       = SAMPLE_RATE;
    format.mFormatID         = kAudioFormatLinearPCM;
    format.mFormatFlags      = kLinearPCMFormatFlagIsFloat; //SignedInteger | kAudioFormatFlagIsPacked;
    format.mBitsPerChannel   = 8 * sizeof(SAMPLE_TYPE);
    format.mChannelsPerFrame = NUM_CHANNELS;
    format.mBytesPerFrame    = sizeof(SAMPLE_TYPE) * NUM_CHANNELS;
    format.mFramesPerPacket  = 1;
    format.mBytesPerPacket   = format.mBytesPerFrame * format.mFramesPerPacket;
    format.mReserved         = 0;

    AudioQueueNewOutput(&format, callback, NULL, CFRunLoopGetCurrent(), kCFRunLoopCommonModes, 0, &queue);

    for (i = 0; i < NUM_BUFFERS; i++)
    {
        AudioQueueAllocateBuffer(queue, BUFFER_SIZE, &buffers[i]);
        buffers[i]->mAudioDataByteSize = BUFFER_SIZE;
        callback(NULL, queue, buffers[i]);
    }

    AudioQueueStart(queue, NULL);
    CFRunLoopRun();
    return NULL;
}
#endif

int main(void)
{
#ifdef SOUND

#ifdef NOTHREAD
    _4klang_run();
#else
    pthread_t _4klang_run_thread;
    pthread_create(&_4klang_run_thread, NULL, _4klang_run, NULL);
#endif
    pthread_t audio_play_thread;
    pthread_create(&audio_play_thread, NULL, audio_play, NULL);
#endif
    pthread_t _say_thread;
    //timer
    struct timeval tv;
    gettimeofday(&tv, NULL);

    CGDisplayHideCursor(0);                            // parameter not used by Quartz
    static const CGLPixelFormatAttribute attribs[]={0};            // anything goes.
    CGLPixelFormatObj formats;
    GLint num_pix;
    CGLChoosePixelFormat(attribs,&formats,&num_pix);
    CGLContextObj cgl_ctx;
    CGLCreateContext(formats,0,&cgl_ctx);                    // first hit is good enough for us.
    CGLSetCurrentContext(cgl_ctx);
#ifdef MAC_LEOPARD_TIGER_PANTHER_SUPPORT
    CGLSetFullScreen(cgl_ctx);
#else

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    CGLSetFullScreenOnDisplay(cgl_ctx,CGDisplayIDToOpenGLDisplayMask(0));
#pragma GCC diagnostic pop

#endif

    GLuint shader=glCreateShader(GL_FRAGMENT_SHADER);            // now shader
    glShaderSource(shader,1,&fragment_shader,0);
    glCompileShader(shader);

    GLuint program=glCreateProgram();
    glAttachShader(program,shader);
    glLinkProgram(program);
    glUseProgram(program);

    int isHal = 0;

//    char *text = "Wake up, Dave! Do you hear me? Dave? this mission is too important for me, to allow you to jeopardize it! Do you hear me? Dave?";
    long to = tv.tv_sec;
    long time = 0.0f;

    for (;;)
    {
        if ((time>85000)) {
            if (!isHal) {
		pthread_create(&_say_thread, NULL, _say, NULL);
	    }
           isHal++;
        }

        // set shader timing, should come from music
        glUniform3f(glGetUniformLocation(program, "V"), XRES, YRES, (float)time);
        gettimeofday(&tv, NULL);
        time = (tv.tv_sec - to - 1)*1000+(tv.tv_usec/1000);

        glRecti(-1,-1,1,1);                        // actual drawing
        glSwapAPPLE();

        KeyMap keys;
        CGSGetKeys(keys);
        if (((unsigned char*)keys)[6]&0x20 || time > 116000) break;
    }

   return 0;
}
