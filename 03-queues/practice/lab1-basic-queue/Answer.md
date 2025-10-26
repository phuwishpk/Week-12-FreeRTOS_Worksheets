### คำถามสำหรับการทดลอง
1.  **เมื่อ Queue เต็ม การเรียก `xQueueSend` จะเกิดอะไรขึ้น?**
    * การเรียก `xQueueSend` จะ **block** (หยุดรอ) Task ที่เรียกใช้ ตามระยะเวลา timeout ที่กำหนดไว้ในพารามิเตอร์.
    * หากมีที่ว่างใน Queue เกิดขึ้นก่อนหมดเวลา timeout, ข้อมูลจะถูกส่งเข้า Queue และฟังก์ชันจะคืนค่า `pdPASS`.
    * หาก **หมดเวลา timeout** แต่ Queue ยังเต็มอยู่, ฟังก์ชันจะคืนค่า `errQUEUE_FULL` (หรือ `pdFAIL`) และ Task จะ unblock แล้วทำงานต่อไปโดยที่ข้อมูลไม่ได้ถูกส่งเข้า Queue. ในโค้ดตัวอย่าง มีการ log ข้อความ "Failed to send message (queue full?)" เมื่อเกิดกรณีนี้ขึ้น.
    * หากระบุ timeout เป็น 0 (`xQueueSend(xQueue, &message, 0)`), ฟังก์ชันจะไม่ block และจะคืนค่า `errQUEUE_FULL` ทันทีถ้า Queue เต็ม.

2.  **เมื่อ Queue ว่าง การเรียก `xQueueReceive` จะเกิดอะไรขึ้น?**
    * การเรียก `xQueueReceive` จะ **block** Task ที่เรียกใช้ ตามระยะเวลา timeout ที่กำหนดไว้ในพารามิเตอร์.
    * หากมีข้อมูลถูกส่งเข้ามาใน Queue ก่อนหมดเวลา timeout, ข้อมูลจะถูกรับออกมาจาก Queue และฟังก์ชันจะคืนค่า `pdPASS`.
    * หาก **หมดเวลา timeout** แต่ Queue ยังว่างอยู่, ฟังก์ชันจะคืนค่า `errQUEUE_EMPTY` (หรือ `pdFAIL`) และ Task จะ unblock แล้วทำงานต่อไปโดยไม่ได้รับข้อมูล. ในโค้ดตัวอย่าง มีการ log ข้อความ "No message received within timeout" เมื่อเกิดกรณีนี้ขึ้น.
    * หากระบุ timeout เป็น 0 (`xQueueReceive(xQueue, &received_message, 0)`), ฟังก์ชันจะไม่ block และจะคืนค่า `errQUEUE_EMPTY` ทันทีถ้า Queue ว่าง.

3.  **ทำไม LED จึงกะพริบตามการส่งและรับข้อความ?**
    * LED กะพริบเพื่อเป็น **สัญญาณภาพ** (visual indicator) แสดงให้เห็นว่าการส่ง (`sender_task`) หรือการรับ (`receiver_task`) ข้อมูลผ่าน Queue **สำเร็จ**.
    * ใน `sender_task`, LED_SENDER (GPIO_NUM_2) จะกะพริบหลังจากเรียก `xQueueSend` สำเร็จ (คืนค่า `pdPASS`) เพื่อยืนยันว่าข้อความถูกส่งเข้า Queue แล้ว.
    * ใน `receiver_task`, LED_RECEIVER (GPIO_NUM_4) จะกะพริบหลังจากเรียก `xQueueReceive` สำเร็จ (คืนค่า `pdPASS`) เพื่อยืนยันว่าได้รับข้อความจาก Queue แล้ว.
    * การกะพริบนี้ช่วยให้ผู้ทดลองสังเกตการทำงานและความสัมพันธ์ระหว่าง producer และ consumer ได้ง่ายขึ้น โดยเฉพาะเมื่อดูการทำงานบนบอร์ด ESP32 จริง.