#include <sys/stat.h>
#include <unistd.h>
#include "../include/utils.h"

int checkDataDir() {
    struct stat st = {0};

    // see if the program is running in the root of the project
    if (stat("CMakeLists.txt", &st) != 0) {
        // if it isn't, check if the upper level is the root of the project
        if (stat("../CMakeLists.txt", &st) == 0) {
            // if it is, change the working dir to the upper level
            if (chdir("..") != 0) {
                // if it occurs an error, return 0
                return 0;
            }
        } else { // if the upper level isn't the root of the project, return 0
            return 0;
        }
    }

    // if the dir data doesn't exist, create or return 0
    if (stat("data", &st) != 0) {
        if (mkdir("data", 0700) == 0) {
            return 1;
        }
        return 0;
    }

    return 1;
}