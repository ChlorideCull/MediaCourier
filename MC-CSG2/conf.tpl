% import mysqldbconn
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
            <td>{{userinfo["registered"]}}</td>
        </tr>
        <tr>
            <td>Last logged in at</td>
            <td>{{userinfo["lastonline"]}}</td>
        </tr>
        <tr>
            <td>Privilege level</td>
            <td>{{userinfo["userlevel"]}}</td>
        </tr>
    </table>
</div>
