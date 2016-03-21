import instanceconfig
import mysqldbconn
from bottle import HTTPResponse, template

@csg2api.route("/api/auth/streamer")
def authstreamer():
    rqst = csg2api.get_request()
    streamer = rqst.forms.name
    streamkey = rqst.forms.key
    
    conn = mysqldbconn.MCDBConnection()
    isvalid = (conn.authenticate_streamkey(streamkey) == streamer)
    if isvalid:
        return HTTPResponse("Authorized", status=200)
    else:
        return HTTPResponse("Unauthorized", status=403)

@csg2api.route("/api/auth/viewer")
def authviewer():
    isvalid = True
    if isvalid:
        return HTTPResponse("Authorized", status=200)
    else:
        return HTTPResponse("Unauthorized", status=403)

@csg2api.route("/watch/<filepath:path>")
def showstream(filepath):
    return template("stream.tpl", streamuri=instanceconfig.streampath.format(filepath), jwplayerkey=instanceconfig.jwplayerkey)

@csg2api.auth
def authsite(user, passwd):
    conn = mysqldbconn.MCDBConnection()
    userid = conn.get_userid(user)
    return conn.authenticate_user(userid, passwd)

# DB MODIFICATION API

@csg2api.route("/auth/changeusername", method="POST")
def changeusername():
    username = csg2api.get_username_of_request()
    conn = mysqldbconn.MCDBConnection()
    userid = conn.get_userid(username)
    rqst = csg2api.get_request()
    if rqst.forms.newusername == '':
        return False
    if conn.set_username(userid, rqst.forms.newusername):
        csg2api.get_response().status = "303 Login again"
        response.set_header("Location", "/auth/logout")
        return True
    else:
        return False

@csg2api.route("/auth/changepassword", method="POST")
def changepassword():
    username = csg2api.get_username_of_request()
    conn = mysqldbconn.MCDBConnection()
    userid = conn.get_userid(username)
    rqst = csg2api.get_request()
    if rqst.forms.newpassword == '':
        return False
    if rqst.forms.newpassword != rqst.forms.newpasswordagain:
        return False
    if conn.set_password(userid, rqst.forms.newpassword):
        csg2api.get_response().status = "303 Login again"
        response.set_header("Location", "/auth/logout")
        return True
    else:
        return False
