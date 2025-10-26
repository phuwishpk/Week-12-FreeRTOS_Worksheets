
## Checklist การทำงาน

- [ ] Flash และ Monitor สำเร็จ
- [ ] เห็น Log messages ต่างๆ
- [ ] ทดสอบ Log levels ต่างๆ
- [ ] ใช้ formatted logging ได้
- [ ] ปรับ Log level ผ่าน menuconfig
- [ ] ทำแบบฝึกหัดครบ

## คำถามทบทวน

1.  **ความแตกต่างระหว่าง `printf()` และ `ESP_LOGI()` คืออะไร?**
    * `printf()`: เป็นฟังก์ชันมาตรฐานของภาษา C ใช้สำหรับพิมพ์ข้อความออกทาง standard output (ปกติคือ Serial port ใน ESP32) โดยไม่มีฟีเจอร์เพิ่มเติมเกี่ยวกับการจัดการ log.
    * `ESP_LOGI()`: เป็นมาโครของ ESP-IDF ที่ออกแบบมาสำหรับการ logging โดยเฉพาะ มีฟีเจอร์เพิ่มเติม เช่น การกำหนดระดับความสำคัญของ log (Info, Warning, Error, Debug, Verbose), การใส่ Tag เพื่อจัดกลุ่ม, การกรอง log ตาม level หรือ tag, และอาจมีการเพิ่ม timestamp หรือ Core ID อัตโนมัติ.

2.  **Log level ไหนที่จะแสดงใน default configuration?**
    * โดยปกติ default log level คือ **Info** (`ESP_LOGI`) ซึ่งหมายความว่า log ที่มี level Info, Warning (`ESP_LOGW`), และ Error (`ESP_LOGE`) จะถูกแสดงออกมา. ส่วน Debug (`ESP_LOGD`) และ Verbose (`ESP_LOGV`) จะไม่แสดง นอกจากจะมีการปรับเปลี่ยน configuration.

3.  **การใช้ `ESP_ERROR_CHECK()` มีประโยชน์อย่างไร?**
    * `ESP_ERROR_CHECK()` ใช้ตรวจสอบค่า return ของฟังก์ชัน ESP-IDF ที่คืนค่า `esp_err_t`. หากค่าที่คืนมาไม่ใช่ `ESP_OK` (เกิด error) มาโครนี้จะพิมพ์ข้อความ error ที่ระบุชื่อไฟล์, บรรทัด, และชื่อฟังก์ชันที่เกิด error แล้ว **abort การทำงาน** ของโปรแกรม. ช่วยให้ดักจับ error ที่ไม่คาดคิดได้เร็วและหยุดการทำงานก่อนที่ระบบจะผิดพลาดต่อไป ทำให้ debug ได้ง่ายขึ้น.

4.  **คำสั่งใดในการออกจาก Monitor mode?**
    * คำสั่งคือ **`Ctrl+]`** (กดปุ่ม Ctrl ค้างไว้แล้วกดปุ่ม `]`).
    * สำหรับ Windows อาจเป็น `Ctrl+T` แล้วตามด้วย `Ctrl+X`.

5.  **การตั้งค่า Log level สำหรับ tag เฉพาะทำอย่างไร?**
    * สามารถทำได้โดยใช้ฟังก์ชัน `esp_log_level_set()` ในโค้ด เช่น `esp_log_level_set("MY_TAG", ESP_LOG_DEBUG);` เพื่อตั้งค่า level ของ tag "MY_TAG" เป็น Debug. หรือสามารถตั้งค่าผ่าน `menuconfig` ได้เช่นกัน.
