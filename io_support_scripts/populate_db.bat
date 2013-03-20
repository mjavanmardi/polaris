rem make sure you have the proper paths in the control file, import.sql and import.bat scripts
Release\Transims2Polaris-mt.exe Transims2Polaris\control\Transims2PolarisChicago.ctl
cd Io\csv
import.bat
cd ..\..