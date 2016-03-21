% import instanceconfig
<div id="mcufStreamContainer"></div>
<script src="/static/jwplayer-7.3.4/jwplayer.js"></script>
<script language='javascript' type='text/javascript'>jwplayer.key='{{instanceconfig.jwplayerkey}}'</script>
<script>
    var player = jwplayer("mcufStreamContainer");
	player.setup({
        file: "{{instanceconfig.streampath.format(pathargs[0])}}",
        height: 720,
        width: 1280
	});
</script>
