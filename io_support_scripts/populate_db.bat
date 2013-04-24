rem make sure you have the proper paths in the control file, import.sql and import.bat scripts
rm z:\POLARIS\interchange\with_its\*.sqlite
set origin=%CD%
Release\Transims2Polaris-mt.exe Transims2Polaris\control\Transims2PolarisChicago.ctl
cd Io\csv
call import.bat
cd /d z:\POLARIS\ITS
call populate_db.bat
cd /d %origin%
Release\ItsMockup.exe z:\POLARIS\interchange\with_its\chicago