// python_setup.c
// Ashish D'Souza

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "apt.h"
#include "python_setup.h"

int pythonSetup(char *version) {
    if (strcmp(version, "NULL") == 0) return 0;

    if (installPython(version)) return 1;

    return 0;
}

int installPython(char *version) {
    // Download wget
    char *packages[] = {"wget", "gcc", "make", "zlib1g-dev", "libssl-dev"};
    if (installPackages(5, packages)) return 1;

    // Download gzipped Python tarball
    printf("Downloading...\n");
    fflush(stdout);
    unsigned int periods = 0;
    unsigned int versionLength;
    for (versionLength = 0; version[versionLength] != 0; versionLength++) if (version[versionLength] == '.') periods++;

    char newVersion[10];
    strcpy(newVersion, version);
    if (periods == 0) {
        newVersion[versionLength++] = '.';
        newVersion[versionLength++] = '0';
        newVersion[versionLength] = 0;
        periods++;
    }

    if (periods == 1) {
        char command[69 + versionLength * 2];
        sprintf(command, "wget https://www.python.org/ftp/python/%s/Python-%s.tgz > /dev/null 2>&1", newVersion, newVersion);
        if (system(command)) {
            newVersion[versionLength++] = '.';
            newVersion[versionLength++] = '0';
            newVersion[versionLength] = 0;
            periods++;
        }
    }

    if (periods == 2) {
        char command[69 + versionLength * 2];
        sprintf(command, "wget https://www.python.org/ftp/python/%s/Python-%s.tgz > /dev/null 2>&1", newVersion, newVersion);
        if (system(command)) {
            printf("Failed to find Python version %s\n", version);
            return 1;
        }
    }

    // Extract gzipped Python tarball
    printf("Extracting...\n");
    fflush(stdout);
    char command[38 + versionLength];
    sprintf(command, "tar -xzf Python-%s.tgz > /dev/null 2>&1", newVersion);
    if (system(command)) {
        printf("Failed to extract gzipped Python tarball\n");
        return 1;
    }

    // Change working directory
    char pythonBuildDirectory[8 + versionLength];
    sprintf(pythonBuildDirectory, "Python-%s", newVersion);
    chdir(pythonBuildDirectory);

    // Prepare Python build
    printf("Preparing Python build...\n");
    fflush(stdout);
    if (system("./configure --enable-optimizations --with-ensurepip=install > /dev/null 2>&1")) {
        printf("Failed to prepare Python build\n");
        return 1;
    }

    // Build Python with make
    printf("Building Python...\n");
    fflush(stdout);
    if (system("make -j 8 > /dev/null 2>&1")) {
        printf("Failed to build Python\n");
        return 1;
    }

    // Install Python
    printf("Installing Python...\n");
    fflush(stdout);
    if (system("make install > /dev/null 2>&1")) {
        printf("Faild to install Python\n");
        return 1;
    }

    // Upgrade pip
    printf("Upgrading pip...\n");
    fflush(stdout);
    if (upgradePip()) return 1;

    return 0;
}

int upgradePip() {
    if (system("pip3 install --upgrade pip > /dev/null 2>&1")) {
        printf("Failed to upgrade pip\n");
        return 1;
    }

    return 0;
}
