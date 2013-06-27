@echo off
rem make sure you have the proper paths in the control file, import.sql and import.bat scripts
set fld=z:\POLARIS\interchange\with_its\
rm %fld%*.sqlite
set origin=%CD%
build\Transims2Polaris\MinSizeRel\transims2polaris.exe Transims2Polaris\control\Transims2PolarisChicago.ctl

cd Io\csv
call import.bat
cd /d z:\POLARIS\ITS
call populate_db.bat
cd /d %origin%
build\ItsMockup\MinSizeRel\its_mockup.exe %fld%chicago