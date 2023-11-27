set env=%1
set plat=%2

set folder="..\%plat%\%env%\WtPorter\parsers\"
if not exist %folder% md %folder%

set folder="..\%plat%\%env%\WtPorter\traders\"
if not exist %folder% md %folder%

set folder="..\%plat%\%env%\WtPorter\executer\"
if not exist %folder% md %folder%

xcopy ..\%plat%\%env%\ParserUDP.dll ..\%plat%\%env%\WtPorter\parsers\ /C /Y
xcopy ..\%plat%\%env%\ParserShm.dll ..\%plat%\%env%\WtPorter\parsers\ /C /Y
xcopy ..\%plat%\%env%\TraderCTP.dll ..\%plat%\%env%\WtPorter\traders\ /C /Y
xcopy ..\%plat%\%env%\TraderXTP.dll ..\%plat%\%env%\WtPorter\traders\ /C /Y
xcopy ..\%plat%\%env%\TraderMocker.dll ..\%plat%\%env%\WtPorter\traders\ /C /Y
xcopy ..\%plat%\%env%\TraderCTPMini.dll ..\%plat%\%env%\WtPorter\traders\ /C /Y
xcopy ..\%plat%\%env%\TraderCTPOpt.dll ..\%plat%\%env%\WtPorter\traders\ /C /Y
xcopy ..\%plat%\%env%\TraderFemas.dll ..\%plat%\%env%\WtPorter\traders\ /C /Y

xcopy ..\%plat%\%env%\WtExeFact.dll ..\%plat%\%env%\WtPorter\executer\ /C /Y

xcopy ..\%plat%\%env%\WtDataStorage.dll ..\%plat%\%env%\WtPorter\ /C /Y
xcopy ..\%plat%\%env%\WtDataStorageAD.dll ..\%plat%\%env%\WtPorter\ /C /Y
xcopy ..\%plat%\%env%\WtRiskMonFact.dll ..\%plat%\%env%\WtPorter\ /C /Y
xcopy ..\%plat%\%env%\WtMsgQue.dll ..\%plat%\%env%\WtPorter\ /C /Y
