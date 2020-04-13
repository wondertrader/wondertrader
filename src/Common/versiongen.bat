set datestr=%date:~0,4%.%date:~5,2%.%date:~8,2%
set datestr=%datestr: =0%
set timestr=%time:~0,8%

echo #pragma once > version.h
echo static const char* GetVersionStr() >> version.h
echo { >> version.h
echo     return "v0.3.0 Build@%datestr% %timestr%"; >> version.h
echo } >> version.h
