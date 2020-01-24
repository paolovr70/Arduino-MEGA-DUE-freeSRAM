/*    ARDUINO MEGA2560 / DUE
 *    2020 paolovr70@tiscali.it
 *    FREEMEM MEGA - DUE  
 *    
 *    board codes:
 *    1 - "ATmega2560"  2 - "ARMdueSAM"  3 - "Unknown"   
 * //=================================================
*/

#if defined(ARDUINO_AVR_MEGA2560) // __AVR_ATmega2560__
   const int board = 1;
   extern unsigned int __bss_end;
   extern unsigned int __heap_start;
   extern void *__brkval;
 #elif defined(ARDUINO_SAM_DUE)
   const int board = 2;
   #include <malloc.h>
   extern char _end;
   extern "C" char *sbrk(int i);
  #else
     const int board = 3;
#endif

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.print('\n');
  Serial.print("BOARD_TYPE:");
  Serial.println(board);
}

void loop() {
  uint32_t a = getFreeSram();
  Serial.println(a);
  delay (1000);
}

uint32_t getFreeSram() { 
 #if defined(ARDUINO_SAM_DUE) // ARM_ARDUINO_DUE
   char *ramstart=(char *)0x20070000;
   char *ramend=(char *)0x20088000;
   char *heapend=sbrk(0);
   register char * stack_ptr asm("sp");
   struct mallinfo mi=mallinfo();
   Serial.print("Est. Free RAM(DUE):");
   Serial.println((stack_ptr - heapend + mi.fordblks));
   return (stack_ptr - heapend + mi.fordblks);
 #endif

 #if defined(ARDUINO_AVR_MEGA2560) // AVR_ATmega2560 
   uint8_t newVariable; // heap is empty, use bss as start memory address
   if ((uint16_t)__brkval == 0){
     Serial.print("Est. Free RAM(MEGA):");
     Serial.println((((uint16_t)&newVariable) - ((uint16_t)&__bss_end)));
     return (((uint16_t)&newVariable) - ((uint16_t)&__bss_end));
    }
   else {  // use heap end as the start of the memory address
     Serial.print("Est. Free RAM(MEGA):");
     Serial.println((((uint16_t)&newVariable) - ((uint16_t)__brkval)));
     return (((uint16_t)&newVariable) - ((uint16_t)__brkval));
    }
 #endif
}
