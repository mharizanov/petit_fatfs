#include <petit_fatfs.h>


byte rx()
{
        USIDR = 0xFF;
        USISR = _BV(USIOIF);
        do {
                USICR = _BV(USIWM0) | _BV(USICS1) |
                        _BV(USICLK) | _BV(USITC);
        } while ((USISR & _BV(USIOIF)) == 0);
        return USIDR;
}

void tx(byte d)
{
        USIDR = d;
        USISR = _BV(USIOIF);
        do {
                USICR = _BV(USIWM0) | _BV(USICS1) |
                        _BV(USICLK) | _BV(USITC);
        } while ((USISR & _BV(USIOIF)) == 0);
}

void spi_init()
{
/*
 * DON'T use the MOSI/MISO pins. They're for ISP programming only!
 * Read the datasheet.
 */

// USI stuff
  DDRB |= _BV(PB1); // as output (latch)
  DDRA |= _BV(PA5); // as output (DO)
  DDRA |= _BV(PA4); // as output (USISCK)
  DDRA &= ~_BV(PA6); // as input (DI)
  PORTA |= _BV(PA6); // pullup on (DI)
  
 // USICR = bit(USIWM0); //?????????????????????????????
} 



void setup()
{
  DDRA |= _BV(PA3); // set LED pin as output
  PORTA |= _BV(PA3); // turn the LED on
  delay(5000);     
  PORTA ^= _BV(PA3); // toggle LED
	
  char * s = (char*)calloc(65, sizeof(char));
  
  spi_init();
  Serial.begin(9600);
  
  PFFS.begin(1, rx, tx);
  
  fileReadTest(PFFS.open_file("test.txt"), "test.txt");
  
}


void fileReadTest(int err, char * fp)
{
  if (err == 0)
  {  
    Serial.print("Reading File "); Serial.print(fp); Serial.print(" (err = "); Serial.print(err); Serial.println(" == 0) :");
    

      
      
    char * s = (char*)calloc(65, sizeof(char));
    int bytes_read;
    int bytes_cnt = 0;
    
    do
    {
	PFFS.read_file(s, 64, &bytes_read);
	bytes_cnt += bytes_read;

	PORTA ^= _BV(PA3); // toggle LED
	delay(100);     
	PORTA ^= _BV(PA3); // toggle LED
      
	Serial.print(s);
    }
    while (bytes_read == 64);

    free(s);
    
    Serial.println();
    Serial.print("Read "); Serial.print(bytes_cnt); Serial.println(" bytes");
  }
  else
  {
    Serial.print("Error code "); Serial.print(err); Serial.print(" while opening "); Serial.println(fp);
  }
}

void loop()
{
  
} 

