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

#ifndef FFMPEGINTERFACE_H
#define FFMPEGINTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
    
#include "misc.h"
    
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
     * @return              the pid of the ffmpeg thread, or (pid_t)-1 if failed
     */
    pid_t mux(char* ffmpegPath, char* framerate, char* input, char* output);

    /**
     * Make a thumbnail for a video, using ffmpeg to extract and resize the
     * first frame of a video to an webp image file.
     * The thumbnail is written to the outputDir with the same name  as the
     * input file, but with the webp extension.
     * 
     * @param ffmpegPath    full path to ffmpeg bin
     * @param input         input video file
     * @param outputDir     output directory
     * 
     * @return              the pid of the ffmpeg thread, or (pid_t)-1 if failed
     */
    
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
    pid_t makeThumbnail(char* ffmpegPath, char* width, char* format, char* input, char* outputDir);
    
    
#ifdef __cplusplus
}
#endif

#endif /* FFMPEGINTERFACE_H */

