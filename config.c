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
 * Created on July 15, 2016, 9:15 PM
 */

#include "config.h"

/**
 * Parse the configuration file /etc/okamera/okamera_conf and return it's data
 * though the parameter.
 * 
 * @param config where to read the configuration to
 * @return 0 if configuration is valid, 1 if there's an error
 */
unsigned char parseConfigFile(struct Config* config) {

    FILE* configFile = fopen(CONFIG_FILE_PATH, "r");

    if (configFile == NULL) {
        //Error opening the configuration file
        syslog(LOG_ERR, "Error opening the configuration file at %s.", CONFIG_FILE_PATH);
        return 1;
    }

    unsigned char state = STATE_BEFORE_PARAMETER_NAME;
    char parameterName[64];
    unsigned int parameterNamePos = 0;
    char parameterValue[256];
    unsigned int parameterValuePos = 0;

    int ilchar = fgetc(configFile);
    while (ilchar != EOF) {
        char lchar = (char) ilchar;

        //Skipping comments
        if (lchar == '#') {
            ilchar = skipLine(configFile);
        } else {
            switch (state) {
                case STATE_BEFORE_PARAMETER_NAME:
                {
                    //Skipping spaces, tabs and newlines
                    if ((lchar != ' ') && (lchar != '\t') && (lchar != '\n') && (lchar != '\r')) {
                        state = STATE_WHILE_PARAMETER_NAME;
                        parameterName[parameterNamePos] = lchar;
                        parameterNamePos++;
                    }
                }
                    break;
                case STATE_WHILE_PARAMETER_NAME:
                {
                    if ((lchar != ' ') && (lchar != '\t') && (lchar != '\n') && (lchar != '\r')) {
                        parameterName[parameterNamePos] = lchar;
                        parameterNamePos++;
                    } else {
                        state = STATE_BEFORE_PARAMETER_VALUE;
                        parameterName[parameterNamePos] = '\0';
                    }
                }
                    break;
                case STATE_BEFORE_PARAMETER_VALUE:
                {
                    //Skipping spaces, tabs and newlines
                    if ((lchar != ' ') && (lchar != '\t') && (lchar != '\n') && (lchar != '\r')) {
                        state = STATE_WHILE_PARAMETER_VALUE;
                        parameterValue[parameterValuePos] = lchar;
                        parameterValuePos++;
                    }
                }
                    break;
                case STATE_WHILE_PARAMETER_VALUE:
                {
                    if ((lchar != '\n') && (lchar != '\r')) {
                        parameterValue[parameterValuePos] = lchar;
                        parameterValuePos++;
                    } else {
                        state = STATE_BEFORE_PARAMETER_NAME;
                        parameterValue[parameterValuePos] = '\0';

                        if (setParameter(config, parameterName, parameterValue) != 0) {
                            //Parameter does not exit
                            return 1;
                        }

                        //Reset buffers
                        parameterNamePos = 0;
                        parameterValuePos = 0;
                    }
                }
                    break;
            }
            ilchar = fgetc(configFile);
        }
    }

    fclose(configFile);
    return 0;
}

/**
 * Allocate the size of the source and copy the source to destination
 * This function won't free the destination, so you have to do it before
 * Also, remember to free the destination when you don't need it anymore
 * 
 * @param destination   where to copy the value
 * @param value         where to copy from
 * @return              0 if success, 1 if there was a problem allocating
 * the space
 */
unsigned char stringSafeCopy(char** destination, char* value) {
    *destination = NULL;
    *destination = malloc(strlen(value)+1);
    
    if (*destination == NULL) {
        syslog(LOG_ERR, "Error allocating memory for configuration string.");
        return 1;
    }
    strcpy(*destination, value);
    return 0;
}

/**
 * Search and set a parameter in the configuration, by name, while doing some
 * very basic validation on some parameters
 * 
 * @param config    the struct where to set the parameter
 * @param name      the name of the parameter
 * @param value     the value to set
 * 
 * @return 0 if the parameter was found and set, 1 if the parameter was not
 * found
 */
unsigned char setParameter(struct Config* config, char* name, char* value) {
    if (strcmp(VIDEO_SAVE_DIRECTORY, name) == 0) {
        return stringSafeCopy(&config->VideoSaveDirectory, value);
    } else if (strcmp(THUMBNAIL_SAVE_DIRECTORY, name) == 0) {
        return stringSafeCopy(&config->ThumbnailSaveDirectory, value);
    } else if (strcmp(FFMPEG_PATH, name) == 0) {
        return stringSafeCopy(&config->ffmpegPath, value);
    } else if (strcmp(RASPIVID_ROTATION, name) == 0) {
        if (
                (strcmp(value, "0") == 0) ||
                (strcmp(value, "90") == 0) ||
                (strcmp(value, "180") == 0) ||
                (strcmp(value, "270") == 0)
            ) {
            return stringSafeCopy(&config->RaspividRotation, value);
        }
        syslog(LOG_ERR, "RaspividRotation must be 0, 90, 180 or 270.");
        return 1;
    } else if (strcmp(RASPIVID_WIDTH, name) == 0) {
        return stringSafeCopy(&config->RaspividWidth, value);
    } else if (strcmp(RASPIVID_HEIGHT, name) == 0) {
        return stringSafeCopy(&config->RaspividHeight, value);
    } else if (strcmp(RASPIVID_CRF, name) == 0) {
        return stringSafeCopy(&config->RaspividCRF, value);
    } else if (strcmp(RASPIVID_PREVIEW, name) == 0) {
        return stringSafeCopy(&config->RaspividPreview, value);
    } else if (strcmp(RASPIVID_FRAMERATE, name) == 0) {
        return stringSafeCopy(&config->RaspividFramerate, value);
    } else if (strcmp(RASPIVID_SEGMENT_DURATION, name) == 0) {
        return stringSafeCopy(&config->RaspividSegmentDuration, value);
    } else if (strcmp(RASPIVID_INTRAFRAME_INTERVAL, name) == 0) {
        return stringSafeCopy(&config->RaspividIntraframeInterval, value);
        /*
        if ( (config->RaspividSegmentDuration != NULL) && (config->RaspividFramerate != NULL) ) {
            int segmentDuration = strtol(config->RaspividSegmentDuration, NULL, 10);
            int framerate = strtol(config->RaspividFramerate, NULL, 10);
            int iframeInterval = strtol(value, NULL, 10);
            if (iframeInterval > (segmentDuration * framerate)) {
                syslog(LOG_WARNING, "RaspividIntraframeInterval %d is too high. Using %d.", iframeInterval, segmentDuration * framerate);
                iframeInterval = segmentDuration * framerate;
                char temp[32];
                snprintf(temp, 32, "%d", iframeInterval);
                return stringSafeCopy(&config->RaspividIntraframeInterval, temp);
            }
        } else {
            syslog(LOG_ERR, "RaspividFramerate and RaspividSegmentDuration must be set before RaspividIntraframeInterval.");
            return 1;
        }
        */ 
        return stringSafeCopy(&config->RaspividIntraframeInterval, value);
    } else if (strcmp(RASPIVID_EXPOSURE, name) == 0) {
        return stringSafeCopy(&config->RaspividExposure, value);
    } else if (strcmp(RASPIVID_WHITEBLANCE, name) == 0) {
        return stringSafeCopy(&config->RaspividAWB, value);
    } else if (strcmp(RASPIVID_METERING, name) == 0) {
        return stringSafeCopy(&config->RaspividMetering, value);
    } else if (strcmp(RASPIVID_PROFILE, name) == 0) {
        return stringSafeCopy(&config->RaspividProfile, value);
    } else if (strcmp(THUMBNAIL_WIDTH, name) == 0) {
        return stringSafeCopy(&config->ThumbnailWidth, value);
    } else if (strcmp(THUMBNAIL_FORMAT, name) == 0) {
        return stringSafeCopy(&config->ThumbnailFormat, value);
    } else if (strcmp(THUMBNAIL_OPTIONS, name) == 0) {
        return stringSafeCopy(&config->ThumbnailOptions, value);
    } else if (strcmp(MINIMUM_FREE_DISK_SPACE, name) == 0) {
        config->MinimumFreeDiskSpace = strtoul(value, NULL, 10);
        if (config->MinimumFreeDiskSpace == 0) {
            syslog(LOG_ERR, "Error parsing MinimumFreeDiskSpace.");
            return 1;
        }
        return 0;
    } else if (strcmp(TEMPORARY_DIRECTORY, name) == 0) {
        return stringSafeCopy(&config->TemporaryDirectory, value);
    } else if (strcmp(NUMBER_TEMPORARY_RAW_FILES, name) == 0) {
        config->NumberTemporaryRawFiles = (unsigned char)strtoul(value, NULL, 10);
        if ( (config->NumberTemporaryRawFiles < 2) || (config->NumberTemporaryRawFiles > 9) ) {
            syslog(LOG_ERR, "Invalid NumberTemporaryRawFiles (%d): must be between 2 and 9.");
            return 1;
        }
        return 0;
    } else {
        syslog(LOG_ERR, "Invalid parameter %s.", name);
        return 1;
    }
}

/**
 * Skip until the next line
 * 
 * @param configFile the file where characters should be skipped
 * @return the character after the next '\n' or EOF
 */
int skipLine(FILE* configFile) {
    int ilchar;
    do {
        ilchar = fgetc(configFile);
    } while ((ilchar != EOF) && (ilchar != '\n'));
    return fgetc(configFile);
}

/**
 * Load default configurations in a Config struct
 * 
 * @param config the struct where default values should be set
 */
void setDefault(struct Config* config) {
    config->VideoSaveDirectory = NULL;
    config->ThumbnailSaveDirectory = NULL;
    config->ffmpegPath = NULL;
    config->RaspividRotation = NULL;
    config->RaspividWidth = NULL;
    config->RaspividHeight = NULL;
    config->RaspividCRF = NULL;
    config->RaspividPreview = NULL;
    config->RaspividFramerate = NULL;
    config->RaspividSegmentDuration = NULL;
    config->RaspividIntraframeInterval = NULL;
    config->RaspividExposure = NULL;
    config->RaspividAWB = NULL;
    config->RaspividMetering = NULL;
    config->RaspividProfile = NULL;
    config->ThumbnailWidth = NULL;
    config->ThumbnailFormat = NULL;
    config->ThumbnailOptions = NULL;
    config->TemporaryDirectory = NULL;
    stringSafeCopy(&config->RaspividRotation, DEFAULT_RASPIVID_ROTATION);
    stringSafeCopy(&config->RaspividWidth, DEFAULT_RASPIVID_WIDTH);
    stringSafeCopy(&config->RaspividHeight, DEFAULT_RASPIVID_HEIGHT);
    stringSafeCopy(&config->RaspividCRF, DEFAULT_RASPIVID_CRF);
    stringSafeCopy(&config->RaspividPreview, DEFAULT_RASPIVID_PREVIEW);
    stringSafeCopy(&config->RaspividFramerate, DEFAULT_RASPIVID_FRAMERATE);
    stringSafeCopy(&config->RaspividSegmentDuration, DEFAULT_RASPIVID_SEGMENT_DURATION);
    stringSafeCopy(&config->RaspividIntraframeInterval, DEFAULT_RASPIVID_INTRAFRAME_INTERVAL);
    stringSafeCopy(&config->RaspividExposure, DEFAULT_RASPIVID_EXPOSURE);
    stringSafeCopy(&config->RaspividAWB, DEFAULT_RASPIVID_WHITEBLANCE);
    stringSafeCopy(&config->RaspividMetering, DEFAULT_RASPIVID_METERING);
    stringSafeCopy(&config->RaspividProfile, DEFAULT_RASPIVID_PROFILE);
    stringSafeCopy(&config->ThumbnailWidth, DEFAULT_THUMBNAIL_WIDTH);
    stringSafeCopy(&config->ThumbnailFormat, DEFAULT_THUMBNAIL_FORMAT);
    stringSafeCopy(&config->ThumbnailOptions, DEFAULT_THUMBNAIL_OPTIONS);
    config->MinimumFreeDiskSpace = DEFAULT_MINIMUM_FREE_DISK_SPACE;
    stringSafeCopy(&config->TemporaryDirectory, DEFAULT_TEMPORARY_DIRECTORY);
    config->NumberTemporaryRawFiles = DEFAULT_NUMBER_TEMPORARY_RAW_FILES;
}

/**
 * Free the memory alocated by a config struct and set default values for
 * non-string parameters
 * 
 * @param config the config that should be freed
 */
void freeConfig(struct Config* config) {
    free(config->VideoSaveDirectory);
    free(config->ThumbnailSaveDirectory);
    free(config->ffmpegPath);
    free(config->RaspividRotation);
    free(config->RaspividWidth);
    free(config->RaspividHeight);
    free(config->RaspividCRF);
    free(config->RaspividPreview);
    free(config->RaspividFramerate);
    free(config->RaspividSegmentDuration);
    free(config->RaspividIntraframeInterval);
    free(config->RaspividExposure);
    free(config->RaspividAWB);
    free(config->RaspividMetering);
    free(config->RaspividProfile);
    free(config->ThumbnailWidth);
    free(config->ThumbnailFormat);
    free(config->ThumbnailOptions);
    config->MinimumFreeDiskSpace = DEFAULT_MINIMUM_FREE_DISK_SPACE;
    free(config->TemporaryDirectory);
    config->NumberTemporaryRawFiles = DEFAULT_NUMBER_TEMPORARY_RAW_FILES;
}

/**
 * Checks if all the values of the config are set
 * 
 * @param the config struct to be checked
 * @return 0 if all are set, 1 if not.
 */
unsigned char checkConfig(struct Config* config) {
    if(config->VideoSaveDirectory == NULL) {
        return 1;
    }
    if(config->ThumbnailSaveDirectory == NULL) {
        return 1;
    }
    if(config->ffmpegPath == NULL) {
        return 1;
    }
    if(config->RaspividRotation == NULL) {
        return 1;
    }
    if(config->RaspividWidth == NULL) {
        return 1;
    }
    if(config->RaspividHeight == NULL) {
        return 1;
    }
    if(config->RaspividCRF == NULL) {
        return 1;
    }
    if(config->RaspividPreview == NULL) {
        return 1;
    }
    if(config->RaspividFramerate == NULL) {
        return 1;
    }
    if(config->RaspividSegmentDuration == NULL) {
        return 1;
    }
    if(config->RaspividIntraframeInterval == NULL) {
        return 1;
    }
    if(config->RaspividExposure == NULL) {
        return 1;
    }
    if(config->RaspividAWB == NULL) {
        return 1;
    }
    if(config->RaspividMetering == NULL) {
        return 1;
    }
    if(config->RaspividProfile == NULL) {
        return 1;
    }
    if(config->ThumbnailWidth == NULL) {
        return 1;
    }
    if(config->ThumbnailFormat == NULL) {
        return 1;
    }
    if(config->ThumbnailOptions == NULL) {
        return 1;
    }
    if(config->MinimumFreeDiskSpace == 0) {
        return 1;
    }
    if(config->TemporaryDirectory == NULL) {
        return 1;
    }
    if(config->NumberTemporaryRawFiles == 0) {
        return 1;
    }
    return 0;
}

/**
 * Get the amount of args for raspivid
 * 
 *@param where to search for the args
 */
unsigned int getNumberOfRaspividArgs(struct Config* config) {
    //bin                       1
    //mode                      2
    //annotate                  4
    //annotateex                2
    //bitrate                   2
    //output                    2
    //timeout                   2
    //inline                    1
    //SegmentDuration           2
    //IntraframeInterval        2
    //NumberTemporaryRawFiles   3
    //start                     2
    unsigned int numArgs = 25;
    
    if (config->RaspividRotation != NULL) {
        numArgs += 2;
    }
    if (config->RaspividWidth != NULL) {
        numArgs += 2;
    }
    if (config->RaspividHeight != NULL) {
        numArgs += 2;
    }
    if (config->RaspividCRF != NULL) {
        numArgs += 2;
    }
    if (config->RaspividPreview != NULL) {
        numArgs += 2;
    }
    if (config->RaspividFramerate != NULL) {
        numArgs += 2;
    }
    if (config->RaspividExposure != NULL) {
        numArgs += 2;
    }
    if (config->RaspividAWB != NULL) {
        numArgs += 2;
    }
    if (config->RaspividMetering != NULL) {
        numArgs += 2;
    }
    if (config->RaspividProfile != NULL) {
        numArgs += 2;
    }
    
    return numArgs;
}
