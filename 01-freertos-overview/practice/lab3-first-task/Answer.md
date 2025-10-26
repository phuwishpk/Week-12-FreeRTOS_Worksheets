
## Checklist การทำงาน

- [ ] สร้าง Task พื้นฐานสำเร็จ
- [ ] เข้าใจ Task parameters และ return values
- [ ] ทดสอบ Task priorities
- [ ] ใช้ Task management APIs (suspend/resume)
- [ ] แสดง runtime statistics
- [ ] ทำแบบฝึกหัดครบ

## คำถามทบทวน

11.  **เหตุใด Task function ต้องมี infinite loop?**
    * ฟังก์ชัน Task ใน FreeRTOS ถูกออกแบบมาให้ทำงานต่อเนื่อง. หากฟังก์ชัน Task ทำงานจนจบ (return) โดยไม่มี `while(1)` หรือ loop ไม่รู้จบอื่นๆ ระบบปฏิบัติการจะถือว่า Task ทำงานเสร็จสิ้นและอาจจะลบ Task นั้นทิ้ง (หรือเข้าสู่ state ที่จัดการไม่ได้ ขึ้นอยู่กับการตั้งค่า). การมี `while(1)` ช่วยให้ Task ยังคงทำงานอยู่ใน scheduler เพื่อวนรอบทำงานตามที่กำหนดไปเรื่อยๆ. หากต้องการให้ Task หยุดทำงาน ควรเรียก `vTaskDelete(NULL)` อย่างชัดเจนภายใน loop.

2.  **ความหมายของ stack size ใน `xTaskCreate()` คืออะไร?**
    * Stack size คือ **ขนาดของหน่วยความจำ (RAM)** ที่จองไว้ให้ Task นั้นๆ โดยเฉพาะ เพื่อใช้เก็บ **local variables**, **พารามิเตอร์ของฟังก์ชัน**, **return addresses** เมื่อมีการเรียกฟังก์ชันซ้อนกัน และ **context** ของ Task เมื่อถูกสลับการทำงาน (preempted). ขนาด stack ต้องเพียงพอต่อการใช้งานทั้งหมดของ Task นั้นๆ หากไม่พอจะเกิด Stack Overflow ซึ่งเป็นปัญหาที่ร้ายแรง. หน่วยที่ระบุใน ESP-IDF มักจะเป็น **bytes** (ไม่ใช่ words เหมือนใน FreeRTOS ดั้งเดิมบาง port).

3.  **ความแตกต่างระหว่าง `vTaskDelay()` และ `vTaskDelayUntil()`?**
    * `vTaskDelay(ticksToDelay)`: หน่วงเวลา **นับจากเวลาปัจจุบันที่เรียกฟังก์ชัน** ไปอีก `ticksToDelay` ticks. การหน่วงเวลาจริงอาจคลาดเคลื่อนเล็กน้อย ขึ้นอยู่กับเวลาที่ Task กลับมาทำงานอีกครั้ง. เหมาะสำหรับการหน่วงเวลาทั่วไปที่ไม่ต้องการความแม่นยำสูงมาก.
    * `vTaskDelayUntil(&previousWakeTime, ticksToDelay)`: หน่วงเวลา **จนกระทั่งถึงเวลาที่กำหนด** โดยอิงจาก `previousWakeTime` บวกด้วย `ticksToDelay`. ฟังก์ชันนี้จะคำนวณเวลาตื่น (wake time) ที่แน่นอน ทำให้ Task ทำงานเป็นคาบเวลาที่ค่อนข้างแม่นยำ แม้ว่าระหว่างรอบการทำงานจะใช้เวลาไม่เท่ากันก็ตาม. เหมาะสำหรับ Task ที่ต้องการทำงานเป็นรอบเวลาที่คงที่ (periodic task).

4.  **การใช้ `vTaskDelete(NULL)` vs `vTaskDelete(handle)` ต่างกันอย่างไร?**
    * `vTaskDelete(NULL)`: ใช้สำหรับให้ Task **ลบตัวเอง**. Task ที่เรียกฟังก์ชันนี้จะถูกลบออกจากระบบ.
    * `vTaskDelete(handle)`: ใช้สำหรับให้ Task หนึ่ง **ลบ Task อื่น** โดยระบุ `handle` (Task Handle) ของ Task ที่ต้องการลบ. Task ที่ถูกลบจะถูกนำออกจาก scheduler และทรัพยากรที่เกี่ยวข้อง (Stack, TCB) จะถูกคืนให้กับระบบ.

5.  **Priority 0 กับ Priority 24 อันไหนสูงกว่า?**
    * ใน FreeRTOS (รวมถึงเวอร์ชันใน ESP-IDF), **ค่า Priority ที่สูงกว่า หมายถึง ลำดับความสำคัญที่สูงกว่า**. ดังนั้น **Priority 24** จึงมีลำดับความสำคัญสูงกว่า **Priority 0**. โดย Priority 0 มักจะถูกสงวนไว้สำหรับ Idle Task. Task ที่มี Priority สูงกว่าจะสามารถ preempt (ขัดจังหวะ) Task ที่มี Priority ต่ำกว่าได้.