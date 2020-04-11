set env=%1
set plat=%2
set pid=%plat:~-2%

set folder="..\%plat%\%env%\WtPorter\parsers\"
if not exist %folder% md %folder%

set folder="..\%plat%\%env%\WtPorter\traders\"
if not exist %folder% md %folder%

set folder="..\%plat%\%env%\WtPorter\executer\"
if not exist %folder% md %folder%

xcopy ..\%plat%\%env%\ParserUDP%pid%.dll ..\%plat%\%env%\WtPorter\parsers\ /C /Y
xcopy ..\%plat%\%env%\TraderCTP%pid%.dll ..\%plat%\%env%\WtPorter\traders\ /C /Y
xcopy ..\%plat%\%env%\WtExeFact%pid%.dll ..\%plat%\%env%\WtPorter\executer\ /C /Y

xcopy ..\%plat%\%env%\WtDataReader%pid%.dll ..\%plat%\%env%\WtPorter\ /C /Y
xcopy ..\%plat%\%env%\WtRiskMonFact%pid%.dll ..\%plat%\%env%\WtPorter\ /C /Y
