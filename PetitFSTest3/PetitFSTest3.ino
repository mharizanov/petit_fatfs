#include <petit_fatfs.h>

/*
The Petit FatFS:
http://elm-chan.org/fsw/ff/00index_p.html

The Arduino IDE wrapper topic:
http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1251776162


*/

//Attiny84
#define SS_DDR      DDRB
#define SS_PORT     PORTB
#define SS_BIT      1

#define SPI_SS      1     // PB1, pin 3
#define SPI_MISO    4     // PA6, pin 7
#define SPI_MOSI    5     // PA5, pin 8
#define SPI_SCK     6     // PA4, pin 9



byte rx()
{

      // ATtiny
    USIDR = 0xFF;
    byte v1 = bit(USIWM0) | bit(USITC);
    byte v2 = bit(USIWM0) | bit(USITC) | bit(USICLK);
#if F_CPU <= 5000000
    // only unroll if resulting clock stays under 2.5 MHz
    USICR = v1; USICR = v2;
    USICR = v1; USICR = v2;
    USICR = v1; USICR = v2;
    USICR = v1; USICR = v2;
    USICR = v1; USICR = v2;
    USICR = v1; USICR = v2;
    USICR = v1; USICR = v2;
    USICR = v1; USICR = v2;
#else
    for (uint8_t i = 0; i < 8; ++i) {
        USICR = v1;
        USICR = v2;
    }
#endif
    return USIDR;


}

void tx(byte d)
{

      // ATtiny
    USIDR = d;
    byte v1 = bit(USIWM0) | bit(USITC);
    byte v2 = bit(USIWM0) | bit(USITC) | bit(USICLK);
#if F_CPU <= 5000000
    // only unroll if resulting clock stays under 2.5 MHz
    USICR = v1; USICR = v2;
    USICR = v1; USICR = v2;
    USICR = v1; USICR = v2;
    USICR = v1; USICR = v2;
    USICR = v1; USICR = v2;
    USICR = v1; USICR = v2;
    USICR = v1; USICR = v2;
    USICR = v1; USICR = v2;
#else
    for (uint8_t i = 0; i < 8; ++i) {
        USICR = v1;
        USICR = v2;
    }
#endif



}

void spi_init()
{
    bitSet(SS_PORT, SS_BIT);
    bitSet(SS_DDR, SS_BIT);
    digitalWrite(SPI_SS, 1);
    pinMode(SPI_SS, OUTPUT);
    pinMode(SPI_MOSI, OUTPUT);
    pinMode(SPI_MISO, INPUT);
    pinMode(SPI_SCK, OUTPUT);
#ifdef SPCR    
    SPCR = _BV(SPE) | _BV(MSTR);
#if F_CPU > 10000000
    // use clk/2 (2x 1/4th) for sending (and clk/8 for recv, see rf12_xferSlow)
    SPSR |= _BV(SPI2X);
#endif
#else
    // ATtiny
    USICR = bit(USIWM0);
#endif    

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

