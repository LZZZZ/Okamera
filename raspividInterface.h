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
    
#include "config.h"

#define RASPIVID_BIN_PATH               "/opt/vc/bin/raspivid"
    
#define RASPIVID_PARAMETER_PREVIEW      "--preview"
#define RASPIVID_PARAMETER_FULLSCREEN   "--fullscreen"
#define RASPIVID_PARAMETER_NOPREVIEW    "--nopreview"
#define RASPIVID_PARAMETER_OPACITY      "--opacity"
#define RASPIVID_PARAMETER_SHARPNESS    "--sharpness"
#define RASPIVID_PARAMETER_CONTRAST     "--contrast"
#define RASPIVID_PARAMETER_BRIGHTNESS   "--brightness"
#define RASPIVID_PARAMETER_SATURATION   "--saturation"
#define RASPIVID_PARAMETER_ISO          "--ISO"
#define RASPIVID_PARAMETER_VSTAB        "--vstab"
#define RASPIVID_PARAMETER_EV           "--ev"
#define RASPIVID_PARAMETER_EXPOSURE     "--exposure"
#define RASPIVID_PARAMETER_AWB          "--awb"
#define RASPIVID_PARAMETER_IMXFX        "--imxfx"
#define RASPIVID_PARAMETER_COLFX        "--colfx"
#define RASPIVID_PARAMETER_METERING     "--metering"
#define RASPIVID_PARAMETER_ROTATION     "--rotation"
#define RASPIVID_PARAMETER_HFLIP        "--hflip"
#define RASPIVID_PARAMETER_VFLIP        "--vflip"
#define RASPIVID_PARAMETER_ROI          "--roi"
#define RASPIVID_PARAMETER_SHUTTER      "--shutter"
#define RASPIVID_PARAMETER_DRC          "--drc"
#define RASPIVID_PARAMETER_STATS        "--stats"
#define RASPIVID_PARAMETER_AWBGAINS     "--awbgains"
#define RASPIVID_PARAMETER_MODE         "--mode"
#define RASPIVID_PARAMETER_CAMSELECT    "--camselect"
#define RASPIVID_PARAMETER_ANNOTATE     "--annotate"
#define RASPIVID_PARAMETER_ANNOTATEEX   "--annotateex"
#define RASPIVID_PARAMETER_WIDTH        "--width"
#define RASPIVID_PARAMETER_HEIGHT       "--height"
#define RASPIVID_PARAMETER_BITRATE      "--bitrate"
#define RASPIVID_PARAMETER_OUTPUT       "--output"
#define RASPIVID_PARAMETER_VERBOSE      "--verbose"
#define RASPIVID_PARAMETER_TIMEOUT      "--timeout"
#define RASPIVID_PARAMETER_DEMO         "--demo"
#define RASPIVID_PARAMETER_FRAMERATE    "--framerate"
#define RASPIVID_PARAMETER_PENC         "--penc"
#define RASPIVID_PARAMETER_INTRA        "--intra"
#define RASPIVID_PARAMETER_CRF          "--qp"
#define RASPIVID_PARAMETER_PROFILE      "--profile"
#define RASPIVID_PARAMETER_INLINE       "--inline"
#define RASPIVID_PARAMETER_TIMED        "--timed"
#define RASPIVID_PARAMETER_KEYPRESS     "--keypress"
#define RASPIVID_PARAMETER_SIGNAL       "--signal"
#define RASPIVID_PARAMETER_INITIAL      "--initial"
#define RASPIVID_PARAMETER_SEGMENT      "--segment"
#define RASPIVID_PARAMETER_WRAP         "--wrap"
#define RASPIVID_PARAMETER_START        "--start"
    
    /**
     * Starts the raspivid camera in segment mode with the options defined in
     * config
     * 
     * @param config config struct with raspivid options
     * 
     * @return the pid of the child thread
     */
    pid_t startCamera(struct Config* config);

#ifdef __cplusplus
}
#endif

#endif /* RASPIVIDINTERFACE_H */

