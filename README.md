# Okamera
Raspbverry Pi continuous video recorder

Okamera is a simple C program made to run as a daemon and continuously record video (using the Raspberry Pi Camera Module).
It uses raspivid to do the recording, witch uses the hardware encoder, so it can record video up to 1920x1080 at 30 frames per second.
Using a lower 1296x968 resolution is recommended, because it uses the full camera sensor with binning, reducing noise and saving disk space.

There is no motion detection so it's intended to be used in external environments (where there's always motion) and with external USB hard drive, or a very large USB flash memmory or SD card.

It's recommended to have any webserver with php to enable remote access to the videos, and ffmpeg is required, for muxing and creating thumbnails. The raspibian repo has avconv, witch does NOT work. You need to compile ffmpeg, with can take a few hours.

Using a RTC module is also recommended.

Currently, Okamera is working, but with some important missing features.



# Installing


Currently, installation must be done manually.

1. Make sure that your Raspberry Pi and it's camera module are working (you have to enable the camera using raspi-config)

2. Install libwebp-dev: "sudo apt-get install libwebp-dev"

3. Download and compile ffmpeg following this guide: https://trac.ffmpeg.org/wiki/CompilationGuide/Ubuntu. There's no need for libass, libfdk-aac, libmp3lame, libopus, libtheora, libvorbis and libx265 so you don't have to install them. Just remove the --enable option when running configure. You do need libwebp, so add --enable-libwebp

4. If you want to use USB HDD or flash memmory, remember to add them to fstab

5. Download this project files and compile them with "gcc -o okamera *.c"

6. Copy the okamera bin to /usr/local/bin: "sudo cp okamera /usr/local/bin/"

7. Create the directory /etc/okamera with "sudo mkdir /etc/okamera", and then copy the configuration file res/okamera_config there with "sudo cp res/okamera_config /etc/okamera"

8. Edit the configuration file to suit your needs with "sudo nano /etc/okamera/okamera_config"

9. Copy the okamera init script in res/okamera to /etc/init.d/okamera "sudo cp res/okamera /etc/init.d/okamera"

10. Run "sudo update-rc.d okamera defaults" to make okamera run when the system is started

11. Run "sudo /etc/init.d/okamera start" to start recording


If you want to access the files remotely, install and configure an web server and copy the files in the web folder to where you set up in your web server.
The video and thumbnail directory set up in the okamera configuration file (/etc/okamera/okamera_config) must be inside the shared folder.
Edit index.php and videoplayer.php and set the directory parameters. You probably should set up basic http auth too.


Okamera logs to syslog so you can read the log messages there (/var/log/syslog). To stop okamera you can use the init script: sudo "/etc/init.d/okamera stop".


# TODO

Make scripts for compiling and installing
Use a SQLite db to store data, instead of always parsing the folders
Use ffprobe to determine the length of the videos
Improve the web interface
Create html files so PHP is not required
Add a simple webserver so no external webserver is required
Use MMAL instead of raspivid to record the video files
Use libavformat instead of ffmpeg to mux the video files