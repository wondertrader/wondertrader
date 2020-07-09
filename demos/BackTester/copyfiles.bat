set env=%1
set plat=%2

xcopy ..\%plat%\%env%\WtCtaStraFact.dll ..\%plat%\%env%\BackTester\ /C /Y
xcopy ..\%plat%\%env%\WtExeFact.dll ..\%plat%\%env%\BackTester\ /C /Y
xcopy ..\%plat%\%env%\WtHftStraFact.dll ..\%plat%\%env%\BackTester\ /C /Y
xcopy ..\%plat%\%env%\WtSelStraFact.dll ..\%plat%\%env%\BackTester\ /C /Y
