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
 * Created on July 18, 2016, 12:29 AM
 */

#ifndef MISC_H
#define MISC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stddef.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/statfs.h>

    /**
     * Gets a pointer to the extension of a filename or file path.
     * "/some/folder/vid0.h264" -> "h264"
     *                    ^
     * @param filename
     * @return the extension, if found, or NULL if not found
     */
    const char* getFileExtension(const char *filename);

    /**
     * @return the amount of free space from the selected directory, in KiB, or 0 if
     * there's an error checking for it.
     */
    unsigned long long getFreeSpace(char* directory);

    /**
     * Redirect the specified descriptor to/from the device/file.
     * Returns 0 if successful (with errno unmodified), errno if error.
     * 
     * http://www.linuxquestions.org/questions/programming-9/system-call-produces-wrong-exit-status-in-daemon-948708/
     */
    int redirect(const int descriptor, const char *const device, const int flags);

#ifdef __cplusplus
}
#endif

#endif /* MISC_H */

