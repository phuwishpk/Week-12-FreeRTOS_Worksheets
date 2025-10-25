## คำถามทบทวน (จาก Lab 1 และ Lab 3)

1.  **ไฟล์ใดบ้างที่จำเป็นสำหรับโปรเจกต์ ESP-IDF ขั้นต่ำ?**
    * ไฟล์ที่จำเป็นขั้นต่ำคือ:
        * `CMakeLists.txt` (ใน root ของโปรเจกต์)
        * โฟลเดอร์ `main/` ซึ่งภายในต้องมี:
            * `CMakeLists.txt` (สำหรับ component 'main')
            * ไฟล์ source code ภาษา C (เช่น `hello_esp32.c`) ที่มีฟังก์ชัน `app_main`

2.  **ความแตกต่างระหว่าง `hello_esp32.bin` และ `hello_esp32.elf` คืออะไร?**
    * `hello_esp32.bin`: เป็น **binary image** ที่พร้อมจะ flash ลงบนหน่วยความจำ flash ของ ESP32 โดยตรง ไม่มีข้อมูลการ debug
    * `hello_esp32.elf`: เป็นไฟล์ **Executable and Linkable Format (ELF)** ซึ่งเป็น output มาตรฐานของ compiler มีข้อมูล binary ของโปรแกรม รวมถึงข้อมูลการ debug (เช่น symbol table, source code line information) ซึ่งใช้สำหรับการ debug ด้วย GDB

3.  **คำสั่ง `idf.py set-target` ทำอะไร?**
    * คำสั่งนี้ใช้ **กำหนดรุ่นของชิป ESP** ที่จะใช้สำหรับ build โปรเจกต์ (เช่น `esp32`, `esp32s2`, `esp32c3`) เพื่อให้ ESP-IDF ใช้ toolchain, library และ configuration ที่ถูกต้องสำหรับชิปนั้นๆ

4.  **โฟลเดอร์ `build/` มีไฟล์อะไรบ้าง?**
    * โฟลเดอร์ `build/` จะมีไฟล์ผลลัพธ์จากการ build โปรเจกต์ ตัวอย่างไฟล์สำคัญๆ ได้แก่:
        * `bootloader/bootloader.bin`: ไฟล์ binary ของ bootloader
        * `partition_table/partition-table.bin`: ไฟล์ binary ของ partition table
        * `<project_name>.bin`: ไฟล์ binary หลักของแอปพลิเคชัน
        * `<project_name>.elf`: ไฟล์ ELF ของแอปพลิเคชัน (มีข้อมูล debug)
        * `<project_name>.map`: ไฟล์ map แสดง memory layout
        * `flasher_args.json` (หรือ `flash_args` ในเวอร์ชันเก่า): อาร์กิวเมนต์สำหรับใช้ในการ flash
        * ไฟล์ `.o` (object files) และ `.a` (archive files) ต่างๆ ที่เกิดจากกระบวนการ compile

5.  **การใช้ `vTaskDelay()` แทน `delay()` มีความสำคัญอย่างไร?**
    * `vTaskDelay()` เป็นฟังก์ชันของ **FreeRTOS** ที่ออกแบบมาสำหรับการหน่วงเวลาในระบบ multitasking
    * เมื่อ Task เรียก `vTaskDelay()`, **Scheduler ของ FreeRTOS จะทราบ** และสามารถสลับไปให้ Task อื่นๆ ที่พร้อมทำงาน (Ready state) ได้ทำงานแทนในช่วงเวลาที่ Task นั้นๆ กำลังหน่วงเวลาอยู่ ทำให้ **CPU ไม่ถูก block** และระบบโดยรวมสามารถทำงานอื่นไปพร้อมๆ กันได้
    * ส่วนฟังก์ชัน `delay()` (ถ้ามี มักจะเป็น busy-wait loop) จะเป็นการ **block การทำงานของ CPU** โดยเปล่าประโยชน์ ทำให้ Task อื่นๆ ไม่สามารถทำงานได้ในช่วงเวลานั้น ซึ่งไม่เหมาะกับระบบ multitasking

---

## คำถามทบทวน (จาก Lab 2)

1.  **ความแตกต่างระหว่าง `printf()` และ `ESP_LOGI()` คืออะไร?**
    * `printf()`: เป็นฟังก์ชัน **มาตรฐานของภาษา C** สำหรับการพิมพ์ข้อความออกทาง standard output (ปกติคือ Serial port ใน ESP32) ไม่มีฟีเจอร์เพิ่มเติมเกี่ยวกับการจัดการ log
    * `ESP_LOGI()`: เป็น **มาโครของ ESP-IDF** สำหรับการ logging มีฟีเจอร์เพิ่มเติม เช่น:
        * **Log Levels**: สามารถกำหนดระดับความสำคัญของ log ได้ (Error, Warning, Info, Debug, Verbose)
        * **Tagging**: สามารถระบุ tag เพื่อจัดกลุ่ม log message ได้
        * **Filtering**: สามารถกรอง log message ตาม level หรือ tag ได้ง่ายทั้งตอน compile และ runtime
        * **Timestamp/Core ID**: อาจมีการเพิ่มข้อมูลเวลาและ Core ที่ทำงาน (ขึ้นอยู่กับการตั้งค่า)

2.  **Log level ไหนที่จะแสดงใน default configuration?**
    * โดยปกติ default log level คือ **Info** (`ESP_LOGI`) ซึ่งหมายความว่า log ที่มี level Info, Warning (`ESP_LOGW`), และ Error (`ESP_LOGE`) จะถูกแสดงออกมา ส่วน Debug (`ESP_LOGD`) และ Verbose (`ESP_LOGV`) จะไม่แสดง นอกจากจะมีการปรับเปลี่ยน configuration

3.  **การใช้ `ESP_ERROR_CHECK()` มีประโยชน์อย่างไร?**
    * `ESP_ERROR_CHECK()` เป็นมาโครที่ใช้ **ตรวจสอบค่า return** ของฟังก์ชัน ESP-IDF ต่างๆ ที่คืนค่าเป็น `esp_err_t`
    * หากค่าที่ return **ไม่ใช่ `ESP_OK`** (แสดงว่าเกิด error), มาโครนี้จะ **พิมพ์ข้อความ error** ที่ระบุชื่อไฟล์, บรรทัด, ชื่อฟังก์ชันที่เกิด error และ **abort การทำงาน** ของโปรแกรม (ทำให้เกิด panic และ restart)
    * มีประโยชน์อย่างมากในการ **ดักจับ error ที่ไม่คาดคิด** ตั้งแต่เนิ่นๆ และหยุดการทำงานก่อนที่ระบบจะทำงานผิดพลาดต่อไปใน state ที่ไม่ถูกต้อง ทำให้การ debug ง่ายขึ้น

4.  **คำสั่งใดในการออกจาก Monitor mode?**
    * คำสั่งคือ **`Ctrl+]`** (กดปุ่ม Ctrl ค้างไว้แล้วกดปุ่ม `]`)
    * สำหรับ Windows อาจเป็น `Ctrl+T` แล้วตามด้วย `Ctrl+X`

5.  **การตั้งค่า Log level สำหรับ tag เฉพาะทำอย่างไร?**
    * สามารถทำได้โดยใช้ฟังก์ชัน `esp_log_level_set()` ในโค้ด เช่น:
        ```c
        esp_log_level_set("MY_TAG", ESP_LOG_DEBUG); // ตั้งค่า level ของ tag "MY_TAG" เป็น Debug
        ```
    * หรือสามารถตั้งค่าผ่าน `menuconfig` ได้เช่นกัน โดยไปที่ `Component config → Log output → Maximum log verbosity for runtime tag filtering` แล้วกำหนด level สำหรับ tag ที่ต้องการ

---

## Checklist การทำงาน (รวม)

* [ ] ตรวจสอบการติดตั้ง ESP-IDF
* [ ] สร้างโปรเจกต์ใหม่สำเร็จ
* [ ] Build และ Flash โปรแกรมสำเร็จ
* [ ] ดู Serial Output ได้ (Monitor สำเร็จ)
* [ ] เห็น Log messages ต่างๆ
* [ ] ทดสอบ Log levels ต่างๆ
* [ ] ใช้ formatted logging ได้
* [ ] ปรับ Log level ผ่าน menuconfig
* [ ] สร้าง Task ขั้นพื้นฐานได้
* [ ] ทำความเข้าใจ Task lifecycle
* [ ] ทำแบบฝึกหัดครบ (ทั้ง 3 Labs)