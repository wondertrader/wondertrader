import math
import json

class SectionInfo:

    def __init__(self):
        self.stime = 0
        self.etime = 0

class SessionInfo:

    def __init__(self):
        self.id = ""
        self.name = ""
        self.sections = list()
        self.offset = 0
        self.totalMins = 0

    def offsetTime(self, rawTime:int):
        curMinute = math.floor(rawTime/100)*60 + rawTime%100
        curMinute += self.offset
        if curMinute >= 1440:
            curMinute -= 1440
        elif curMinute < 0:
            curMinute += 1440
        
        return math.floor(curMinute/60)*100 + curMinute%60

    def originalTime(self, offTime:int):
        curMinute = math.floor(offTime/100)*60 + offTime%100
        curMinute -= self.offset
        if curMinute >= 1440:
            curMinute -= 1440
        elif curMinute < 0:
            curMinute += 1440
        
        return math.floor(curMinute/60)*100 + curMinute%60

    def getOpenTime(self, bOffset:bool = False):
        if len(self.sections) == 0:
            return 0

        opentm = self.sections[0].stime
        if not bOffset:
            return self.originalTime(opentm)
        else:
            return opentm

    def getCloseTime(self, bOffset:bool = False):
        if len(self.sections) == 0:
            return 0

        opentm = self.sections[-1].stime
        if not bOffset:
            return self.originalTime(opentm)
        else:
            return opentm

    def getTradingMins(self):
        if len(self.sections) == 0:
            return 0
        
        if self.totalMins == 0:
            for sec in self.sections:
                s = sec.stime
                e = sec.etime
                h = math.floor(e / 100) - math.floor(s / 100)
                m = (e%100) - (s%100)
                self.totalMins += (h*60 + m)
        return self.totalMins

    def getTradingSecs(self):
        return self.getTradingMins()*60


    def isLastOfSection(self, rawTime:int):
        offTime = self.offsetTime(rawTime)

        for sec in self.sections:
            if sec.etime == offTime:
                return True
        
        return False

    def isFirstOfSection(self, rawTime:int):
        offTime = self.offsetTime(rawTime)

        for sec in self.sections:
            if sec.stime == offTime:
                return True
        
        return False

    def timeToMinutes(self, rawTime:int):
        if len(self.sections) == 0:
            return -1

        offTime = self.offsetTime(rawTime)

        bFound = False
        offset = 0
        for sec in self.sections:
            if sec.stime <= offTime and offTime <= sec.etime:
                hour = math.floor(offTime / 100) - math.floor(sec.stime / 100)
                minute = offTime % 100 - sec.stime % 100
                offset += hour*60 + minute
                bFound = True
                break
            else:
                hour = math.floor(sec.etime / 100) - math.floor(sec.stime / 100)
                minute = sec.etime % 100 - sec.stime % 100
                offset += hour*60 + minute

        if not bFound:
            return -1

        return offset

    def minutesToTime(self, minutes:int, bHeadFirst:bool = False):
        if len(self.sections) == 0:
            return -1

        offset = minutes
        for sec in self.sections:
            startMin = math.floor(sec.stime / 100)*60 + sec.stime % 100
            stopMin = math.floor(sec.etime / 100)*60 + sec.etime % 100

            if not bHeadFirst:
                if startMin + offset >= stopMin:
                    offset -= (stopMin - startMin)
                    if offset == 0:
                        return self.originalTime(math.floor(stopMin / 60) * 100 + stopMin % 60)
                else:
                    desMin = startMin + offset
                    if desMin > 1440:
                        desMin -= 1440

                    return self.originalTime(math.floor(desMin / 60) * 100 + desMin % 60)
            else:
                if startMin + offset >= stopMin:
                    offset -= (stopMin - startMin)
                else:
                    desMin = startMin + offset
                    if desMin > 1440:
                        desMin -= 1440

                    return self.originalTime(math.floor(desMin / 60) * 100 + desMin % 60)

        return self.getCloseTime()

class SessionMgr:

    def __init__(self):
        self.__sessions__ = dict()
        return


    def load(self, fname:int):
        f = open(fname, 'r', encoding="gbk")
        content = f.read()
        f.close()

        sessions_dict = json.loads(content)
        for sid in sessions_dict:            
            if sid in self.__sessions__:
                continue

            sObj = sessions_dict[sid]
            sInfo = SessionInfo()
            sInfo.id = sid
            sInfo.offset = sObj["offset"]
            sInfo.name = sObj["name"]

            for secObj in sObj["sections"]:
                secInfo = SectionInfo()
                secInfo.stime = sInfo.offsetTime(secObj["from"])
                secInfo.etime = sInfo.offsetTime(secObj["to"])
                sInfo.sections.append(secInfo)

            self.__sessions__[sid] = sInfo


    def getSession(self, sid:int):
        if sid not in self.__sessions__:
            return None

        return self.__sessions__[sid]