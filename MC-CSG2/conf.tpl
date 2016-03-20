% import mysqldbconn
% from datetime import timezone, datetime
% conn = mysqldbconn.MCDBConnection()
<div class="contentbox"> 
    <table>
        <tr>
            <td>Username</td>
            <td>{{csg2api.get_username_of_request()}}</td>
        </tr>
        <tr>
            <td>Stream Key</td>
            <td>{{conn.get_streamkey(csg2api.get_username_of_request())}}</td>
        </tr>
        % userinfo = conn.get_user_info(csg2api.get_username_of_request())
        <tr>
            <td>Registered at</td>
            <td>{{'{:%Y-%m-%d %H:%M:%S} UTC'.format(datetime.fromtimestamp(userinfo["registered"], tz=timezone.utc))}}</td>
        </tr>
        <tr>
            <td>Last logged in at</td>
            <td>{{'{:%Y-%m-%d %H:%M:%S} UTC'.format(datetime.fromtimestamp(userinfo["lastonline"], tz=timezone.utc))}}</td>
        </tr>
        <tr>
            <td>Privilege level</td>
            <td>{{userinfo["userlevel"]}}</td>
        </tr>
    </table>
</div>
