<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
	<link rel="stylesheet" type="text/css" href="estilo.css" />
        <title>Okamera</title>
    </head>
    <body>
        <h1>Okamera - Visualizador de vídeo</h1>
        <hr />
        <a href="javascript:history.back()">Voltar</a> | <a href="videos/<?php echo $_GET['video']; ?>.mp4">Download do vídeo</a>
        <hr />
        <video controls="controls" autoplay="autoplay">
            <source src="flash/video/<?php echo $_GET['video']; ?>.mp4" type="video/mp4" />
        </video>
        <hr />
    </body>
</html>
