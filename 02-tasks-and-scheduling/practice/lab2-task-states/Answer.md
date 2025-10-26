## คำถามสำหรับวิเคราะห์

1.  **Task อยู่ใน Running state เมื่อไหร่บ้าง?**
    * Task จะอยู่ใน **Running state** เมื่อมัน **กำลังทำงานบน CPU** อยู่จริง ๆ. ในระบบที่มี CPU core เดียว จะมีเพียง Task เดียวที่อยู่ใน Running state ณ เวลาใดเวลาหนึ่ง. Task ที่มี priority สูงสุดในบรรดา Task ที่พร้อมทำงาน (Ready state) จะถูก Scheduler เลือกให้เข้าสู่ Running state. โค้ดใน Lab 2 แสดง Running state โดยการเปิด LED_RUNNING และทำงานคำนวณ.

2.  **ความแตกต่างระหว่าง Ready และ Blocked state คืออะไร?**
    * **Ready State**: Task **พร้อมที่จะทำงาน** แต่กำลัง **รอ CPU ว่าง** เพราะมี Task อื่น (ที่มี priority เท่ากันหรือสูงกว่า) กำลังทำงานอยู่. Task ใน Ready state อยู่ในคิวรอของ Scheduler.
    * **Blocked State**: Task **ไม่สามารถทำงานได้** เพราะกำลัง **รอเหตุการณ์บางอย่าง** เกิดขึ้น เช่น รอเวลาครบกำหนด (จากการเรียก `vTaskDelay` หรือ `vTaskDelayUntil`), รอ Semaphore, รอข้อมูลใน Queue, หรือรอ Event Group. Task ใน Blocked state จะไม่ถูกพิจารณาโดย Scheduler จนกว่าเหตุการณ์ที่รอจะเกิดขึ้น.

3.  **การใช้ `vTaskDelay()` ทำให้ task อยู่ใน state ใด?**
    * การเรียก `vTaskDelay()` จะทำให้ Task เข้าสู่ **Blocked state**. Task จะถูกย้ายออกจาก Running state และจะไม่ถูกพิจารณาโดย Scheduler จนกว่าจะครบตามระยะเวลาที่กำหนดไว้ เมื่อครบเวลา Task จะย้ายกลับไปสู่ Ready state เพื่อรอ CPU ทำงานต่อไป.

4.  **การ Suspend task ต่างจาก Block อย่างไร?**
    * **Blocked**: Task เข้าสู่ state นี้ **โดยอัตโนมัติ** เมื่อต้องรอเหตุการณ์ (เช่น delay, semaphore, queue) และจะออกจาก state นี้ **โดยอัตโนมัติ** เมื่อเหตุการณ์นั้นเกิดขึ้น.
    * **Suspended**: Task เข้าสู่ state นี้ **โดยการเรียก API `vTaskSuspend()`** จาก Task อื่น หรือจากตัวเอง และจะออกจาก state นี้ได้ **ก็ต่อเมื่อมีการเรียก `vTaskResume()`** จาก Task อื่นเท่านั้น. Task ใน Suspended state จะไม่ถูกพิจารณาโดย Scheduler เลย ไม่ว่าจะเกิดเหตุการณ์ใดๆ ขึ้นก็ตาม จนกว่าจะถูก Resume.

5.  **Task ที่ถูก Delete จะกลับมาได้หรือไม่?**
    * **ไม่ได้**. เมื่อ Task ถูกลบโดยการเรียก `vTaskDelete()` (ไม่ว่าจะเป็นการลบตัวเองด้วย `vTaskDelete(NULL)` หรือถูก Task อื่นลบด้วย `vTaskDelete(handle)`), ทรัพยากรที่เกี่ยวข้องกับ Task นั้น (เช่น Task Control Block - TCB และ Stack memory) จะถูกคืนให้กับระบบ. Task นั้นจะหายไปจาก Scheduler อย่างถาวรและไม่สามารถกลับมาทำงานได้อีก หากต้องการให้ Task ทำงานใหม่ จะต้องสร้าง Task นั้นขึ้นมาใหม่ด้วย `xTaskCreate()`.