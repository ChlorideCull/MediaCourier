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
    </table>
</div>