# Okamera
Raspbverry Pi continuous recorder

Okamera is a simple C program made to run as a daemon and continuously record video (using the Raspberry Pi Camera Module), for security pruposes.

There is no motion detection so it's intended to be used in external enviroments, where there's always motion.

It uses raspivid to do the recording, ffmpeg to mux and create thumbnails and any webserver if you want to check the videos through the web.

Currently, it is working, but missing important features, like using SQLite to save information about the videos, instead of scanning it from the directory every time.
