set env=%1
set plat=%2

set folder="..\%plat%\%env%\QuoteFactory\parsers\"
if not exist %folder% md %folder%

xcopy ..\%plat%\%env%\WtDataStorage.dll ..\%plat%\%env%\QuoteFactory\ /C /Y
xcopy ..\%plat%\%env%\WtDataStorageAD.dll ..\%plat%\%env%\QuoteFactory\ /C /Y
xcopy ..\%plat%\%env%\ParserCTP.dll ..\%plat%\%env%\QuoteFactory\parsers\ /C /Y
xcopy ..\%plat%\%env%\ParserXTP.dll ..\%plat%\%env%\QuoteFactory\parsers\ /C /Y
xcopy ..\%plat%\%env%\ParserFemas.dll ..\%plat%\%env%\QuoteFactory\parsers\ /C /Y
xcopy ..\%plat%\%env%\ParserCTPMini.dll ..\%plat%\%env%\QuoteFactory\parsers\ /C /Y
xcopy ..\%plat%\%env%\ParserCTPOpt.dll ..\%plat%\%env%\QuoteFactory\parsers\ /C /Y
