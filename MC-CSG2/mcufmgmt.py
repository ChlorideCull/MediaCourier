from bottle import HTTPResponse, template

@csg2api.route("/api/auth/streamer")
def authstreamer():
    isvalid = True
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
    return template("stream.tpl", streamuri="rtmp://255.255.255.255/", jwplayerkey="nokeyforu")

@csg2api.auth
def authsite(user, passwd):
    return True

@csg2api.route("/debug/username")
def showuser():
    return csg2api.get_username_of_request()

