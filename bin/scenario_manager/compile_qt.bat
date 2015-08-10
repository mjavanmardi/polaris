set python_path=C:\Anaconda\Lib\site-packages\PyQt4\
%python_path%pyrcc4.exe -o resources.py resources.qrc
%python_path%pyuic4.bat sm.ui -o sm_ui.py