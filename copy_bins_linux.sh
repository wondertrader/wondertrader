despath=$1
if test "$despath"==""; then 
	despath="../wtpy"
	echo $despath
fi
cp -rf ./src/build_all/build_x64/Release/bin/Loader/*.so $despath/wtpy/wrapper/linux
cp -rf ./src/build_all/build_x64/Release/bin/WtBtPorter/*.so $despath/wtpy/wrapper/linux
cp -rf ./src/build_all/build_x64/Release/bin/WtDtPorter/*.so $despath/wtpy/wrapper/linux
