# Okamera

Raspberry Pi Security Camera Recorder

Okamera is a simple program for continuously recording video using the Raspberry Pi Camera Module.

It uses raspivid for the recording, so it can record video up to raspivid limit (depends on raspi and camera module versions).

There is no motion detection. Okamera is designed to be used in external environments (where there's always motion) and with an external USB hard drive or a large USB flash memory/SD card. It can record up to 7 days using a 64GB SD card, at a quality level far superior from traditional cameras.

You can use an web server with php to access the videos remotely.

Using a RTC module is also recommended.


# Compilation and installation

1. Make sure that your Raspberry Pi and it's camera module are working, and that your system is up to date

2. Install ffmpeg
  `sudo apt install ffmpeg`
  
3. Download and compile
  ```
  cd ~
  git clone https://github.com/LZZZZ/Okamera.git
  cd Okamera
  make
  sudo make install
  ```

4. Edit the configuration file to suit your needs
  `sudo nano /etc/okamera/okamera_config`

5. Make Okamera run when the system is started
  `sudo update-rc.d okamera defaults`

6. Start recording
  `sudo service okamera start`


# Web server

If you want to access the files remotely, install and configure an web server and php and use the files in the web folder. 
The video and thumbnail directories set up in the okamera configuration file must be shared by the web server. 
Edit index.php and videoplayer.php and set the directory parameters. You probably should set up basic http auth too.


# Using

Okamera logs to syslog so you can read the log messages there (/var/log/syslog).
To stop okamera you can use the init script `sudo service okamera stop`.


# TODO

Store data to a SQLite database

Get video information using ffprobe

Improve the web interface

Serve web pages directly

Use MMAL instead of raspivid to record the video files

Use libavformat instead of ffmpeg to mux the video files
