//
// Created by chloridecull on 9/29/15.
//

#include <unistd.h>
#include "Util.hpp"

std::array<int, 3> Util::LaunchSubprocess(std::string Path, std::vector<std::string> Args,
                                       std::vector<std::pair<std::string, std::string>> Env) {
    std::array<int, 3> output;
    std::array<int, 3> childside;
    for (int i = 0; i <3; ++i) {
        int pipeout[2];
        pipe(pipeout);
        output[i] = pipeout[0];
        childside[i] = pipeout[1];
    }
    int forkret = fork();
    if (forkret == 0) {
        // We are child, close parent side
        close(output[0]);
        close(output[1]);
        close(output[2]);
        //Redirect stdio
        dup2(childside[0], 0); //stdin
        dup2(childside[1], 1); //stdout
        dup2(childside[2], 2); //stderr
        // Juggle data to make it fit execvpe()
        const char* cargs[Args.size()+1];
        const char* cenv[(Env.size()*2)+1];
        for (int i = 0; i < Args.size(); ++i) {
            cargs[i] = Args[i].c_str();
        }
        cargs[Args.size()] = nullptr;
        for (int j = 0; j < Env.size()*2; j += 2) {
            cenv[j] = Env[j].first.c_str();
            cenv[j+1] = Env[j].second.c_str();
        }
        cenv[Env.size()*2] = nullptr;
        // Execute program
        execvpe(Path.c_str(), (char* const *)cargs, (char* const *)cenv); //Horrible hacky stuff - please slap me if you see me
        // Shouldn't happen - execvpe replaced the process.
        throw std::runtime_error("Failed to exec!");
    } else if (forkret == -1) {
        throw std::runtime_error("Failed to fork!");
    }
    // We are parent, close child side
    close(childside[0]);
    close(childside[1]);
    close(childside[2]);
    // Return parent side
    return output;
}
