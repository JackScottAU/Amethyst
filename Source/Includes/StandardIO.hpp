/**
 *  Amethyst Operating System - Stream I/O definition.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#ifndef INCLUDES_STANDARDIO_HPP_
#define INCLUDES_STANDARDIO_HPP_

#include <Types.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @brief Represents the standard I/O streams and provides access to them.
class StandardIO {
 public:
    /// @brief Creates a new StandardIO instance using the specified stdin and stdout character devices.
    /// @param stdout A function that takes a single character.
    /// @param stdin A function that returns a single character.
    StandardIO(void (*stdout)(char), char (*stdin)(void));

    /// @brief Formatted printing.
    /// @param formatString 
    /// @param  
    void Print(const char* formatString, ...);

    char* ReadLine(bool echo);
    
    void (*stdout)(char);
    
 private:
    char (*stdin)(void);
};

#ifdef __cplusplus
}
#endif

#endif  // INCLUDES_STANDARDIO_HPP_
