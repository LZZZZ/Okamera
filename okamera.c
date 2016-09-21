/* 
 * This file is part of Okamera.
 *
 * Okamera is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Foobar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Okamera.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/* 
 * File:   okamera.c
 * Author: Leonardo Goetz Zenzen
 * lzzbra@gmail.com
 *
 * Created on July 8, 2016, 4:15 PM
 */

#include "okamera.h"
#include "raspividInterface.h"

/**
 * Sorting function for qsort
 * 
 * @param a the first fileWithCtime to be compared
 * @param b the second fileWithCtime to be compared
 * 
 * @return the difference between their's ctime (a - b)
 */
inline int fileWithCtimeComparator(const void *a, const void *b) {
    struct fileWithCtime *fileA = (struct fileWithCtime *) a;
    struct fileWithCtime *fileB = (struct fileWithCtime *) b;
    
    return fileA->ctime - fileB->ctime;
}

/**
 * Signal handler callback: closes the program gracefully when SIGTERM is
 * received.
 * 
 * @param signum
 * 
 * TODO: MUX all remaining h264 files before finishing
 */
void signalCallbackHandler(int signum) {
    if ((signum == SIGINT) || (signum == SIGTERM)) {
        syslog(LOG_NOTICE, "%s ending.", PROGRAM_NAME);
        
        //Stopping raspivid
        kill(raspivid, SIGINT);
        sleep(1);
        kill(raspivid, SIGTERM);
        sleep(1);
        kill(raspivid, SIGKILL);
        wait(NULL);
        
        //Muxing and removing any remaining h264 file
        muxAll();
        
        //Ending main loop
        run = 0;
        
        //removing lock file
        remove(LOCK_FILE);
        
        wait(NULL);
    }
}

/**
 * Fork and stuff to run as a daemon
 */
static void daemonize() {
    pid_t pid;

    //Fork off the parent process
    pid = fork();

    //An error occurred
    if (pid < 0) {
        printf("Okamera: failed to fork.");
        exit(EXIT_FAILURE);
    }

    //Success: Let the parent terminate
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    //On success: The child process becomes session leader
    if (setsid() < 0) {
        printf("Okamera: failed to set the child process as session leader.");
        exit(EXIT_FAILURE);
    }
    
    //Catch, ignore and handle signals
    signal(SIGCHLD, SIG_IGN); //ignore child
    signal(SIGTSTP, SIG_IGN); //ignore tty signals
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGHUP, SIG_IGN);
    signal(SIGINT, signalCallbackHandler);
    signal(SIGTERM, signalCallbackHandler);

    //Fork off for the second time
    pid = fork();

    //An error occurred
    if (pid < 0) {
        printf("Okamera: failed to fork the second time.");
        exit(EXIT_FAILURE);
    }

    //Success: Let the parent terminate
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    //Locking a file to prevent multiple instances
    mkdir("/dev/shm/okamera", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    int lockFile = open(LOCK_FILE, O_RDWR|O_CREAT, 0640);
    if (lockFile < 0) {
        //Can't open
        printf("Okamera: failed to open lock file.");
        exit(EXIT_FAILURE);
    }
    if (lockf(lockFile, F_TLOCK, 0) < 0) {
        //Can't lock
        printf("Okamera: only one instance can run at a time.");
        exit(EXIT_SUCCESS);
    }
    //Recording pid to lock file
    char str[8];
    sprintf(str, "%d\n", getpid());
    write(lockFile, str, strlen(str));
    
    //Set new file permissions
    umask(0);

    //Change the working directory to the root directory
    //or another appropriated directory
    chdir("/");

    //Close all open file descriptors
    int i;
    for (i = sysconf(_SC_OPEN_MAX); i > 0; i--) {
        close(i);
    }

    //Open the log file
    openlog(PROGRAM_NAME, LOG_PID, LOG_DAEMON);
}

/**
 * Mux all h264 files in the raw directory and remove them.
 */
void muxAll() {
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(config.TemporaryDirectory)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            //Ignoring "." and ".."
            if (strcmp(ent->d_name, ".") == 0) {
                continue;
            }
            if (strcmp(ent->d_name, "..") == 0) {
                continue;
            }

            //Check if the file has an h264 extension
            if (strcmp(getFileExtension(ent->d_name), "h264") == 0) {
                //Writing the full file name with path to a variable
                char input[256];
                snprintf(input, 256, "%s/%s", config.TemporaryDirectory, ent->d_name);

                //Getting the file details
                struct stat stats;
                stat(input, &stats);

                //Muxing
                muxAndCreateThumbnail(input, stats.st_ctime);
            }
        }
        closedir(dir);
    }
}

/**
 * Checks if there are any ready h264 files in the temp folder and, if there
 * are, mux them to mp4, create thumbnail image and delete the raw h264 file.
 * 
 * This function checks if a file is ready to be muxed by verifying if there's a
 * newer file on the temp folder.
 * 
 */
void checkForReadyh264Files() {
    //Vars to parse the dir
    DIR *dir;
    struct dirent *ent;

    //Array to store the files in the dir
    struct fileWithCtime raws[config.NumberTemporaryRawFiles];
    unsigned short size = 0;
    
    //Parsing the dir
    if ((dir = opendir(config.TemporaryDirectory)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            //Ignoring "." and ".."
            if (strcmp(ent->d_name, ".") == 0) {
                continue;
            }
            if (strcmp(ent->d_name, "..") == 0) {
                continue;
            }

            //Check if the file has an h264 extension
            if (strcmp(getFileExtension(ent->d_name), "h264") == 0) {
                //Check if there's space in the array
                if (size < config.NumberTemporaryRawFiles) {
                    //Writing the full file name with path to a variable
                    char filepath[256];
                    snprintf(filepath, 256, "%s/%s", config.TemporaryDirectory, ent->d_name);
                    
                    //Getting the file details
                    struct stat stats;
                    stat(filepath, &stats);
                    
                    //Adding them to the file array
                    raws[size].ctime = stats.st_ctime;
                    raws[size].name = ent->d_name;
                    size++;
                }
            }
        }
        closedir(dir);

        //If there are more then one h264 file, then all but the newest are ready to mux
        if (size > 0) {
            //Sorting files by ctime
            qsort(raws, size, sizeof(struct fileWithCtime), fileWithCtimeComparator);
            
            //For every file but the newest
            int i;
            for (i = 0; i < (size - 1); i++) {
                //If there are two files with the same ctime, none of them should be muxed yet,
                //since we still don't know witch one is being recorded to.
                if (raws[i].ctime != raws[i+1].ctime) {
                    char input[256];
                    snprintf(input, 256, "%s/%s", config.TemporaryDirectory, raws[i].name);
                    muxAndCreateThumbnail(input, raws[i].ctime);
                }
            }
        }
    } else {
        syslog(LOG_ERR, "Error listing files in %s.", config.TemporaryDirectory);
    }
}

/**
 * Mux a h264 file to mp4, remove h264 and create a thumbnail
 * 
 * @param input the full path of the h264 file
 * @param outputCTime the ctime of the input file
 */
void muxAndCreateThumbnail(char* input, int ctime) {
    char videoOutput[256];
    snprintf(videoOutput, 256, "%s/%d.mp4", config.VideoSaveDirectory, ctime);

    syslog(LOG_INFO, "Muxing: %s -> %s.", input, videoOutput);
    
    //Muxing h264 to mp4
    {
        pid_t muxPid = mux(config.ffmpegPath, config.RaspividFramerate, input, videoOutput);
        if (muxPid < 0) {
            syslog(LOG_ERR, "Failed to create muxer thread.");
        } else {
            waitpid(muxPid, NULL, 0);   //Waiting
            syslog(LOG_INFO, "Video %s saved.", videoOutput);
        }
    }

    //Removing h264
    {
        int status = remove(input);
        if (status == 0) {
            syslog(LOG_INFO, "Temporary raw file %s removed.", input);
        } else {
            syslog(LOG_ERR, "Failed to remove %s: %d.", input, status);
        }
    }

    //Making a thumbnail for this video
    {
        pid_t thumbnailPid = makeThumbnail(config.ffmpegPath, config.ThumbnailWidth, config.ThumbnailFormat, videoOutput, config.ThumbnailSaveDirectory);
        if (thumbnailPid < 0) {
            syslog(LOG_ERR, "Failed to create thumbnail thread.");
        } else {
            waitpid(thumbnailPid, NULL, 0);  //Waiting
            syslog(LOG_INFO, "Thumbnail saved.");
        }
    }
}

/**
 * Deletes the oldest video and it's thumbnail
 */
void deleteOldestVideo() {
    DIR *dir;
    struct dirent *ent;
    time_t oldest = time(NULL); //all files will be older then now

    if ((dir = opendir(config.VideoSaveDirectory)) != NULL) {
        //For every file in the videos directory
        while ((ent = readdir(dir)) != NULL) {
            //If the file is a mp4
            const char* extensao = getFileExtension(ent->d_name);
            if (extensao != NULL) {
                if (strcmp(extensao, "mp4") == 0) {
                    //Converting the timestamp to time_t
                    time_t timestamp = atol(ent->d_name);

                    //If it's older then the oldest one until now, set this as oldest
                    if (timestamp < oldest) {
                        oldest = timestamp;
                    }
                }
            }
        }
        closedir(dir);

        //Removing the video
        char temp[256];
        snprintf(temp, 256, "%s/%d.mp4", config.VideoSaveDirectory, oldest);
        int status = remove(temp);
        if (status == 0) {
            syslog(LOG_INFO, "Video %s removed.", temp);
        } else {
            syslog(LOG_ERR, "Failed to remove video %s: %d.", temp, status);
        }

        //And it's thumbnail
        snprintf(temp, 256, "%s/%d.%s", config.ThumbnailSaveDirectory, oldest, config.ThumbnailFormat);
        status = remove(temp);
        if (status == 0) {
            syslog(LOG_INFO, "Thumbnail %s removed.", temp);
        } else {
            syslog(LOG_ERR, "Failed to remove thumbnail %s: %d.", temp, status);
        }
    }
}

int main() {
    //Do forks and stuff to run as a service
    daemonize();
    syslog(LOG_INFO, "%s version %d started.", PROGRAM_NAME, PROGRAM_VERSION);
    
    //Reading the configuration
    setDefault(&config);
    if (parseConfigFile(&config) == 0) {
        if (checkConfig(&config) == 0) {
            /*
            syslog(LOG_INFO, "%s %s", VIDEO_SAVE_DIRECTORY, config.VideoSaveDirectory);
            syslog(LOG_INFO, "%s %s", THUMBNAIL_SAVE_DIRECTORY, config.ThumbnailSaveDirectory);
            syslog(LOG_INFO, "%s %s", FFMPEG_PATH, config.ffmpegPath);
            syslog(LOG_INFO, "%s %lu", MINIMUM_FREE_DISK_SPACE, config.MinimumFreeDiskSpace);
            syslog(LOG_INFO, "%s %s", RASPIVID_ROTATION, config.RaspividRotation);
            syslog(LOG_INFO, "%s %s", RASPIVID_WIDTH, config.RaspividWidth);
            syslog(LOG_INFO, "%s %s", RASPIVID_HEIGHT, config.RaspividHeight);
            syslog(LOG_INFO, "%s %s", RASPIVID_CRF, config.RaspividCRF);
            syslog(LOG_INFO, "%s %s", RASPIVID_PREVIEW, config.RaspividPreview);
            syslog(LOG_INFO, "%s %s", RASPIVID_FRAMERATE, config.RaspividFramerate);
            syslog(LOG_INFO, "%s %s", RASPIVID_SEGMENT_DURATION, config.RaspividSegmentDuration);
            syslog(LOG_INFO, "%s %s", RASPIVID_INTRAFRAME_INTERVAL, config.RaspividIntraframeInterval);
            syslog(LOG_INFO, "%s %s", RASPIVID_EXPOSURE, config.RaspividExposure);
            syslog(LOG_INFO, "%s %s", RASPIVID_WHITEBLANCE, config.RaspividAWB);
            syslog(LOG_INFO, "%s %s", RASPIVID_METERING, config.RaspividMetering);
            syslog(LOG_INFO, "%s %s", RASPIVID_PROFILE, config.RaspividProfile);
            syslog(LOG_INFO, "%s %s", THUMBNAIL_WIDTH, config.ThumbnailWidth);
            syslog(LOG_INFO, "%s %s", THUMBNAIL_FORMAT, config.ThumbnailFormat);
            syslog(LOG_INFO, "%s %s", THUMBNAIL_OPTIONS, config.ThumbnailOptions);
            syslog(LOG_INFO, "%s %s", TEMPORARY_DIRECTORY, config.TemporaryDirectory);
            syslog(LOG_INFO, "%s %hho", NUMBER_TEMPORARY_RAW_FILES, config.NumberTemporaryRawFiles);
            */
            
            //Creating directories
            mkdir(config.TemporaryDirectory, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
            mkdir(config.VideoSaveDirectory, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
            mkdir(config.ThumbnailSaveDirectory, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

            //Muxing and removing any h264 files in temp folder
            muxAll();
            
            //While there's not enough free space, delete old videos
            unsigned long long freeSpace = getFreeSpace(config.VideoSaveDirectory);
            while (freeSpace < config.MinimumFreeDiskSpace) {
                deleteOldestVideo();
                freeSpace = getFreeSpace(config.VideoSaveDirectory);
            }
            
            //Start the camera
            raspivid = startCamera(&config);
            if (raspivid > 0) {
                syslog(LOG_INFO, "Raspivid started with pid %d.", raspivid);

                //Waiting for a bit to get a little behind the camera
                sleep(1);

                //Main loop
                run = 1;
                while (run) {
                    //Wait some time before looking for raw files
                    sleep(20);

                    //While there's not enough free space, delete old videos
                    unsigned long long freeSpace = getFreeSpace(config.VideoSaveDirectory);
                    while (freeSpace < config.MinimumFreeDiskSpace) {
                        deleteOldestVideo();
                        freeSpace = getFreeSpace(config.VideoSaveDirectory);
                    }

                    //Mux and delete h264 files that aren't being recorded to
                    checkForReadyh264Files();
                }
            } else {
                syslog(LOG_ERR, "Error starting raspivid.", raspivid);
            }
        }
    }
    
    //Freeing up memory from config struct
    freeConfig(&config);
    
    syslog(LOG_NOTICE, "%s terminated.", PROGRAM_NAME);
    closelog();
    
    return (EXIT_SUCCESS);
}
