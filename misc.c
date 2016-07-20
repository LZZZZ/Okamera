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

#include "misc.h"

/**
 * Gets a pointer to the extension of a filename or file path.
 * "/some/folder/vid0.h264" -> "h264"
 *                    ^
 * @param filename
 * @return the extension, if found, or NULL if not found
 */
const char* getFileExtension(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if (!dot || dot == filename) {
        return NULL;
    }
    return dot + 1;
}

/**
 * @return the amount of free space from the selected directory, in KiB, or 0 if
 * there's an error checking for it.
 */
unsigned long long getFreeSpace(char* directory) {
    struct statfs driveStats;
    if ((statfs(directory, &driveStats)) < 0) {
        return 0;
    } else {
        return driveStats.f_bavail * (driveStats.f_bsize / 1024);
    }
}

/**
 * Redirect the specified descriptor to/from the device/file.
 * Returns 0 if successful (with errno unmodified), errno if error.
 * 
 * http://www.linuxquestions.org/questions/programming-9/system-call-produces-wrong-exit-status-in-daemon-948708/
 */
int redirect(const int descriptor, const char *const device, const int flags) {
    int fd, result, saved_errno;

    //Check for invalid parameters.
    if (descriptor == -1 || !device || !*device) {
        return errno = EINVAL;
    }

    saved_errno = errno;

    //Open the device
    do {
        fd = open(device, flags);
    } while (fd == -1 && errno == EINTR);
    if (fd == -1) {
        return errno;
    }

    //Did we get lucky, and got the desired descriptor?
    if (fd == descriptor) {
        errno = saved_errno;
        return 0;
    }

    //Duplicate to the desired descriptor.
    do {
        result = dup2(fd, descriptor);
    } while (result == -1 && errno == EINTR);
    if (result == -1) {
        saved_errno = errno;
        do {
            result = close(fd);
        } while (result == -1 && errno == EINTR);
        return errno = saved_errno;
    }

    //Close the unneeded descriptor.
    do {
        result = close(fd);
    } while (result == -1 && errno == EINTR);
    //This time we ignore that exit status;
    //the redirection is done, and this was just
    //a temporary descriptor.

    //Return success.
    errno = saved_errno;
    return 0;
}
