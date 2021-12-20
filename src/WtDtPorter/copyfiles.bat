set env=%1
set plat=%2

set folder="..\%plat%\%env%\WtDtPorter\parsers\"
if not exist %folder% md %folder%

xcopy ..\%plat%\%env%\WtDataStorage.dll ..\%plat%\%env%\WtDtPorter\ /C /Y
xcopy ..\%plat%\%env%\ParserCTP.dll ..\%plat%\%env%\WtDtPorter\parsers\ /C /Y
xcopy ..\%plat%\%env%\ParserXTP.dll ..\%plat%\%env%\WtDtPorter\parsers\ /C /Y
xcopy ..\%plat%\%env%\ParserCTPMini.dll ..\%plat%\%env%\WtDtPorter\parsers\ /C /Y
xcopy ..\%plat%\%env%\ParserCTPOpt.dll ..\%plat%\%env%\WtDtPorter\parsers\ /C /Y
xcopy ..\%plat%\%env%\ParserFemas.dll ..\%plat%\%env%\WtDtPorter\parsers\ /C /Y