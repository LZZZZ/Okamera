<!DOCTYPE html>
<?php
    $VIDEO_DIRECTORY = 'video';

    //função simples para retornar o tamanho do arquivo em formato legível
    function human_filesize($bytes, $decimals = 2) {
        $size = array('B', 'KiB', 'MiB', 'GiB', 'TiB', 'PiB');
        $factor = floor((strlen($bytes) - 1) / 3);
        return sprintf("%.{$decimals}f", $bytes / pow(1024, $factor)) . ' ' . $size[$factor];
    }

    $video = filter_input(INPUT_GET, 'video', FILTER_VALIDATE_INT);
?>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
	<link rel="stylesheet" type="text/css" href="estilo.css" />
        <link rel="icon" type="image/png" href="okamera.png">
        <title>Okamera</title>
    </head>
    <body>
        <video controls="controls" autoplay="autoplay">
            <source src="<?php echo ($VIDEO_DIRECTORY . '/' . $video . '.mp4'); ?>" type="video/mp4" />
        </video>
        <p>
            <?php echo ($video . '.mp4'); ?> | 
            <?php echo date('H\h i\m\i\n s\s', $video); ?> até <?php echo date('H\h i\m\i\n s\s', ($video + 60)); ?> | 
            <?php echo date('d/m/Y', ($video + 60)); ?> | 
            <?php echo human_filesize(filesize("$VIDEO_DIRECTORY/$video.mp4")); ?> | 
            <a href="<?php echo ($VIDEO_DIRECTORY . '/' . $video . '.mp4'); ?>">Download</a>
        </p>
    </body>
</html>
