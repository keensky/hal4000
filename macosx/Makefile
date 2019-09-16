all:
	nasm -f macho 4klang.asm
	gcc -o hal4000_macosx_1280x800 hal4000_mac.c 4klang.o -Wimplicit -framework AudioToolbox -framework CoreFoundation -framework OpenGL -framework CoreGraphics -lm -m32 -v -DXRES=1280.0f -DYRES=800.0f
	gcc -o hal4000_macosx_1920x1080 hal4000_mac.c 4klang.o -Wimplicit -framework AudioToolbox -framework CoreFoundation -framework OpenGL -framework CoreGraphics -lm -m32 -v -DXRES=1920.0f -DYRES=1080.0f
	gcc -o hal4000_macosx_1280x720 hal4000_mac.c 4klang.o -Wimplicit -framework AudioToolbox -framework CoreFoundation -framework OpenGL -framework CoreGraphics -lm -m32 -v -DXRES=1280.0f -DYRES=720.0f
	gcc -o hal4000_macosx_1280x800_prerender hal4000_mac.c 4klang.o -Wimplicit -framework AudioToolbox -framework CoreFoundation -framework OpenGL -framework CoreGraphics -lm -m32 -v -DNOTHREAD -DXRES=1280.0f -DYRES=800.0f
	gcc -o hal4000_macosx_1920x1080_prerender hal4000_mac.c 4klang.o -Wimplicit -framework AudioToolbox -framework CoreFoundation -framework OpenGL -framework CoreGraphics -lm -m32 -v -DNOTHREAD -DXRES=1920.0f -DYRES=1080.0f
	gcc -o hal4000_macosx_1280x720_prerender hal4000_mac.c 4klang.o -Wimplicit -framework AudioToolbox -framework CoreFoundation -framework OpenGL -framework CoreGraphics -lm -m32 -v -DNOTHREAD -DXRES=1280.0f -DYRES=720.0f
pack:
	shelldroper.sh hal4000_macosx_1280x800
	shelldroper.sh hal4000_macosx_1920x1080
	shelldroper.sh hal4000_macosx_1280x720
	shelldroper.sh hal4000_macosx_1280x800_prerender
	shelldroper.sh hal4000_macosx_1920x1080_prerender
	shelldroper.sh hal4000_macosx_1280x720_prerender
4klang:
	gcc -Wl,-no_pie -m32 4klang.o 4klang_gen.c -o 4klang_gen
	say -o Dave.wav --data-format=LEF32@44100 "Wake up, Dave!,Do you hear me? Dave?,,,this mission is too important for me, to allow you to jeopardize it!,,,,,,Do you hear me? Dave?"
	4klang_gen
	mv 4klang.pcm 4klang.raw
	sox -e floating-point -b 32 --endian little -c 2 -r 44100 4klang.raw 4klang.wav
clear:
	rm 4klang.o hal4000_mac.o hal4000_macosx_1280x720_prerender hal4000_macosx_1920x1080_prerender hal4000_macosx_1280x800_prerender hal4000_macosx_1280x720 hal4000_macosx_1920x1080 hal4000_macosx_1280x800 4klang_gen Dave.wav 4klang.raw 4klang.wav
