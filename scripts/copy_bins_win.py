import os
import shutil
from datetime import datetime
import sys

'''
!!!!重要提示!!!!
如果直接到scripts目录下执行这个py脚本
会有相对路径问题
而且需要wondertrader和wtpy在同一个目录级别
所以如果要使用这个脚本，大概率要自己改一下
'''

subdirs = ['WtPorter','WtDtPorter','WtBtPorter','Loader']

def copyBin(srcDir:str, desDir:str, pdbBakDir:str):

    pdbSearchDir = srcDir
    if not os.path.exists(pdbBakDir):
        os.mkdir(pdbBakDir)

    for subdir in subdirs:
        subpath = os.path.join(srcDir, subdir)
        fnames = os.listdir(subpath)
        fnames.sort()

        for fname in fnames:
            fpath = os.path.join(srcDir, subdir, fname)
            if os.path.isdir(fpath):
                dllnammes = os.listdir(fpath)
                desSubDir = os.path.join(desDir, fname)
                if not os.path.exists(desSubDir):
                    os.mkdir(desSubDir)
                    
                for dllname in dllnammes:
                    if dllname.lower()[-4:] != ".dll":
                        continue
                    shutil.copy(os.path.join(fpath, dllname), os.path.join(desSubDir, dllname))
                    print("dll copied: %s -> %s" % (os.path.join(fpath, dllname), os.path.join(desSubDir, dllname)))
                    pdbname = fname[:-4]+".pdb"
                    pdb_path = os.path.join(pdbSearchDir, pdbname)
                    if os.path.exists(pdb_path):
                        shutil.copy(pdb_path, os.path.join(pdbBakDir, pdbname))
                        print("pdb copied: %s -> %s" % (pdb_path, os.path.join(pdbBakDir, pdbname)))


            if fname.lower()[-4:] != ".dll":
                continue
            
            shutil.copy(os.path.join(subpath, fname), os.path.join(desDir, fname))
            print("dll copied: %s -> %s" % (os.path.join(subpath, fname), os.path.join(desDir, fname)))
            pdbname = fname[:-4]+".pdb"
            pdb_path = os.path.join(srcDir, subdir, pdbname)
            if not os.path.exists(pdb_path):
                pdb_path = os.path.join(pdbSearchDir, pdbname)
            if os.path.exists(pdb_path):
                shutil.copy(pdb_path, os.path.join(pdbBakDir, pdbname))
                print("pdb copied: %s -> %s" % (pdb_path, os.path.join(pdbBakDir, pdbname)))


despath = sys.argv[1]

now = datetime.now().strftime("%m%d_%H%M")
pdbBakDir = os.path.join(despath, "pdb_"+now)
if not os.path.exists(pdbBakDir):
    os.mkdir(pdbBakDir)

#复制x86
copyBin("./src/Win32/Release", os.path.join(despath, "wtpy/wrapper/x86"), os.path.join(pdbBakDir,"x86"))
#复制x64
copyBin("./src/x64/Release", os.path.join(despath, "wtpy/wrapper/x64"), os.path.join(pdbBakDir,"x64"))

#压缩pdb备份目录
paths = os.path.split(__file__)
a = (paths[:-1] + ("Rar.exe",))
rar_path = os.path.join(*a)
#调用scripts下的Rar.exe压缩pdb文件
import subprocess
cmd = '"%s" a -ep1 -r -o+ -m5 -df "%s.rar" "%s"' % (rar_path, os.path.realpath(pdbBakDir), os.path.realpath(pdbBakDir))
subprocess.call(cmd)
