nasmw.exe -fwin32 -o"4klang.obj" 4klang.asm

echo f|xcopy /s/y "..\Release\nwep.obj" nwep.obj

REM crinkler.exe nwep.obj 4klang.obj kernel32.lib user32.lib winmm.lib /ENTRY:entrypoint /UNSAFEIMPORT /COMPMODE:FAST /NOINITIALIZERS /HASHSIZE:100 /HASHTRIES:0 /ORDERTRIES:0
crinkler.exe nwep.obj 4klang.obj /OUT:nwep_audio.exe /MANIFEST:NO /LTCG:incremental /NXCOMPAT:NO /DYNAMICBASE:NO "winmm.lib" "kernel32.lib" "user32.lib" /MACHINE:X86 /ENTRY:"entrypoint" /OPT:REF /SAFESEH:NO /INCREMENTAL:NO /SUBSYSTEM:WINDOWS /MANIFESTUAC:"level='asInvoker' uiAccess='false'" /OPT:ICF /ERRORREPORT:PROMPT /NOLOGO /NODEFAULTLIB /TLBID:1 

pause
