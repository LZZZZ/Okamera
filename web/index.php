<?php
$VIDEO_DIRECTORY = 'video';
$THUMBNAIL_DIRECTORY = 'thumbnail';

$VIDEO_FORMAT = 'mp4';
$THUMBNAIL_FORMAT = 'webm';

$VIDEO_DURATION_SECONDS = 60;

$VIDEOS_POR_PAGINA = 60;

$TEMPO_MIN = 1400000000;
$TEMPO_MAX = 5000000000;

//função simples para retornar o tamanho do arquivo em formato legível
function human_filesize($bytes, $decimals = 2) {
    $size = array('B', 'KiB', 'MiB', 'GiB', 'TiB', 'PiB');
    $factor = floor((strlen($bytes) - 1) / 3);
    return sprintf("%.{$decimals}f", $bytes / pow(1024, $factor)) . ' ' . $size[$factor];
}

//função para obter o tempo total dos vídeos salvos em formato legível
function human_time($seconds) {
    $dtF = new \DateTime('@0');
    $dtT = new \DateTime("@$seconds");
    return $dtF->diff($dtT)->format('%ad %hh %imin');
}

$videoDirectory = scandir($VIDEO_DIRECTORY, 1);
foreach ($videoDirectory as $i => $file) {
    if (pathinfo($file, PATHINFO_EXTENSION) != $VIDEO_FORMAT) {
        unset($videoDirectory[$i]);
    } else {
        $videoDirectory[$i] = pathinfo($file, PATHINFO_FILENAME);
    }
}
arsort($videoDirectory);

$primeiro = 0;
if (isset($_GET['inicio']) && ($_GET['inicio'] > 0)) {
    $primeiro = $_GET['inicio'];
}
$total = count($videoDirectory);
$ultimo = $primeiro + $VIDEOS_POR_PAGINA;
if ($ultimo > $total) {
    $ultimo = $total;
}

$headline = ' ';
if ($primeiro > 0) {
    $antes = $primeiro - $VIDEOS_POR_PAGINA;
    if ($antes < 0) {
        $antes = 0;
    }
    $headline .= "<a href=\"?inicio=$antes\"><</a>";
}
$headline .= " $primeiro - " . ($ultimo-1) . ' ';
if ($ultimo < $total) {
    $headline .= "<a href=\"?inicio=$ultimo\">></a>";
}
$headline .=
        ' ] [ ' . human_time($total * $VIDEO_DURATION_SECONDS) .
        ' ] [ ' . human_filesize(disk_free_space($VIDEO_DIRECTORY), 2) .
        ' free of ' . human_filesize(disk_total_space($VIDEO_DIRECTORY), 2) .
        ' ] [ ' . exec('/opt/vc/bin/vcgencmd measure_temp');
?>
<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
        <link rel="stylesheet" type="text/css" href="estilo.css" />
        <link rel="icon" type="image/png" href="okamera.png">
        <title>Okamera</title>
        <script type="text/javascript" src="js/webpjs-0.0.2.min.js"></script>
    </head>
    <body>
        <p class="title">Okamera [ <?php echo $headline; ?> ]</p>
        <div id="thumbnails"><?php
                $pos = 0;
                foreach ($videoDirectory as $file) {
                    if (($pos >= $primeiro) && ($pos < $ultimo)) {
                        $nome = date('Y/m/d H\hi\m\i\ns\s', $file) . ' (' . human_filesize(filesize("$VIDEO_DIRECTORY/$file.mp4")) . ')';
                        $nomeMin = date('H\h i\m\i\n s\s', $file) . ' - ' . human_filesize(filesize("$VIDEO_DIRECTORY/$file.mp4"));
?>
            <div class="thumbContainer">
                <a href="videoplayer.php?video=<?php echo $file ?>" target="_blank"><img class="thumbnail" alt="<?php echo $nome; ?>" title="<?php echo $nome; ?>" src="<?php echo "$THUMBNAIL_DIRECTORY/$file.webp" ?>" /></a>
                <p class="thumbText"><?php echo $nomeMin; ?></p>
            </div>
            <?php
                    }
                    $pos++;
                }
?>
            
        </div>
    </body>
</html>
