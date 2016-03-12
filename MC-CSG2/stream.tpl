<!DOCTYPE html>
<html>
	<head>
		<title>Media Courier UF - Stream</title>
		<script src="/static/jwplayer-7.3.4/jwplayer.js"></script>
		<script language='javascript' type='text/javascript'>jwplayer.key='{{jwplayerkey}}'</script>
	</head>
	<body>
		<div id="mcufStreamContainer"></div>
		<script>var player = jwplayer("mcufStreamContainer");
			player.setup({
				file: "{{streamuri}}",
				height: 1280,
				width: 720
			});
		</script>
	</body>
</html>
