void clock_init(void);
void clock_shutdown(void);
void clock_handler_PIC(void);
unsigned long Clock_Uptime(void);

void Clock_SetHertz(unsigned int Hertz);

#define CLOCK_HERTZ 1000

typedef struct {
	long secondssinceepoch;
	int milliseconds;
} time_t;
