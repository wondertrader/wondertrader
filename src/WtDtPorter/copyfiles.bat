set env=%1
set plat=%2

xcopy ..\%plat%\%env%\ParserCTP.dll ..\%plat%\%env%\WtDtPorter\ /C /Y
xcopy ..\%plat%\%env%\ParserXTP.dll ..\%plat%\%env%\WtDtPorter\ /C /Y
xcopy ..\%plat%\%env%\WtDataWriter.dll ..\%plat%\%env%\WtDtPorter\ /C /Y
