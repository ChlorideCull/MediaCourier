//
// Created by chloridecull on 2016-01-17.
//

#include <cstring>
#include <iostream>

using namespace std;

void print_usage() {
    cout << "Media Courier Suite" << endl << "Changeling Shim" << endl << endl;
    cout << "This program should only be called by the nginx config, and is purposefully left undocumented." << endl;
    // Just kidding, can't be bothered to document it.
}

int main(int argc, char* argv[]) {
    if (argc < 2)
        print_usage();
    if (strcmp(argv[1], "fcgi")) {
        //TODO: Launch FCGI server
    } else if (strcmp(argv[1], "start-publish")) {
        //TODO: Contact Hive to get ffmpeg arguments, and launch them
    } else if (strcmp(argv[1], "finalize-recording")) {
        //TODO: Compress file in argument visually losslessly, and store
    } else {
        print_usage();
    }
}