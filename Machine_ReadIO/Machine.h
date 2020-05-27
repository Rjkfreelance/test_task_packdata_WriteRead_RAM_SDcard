#include <Arduino.h>
#include <Wire.h>

#define sw1 0x1A //PIN 26
#define sw2 0x19 //PIN 25
#define sw3 0x1B //PIN 27
#define sw4 0x23 //PIN 35
#define sw5 0x22 //PIN 34
#define sw6 0x20 //PIN 32
#define sw7 0x27 //PIN 39
#define sw8 0x24 //PIN 36


#define EEPROM_I2C_ADDRESS 0x50
#define EEPROM_PAGE 127
#define EEPROM_size 32000
#define TOTAL_PAGE 128
#define SEND_TOTALPAGE 10
#define readaddr_eeprom1 32003
#define readaddr_eeprom2 32004
#define writeaddr_eeprom1 32001
#define writeaddr_eeprom2 32002


class Machine
{
  public:

    boolean begin(void);
    int READ_DATASW(int pin);
    String setLEDblink(int pin, float timeon, float timeoff);
    void writeAddress(int address, byte val);
    byte readAddress(int address);

    unsigned long write_page(int addr,uint8_t data[]);
	unsigned long write_pagetcp(int addr,uint8_t data[]);
    String read_all();
	String read_pagetcp(int address);
    int dec2hex(int dec_in);
	int Scan_data_sstv(int addr_write);
	void read_data_mqtt();
	String make_send_string(String datamqtt);
	void Check_senddata_fram(int addr_write);

  private:
      int _pin;
      float _timeon, _timeoff;
      String _response;

      public:

		  int bufread_eeprom1;
		  int bufread_eeprom2;
		  unsigned int read_addeeprom = 0;
		  int addr_Read[10];
		  int flag_input;
		  unsigned long add_next = 0;
          char send_tcphex[128];
          unsigned char DATA_PACKTCP[85];
          unsigned char DATA_PACKFRAM[64];
          unsigned int data_IO;
          char RTC_Buffer[50];
          char Chipid_buf[12];
          uint64_t chipid;
          char v_fw[4];
		  String dataascii;
          char day_buf;
          char dd,mm,yy,hh,mn,ss;
          int i=5;
		  String datainram1;
		  String datainram2;
		  String datainram3;
		  String datainram4;
		  String datainram5;
		  String datainram6;
		  String datainram7;
		  String datainram8;
		  String datainram9;
		  String datainram10;
		  String packet_header = "";

};
