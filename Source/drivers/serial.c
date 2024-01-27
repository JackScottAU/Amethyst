
#include <interrupts.h>
#include <memoryManager.h>
#include <portIO.h>
#include <vgaConsole.h>
#include <serial.h>

#define PORT 0x3f8          // COM1

void serial_registerHandler(void);
 
int serial_init() {
   portIO_write8(PORT + 1, 0x00);    // Disable all interrupts
   portIO_write8(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
   portIO_write8(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
   portIO_write8(PORT + 1, 0x00);    //                  (hi byte)
   portIO_write8(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
   portIO_write8(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   portIO_write8(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
   portIO_write8(PORT + 4, 0x1E);    // Set in loopback mode, test the serial chip
   portIO_write8(PORT + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)
 
   // Check if serial is faulty (i.e: not same byte as sent)
   if(portIO_read8(PORT + 0) != 0xAE) {
      return 1;
   }

   portIO_write8(PORT + 1, 0x01);    // Enable data interrupts

   serial_registerHandler();
 
   // If serial is not faulty set it in normal operation mode
   // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
   portIO_write8(PORT + 4, 0x0F);
   return 0;
}

char* serial_buffer;
uint32 serial_bufferLocation;
int serial_received(void);

void serial_interruptHandler(uint32 eventData);

void serial_interruptHandler(uint32 eventData) {
    eventData++;

    vgaConsole_printf("Serial: ");

    serial_buffer[serial_bufferLocation] = portIO_read8(PORT);

        vgaConsole_putChar(serial_buffer[serial_bufferLocation]);
        serial_writeByte(serial_buffer[serial_bufferLocation]);

        serial_bufferLocation++;

        vgaConsole_printf("\n");
    
}

void serial_registerHandler() {
    interrupts_addHandler(0x24,0,(*serial_interruptHandler));

    serial_buffer = memoryManager_allocate(sizeof(uint8) * 1024);
    serial_bufferLocation = 0;
}

int serial_received() {
   return portIO_read8(PORT + 5) & 1;
}
 
char serial_readByte() {
   while (serial_received() == 0);
 
   return portIO_read8(PORT);
}

int is_transmit_empty(void);
int is_transmit_empty() {
   return portIO_read8(PORT + 5) & 0x20;
}

void serial_writeByte(char a) {
   while (is_transmit_empty() == 0);
 
   portIO_write8(PORT,a);
}
