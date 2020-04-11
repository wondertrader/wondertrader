set env=%1
set plat=%2
set pid=%plat:~-2%

xcopy ..\%plat%\%env%\WtDataWriter%pid%.dll ..\%plat%\%env%\QuoteFactory\ /C /Y
xcopy ..\%plat%\%env%\ParserCTP%pid%.dll ..\%plat%\%env%\QuoteFactory\ /C /Y
xcopy ..\%plat%\%env%\ParserXTP%pid%.dll ..\%plat%\%env%\QuoteFactory\ /C /Y
xcopy ..\%plat%\%env%\ParserUT%pid%.dll ..\%plat%\%env%\QuoteFactory\ /C /Y
