@echo off
set python_path=..\..\..\..\python27
set PythonPath=%PythonPath%;%python_path%\lib\site-packages
%python_path%\lib\site-packages\PyQt4\pyrcc4.exe -o resources_rc.py resources.qrc
c:\Python27\python.exe %python_path%\lib\site-packages\PyQt4\uic\pyuic.py ui_polaris.ui -o ui_polaris.py