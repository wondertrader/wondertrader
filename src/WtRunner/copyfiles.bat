set env=%1
set plat=%2

set folder="..\%plat%\%env%\WtRunner\parsers\"
if not exist %folder% md %folder%

set folder="..\%plat%\%env%\WtRunner\traders\"
if not exist %folder% md %folder%

set folder="..\%plat%\%env%\WtRunner\executer\"
if not exist %folder% md %folder%

xcopy ..\%plat%\%env%\ParserUDP.dll ..\%plat%\%env%\WtRunner\parsers\ /C /Y
xcopy ..\%plat%\%env%\TraderCTP.dll ..\%plat%\%env%\WtRunner\traders\ /C /Y
xcopy ..\%plat%\%env%\WtExeFact.dll ..\%plat%\%env%\WtRunner\executer\ /C /Y
