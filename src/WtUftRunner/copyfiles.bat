set env=%1
set plat=%2

set folder="..\%plat%\%env%\WtUftRunner\parsers\"
if not exist %folder% md %folder%

set folder="..\%plat%\%env%\WtUftRunner\traders\"
if not exist %folder% md %folder%

xcopy ..\%plat%\%env%\ParserCTP.dll ..\%plat%\%env%\WtUftRunner\parsers\ /C /Y
xcopy ..\%plat%\%env%\TraderCTP.dll ..\%plat%\%env%\WtUftRunner\traders\ /C /Y