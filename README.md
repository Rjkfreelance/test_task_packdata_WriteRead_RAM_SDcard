# test_task_packdata_WriteRead_RAM_SDcard
27/05/2020 version นี้  ทดสอบ ทำ Task Packdata I/O Write Ram read ram มา show
และถ้า Ram เต็ม copy ไปเขียน history ใน SD card และ ลบข้อมูลออกจาก RAM เพื่อบันทึกใหม่ 
ยังไม่มีการส่งข้อมูล MQTT ใดๆ  

ฝาก น๊อต ช่วยเช็คให้หน่อยครับ เนื่องจาก ใช้ Library  Machine.h  ที่น๊อตทำไว้ (ดูว่าการ Pack ช้อมูลต่างๆ
ถูกต้องหรือไม่)

***
ใน void loop(){} ไม่มีการ run Function  Run จาก Task เท่านั้น 

Task Run ทุกๆ 1 วินาที  จากการทดสอบ พบว่า Please wait for read RAM To SDCARD

มีการหยุดรอ Process นี้  คงต้อง แยก Task Write SD card from RAM  เมื่อ ตรวจสอบพบ RAM
เต็ม Task นี้ทำงาน  

![img](https://iotfmx.com/imgtest/sd_1.png)
