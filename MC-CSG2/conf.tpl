% import mysqldbconn
% import instanceconfig
% conn = mysqldbconn.MCDBConnection()
% userid = conn.get_userid(csg2api.get_username_of_request())
<div class="contentbox">
    <h2>Change Username</h2>
    <p>Remember that you need to type this exactly the same way as here in order to log in.</p>
    <form action="/auth/changeusername" method="post">
        <input name="newusername" placeholder="New Password" type="text">
        <input value="Submit" type="submit">
    </form>
</div>
<div class="contentbox">
    <h2>Change Password</h2>
    <p>Remember that you need to type this exactly the same way as here in order to log in.</p>
    <form action="/auth/changepassword" method="post">
        <input name="newpassword" placeholder="New Password" type="password">
        <input name="newpasswordagain" placeholder="New Password Again" type="password">
        <input value="Submit" type="submit">
    </form>
</div>
<div class="contentbox">
    <h2>Configuring your streaming client</h2>
    <div class="contentbox" style="margin: 15px;">
        <b>Your streaming URI: </b>
        % streamkey = conn.get_streamkey(userid)
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
