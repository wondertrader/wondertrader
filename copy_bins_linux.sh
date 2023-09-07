despath=$1
if test "$despath" = ""; then 
	despath="../wtpy"
fi

echo "wtpy path is $despath"

root="./src/build_all/build_x64/Release/bin"
folders=("Loader" "WtBtPorter" "WtDtPorter" "WtPorter")
for folder in ${folders[@]}
do
	cp -rvf $root/$folder/*.so $despath/wtpy/wrapper/linux
	for file in `ls $root/$folder`
	do
		if [ -d $root"/"$folder"/"$file ]
		then
			cp -rvf $root/$folder/$file/*.so $despath/wtpy/wrapper/linux/$file
		fi
	done
done
