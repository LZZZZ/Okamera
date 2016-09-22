# Okamera

Raspbverry Pi security camera

Okamera is a simple program for continuously recording video using the Raspberry Pi Camera Module.

It uses raspivid to do the recording, so it can record video up to 1080p at 30 frames per second.

There is no motion detection because it's designed to be used in external environments, where there's always motion, and with an external USB hard drive or a very large USB flash memory/SD card.

You can use an webserver with php to access the videos remotely, and ffmpeg is required, for muxing and creating thumbnails. Avconv does NOT work. You need to compile ffmpeg, with can take a few hours.

Using a RTC module is also highly recommended.


# Compilation and installation

1. Make sure that your Raspberry Pi and it's camera module are working

2. Install libwebp-dev
  `sudo apt-get install libwebp-dev`

3. Download and compile ffmpeg following this guide: https://trac.ffmpeg.org/wiki/CompilationGuide/Ubuntu. There's no need for libass, libfdk-aac, libmp3lame, libopus, libtheora, libvorbis and libx265 so you can remove those options when running configure. You do need libwebp, so add `--enable-libwebp`

4. If you want to use USB HDD or flash memory, remember to add them to fstab

5. Download and compile
  ```
  cd ~
  git clone https://github.com/LZZZZ/Okamera.git
  cd Okamera
  make
  sudo make install
  ```

6. Edit the configuration file to suit your needs
  `sudo nano /etc/okamera/okamera_config`

7. Make Okamera run when the system is started
  `sudo update-rc.d okamera defaults`

8. Start recording
  `sudo /etc/init.d/okamera start`


# Web server

If you want to access the files remotely, install and configure an web server and php and use the files in the web folder. 
The video and thumbnail directories set up in the okamera configuration file must be shared by the web server. 
Edit index.php and videoplayer.php and set the directory parameters. You probably should set up basic http auth too.


# Using

Okamera logs to syslog so you can read the log messages there (/var/log/syslog).
To stop okamera you can use the init script `sudo /etc/init.d/okamera stop`.


# TODO

Store data to a a SQLite database

Get video information using ffprobe

Improve the web interface

Serve web pages directly

Use MMAL instead of raspivid to record the video files

Use libavformat instead of ffmpeg to mux the video files
