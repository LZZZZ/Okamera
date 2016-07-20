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
 * Author: Leonardo Goetz Zenzen
 * lzzbra@gmail.com
 *
 * Created on July 18, 2016, 12:48 AM
 */

#include "ffmpegInterface.h"

/**
 * Mux a raw h264 stream to a mp4 file using ffmpeg.
 * You may need to call waitpid() after calling this to avoid muxing many
 * files at the same time.
 * 
 * Note: this function should work with other containers, by changing the
 * extension of the output file name, since ffmpeg guesses the format from
 * there.
 * 
 * @param ffmpegPath    full path to ffmpeg bin
 * @param input         input h264 file
 * @param output        output file
 * @param framerate     the framerate of the input h264 stream
 * 
 * @return              the pid of the mux thread, or (pid_t)-1 if failed.
 */
pid_t mux(char* ffmpegPath, char* framerate, char* input, char* output) {

    pid_t pid = fork();

    if (pid == 0) {
        //Redirect standard input, output and error to /dev/null.
        //If an error occurs, abort with exit status 125
        if (redirect(STDIN_FILENO, "/dev/null", O_RDONLY)) {
            exit(125);
        }
        if (redirect(STDOUT_FILENO, "/dev/null", O_WRONLY)) {
            exit(125);
        }
        if (redirect(STDERR_FILENO, "/dev/null", O_WRONLY)) {
            exit(125);
        }
        
        //Calling ffmpeg
        char* args[] = {
            "/usr/bin/nohup",
            ffmpegPath,
            "-r", framerate,
            "-i", input,
            "-vcodec", "copy",
            "-an",
            output,
            NULL
        };
        execv(args[0], args);
        exit(EXIT_SUCCESS);
    }
    
    return pid;
}

/**
 * Make a thumbnail for a video, using ffmpeg to extract and resize the
 * first frame of a video to an webp image file.
 * The thumbnail is written to the outputDir with the same name  as the
 * input file, but with the webp extension.
 * 
 * @param ffmpegPath    full path to ffmpeg bin
 * @param width         the desired width for the thumbnail
 * @param format        the extension of the image format to use
 * @param input         input video file
 * @param outputDir     output directory
 * 
 * @return              the pid of the ffmpeg thread, or (pid_t)-1 if failed
 */
pid_t makeThumbnail(char* ffmpegPath, char* width, char* format, char* input, char* outputDir) {

    pid_t pid = fork();

    if (pid == 0) {
        //Getting the file name form the path
        char* bname = basename(input);

        //Copying the filename somewhere else so we can edit
        char filename[128];
        strcpy(filename, bname);

        //Removing extension from the filename
        char* lastdot = strrchr(filename, '.');
        if (lastdot != NULL) {
            *lastdot = '\0';
        }

        //Creating the command line for ffmpeg
        char imageScale[32];
        char output[256];
        snprintf(imageScale, 32, "scale=%s:-1", width);
        snprintf(output, 256, "%s/%s.%s", outputDir, filename, format);
        //TODO: implement ThumbnailOptions configuration

        //Redirect standard input, output and error to /dev/null.
        //If an error occurs, abort with exit status 125
        if (redirect(STDIN_FILENO, "/dev/null", O_RDONLY)) {
            exit(125);
        }
        if (redirect(STDOUT_FILENO, "/dev/null", O_WRONLY)) {
            exit(125);
        }
        if (redirect(STDERR_FILENO, "/dev/null", O_WRONLY)) {
            exit(125);
        }
        
        //Calling ffmpeg
        char* args[] = {
            "/usr/bin/nohup",
            ffmpegPath,
            "-i", input,
            "-vf", imageScale,
            "-f", "image2",
            "-vframes", "1",
            "-compression_level", "6",
            "-q:v", "85",
            "-preset", "photo",
            output,
            NULL
        };
        execv(args[0], args);
        exit(EXIT_SUCCESS);
    }
    
    return pid;
}
