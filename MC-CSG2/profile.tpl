% if len(pathargs) == 0:
%     pathargs = (csg2api.get_username_of_request(),)
% end
% import mysqldbconn
% import instanceconfig
% from datetime import timezone, datetime
% conn = mysqldbconn.MCDBConnection()
% userid = conn.get_userid(pathargs[0])
% userinfo = conn.get_user_info(userid)
<div class="contentbox">
    <div class="accesspass">
        <h3>{{userinfo["userlevel"]}}</h3>
        <img src="{{userinfo["avatarurl"]}}"></img>
        <h1>{{pathargs[0]}}</h1>
        <h2>#{{userid}}</h2>
        <table>
            <tr>
                <td>Registered at</td>
                <td>{{'{:%Y-%m-%d %H:%M:%S} UTC'.format(datetime.fromtimestamp(userinfo["registered"], tz=timezone.utc))}}</td>
            </tr>
            <tr>
                <td>Last logged in at</td>
                <td>{{'{:%Y-%m-%d %H:%M:%S} UTC'.format(datetime.fromtimestamp(userinfo["lastonline"], tz=timezone.utc))}}</td>
            </tr>
        </table>
    </div>
</div>
