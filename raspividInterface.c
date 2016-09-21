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
#include "config.h"

/**
 * Starts the raspivid camera in segment mode with the options defined in
 * config
 * 
 * @param config config struct with raspivid options
 * 
 * @return the pid of the child thread
 */
pid_t startCamera(struct Config* config) {
    
    //preparing arguments for raspivid
    char** args;
    args = malloc((getNumberOfRaspividArgs(config) + 1) * sizeof(char*));
    int pos = 0;
    
    //raspivid bin
    {
        int len = strlen(RASPIVID_BIN_PATH) + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, RASPIVID_BIN_PATH);
        pos++;
    }
    
    //preview
    if (config->RaspividPreview != NULL) {
        int len = strlen(RASPIVID_PARAMETER_PREVIEW) + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, RASPIVID_PARAMETER_PREVIEW);
        pos++;
        
        len = strlen(config->RaspividPreview) + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, "%s", config->RaspividPreview);
        pos++;
    }
    
    //ISO
    
    //exposure
    if (config->RaspividExposure != NULL) {
        int len = strlen(RASPIVID_PARAMETER_EXPOSURE) + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, RASPIVID_PARAMETER_EXPOSURE);
        pos++;
        
        len = strlen(config->RaspividExposure) + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, "%s", config->RaspividExposure);
        pos++;
    }
    
    //AWB
    if (config->RaspividAWB != NULL) {
        int len = strlen(RASPIVID_PARAMETER_AWB) + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, RASPIVID_PARAMETER_AWB);
        pos++;
        
        len = strlen(config->RaspividAWB) + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, "%s", config->RaspividAWB);
        pos++;
    }
    
    //metering
    if (config->RaspividMetering != NULL) {
        int len = strlen(RASPIVID_PARAMETER_METERING) + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, RASPIVID_PARAMETER_METERING);
        pos++;
        
        len = strlen(config->RaspividMetering) + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, "%s", config->RaspividMetering);
        pos++;
    }
    
    //rotation
    if (config->RaspividRotation != NULL) {
        int len = strlen(RASPIVID_PARAMETER_ROTATION) + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, RASPIVID_PARAMETER_ROTATION);
        pos++;
        
        len = strlen(config->RaspividRotation) + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, "%s", config->RaspividRotation);
        pos++;
    }
    
    //mode
    {
        int len = strlen(RASPIVID_PARAMETER_MODE) + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, RASPIVID_PARAMETER_MODE);
        pos++;
        
        len = strlen("4") + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, "4");
        pos++;
    }
    
    //annotate
    {
        int len = strlen(RASPIVID_PARAMETER_ANNOTATE) + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, RASPIVID_PARAMETER_ANNOTATE);
        pos++;
        
        len = strlen("8") + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, "4");
        pos++;
        
        len = strlen(RASPIVID_PARAMETER_ANNOTATE) + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, RASPIVID_PARAMETER_ANNOTATE);
        pos++;
        
        len = strlen("Ok01 %%Y-%%m-%%d %%H:%%M:%%S") + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, "Ok01 %%Y-%%m-%%d %%H:%%M:%%S");
        pos++;
    }
    
    //annotateex
    {
        int len = strlen(RASPIVID_PARAMETER_ANNOTATEEX) + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, RASPIVID_PARAMETER_ANNOTATEEX);
        pos++;
        
        len = strlen("18,0xff,0x808000") + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, "18,0xff,0x808000");
        pos++;
    }
    
    //width
    if (config->RaspividWidth != NULL) {
        int len = strlen(RASPIVID_PARAMETER_WIDTH) + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, RASPIVID_PARAMETER_WIDTH);
        pos++;
        
        len = strlen(config->RaspividWidth) + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, "%s", config->RaspividWidth);
        pos++;
    }
    
    //height
    if (config->RaspividHeight != NULL) {
        int len = strlen(RASPIVID_PARAMETER_HEIGHT) + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, RASPIVID_PARAMETER_HEIGHT);
        pos++;
        
        len = strlen(config->RaspividHeight) + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, "%s", config->RaspividHeight);
        pos++;
    }
    
    //bitrate
    {
        int len = strlen(RASPIVID_PARAMETER_BITRATE) + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, RASPIVID_PARAMETER_BITRATE);
        pos++;
        
        len = strlen("0") + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, "0");
        pos++;
    }
    
    //output
    {
        int len = strlen(RASPIVID_PARAMETER_OUTPUT) + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, RASPIVID_PARAMETER_OUTPUT);
        pos++;
        
        char videoOutput[512];
        snprintf(videoOutput, 512, "%s/vid%%d.h264", config->TemporaryDirectory);
        len = strlen(videoOutput) + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, "%s", videoOutput);
        pos++;
    }
    
    //timeout
    {
        int len = strlen(RASPIVID_PARAMETER_TIMEOUT) + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, RASPIVID_PARAMETER_TIMEOUT);
        pos++;
        
        len = strlen("0") + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, "0");
        pos++;
    }
    
    //framerate
    if (config->RaspividFramerate != NULL) {
        int len = strlen(RASPIVID_PARAMETER_FRAMERATE) + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, RASPIVID_PARAMETER_FRAMERATE);
        pos++;
        
        len = strlen(config->RaspividFramerate) + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, "%s", config->RaspividFramerate);
        pos++;
    }
    
    //intra
    if (config->RaspividIntraframeInterval != NULL) {
        int len = strlen(RASPIVID_PARAMETER_INTRA) + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, RASPIVID_PARAMETER_INTRA);
        pos++;
        
        len = strlen(config->RaspividIntraframeInterval) + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, "%s", config->RaspividIntraframeInterval);
        pos++;
    }
    
    //qp
    if (config->RaspividCRF != NULL) {
        int len = strlen(RASPIVID_PARAMETER_CRF) + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, RASPIVID_PARAMETER_CRF);
        pos++;
        
        len = strlen(config->RaspividCRF) + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, "%s", config->RaspividCRF);
        pos++;
    }
    
    //profile
    if (config->RaspividProfile != NULL) {
        int len = strlen(RASPIVID_PARAMETER_PROFILE) + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, RASPIVID_PARAMETER_PROFILE);
        pos++;
        
        len = strlen(config->RaspividProfile) + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, "%s", config->RaspividProfile);
        pos++;
    }
    
    //inline
    {
        int len = strlen(RASPIVID_PARAMETER_INLINE) + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, RASPIVID_PARAMETER_INLINE);
        pos++;
    }
    
    //segment
    if (config->RaspividSegmentDuration != NULL) {
        int len = strlen(RASPIVID_PARAMETER_SEGMENT) + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, RASPIVID_PARAMETER_SEGMENT);
        pos++;
        
        //Converting segment duration from seconds to milliseconds and making it
        //a little bit shorter to avoid problems with intra frames.
        int segmentDurationInt = atoi(config->RaspividSegmentDuration) - 1;
        if (segmentDurationInt < 1) {
            segmentDurationInt = 59;
        }
        char segmentDurationMs[32];
        snprintf(segmentDurationMs, 32, "%d900", segmentDurationInt);
        
        len = strlen(segmentDurationMs) + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, "%s", segmentDurationMs);
        pos++;
    }
    
    //wrap
    {
        int len = strlen(RASPIVID_PARAMETER_WRAP) + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, RASPIVID_PARAMETER_WRAP);
        pos++;
        
        //Converting number of temporary files from int to string
        char tempRawFiles[4];
        snprintf(tempRawFiles, 4, "%d", config->NumberTemporaryRawFiles);
        
        len = strlen(tempRawFiles) + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, "%s", tempRawFiles);
        pos++;
    }
    
    //start
    {
        int len = strlen(RASPIVID_PARAMETER_START) + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, RASPIVID_PARAMETER_START);
        pos++;
        
        len = strlen("1") + 1;
        args[pos] = malloc(len);
        snprintf(args[pos], len, "1");
        pos++;
    }
    
    //NULL
    args[pos] = NULL;
    
    
    //forking and calling raspivid
    pid_t pid = fork();

    if (pid == 0) {
        execv(args[0], args);
        exit(EXIT_SUCCESS);
    }
    
    return pid;
}
