@echo off
if not exist "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" (
	echo The vcvarsall.bat file was not found, cannot run the msbuild tool
	Exit
)
call "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" x86_amd64

echo Cleaning the solution
msbuild /t:clean > Clean.log 2>Clean.err
for %%x in (Release_NetworkModel, Release_NetworkModel_Antares, Release_Integrated, Release_Integrated_Antares) do (
   echo Building %%x
   msbuild /p:configuration=%%x /p:platform=x64 > %%x.log
	IF ERRORLEVEL 1 (
		ECHO error level is 1 or more
		ECHO see %%x.log for details
		EXIT /B ERRORLEVEL
	)
)

echo Passed all of the commit checks!