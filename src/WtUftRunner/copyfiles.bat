set env=%1
set plat=%2

set folder="..\%plat%\%env%\WtUftRunner\parsers\"
if not exist %folder% md %folder%

set folder="..\%plat%\%env%\WtUftRunner\traders\"
if not exist %folder% md %folder%

set folder="..\%plat%\%env%\WtUftRunner\executer\"
if not exist %folder% md %folder%

xcopy ..\%plat%\%env%\ParserCTP.dll ..\%plat%\%env%\WtUftRunner\parsers\ /C /Y
xcopy ..\%plat%\%env%\ParserUDP.dll ..\%plat%\%env%\WtUftRunner\parsers\ /C /Y
xcopy ..\%plat%\%env%\TraderCTP.dll ..\%plat%\%env%\WtUftRunner\traders\ /C /Y
xcopy ..\%plat%\%env%\TraderXTP.dll ..\%plat%\%env%\WtUftRunner\traders\ /C /Y
xcopy ..\%plat%\%env%\TraderMocker.dll ..\%plat%\%env%\WtUftRunner\traders\ /C /Y
xcopy ..\%plat%\%env%\TraderCTPMini.dll ..\%plat%\%env%\WtUftRunner\traders\ /C /Y
xcopy ..\%plat%\%env%\TraderCTPOpt.dll ..\%plat%\%env%\WtUftRunner\traders\ /C /Y
xcopy ..\%plat%\%env%\TraderFemas.dll ..\%plat%\%env%\WtUftRunner\traders\ /C /Y

xcopy ..\%plat%\%env%\WtExeFact.dll ..\%plat%\%env%\WtUftRunner\executer\ /C /Y

xcopy ..\%plat%\%env%\WtDataStorage.dll ..\%plat%\%env%\WtUftRunner\ /C /Y
xcopy ..\%plat%\%env%\WtDataStorageAD.dll ..\%plat%\%env%\WtUftRunner\ /C /Y
xcopy ..\%plat%\%env%\WtRiskMonFact.dll ..\%plat%\%env%\WtUftRunner\ /C /Y
xcopy ..\%plat%\%env%\WtMsgQue.dll ..\%plat%\%env%\WtPorter\ /C /Y
