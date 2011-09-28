inline unsigned char inportb(unsigned int port)
{
	unsigned char ret;
	asm volatile ("inb %%dx,%%al":"=a" (ret):"d" (port));
	return ret;
};

inline void outportb(unsigned int port, unsigned char value)
{
	asm volatile ("outb %%al,%%dx": :"d" (port), "a" (value));
};
