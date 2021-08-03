set env=%1
set plat=%2

xcopy ..\%plat%\%env%\WtMsgQue.dll ..\%plat%\%env%\WtMsgQue\ /C /Y
