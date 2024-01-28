
#include <interrupts.h>
#include <memoryManager.h>
#include <portIO.h>
#include <vgaConsole.h>
#include <serial.h>

#define SERIAL_CLOCKSPEED 115200

void serial_registerHandler(void);
int serial_testLoopBack(uint16 baseAddress);

/**
 * 0 if there is a device. any other number is no device.
*/
int serial_detect(uint16 baseAddress) {
    portIO_write8(baseAddress + 7, 0xAE);

    uint8 value = portIO_read8(baseAddress + 7);

    if(value != 0xAE) {
        return 1;
    }

    if(serial_testLoopBack(baseAddress)) {
        return 1;
    }

    return 0;
}
 
int serial_init(uint16 baseAddress, uint8 divisor) {
    
    portIO_write8(baseAddress + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    portIO_write8(baseAddress + 0, divisor);    // Set divisor to 3 (lo byte) 38400 baud
    portIO_write8(baseAddress + 1, 0x00);    //                  (hi byte)
    portIO_write8(baseAddress + 3, 0x03);    // 8 bits, no parity, one stop bit (disabled DLAB)
    portIO_write8(baseAddress + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    portIO_write8(baseAddress + 4, 0x0B);    // IRQs enabled, RTS/DSR set
    portIO_write8(baseAddress + 4, 0x0F);   // Data Terminal Ready, Request To Send, Out 1, Out 2 all enabled.
    portIO_write8(baseAddress + 1, 0x01);   // Enable data interrupts

    if(serial_testLoopBack(baseAddress)) {
        return 1;
    }

    serial_registerHandler();
    
    return 0;
}

/**
 * Tests the serial device loopback. Returns 0 on success, any other number is failure. Returns state to how it was before test, if test was successful.
*/
int serial_testLoopBack(uint16 baseAddress) {
    uint8 modemControlRegisterState = portIO_read8(baseAddress + 4);
    uint8 interruptControlRegisterState = portIO_read8(baseAddress + 1);
    

    portIO_write8(baseAddress + 4, 0x1E);    // Set in loopback mode, test the serial chip
    portIO_write8(baseAddress + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)
 
    // Check if serial is faulty (i.e: not same byte as sent)
    if(portIO_read8(baseAddress + 0) != 0xAE) {
        return 1;
    }

    portIO_write8(baseAddress + 4, modemControlRegisterState);
    portIO_write8(baseAddress + 1, interruptControlRegisterState);

    return 0;
}

char* serial_buffer;
uint32 serial_bufferSize;
uint32 serial_bufferReadIndex;
uint32 serial_bufferWriteIndex;

bool serial_echoMode = TRUE;

int serial_received(void);

void serial_interruptHandler(uint32 eventData);

void serial_interruptHandler(uint32 eventData) {

    serial_buffer[serial_bufferWriteIndex] = portIO_read8(SERIAL_COM1);

    if(serial_echoMode) {
        serial_writeChar(serial_buffer[serial_bufferWriteIndex]);
    }

    // vgaConsole_putChar(serial_buffer[serial_bufferWriteIndex]);

    serial_bufferWriteIndex++;

    if(serial_bufferWriteIndex >= serial_bufferSize) {
        serial_bufferWriteIndex = 0;  // Overwriting is better than escaping into memory.
    }
}

void serial_registerHandler() {
    interrupts_addHandler(0x24,0,(*serial_interruptHandler));

    serial_bufferSize = 1024;
    serial_buffer = memoryManager_allocate(sizeof(uint8) * serial_bufferSize);
    serial_bufferWriteIndex = 0;
    serial_bufferReadIndex = 0;

    for(uint32 i = 0; i < serial_bufferSize; i++) {
        serial_buffer[i] = 0;
    }
}

/**
 * Whether the serial read buffer has data. 0 = no data. otherwise returns size of data waiting to be read in bytes.
*/
int serial_canRead() {
    int bufferSize = 0;

    if(serial_bufferWriteIndex >= serial_bufferReadIndex) {
        bufferSize =  serial_bufferWriteIndex - serial_bufferReadIndex;
    } else {
        bufferSize =  serial_bufferSize + serial_bufferWriteIndex - serial_bufferReadIndex;
    }

    return bufferSize;
}

char serial_readChar() {
    // If there's nothing in the buffer, say so.
    if(serial_canRead() == 0) {
        return 0;
    }

    char character = serial_buffer[serial_bufferReadIndex];
    serial_bufferReadIndex++;

    if(serial_bufferReadIndex >= serial_bufferSize) {
        serial_bufferReadIndex = 0;  // Overwriting is better than escaping into memory.
    }

    return character;
}

/**
 * Reads the current buffer contents.
*/
char* serial_readString() {
    int stringSize = serial_canRead();
    char* string = memoryManager_allocate(sizeof(uint8) * (stringSize + 1));

    for(int i = 0; i < stringSize; i++) {
        string[i] = serial_readChar();
    }

    string[stringSize] = 0;

    return string;
}

/** important - blocking! */
char* serial_readLine() {
    bool bufferContainsNewLine = FALSE;

    while(!bufferContainsNewLine) {
        uint32 ourBufferPos = serial_bufferReadIndex;

        while(ourBufferPos != serial_bufferWriteIndex) {
            if(serial_buffer[ourBufferPos] == '\n' || serial_buffer[ourBufferPos] == '\r') {
                bufferContainsNewLine = TRUE;
                break;
            }
            ourBufferPos++;

            if(ourBufferPos >= serial_bufferSize) {
                ourBufferPos = 0;
            }
        }
    }

    // We now have a newline in the buffer!
    int stringSize = serial_canRead(); // bryte force! TODO: change this to use where the newline is and make it potentially smaller.
    char* string = memoryManager_allocate(sizeof(uint8) * (stringSize + 1));

    for(int i = 0; i < stringSize; i++) {
        string[i] = serial_readChar();

        if(string[i] == '\r' || string[i] == '\n') {
            // We've reached the end of the line. But we still have a bit to do.
            // A good terminal should have sent both \r and \n. Some don't. Angry face. We need to send the missing one back, if it is missing.

            string[i+1] = serial_readChar(); // can return zero, may return a \r or a \n. Could in theory return something else, but shouldn't.

            if(string[i] == '\r' && string[i+1] == '\n') {
                // Correct behaviour.
                string[i] = 0;
                return string;
            }
            if(string[i] == '\n' && string[i+1] == '\r') {
                // Correct behaviour.
                string[i] = 0;
                return string;
            }

            if(string[i] == '\r' && string[i+1] != '\n') {
                if(serial_echoMode) {
                    serial_writeChar('\n');
                }

                string[i] = 0;
                return string;
            }

            if(string[i] == '\n' && string[i+1] != '\r') {
                if(serial_echoMode) {
                    serial_writeChar('\r');
                }

                string[i] = 0;
                return string;
            }

            vgaConsole_printf("serial: shouldn't get here.");
        }
    }

    string[stringSize] = 0;

    return string; // we shouldn't get here.
}

int serial_received() {
   return portIO_read8(SERIAL_COM1 + 5) & 1;
}
 
char serial_readByte() {
   while (serial_received() == 0);
 
   return portIO_read8(SERIAL_COM1);
}

int is_transmit_empty(void);
int is_transmit_empty() {
   return portIO_read8(SERIAL_COM1 + 5) & 0x20;
}

void serial_writeChar(char a) {
   while (is_transmit_empty() == 0);
 
   portIO_write8(SERIAL_COM1,a);
}

void serial_writeString(const char* string) {
    int i = 0;

    while(string[i]) {
        serial_writeChar(string[i]);
        i++;
    }
}

void serial_writeLine(const char* string) {
    serial_writeString(string);

    serial_writeChar('\r');
    serial_writeChar('\n');
}
