import pymysql
import pymysql.cursors

class MCDBConnection:
    def __init__(self):
        self.connection = pymysql.connect(host='localhost',
            user='user',
            password='passwd',
            db='mediacourier',
            charset='utf8',
            cursorclass=pymysql.cursors.DictCursor)

