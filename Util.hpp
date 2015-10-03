//
// Created by chloridecull on 9/29/15.
//

#ifndef MEDIACOURIER_UTIL_HPP
#define MEDIACOURIER_UTIL_HPP


#include <vector>
#include <unordered_map>

class Util {
    Util() = delete;
    Util(const Util&) = delete;
    ~Util() = delete;

public:
    static std::array<int, 3> LaunchSubprocess(std::string Path, std::vector<std::string> Args,
                                            std::vector<std::pair<std::string, std::string>> Env);
};


#endif //MEDIACOURIER_UTIL_HPP
