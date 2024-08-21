/**
 *  Amethyst Operating System - Shell.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#include <Types.h>

#ifndef INCLUDES_SHELL_HPP_
#define INCLUDES_SHELL_HPP_

#ifdef __cplusplus
extern "C" {
#endif

class Shell {
 public:
    /// @brief Initialises the shell.
    /// @param stdout The output stream to use.
    /// @param stdin The input stream to use.
    Shell(void (*stdout)(char), char (*stdin)(void));

    /// @brief Main shell loop.
    void Main();
 private:
    void (*stdout)(char);
    char (*stdin)(void);
};

#ifdef __cplusplus
}
#endif

#endif  // INCLUDES_SHELL_HPP_
