@echo off
REM sapi.lib;opengl32.lib;winmm.lib;kernel32.lib;user32.lib;gdi32.lib;winspool.lib;comdlg32.lib;advapi32.lib;shell32.lib;ole32.lib;oleaut32.lib;uuid.lib;odbc32.lib;odbccp32.lib                                                                   /HASHTRIES:8000 /COMPMODE:FAST /REPORT:report-fast.html /UNSAFEIMPORT /NOALIGNMENT /NOINITIALIZERS
set /A tries=0
set /A size=50000
set /A checksize=4000
set /A ordertries=500

:begin
echo begin

:go
echo go
echo file size is %size%
if "%size%" GTR "%checksize%" (
      set /A tries+=1
      echo TRY #%tries%
      echo ORDERTRIES is %ordertries%
      echo the size if not ok
  call :filesize "test\release720p.exe"
REM  call :filesize "test\release1024.exe"
REM  call :filesize "test\release1920p.exe"
      call :link 
  ) else (
      echo the size if ok
      echo TRYES is %tries%
      echo ORDERTRIES is %ordertries%
      goto :end
)

:end
  echo end
  echo file size is %size%
  goto :eof

:: set filesize of 1st argument in %size% variable, and return
:filesize
  echo filesize
  set size=%~z1
  exit /b 0

:link
  echo linking..
  link.exe ReleaseFast\hal4000.obj 4klang.obj sapi.lib opengl32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /CRINKLER /ENTRY:"entrypoint" /SUBSYSTEM:WINDOWS /COMPMODE:FAST /ORDERTRIES:%ordertries% /HASHSIZE:1000 /REPORT:report-slow.html /UNSAFEIMPORT /NOINITIALIZERS /TINYIMPORT /OUT:test\release720p.exe
REM  link.exe ReleaseFast_1024x768\hal4000.obj 4klang.obj sapi.lib opengl32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /CRINKLER /ENTRY:"entrypoint" /SUBSYSTEM:WINDOWS /COMPMODE:FAST /HASHTRIES:200 /ORDERTRIES:%ordertries% /HASHSIZE:1000 /REPORT:report-slow.html /UNSAFEIMPORT /NOINITIALIZERS /TINYIMPORT /OUT:test\release1024.exe
REM  link.exe ReleaseFast_1920x1080\hal4000.obj 4klang.obj sapi.lib opengl32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /CRINKLER /ENTRY:"entrypoint" /SUBSYSTEM:WINDOWS /COMPMODE:FAST /ORDERTRIES:%ordertries% /HASHSIZE:1000 /REPORT:report-slow.html /UNSAFEIMPORT /NOINITIALIZERS /TINYIMPORT /OUT:test\release1920p.exe
  set /A ordertries+="1"
  goto :begin