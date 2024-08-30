/**
 *  Amethyst Operating System - Shell.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#include <Types.h>

#ifndef INCLUDES_SHELL_HPP_
#define INCLUDES_SHELL_HPP_

#include <StandardIO.hpp>
#include <Structures/linkedlist.hpp>

#ifdef __cplusplus
extern "C" {
#endif

class ShellCommand {
   public:
      char* command;
      uint32 (* callback)(StandardIO*);
};

class Shell {
 public:
    Shell(StandardIO* stdio);

    /// @brief Main shell loop.
    void Main();

    void RegisterCommand(char* commandString, uint32 (* callback)(StandardIO*));
 private:
    void ProcessLine();
    StandardIO* stdio;
    LinkedList<ShellCommand*>* commands;
};

#ifdef __cplusplus
}
#endif

#endif  // INCLUDES_SHELL_HPP_
