#include <Types.h>

namespace Time {

    /// @brief Represents an instant of time. Number of ticks (65536ths of a second) since 2000-01-01 in UTC.
    class Instant {
        public:
            Instant();
            
        private:
            sint64 ticks;
    };
};
