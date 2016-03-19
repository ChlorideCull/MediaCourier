% import mysqldbconn
<div class="contentbox"> 
    <table>
        <tr>
            <td>Username</td>
            <td>{{csg2api.get_username_of_request()}}</td>
        </tr>
        <tr>
            <td>Stream Key</td>
            <td>{{mysqldbconn.get_streamkey(csg2api.get_username_of_request())}}</td>
        </tr>
    </table>
</div>