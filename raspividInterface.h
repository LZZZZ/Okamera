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
 * Created on July 18, 2016, 1:08 AM
 */

#ifndef RASPIVIDINTERFACE_H
#define RASPIVIDINTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <time.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

    /**
     * Starts the raspivid camera in segment mode with the defined options
     * 
     * @param width             output video width
     * @param height            output video height
     * @param crf               video encode quality
     * @param framerate         video frames per second
     * @param iframeInterval    interval between h264 intra frames
     * @param segmentDuration   duration of each video, in seconds
     * @param rotation          video rotation: for flipped sensor use 180
     * @param preview           video preview for hdmi or analog output
     * @param profile           video encode profile
     * @param wrap              how many videos to save before wraping
     * @param temporaryDir      where to save the videos
     * 
     * @return                  the pid of the child thread
     * 
     * TODO implement RaspividPreview configuration
     */
    pid_t startCamera(
            char* width,
            char* height,
            char* crf,
            char* framerate,
            char* iframeInterval,
            char* segmentDuration,
            char* rotation,
            char* preview,
            char* profile,
            unsigned char wrap,
            char* temporaryDir
            );

#ifdef __cplusplus
}
#endif

#endif /* RASPIVIDINTERFACE_H */

