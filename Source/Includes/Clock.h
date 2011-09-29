void Clock_Init(void);
void Clock_Handler(void);
unsigned long Clock_Uptime(void);

#define CLOCK_HERTZ 100

typedef struct {
	long secondssinceepoch;
	int milliseconds;
} time_t;
