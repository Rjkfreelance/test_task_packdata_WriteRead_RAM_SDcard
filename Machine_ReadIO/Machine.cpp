#include "Machine.h"


boolean Machine::begin(void)
{
  pinMode(sw1, INPUT);
  pinMode(sw2, INPUT);
  pinMode(sw3, INPUT);
  pinMode(sw4, INPUT);
  pinMode(sw5, INPUT);
  pinMode(sw6, INPUT);
  pinMode(sw7, INPUT);
  pinMode(sw8, INPUT);
  Wire.begin();
}

int Machine::READ_DATASW(int pin)
{
	
  _pin = pin;
 
  int flag_input;
  if(_pin == 36 || _pin == 39){
	
	  if(digitalRead(_pin) == 0) flag_input = 0;
	  else flag_input = 1;
  }
  else{
	  if(digitalRead(_pin) == 0) flag_input = 1;
	  else flag_input = 0;
  }

  return flag_input;
}



void Machine::writeAddress(int address, byte val)
{
  // Serial.print(address); Serial.print(":"); Serial.print(val); Serial.print(" ");
  Wire.beginTransmission(EEPROM_I2C_ADDRESS);
  Wire.write((int)(address >> 8));   // MSB
  Wire.write((int)(address & 0xFF)); // LSB


  Wire.write(val);
  Wire.endTransmission();

 delay(5);

}

unsigned long Machine::write_page(int addr,uint8_t data[])
{

  for (int i = 0; i < EEPROM_PAGE; i++)
  {

      writeAddress(addr+i, data[i]);
      //Serial.print(byte(EEPROM.read(addr))); Serial.print(" ");

      //EEPROM.commit();

  }

  add_next = addr + EEPROM_PAGE;
  Serial.print("ADD NEXT : ");Serial.println(add_next);

  return (add_next);
}

unsigned long Machine::write_pagetcp(int addr,uint8_t data[])
{

  for (int i = 0; i < EEPROM_PAGE; i++)
  {

      writeAddress(addr+i, data[i]);

  }


}



byte Machine::readAddress(int address)
{
  byte rData = 0xFF;

  Wire.beginTransmission(EEPROM_I2C_ADDRESS);

  Wire.write((int)(address >> 8));   // MSB
  Wire.write((int)(address & 0xFF)); // LSB
  Wire.endTransmission();


  Wire.requestFrom(EEPROM_I2C_ADDRESS, 1);

  rData =  Wire.read();

  return rData;
}

String Machine::read_all()
{
  String data;
  int totalpack = 128;

  for(int i = 0;i < EEPROM_size; i++)
  {
   // Serial.printf("%d :",i);Serial.print(byte(readAddress(i))); Serial.println(" ");
	if(i == totalpack){
	
		data += ','; 
		totalpack += 128;
	}
	data += (char(readAddress(i)));

  }
	return data;
}


String Machine::read_pagetcp(int address)
{
  //Serial.print("read_pagetcp address :");
  //Serial.println(address);
  char txt[128];
  String stxt;
  for(int i = 0;i <= EEPROM_PAGE; i++)
  {
    //Serial.printf("%d :",i);// Serial.println(" ");
	char text = byte(readAddress(address));
	sprintf(txt,"%c",text);
	stxt += txt;
	address++;
	
	
  }
   

	//Serial.println(" ");
	//Serial.print(stxt);
	return stxt;
}


int Machine::dec2hex(int dec_in)
{
  return( ( (dec_in /10) << 4 ) + (dec_in % 10) ) ;
}


int Machine::Scan_data_sstv(int addr_write)
{	

	bufread_eeprom1 = readAddress(readaddr_eeprom1);
	bufread_eeprom2 = readAddress(readaddr_eeprom2);
	read_addeeprom = (bufread_eeprom2<<8)+bufread_eeprom1;
	
	//Serial.print("Scan_data_sstv read_addeeprom :");
	//	Serial.println(read_addeeprom);

	
	for(int scan_data=0;scan_data<SEND_TOTALPAGE;scan_data++)
	{
		
			addr_Read[scan_data] = read_addeeprom;
		
		//Serial.printf("Scan_data_sstv Addrscan  %d : ", scan_data);
		//Serial.println(addr_Read[scan_data]);
			if(addr_Read[scan_data] >= addr_write)
			{
				addr_Read[scan_data] = 0;

			}
			
			read_addeeprom += TOTAL_PAGE;
	}
	read_data_mqtt();

}


void Machine::read_data_mqtt(){
	//อ่านข้อมูลจาก RAM ใส่ในตัวแปร datainram ถ้าไม่มีให้ใส่ ""
	if(addr_Read[0] >= 0 ) { datainram1 = read_pagetcp(addr_Read[0]); } else { datainram1 = ""; }
	if(addr_Read[1] > 0 ) { datainram2 = read_pagetcp(addr_Read[1]); } else { datainram2 = ""; }
	if(addr_Read[2] > 0 ) { datainram3 = read_pagetcp(addr_Read[2]); } else { datainram3 = ""; }
	if(addr_Read[3] > 0 ) { datainram4 = read_pagetcp(addr_Read[3]); } else { datainram4 = ""; } 
	if(addr_Read[4] > 0 ) { datainram5 = read_pagetcp(addr_Read[4]); } else { datainram5 = ""; }
	if(addr_Read[5] > 0 ) { datainram6 = read_pagetcp(addr_Read[5]); } else { datainram6 = ""; }
	if(addr_Read[6] > 0 ) { datainram7 = read_pagetcp(addr_Read[6]); } else { datainram7 = ""; }
	if(addr_Read[7] > 0 ) { datainram8 = read_pagetcp(addr_Read[7]); } else { datainram8 = ""; }
	if(addr_Read[8] > 0 ) { datainram9 = read_pagetcp(addr_Read[8]); } else { datainram9 = ""; }
	if(addr_Read[9] > 0 ) { datainram10 = read_pagetcp(addr_Read[9]); } else { datainram10 = ""; } 

}




String Machine::make_send_string(String datamqtt) /* send vai tcp  port  */
{
	 String totaldata;
												  /*
	
	 print_header(1);
	 
	//printf("\r\ntermreceive=:%s\r\n",termreceive);      // open by Botun
	 strcpy(buffer_send,termreceive);                    // copy headerread_data_mqtt
	 
	//printf("\r\ns_text_realtime=:%02X",s_text_realtime);   // open by Botun         
	 change_sms(s_text_realtime);                        // copy arry of realtime to s_text_sms
	       // used comma after 1 pack
	 
	 change_sms(s_text_sendgsm_s1);
	 strcat(buffer_send,s_text_sms);                     
	 strcat(buffer_send,",");
	 
	*/


	 totaldata += packet_header;
	 totaldata += datamqtt;
	 totaldata += ',';
	 totaldata += datainram1;
	 totaldata += ',';
	 
	 //strcat(totaldata,packet_header);               
	 //strcat(totaldata,datamqtt);               
	// strcat(totaldata,",");  

	 //strcat(totaldata,datainram1);                     
	// strcat(totaldata,",");
 
	//Pack ข้อมูลใส่ตัวแปร และนับจำนวน ของDATA  ใน pack
	  if(addr_Read[1]>addr_Read[0]) // has address start from 1
	  { 
		totaldata += datainram2;
		totaldata += ',';
	  }
	  
	  if(addr_Read[2]>addr_Read[1]) // has address start from 1
	  {
		totaldata += datainram3;
		totaldata += ',';
	  }
			   
	  if(addr_Read[3]>addr_Read[2]) // has address start from 1
	  {  
	    totaldata += datainram4;
		totaldata += ',';
	  }
			  
	  if(addr_Read[4]>addr_Read[3])// has address start from 1
	  {
	    totaldata += datainram5;
		totaldata += ',';
	  }
	   if(addr_Read[5]>addr_Read[4])// has address start from 1
	  {
		totaldata += datainram6;
		totaldata += ',';
	  }
	   if(addr_Read[6]>addr_Read[5])// has address start from 1
	  {
		totaldata += datainram7;
		totaldata += ',';
	  }
	   if(addr_Read[7]>addr_Read[6])// has address start from 1
	  {
		totaldata += datainram8;
		totaldata += ',';
	  }
	  if(addr_Read[8]>addr_Read[7])// has address start from 1
	  {
		totaldata += datainram9;
		totaldata += ',';
	  }
	  if(addr_Read[9]>addr_Read[8])// has address start from 1
	  {
		totaldata += datainram10;
		
	  }
	 //totaldata += '1T';

	
	  if(addr_Read[1] <= addr_Read[0]) { totaldata += "1$";}
	  else if(addr_Read[2]<=addr_Read[1]) {totaldata += "2$";}
	  else if(addr_Read[3]<=addr_Read[2]) {totaldata += "3$";}
	  else if(addr_Read[4]<=addr_Read[3]) {totaldata += "4$";}
	  else if(addr_Read[5]<=addr_Read[4]) {totaldata += "5$";}
	  else if(addr_Read[6]<=addr_Read[5]) {totaldata += "6$";}
	  else if(addr_Read[7]<=addr_Read[6]) {totaldata += "7$" ;}
	  else if(addr_Read[8]<=addr_Read[7]) {totaldata += "8$";}
	  else if(addr_Read[9]<=addr_Read[8]) {totaldata += "9$";}
	  else               { totaldata += "10$";}
	 Serial.print("DATAPACK :");
	 Serial.println(totaldata);


	// printf_st(buffer_send);                             // rs232 print 
	 
	 return totaldata;
}

void Machine::Check_senddata_fram(int addr_write){

	//เช็คสถานะว่าส่งเสร็จ 
	//Serial.print("Check_senddata_fram :");
	//Serial.println(addr_write);
	//check 
	  if(addr_Read[0] >= 0) { 
		read_addeeprom = addr_Read[0]+TOTAL_PAGE;
	  }
	  if(addr_Read[1] > addr_Read[0]) { 
		read_addeeprom = addr_Read[1]+TOTAL_PAGE;
	  }
	  if(addr_Read[2] > addr_Read[1]) {
		read_addeeprom = addr_Read[2]+TOTAL_PAGE;
	  }
	  if(addr_Read[3] > addr_Read[2]) {
		read_addeeprom = addr_Read[3]+TOTAL_PAGE;
	  }
	  if(addr_Read[4] > addr_Read[3]) {
		read_addeeprom = addr_Read[4]+TOTAL_PAGE;
	  }
	  if(addr_Read[5] > addr_Read[4]) {
		read_addeeprom = addr_Read[5]+TOTAL_PAGE;
	  }
	  if(addr_Read[6] > addr_Read[5]) {
		read_addeeprom = addr_Read[6]+TOTAL_PAGE;
	  }
	  if(addr_Read[7] > addr_Read[6]) {
		read_addeeprom = addr_Read[7]+TOTAL_PAGE;
	  }	
	  if(addr_Read[8] > addr_Read[7]) {
		read_addeeprom = addr_Read[8]+TOTAL_PAGE;
	  }
	  if(addr_Read[9] > addr_Read[8]) {
		read_addeeprom = addr_Read[9]+TOTAL_PAGE;	
	  }

	  if(read_addeeprom >= addr_write){

		writeAddress(writeaddr_eeprom1,0);
		writeAddress(writeaddr_eeprom2,0);
		writeAddress(readaddr_eeprom1,0);
		writeAddress(readaddr_eeprom2,0);
	  }
	writeAddress(readaddr_eeprom1,read_addeeprom&0xFF);
	writeAddress(readaddr_eeprom2,(read_addeeprom>>8)&0xFF);
	//bufread_eeprom1 = readAddress(readaddr_eeprom1);
	//bufread_eeprom2 = readAddress(readaddr_eeprom2);
	//read_addeeprom = (bufread_eeprom2<<8)+bufread_eeprom1;
}
