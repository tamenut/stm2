@ECHO OFF
set DST_VERSION_FILE=.\include\stm_version.h

"SubWCRev.exe" .\ stm_version.h.in %DST_VERSION_FILE%

IF ERRORLEVEL 1 ( REM 오류가 발생한 경우 
	IF NOT EXIST %DST_VERSION_FILE% GOTO :NOT_EXIST
) 
GOTO :END

:NOT_EXIST
ECHO UpdateRevision error
goto :END

:END
