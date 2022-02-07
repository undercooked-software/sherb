@ECHO OFF
SETLOCAL
  CALL :NORMALIZEPATH "C:\dev-tools\bin\msvc-no-vcvars"
  PUSHD %RETVAL%
    CALL "setup_cl_generic.bat" x64
  POPD

  SET NO_OUTPUT=^>nul 2^>nul
  SET PROJECT_ALIAS=sherb

  SET PREPROCESSOR=/DUNICODE
  SET EXCEPTIONS=/GR- /EHa-
  SET SECURITY=/GS- /Gs9999999
  SET WARNINGS=/WX /Wall /wd4711

  SET SUBSYSTEM=/SUBSYSTEM:CONSOLE

  SET COMMON_COMPILER_FLAGS=%PREPROCESSOR% /nologo %EXCEPTIONS% ^
                            %SECURITY% %WARNINGS% /FC
  SET COMMON_LINKER_FLAGS=/INCREMENTAL:NO /OPT:REF %SUBSYSTEM% /NODEFAULTLIB
  SET OPTIMIZATION_MODE=/O2
  SET STACK_SIZE=/STACK:0x100000,0x100000

  SET INTERNAL_LIBS=kernel32.lib user32.lib

  CL ^
    %COMMON_COMPILER_FLAGS% %OPTIMIZATION_MODE% ^
    /Z7 "%PROJECT_ALIAS%.c" ^
    %INTERNAL_LIBS% /link %COMMON_LINKER_FLAGS% %STACK_SIZE%

ENDLOCAL

:: ========== FUNCTIONS ==========
EXIT /B

:NORMALIZEPATH
  SET RETVAL=%~f1
  GOTO :EOF
