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
 * File:   okamera.h
 * Author: Leonardo Goetz Zenzen
 * lzzbra@gmail.com
 *
 * Created on July 11, 2016, 7:43 PM
 */

#ifndef OKAMERA_H
#define OKAMERA_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <dirent.h>
#include <time.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include "config.h"
#include "misc.h"
#include "ffmpegInterface.h"

#define PROGRAM_NAME "okamera_log"
#define PROGRAM_VERSION 10
#define LOCK_FILE "/dev/shm/okamera/okamera.lock"

    /**
     * For storing file names with their ctime.
     */
    struct fileWithCtime {
        time_t ctime;
        char * name;
    };
    
    struct Config config;
    pid_t raspivid; //Child process running raspivid
    char run; //Main loop control

    /**
    * Sorting function for qsort
    * 
    * @param a the first fileWithCtime to be compared
    * @param b the second fileWithCtime to be compared
    * 
    * @return the difference between their's ctime (a - b)
    */
   int fileWithCtimeComparator(const void *a, const void *b);
    
    /**
     * Signal handler callback: closes the program gracefully when SIGTERM is
     * received.
     * 
     * @param signum
     * 
     * TODO: MUX all remaining h264 files before finishing
     */
    void signalCallbackHandler(int signum);

    /**
     * Fork and stuff to run as a daemon
     */
    static void daemonize();

    /**
     * Mux a h264 file to mp4, remove h264 and create a thumbnail
     * 
     * @param input the full path of the h264 file
     * @param outputCTime the ctime of the input file
     */
    void muxAndCreateThumbnail(char* input, int ctime);

    /**
     * Mux all h264 files in the raw directory and remove them.
     */
    void muxAll();

    /**
     * Checks if there are any ready h264 files in the temp folder and, if there
     * are, mux them to mp4, create thumbnail image and delete the raw h264 file.
     * 
     * This function checks if a file is ready to be muxed by verifying if there's a
     * newer file on the temp folder.
     * 
     */
    void checkForReadyh264Files();

    /**
     * Starts the raspivid camera in segment mode with the defined options in a
     * separated thread.
     * 
     * @return the pid of the child thread
     */
    pid_t startCamera();

    /**
     * Deletes the oldest video and it's thumbnail
     */
    void deleteOldestVideo();

    int main();

#ifdef __cplusplus
}
#endif

#endif /* OKAMERA_H */

