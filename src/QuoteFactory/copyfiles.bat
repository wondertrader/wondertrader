set env=%1
set plat=%2

xcopy ..\%plat%\%env%\WtDataWriter%plat:~-2%.dll ..\%plat%\%env%\QuoteFactory\ /C /Y
xcopy ..\%plat%\%env%\ParserCTP.dll ..\%plat%\%env%\QuoteFactory\ /C /Y
xcopy ..\%plat%\%env%\ParserXTP.dll ..\%plat%\%env%\QuoteFactory\ /C /Y
xcopy ..\%plat%\%env%\ParserUT.dll ..\%plat%\%env%\QuoteFactory\ /C /Y
