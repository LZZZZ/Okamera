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
 * Created on July 15, 2016, 9:10 PM
 */

#ifndef CONFIG_H
#define CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

//#define CONFIG_FILE_PATH                        "G:\\lzz\\Okamera2\\okamera_config"
#define CONFIG_FILE_PATH                        "/etc/okamera/okamera_config"

    //Configuration Parameter Names
#define VIDEO_SAVE_DIRECTORY                    "VideoSaveDirectory"
#define THUMBNAIL_SAVE_DIRECTORY                "ThumbnailSaveDirectory"
#define FFMPEG_PATH                             "ffmpegPath"
#define RASPIVID_ROTATION                       "RaspividRotation"
#define RASPIVID_WIDTH                          "RaspividWidth"
#define RASPIVID_HEIGHT                         "RaspividHeight"
#define RASPIVID_CRF                            "RaspividCRF"
#define RASPIVID_PREVIEW                        "RaspividPreview"
#define RASPIVID_FRAMERATE                      "RaspividFramerate"
#define RASPIVID_SEGMENT_DURATION               "RaspividSegmentDuration"
#define RASPIVID_INTRAFRAME_INTERVAL            "RaspividIntraframeInterval"
#define RASPIVID_PROFILE                        "RaspividProfile"
#define THUMBNAIL_WIDTH                         "ThumbnailWidth"
#define THUMBNAIL_FORMAT                        "ThumbnailFormat"
#define THUMBNAIL_OPTIONS                       "ThumbnailOptions"
#define MINIMUM_FREE_DISK_SPACE                 "MinimumFreeDiskSpace"
#define TEMPORARY_DIRECTORY                     "TemporaryDirectory"
#define NUMBER_TEMPORARY_RAW_FILES              "NumberTemporaryRawFiles"

    //Default configuration values
#define DEFAULT_RASPIVID_ROTATION               "0"
#define DEFAULT_RASPIVID_WIDTH                  "1296"
#define DEFAULT_RASPIVID_HEIGHT                 "968"
#define DEFAULT_RASPIVID_CRF                    "28"
#define DEFAULT_RASPIVID_PREVIEW                ""
#define DEFAULT_RASPIVID_FRAMERATE              "6"
#define DEFAULT_RASPIVID_SEGMENT_DURATION       "60"
#define DEFAULT_RASPIVID_INTRAFRAME_INTERVAL    "180"
#define DEFAULT_RASPIVID_PROFILE                "high"
#define DEFAULT_THUMBNAIL_WIDTH                 "400"
#define DEFAULT_THUMBNAIL_FORMAT                "webp"
#define DEFAULT_THUMBNAIL_OPTIONS               "-compression_level 6 -q:v 85 -preset photo"
#define DEFAULT_MINIMUM_FREE_DISK_SPACE         262144 //256MiB
#define DEFAULT_TEMPORARY_DIRECTORY             "/dev/shm/okamera"
#define DEFAULT_NUMBER_TEMPORARY_RAW_FILES      3

    //Parser states
#define STATE_BEFORE_PARAMETER_NAME             0
#define STATE_WHILE_PARAMETER_NAME              1
#define STATE_BEFORE_PARAMETER_VALUE            2
#define STATE_WHILE_PARAMETER_VALUE             3

    /**
     * Store the Okamera configuration
     */
    struct Config {
        char* VideoSaveDirectory;
        char* ThumbnailSaveDirectory;
        char* ffmpegPath;
        char* RaspividRotation;
        char* RaspividWidth;
        char* RaspividHeight;
        char* RaspividCRF;
        char* RaspividPreview;
        char* RaspividFramerate;
        char* RaspividSegmentDuration;
        char* RaspividIntraframeInterval;
        char* RaspividProfile;
        char* ThumbnailWidth;
        char* ThumbnailFormat;
        char* ThumbnailOptions;
        unsigned long MinimumFreeDiskSpace;
        char* TemporaryDirectory;
        unsigned char NumberTemporaryRawFiles;
    };

    /**
     * Parse the configuration file and return it's data though the parameter.
     * 
     * @param config where to read the configuration to
     * @return 0 if configuration is valid, 1 if there's an error
     */
    unsigned char parseConfigFile(struct Config* config);

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
    unsigned char setParameter(struct Config* config, char* name, char* value);

    /**
     * Skip until the next line
     * 
     * @param configFile the file where characters should be skipped
     * @return the character after the next '\n' or EOF
     */
    int skipLine(FILE* configFile);

    /**
     * Load default configurations in a Config struct
     * 
     * @param config the struct where default values should be set
     */
    void setDefault(struct Config* config);

    /**
     * Free the memory alocated by a config struct and set default values for
     * non-string parameters
     * 
     * @param config the config that should be freed
     */
    void freeConfig(struct Config* config);
    
    /**
     * Checks if all the values of the config are set
     * 
     * @param the config struct to be checked
     * @return 0 if all are set, 1 if not.
     */
    unsigned char checkConfig(struct Config* config);
    
    /**
     * Free the destination pointer, allocate the size of the source and copy
     * the source to destination
     * Remember to free the destination before closing the program
     * 
     * @param destination   where to copy the value
     * @param value         where to copy from
     * @return              0 if success, 1 if there was a problem allocating
     * the space
     */
    unsigned char stringSafeCopy(char** destination, char* value);
#ifdef __cplusplus
    }
#endif

#endif /* CONFIG_H */

