#include <stream.h>
#include <stdarg.h>
#include <Types.h>

void stream_putDecimal(void (*putChar)(char), uint32 arg);
void stream_putHexadecimal(void (*putChar)(char), uint32 arg, uint8 leadingZeroes);
void stream_putHexadecimalInternal(void (*putChar)(char), uint32 arg);

void stream_printf(void (*putChar)(char), const char* formatString, ...) {
	va_list args;
	int i = 0;
	int arg;
    char* sarg;
	
	va_start(args,formatString);

	

	while(formatString[i])
	{
        if(formatString[i] == '\n') {
            // output a carriage return as well.
            putChar('\r'); // the actual \n is done down below.
        }

		if(formatString[i]=='%')
		{
			i++;
			
			if(formatString[i]=='%') {
				putChar('%');
			}

			
			if(formatString[i]=='d')
			{
			    arg = va_arg(args,int);
				stream_putDecimal(putChar, arg);
			}
			if(formatString[i]=='h')
			{
			    arg = va_arg(args,int);
				stream_putHexadecimal(putChar, arg, 0);
			}
			if(formatString[i]=='H')
			{
			    arg = va_arg(args,int);
				stream_putHexadecimal(putChar, arg, 1);
			}

            if(formatString[i] == 's') {
			    sarg = va_arg(args,char*);
                // Write a string.
                while(*sarg) {
                    putChar(*sarg);
                    sarg++;
                }
            }

		} else {
			putChar(formatString[i]);;
		}
		
		i++;
	}

	va_end(args);
}

void stream_putDecimal(void (*putChar)(char), uint32 arg)
{
	if(arg/10 >= 1)
		stream_putDecimal(putChar, arg/10);
	
	putChar((arg%10)+'0');
}

void stream_putHexadecimalInternal(void (*putChar)(char), uint32 arg)
{
	if(arg/16 >= 1)
		stream_putHexadecimalInternal(putChar, arg/16);
	
	if((arg%16)<10)
	{
		putChar('0'+(arg%16));
	} else {
		putChar('A'+((arg%16)-10));
	}
}

void stream_putHexadecimal(void (*putChar)(char), uint32 arg, uint8 leadingZeroes)
{
	stream_printf(putChar, "0x");
	
	if(leadingZeroes)
	{
		int j;

		for(int i=28;i>=0;i-=4)
		{
			j = (arg & (0xF<<i))>>i;

			if(j<10)
			{
				putChar('0'+j);
			} else {
				putChar('A'+(j-10));
			}
		}
	} else {
		stream_putHexadecimalInternal(putChar, arg);
	}
}