## 📊 การสังเกตและบันทึกผล

### ตารางบันทึกผล
| ทดลอง | Events Sent | Events Received | Timer Events | Button Presses | Efficiency |
|-------|-------------|-----------------|--------------|----------------|------------|
| 1 (Normal) | | | | | |
| 2 (Multiple Give) | | | | | |
| 3 (Short Timeout) | | | | | |

### คำถามสำหรับการทดลоง
1. เมื่อ give semaphore หลายครั้งติดต่อกัน จะเกิดอะไรขึ้น?
2. ISR สามารถใช้ `xSemaphoreGive` หรือต้องใช้ `xSemaphoreGiveFromISR`?
3. Binary Semaphore แตกต่างจาก Queue อย่างไร?

## 📋 สรุปผลการทดลอง

### สิ่งที่เรียนรู้:
- [ ] หลักการทำงานของ Binary Semaphore
- [ ] การใช้ Semaphore สำหรับ Task Synchronization
- [ ] การสื่สารระหว่าง ISR และ Task
- [ ] การใช้ Timer interrupt กับ Semaphore
- [ ] การจัดการ Button interrupt