import instanceconfig
import pymysql
import pymysql.cursors
import hashlib

class MCDBConnection:
    def __init__(self):
        self.connection = pymysql.connect(host=instanceconfig.sqlhost,
            user=instanceconfig.sqluser,
            password=instanceconfig.sqlpass,
            db='mediacourier',
            charset='utf8',
            cursorclass=pymysql.cursors.DictCursor)
            
    def authenticate_user(self, userid, password):
        cur = self.connection.cursor()
        cur.execute("SELECT passwdhash,salt FROM oosers WHERE userid=%(userid)i LIMIT 1", {"userid": userid})
        
        try:
            retrow = cur.fetchone()
        except:
            return False
        
        hash = hashlib.sha512(str(str(retrow["salt"]) + ":" + password).encode('utf-8')).hexdigest()
        if hash == retrow["passwdhash"]:
            cur.execute("UPDATE oosers SET lastonline=UNIX_TIMESTAMP() WHERE userid=%(userid)i LIMIT 1", {"userid": userid})
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
    
    def create_new_streamkey(self, userid):
        cur = self.connection.cursor()
        cur.execute("UPDATE oosers SET streamkey=SHA1(FLOOR(RAND() * 4294967295)) WHERE userid=%(userid)i LIMIT 1", {"userid": userid})
        self.connection.commit()
        if cur.rowcount < 1:
            return False
        return True

    def get_streamkey(self, userid):
        cur = self.connection.cursor()
        cur.execute("SELECT streamkey FROM oosers WHERE userid=%(userid)i LIMIT 1", {"userid": userid})
        try:
            retrow = cur.fetchone()
        except:
            return ''
        cur.close()
        return retrow["streamkey"]
    
    def get_user_info(self, userid):
        cur = self.connection.cursor()
        cur.execute("SELECT username,lastonline,registered,userlevel,avatarurl FROM oosers WHERE userid=%(userid)i LIMIT 1", {"userid": userid})
        try:
            retrow = cur.fetchone()
        except:
            return False
        cur.close()
        return retrow
        
    def get_userid(self, username):
        cur = self.connection.cursor()
        cur.execute("SELECT userid FROM oosers WHERE username=%(username)s LIMIT 1", {"username": username})
        try:
            retrow = cur.fetchone()
        except:
            return -1
        cur.close()
        return int(retrow["userid"])
