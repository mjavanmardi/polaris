@echo off
SET MYPYTHONPATH=
echo Python interpreter accessible through the system PATH:
where python
if not errorlevel 1 (
    python -c "from __future__ import print_function; import sys; print(sys.version)"
	FOR /F "tokens=* USEBACKQ" %%F IN (`where python`) DO ( SET MYPYTHONPATH=%%F )
)
IF NOT "%MYPYTHONPATH%" == "" (ECHO %MYPYTHONPATH% & EXIT /B 0)
echo ---
echo PythonCore registered installation:
reg query HKLM\Software\Python\PythonCore\2.7\InstallPath /ve >nul 2>&1
if errorlevel 1 (
    echo No "HKLM\Software\Python\PythonCore\2.7" registry key found
) else (
	for /f "skip=2 tokens=1,2*" %%i in ('reg query HKLM\Software\Python\PythonCore\2.7\InstallPath /ve 2^>nul') do set "handler=%%k"
	if "%handler%"=="(value not set)" (
		echo No install path set for python 2.7
	) else (
		set MYPYTHONPATH=%handler%python.exe
	)
)
IF NOT "%MYPYTHONPATH%" == "" ( ECHO %MYPYTHONPATH% & EXIT /B 0 )
:: Not found
EXIT /B 1