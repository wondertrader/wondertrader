import json

class BaseIndexWriter:
    '''
    基础指标输出工具
    '''

    def __init__(self):
        return

    def write_indicator(self, id, tag, time, data):
        '''
        将指标数据出\n
        @id     指标ID\n
        @tag    数据标记\n
        @time   指标时间\n
        @data   数据对象，一个dict
        '''
        raise Exception("Basic writer cannot output index data to any media")


class MysqlIndexWriter(BaseIndexWriter):
    '''
    Mysql指标数据写入器
    '''

    def __init__(self, host, port, user, pwd, dnname, sqlfmt):
        import pymysql
        self.__db_conn__ = pymysql.connect(
            host=host, user=user, passwd=pwd, db=dnname, port=port)
        self.__sql_fmt__ = sqlfmt

    def write_indicator(self, id, tag, time, data):
        sql = self.__sql_fmt__.replace("$ID", id).replace("$TAG", tag).replace("$TIME", str(time)).replace("$DATA", json.dumps(data))
        curConn = self.__db_conn__
        curBase = curConn.cursor()
        curBase.execute(sql)
        curConn.commit()