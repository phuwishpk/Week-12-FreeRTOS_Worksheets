## คำถามสำหรับวิเคราะห์

1.  **Priority ไหนทำงานมากที่สุด? เพราะอะไร?**
    * **High Priority Task (Priority 5)** ทำงานมากที่สุด. เพราะ FreeRTOS ใช้ระบบ **Priority-based Preemptive Scheduling** ซึ่งหมายความว่า Task ที่มี priority สูงกว่าจะได้รับสิทธิ์ในการทำงานก่อน Task ที่มี priority ต่ำกว่าเสมอ และสามารถขัดจังหวะ (preempt) Task ที่มี priority ต่ำกว่าที่กำลังทำงานอยู่ได้. ดังนั้น Task ที่มี priority สูงสุด (5) จึงได้ทำงานบ่อยที่สุดเมื่อมันพร้อมทำงาน.

2.  **เกิด Priority Inversion หรือไม่? จะแก้ไขได้อย่างไร?**
    * **ใช่**, ในโค้ดตัวอย่างส่วน "Priority Inversion Demo" มีการสาธิตให้เห็นถึงปัญหา Priority Inversion. ปัญหานี้เกิดขึ้นเมื่อ Task priority ต่ำ (low priority task) ถือครองทรัพยากร (เช่น `shared_resource_busy = true`) ที่ Task priority สูง (high priority task) ต้องการใช้งาน ทำให้ Task priority สูงต้องรอ. หากในขณะนั้นมี Task priority ปานกลาง (medium priority task) ที่พร้อมทำงานเข้ามา มันจะ preempt Task priority ต่ำ ทำให้ Task priority ต่ำไม่สามารถปล่อยทรัพยากรได้ ส่งผลให้ Task priority สูงต้องรอนานยิ่งขึ้น.
    * **วิธีแก้ไข** ที่พบบ่อยคือการใช้ **Mutex** แทน Semaphore ธรรมดาในการปกป้องทรัพยากรที่ใช้ร่วมกัน โดย Mutex ใน FreeRTOS มักจะมีกลไก **Priority Inheritance** ซึ่งจะ **ยกระดับ priority** ของ Task priority ต่ำที่ถือ Mutex อยู่ ให้สูงเท่ากับ Task priority สูงสุดที่กำลังรอ Mutex นั้นชั่วคราว เพื่อให้ Task priority ต่ำทำงานจนเสร็จและปล่อย Mutex ได้เร็วขึ้น ป้องกันไม่ให้ Task priority ปานกลางเข้ามาแทรกได้. (แม้โค้ด Lab 1 จะแค่สาธิตปัญหา แต่ไม่ได้แสดงโค้ดแก้ไขด้วย Mutex)

3.  **Tasks ที่มี priority เดียวกันทำงานอย่างไร?**
    * Tasks ที่มี priority เท่ากันจะทำงานสลับกันแบบ **Round-Robin**. FreeRTOS Scheduler จะให้ CPU แก่ Task หนึ่งในกลุ่มนั้นทำงานตาม **Time Slice** ที่กำหนด (ถ้าเปิดใช้งาน Time Slicing) เมื่อหมดเวลา หรือเมื่อ Task นั้นๆ block ตัวเอง (เช่น เรียก `vTaskDelay()`), Scheduler ก็จะสลับไปให้ Task ถัดไปในกลุ่มที่มี priority เท่ากันได้ทำงาน.

4.  **การเปลี่ยน Priority แบบ dynamic ส่งผลอย่างไร?**
    * การเปลี่ยน Priority แบบ dynamic (ใช้ `vTaskPrioritySet()`) จะ **เปลี่ยนลำดับการทำงาน** ของ Task นั้นๆ ทันที.
        * หาก **เพิ่ม priority** ของ Task ให้สูงขึ้น มันอาจจะ preempt Task อื่นที่มี priority ต่ำกว่า และได้ทำงานบ่อยขึ้น.
        * หาก **ลด priority** ของ Task ลง มันจะทำงานน้อยลง และอาจถูก Task อื่นที่มี priority สูงกว่า preempt ได้ง่ายขึ้น.
    * ในตัวอย่าง Lab 1 เมื่อ `low_priority_task` ถูก boost priority เป็น 4 มันจะได้ทำงานมากกว่าตอนที่เป็น priority 1.

5.  **CPU utilization ของแต่ละ priority เป็นอย่างไร?**
    * **High Priority Task**: ได้ CPU utilization สูงสุด (เมื่อพร้อมทำงาน) เพราะได้ทำงานก่อนเสมอและ preempt Task อื่นได้. แต่ถ้า Task นี้มีการ block บ่อยๆ (เช่น `vTaskDelay`) CPU utilization โดยรวมอาจไม่สูงเท่า Task ที่ทำงานต่อเนื่องแต่ priority ต่ำกว่า. จากตารางผลคาดหวัง คาดว่าจะได้ CPU ไปประมาณ **40-50%** ของเวลาที่ Task ทั้งหมดทำงาน.
    * **Medium Priority Task**: ได้ CPU utilization รองลงมา ทำงานเมื่อ High Priority Task block อยู่ และ preempt Low Priority Task ได้. คาดว่าจะได้ CPU ประมาณ **25-35%**.
    * **Low Priority Task**: ได้ CPU utilization น้อยที่สุด ทำงานได้เฉพาะเมื่อไม่มี Task ที่มี priority สูงกว่าพร้อมทำงาน. คาดว่าจะได้ CPU ประมาณ **15-25%**.
    * **Equal Priority Tasks**: จะแบ่ง CPU utilization กันในกลุ่มที่มี priority เท่ากัน (ขึ้นอยู่กับ Time Slicing และการ block ของแต่ละ Task).