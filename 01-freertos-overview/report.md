
## คำถามทบทวหน

1. ไฟล์ใดบ้างที่จำเป็นสำหรับโปรเจกต์ ESP-IDF ขั้นต่ำ?
2. ความแตกต่างระหว่าง `hello_esp32.bin` และ `hello_esp32.elf` คืออะไร?
3. คำสั่ง `idf.py set-target` ทำอะไร?
4. โฟลเดอร์ `build/` มีไฟล์อะไรบ้าง?
5. การใช้ `vTaskDelay()` แทน `delay()` มีความสำคัญอย่างไร?


## Checklist การทำงาน

- [ ] Flash และ Monitor สำเร็จ
- [ ] เห็น Log messages ต่างๆ
- [ ] ทดสอบ Log levels ต่างๆ
- [ ] ใช้ formatted logging ได้
- [ ] ปรับ Log level ผ่าน menuconfig
- [ ] ทำแบบฝึกหัดครบ

## คำถามทบทวน

1. ความแตกต่างระหว่าง `printf()` และ `ESP_LOGI()` คืออะไร?
2. Log level ไหนที่จะแสดงใน default configuration?
3. การใช้ `ESP_ERROR_CHECK()` มีประโยชน์อย่างไร?
4. คำสั่งใดในการออกจาก Monitor mode?
5. การตั้งค่า Log level สำหรับ tag เฉพาะทำอย่างไร?


## 📝 Checklist
- [ ] ตรวจสอบการติดตั้ง ESP-IDF
- [ ] สร้างโปรเจกต์ใหม่สำเร็จ
- [ ] Build และ Flash โปรแกรมสำเร็จ
- [ ] ดู Serial Output ได้
- [ ] สร้าง Task ขยั้นพื้นฐานได้
- [ ] ทำความเข้าใจ Task lifecycle