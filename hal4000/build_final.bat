REM run hal4000.exe from Capure folder at least one time before this
cd ..

COPY hal4000\ffmpeg.exe FINAL\ffmpeg.exe
COPY hal4000\FILE_ID.DIZ FINAL\FILE_ID.DIZ
COPY hal4000\readme.txt FINAL\readme.txt

COPY Capture\4klang.wav FINAL\song_no_sapi.wav
COPY Capture\merged.wav FINAL\full_song.wav
COPY Capture\capture_1920x1080.mp4 FINAL\capture_1920x1080.mp4
COPY Capture\hal4000.exe FINAL\hal4000_capture_final.exe

COPY Release\hal4000.exe FINAL\hal4000_720p_final.exe
COPY Release_1024x768\hal4000.exe FINAL\hal4000_1024x768_final.exe
COPY Release_1920x1080\hal4000.exe FINAL\hal4000_1920x1080_final.exe

cd FINAL
merge.bat

