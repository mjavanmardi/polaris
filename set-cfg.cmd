:: ------------------------------------------------------
::  http://forums.petri.com/showthread.php?t=36286
:: ------------------------------------------------------

@echo off & setlocal enabledelayedexpansion

Set "inputFile=Sample.txt"
Set "outputFile=%inputFile%-TMP.txt"

>"%outputFile%" (
   For /f "delims=" %%* in (
     'find.exe /n /v ""^<"%inputFile%"'
     ) Do (
     Call:GoParse "''%%*"
     (echo\!WriteLine!)
   )   
)

::# replace original file (! remove 'rem' !)
rem Move /Y "%outputFile%" "%inputFile%"

:end   ----------

goto:eof
:GoParse lines
(Set /a i = %i% +1)&(Set "WriteLine=")
(Set Line=%1)&(Set Line=!Line:''[%i%]=!)

(Set EQsn==)
>nul (@echo\%Line% |Find.exe /i "username=" |Find.exe /i "password="
   )&&(For /f "tokens=3,5 delims=/%EQsn% " %%a in (
     'cmd /c @echo\%line%') Do (
     (Set Line=!Line:%%~a=%%username%%!)
     (Set Line=!Line:%%~b=%%username%%!)
   )
)
(Set WriteLine=!Line:~1,-1!)
goto:eof