import instanceconfig
import pymysql
import pymysql.cursors

class MCDBConnection:
    def __init__(self):
        self.connection = pymysql.connect(host=instanceconfig.sqlhost,
            user=instanceconfig.sqluser,
            password=instanceconfig.sqlpass,
            db='mediacourier',
            charset='utf8',
            cursorclass=pymysql.cursors.DictCursor)
            
    def authenticate_user(self, username, password):
        cur = self.connection.cursor()
        cur.execute("SELECT passwdhash,salt FROM oosers WHERE username=%(username)s LIMIT 1", {"username": username})
        
        try:
            retrow = cur.fetchone()
        except:
            return False
        
        cur.execute("SELECT sha2(%(hashin)s, 512)", {"hashin": retrow["salt"] + ":" + password})
        hashrow = cur.fetchone()
        if hashrow[0] == retrow["passwdhash"]:
            cur.execute("UPDATE oosers SET lastonline=UNIX_TIMESTAMP() WHERE username=%(username)s LIMIT 1", {"username": username})
            cur.close()
            self.connection.commit()
            return True
        else:
            return False
    
    def authenticate_streamkey(self, key):
        cur = self.connection.cursor()
        cur.execute("SELECT username FROM oosers WHERE streamkey=%(streamkey)s LIMIT 1", {"streamkey": key})
        try:
            retrow = cur.fetchone()
        except:
            return ''
        cur.close()
        return retrow["username"]
    
    def create_new_streamkey(self, username):
        cur = self.connection.cursor()
        cur.execute("UPDATE oosers SET streamkey=SHA1(FLOOR(RAND() * 4294967295)) WHERE username=%(username)s LIMIT 1", {"username": username})
        self.connection.commit()
        if cur.rowcount < 1:
            return False
        return True

    def get_streamkey(self, username):
        cur = self.connection.cursor()
        cur.execute("SELECT streamkey FROM oosers WHERE username=%(username)s LIMIT 1", {"username": username})
        try:
            retrow = cur.fetchone()
        except:
            return ''
        cur.close()
        return retrow["streamkey"]
    
    def get_user_info(self, username):
        cur = self.connection.cursor()
        cur.execute("SELECT lastonline,registered,userlevel FROM oosers WHERE username=%(username)s LIMIT 1", {"username": username})
        try:
            retrow = cur.fetchone()
        except:
            return False
        cur.close()
        return retrow
