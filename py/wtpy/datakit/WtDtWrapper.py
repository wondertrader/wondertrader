from ctypes import cdll, c_int, c_char_p, c_longlong, c_bool, c_void_p, c_ulong, c_uint, c_uint64, c_double
import platform
import os

def isPythonX64():
    ret = platform.architecture()
    return (ret[0] == "64bit")

def isWindows():
    if "windows" in platform.system().lower():
        return True

    return False

# Python对接C接口的库
class WtDtWrapper:
    '''
    Wt平台数据组件C接口底层对接模块
    '''

    # api可以作为公共变量
    api = None
    ver = "Unknown"
    
    # 构造函数，传入动态库名
    def __init__(self):
        paths = os.path.split(__file__)
        if isWindows(): #windows平台
            if isPythonX64():
                dllname = "x64/WtDtPorter.dll"
                a = (paths[:-1] + (dllname,))
                _path = os.path.join(*a)
                self.api = cdll.LoadLibrary(_path)
            else:
                dllname = "x86/WtDtPorter.dll"
                a = (paths[:-1] + (dllname,))
                _path = os.path.join(*a)
                self.api = cdll.LoadLibrary(_path)
        else:#Linux平台
            dllname = "linux/libWtDtPorter.so"
            a = (paths[:-1] + (dllname,))
            _path = os.path.join(*a)
            print(_path)
            self.api = cdll.LoadLibrary(_path)
            print(self.api)
        self.api.get_version.restype = c_char_p
        self.ver = bytes.decode(self.api.get_version())

    def run_datakit(self):
        '''
        启动数据组件
        '''
        self.api.start()

    def write_log(self, level, message:str, catName:str = ""):
        '''
        向组件输出日志
        '''
        self.api.write_log(level, bytes(message, encoding = "utf8").decode('utf-8').encode('gbk'), bytes(catName, encoding = "utf8"))

    def initialize(self, cfgfile:str = "dtcfg.json", logprofile:str = "logcfgdt.json"):
        '''
        C接口初始化
        '''
        try:
            self.api.initialize(bytes(cfgfile, encoding = "utf8"), bytes(logprofile, encoding = "utf8"))
        except OSError as oe:
            print(oe)

        self.write_log(102, "Wt数据组件已初始化完成，组件版本号：%s" % (self.ver))