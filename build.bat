@echo off

SET INCLUDE_DIR=include
SET COMPILER_OPTS=/nologo /EHsc /c /Zi /Fobuild\ /Fdbuild\ /W3 /wd4996
SET LINK_OPTS=/NOLOGO /DEBUG
SET LIBS=user32.lib gdi32.lib opengl32.lib

IF EXIST build GOTO compile

mkdir build

:compile
    FOR /R src\ %%f IN ("*.cpp") DO cl %COMPILER_OPTS% /I include\ "%%f"
    
    pushd build

    link %LINK_OPTS% *.obj %LIBS% /OUT:opengl.exe

    popd