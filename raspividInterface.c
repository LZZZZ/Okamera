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

#include "raspividInterface.h"

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
) {
    pid_t pid = fork();

    if (pid == 0) {
        char videoOutput[256];
        snprintf(videoOutput, 256, "%s/vid%%d.h264", temporaryDir);

        //Converting segment duration from seconds to milliseconds
        char segmentDurationMs[16];
        snprintf(segmentDurationMs, 16, "%s000", segmentDuration);

        //Converting number of temporary files from int to string
        char tempRawFiles[4];
        snprintf(tempRawFiles, 4, "%d", wrap);
        
        //Creating args array
        //No need to free memory here because when execv is called, the process
        //is finished and all it's memory is released by the system.
        /*
        int nArgs = 32;
        if (strlen(config.RaspividPreview) > 0) {
            nArgs += 2;
        }
        
        char** argsd = malloc(nArgs * sizeof(char*));
        if (argsd == NULL) {
            return (pid_t)-1;
        }
        
        int p = 0;
        if (stringSafeCopy(argsd[p], "/opt/vc/bin/raspivid") != 0) return (pid_t)-1;
        p++;
        
        if (stringSafeCopy(argsd[p], "--width") != 0) return (pid_t)-1;
        p++;
        argsd[p] = &config.RaspividWidth;
        p++;
        
        if (stringSafeCopy(argsd[p], "--height") != 0) return (pid_t)-1;
        p++;
        argsd[p] = &config.RaspividHeight;
        p++;
        
        if (stringSafeCopy(argsd[5], "--rotation") != 0) return (pid_t)-1;
        argsd[6] = &config.RaspividRotation;
        
        if (stringSafeCopy(argsd[7], "--output") != 0) return (pid_t)-1;
        argsd[8] = malloc(128);
        if (argsd[8] == NULL) return (pid_t)-1;
        snprintf(argsd[8], 128, "%s/vid%%d.h264", config.TemporaryDirectory);
        
        if (stringSafeCopy(argsd[9], "--timeout") != 0) return (pid_t)-1;
        if (stringSafeCopy(argsd[10], "0") != 0) return (pid_t)-1;
        
        if (stringSafeCopy(argsd[11], "--framerate") != 0) return (pid_t)-1;
        argsd[12] = &config.RaspividFramerate;
        
        if (stringSafeCopy(argsd[13], "--intra") != 0) return (pid_t)-1;
        argsd[14] = &config.RaspividIntraframeInterval;
        
        if (stringSafeCopy(argsd[15], "--profile") != 0) return (pid_t)-1;
        argsd[16] = &config.RaspividProfile;
        
        if (stringSafeCopy(argsd[17], "--qp") != 0) return (pid_t)-1;
        argsd[18] = &config.RaspividCRF;
        
        if (stringSafeCopy(argsd[19], "--segment") != 0) return (pid_t)-1;
        argsd[20] = &config.RaspividSegmentDuration;
        
        if (stringSafeCopy(argsd[21], "--wrap") != 0) return (pid_t)-1;
        argsd[22] = &config.NumberTemporaryRawFiles;
        
        if (stringSafeCopy(argsd[23], "--start") != 0) return (pid_t)-1;
        if (stringSafeCopy(argsd[24], "1") != 0) return (pid_t)-1;
        
        if (stringSafeCopy(argsd[25], "--level") != 0) return (pid_t)-1;
        if (stringSafeCopy(argsd[26], "4.2") != 0) return (pid_t)-1;
        
        if (stringSafeCopy(argsd[27], "--awb") != 0) return (pid_t)-1;
        if (stringSafeCopy(argsd[28], "auto") != 0) return (pid_t)-1;
        
        if (stringSafeCopy(argsd[29], "--metering") != 0) return (pid_t)-1;
        if (stringSafeCopy(argsd[30], "backlit") != 0) return (pid_t)-1;
        
        argsd[31] = NULL;
        */
        //calling raspivid
        char* args[] = {
            "/opt/vc/bin/raspivid",
            "--width", width,
            "--height", height,
            "--rotation", rotation,
            "--preview", preview,
            "--output", videoOutput,
            "--timeout", "0",
            "--qp", crf,
            "--bitrate", "0",
            "--framerate", framerate,
            "--intra", iframeInterval,
            "--segment", segmentDurationMs,
            "--profile", profile,
            "--wrap", tempRawFiles,
            "--start", "1",
            "--level", "4.2",
            "--exposure", "night",
            "--awb", "auto",
            "--metering", "backlit",
            NULL
        };
        execv(args[0], args);
        exit(EXIT_SUCCESS);
    }
    
    return pid;
}
