//
// Created by chloridecull on 10/18/15.
//

#ifndef MEDIACOURIER_DBGUTILS_HPP
#define MEDIACOURIER_DBGUTILS_HPP
#include <iostream>

#ifdef DEBUG
#define DBGPRINT(x) std::cerr << "DEBUG: " << x << std::endl; std::cout.flush()
#define SPECPRINT(tool, x) std::cerr << tool << ": " << x << std::endl; std::cout.flush()
#else
#define DBGPRINT(x)
#define SPECPRINT(tool, x)
#endif

#endif //MEDIACOURIER_DBGUTILS_HPP
