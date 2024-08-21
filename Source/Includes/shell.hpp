/**
 *  Amethyst Operating System - Shell.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#include <Types.h>

#ifndef INCLUDES_SHELL_HPP_
#define INCLUDES_SHELL_HPP_

#include <StandardIO.hpp>

#ifdef __cplusplus
extern "C" {
#endif

class Shell {
 public:
    Shell(StandardIO* stdio);

    /// @brief Main shell loop.
    void Main();
 private:
    StandardIO* stdio;
};

#ifdef __cplusplus
}
#endif

#endif  // INCLUDES_SHELL_HPP_
