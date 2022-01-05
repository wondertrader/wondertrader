set env=%1
set plat=%2

set folder="..\%plat%\%env%\WtRunner\parsers\"
if not exist %folder% md %folder%

set folder="..\%plat%\%env%\WtRunner\traders\"
if not exist %folder% md %folder%

set folder="..\%plat%\%env%\WtRunner\executer\"
if not exist %folder% md %folder%

xcopy ..\%plat%\%env%\ParserCTP.dll ..\%plat%\%env%\WtRunner\parsers\ /C /Y
xcopy ..\%plat%\%env%\ParserUDP.dll ..\%plat%\%env%\WtRunner\parsers\ /C /Y
xcopy ..\%plat%\%env%\TraderCTP.dll ..\%plat%\%env%\WtRunner\traders\ /C /Y
xcopy ..\%plat%\%env%\TraderXTP.dll ..\%plat%\%env%\WtRunner\traders\ /C /Y
xcopy ..\%plat%\%env%\TraderMocker.dll ..\%plat%\%env%\WtRunner\traders\ /C /Y
xcopy ..\%plat%\%env%\TraderCTPMini.dll ..\%plat%\%env%\WtRunner\traders\ /C /Y
xcopy ..\%plat%\%env%\TraderCTPOpt.dll ..\%plat%\%env%\WtRunner\traders\ /C /Y
xcopy ..\%plat%\%env%\TraderFemas.dll ..\%plat%\%env%\WtRunner\traders\ /C /Y

xcopy ..\%plat%\%env%\WtExeFact.dll ..\%plat%\%env%\WtRunner\executer\ /C /Y

xcopy ..\%plat%\%env%\WtDataStorage.dll ..\%plat%\%env%\WtRunner\ /C /Y
xcopy ..\%plat%\%env%\WtDataStorageAD.dll ..\%plat%\%env%\WtRunner\ /C /Y
xcopy ..\%plat%\%env%\WtRiskMonFact.dll ..\%plat%\%env%\WtRunner\ /C /Y
xcopy ..\%plat%\%env%\WtMsgQue.dll ..\%plat%\%env%\WtPorter\ /C /Y
