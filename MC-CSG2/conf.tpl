% import mysqldbconn
% import instanceconfig
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
        % streamkey = conn.get_streamkey(csg2api.get_username_of_request())
            <td>{{streamkey}}</td>
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
<div class="contentbox">
    <h2>Configuring your streaming client</h2>
    <div class="contentbox" style="margin: 15px;">
        <b>Your streaming URI: </b>
        <pre>{{instanceconfig.streampath.format(csg2api.get_username_of_request()) + "?key={}".format(streamkey)}}</pre>
    </div>
    <p>
        New to streaming? Don't know what to use?
        Media Courier UF recommends <a href="https://obsproject.com/download#mp">OBS Studio</a>.
    </p>
    <h3>OBS Studio</h3>
    <p>
        <i>Step 1)</i> Open OBS Studio and navigate to the settings.
        <br>
        <img src="http://i.imgur.com/tDXJg8e.png" />
        <br>
        <br>
        <i>Step 2)</i> Select "Stream".
        <br>
        <img src="http://i.imgur.com/Wcg4C9q.png" />
        <br>
        <br>
        <i>Step 3)</i> Set Stream Type to "Custom Streaming Server".
        <br>
        <img src="http://i.imgur.com/XFtXdaf.png" />
        <br>
        <br>
        <i>Step 4)</i> Paste your streaming URI in the URL field, and hit Apply.
        <br>
        <img src="http://i.imgur.com/wSW8Lai.png" />
    </p>
</div>
