## คำถามสำหรับวิเคราะห์

1.  **Task ไหนใช้ stack มากที่สุด? เพราะอะไร?**
    * **Heavy Task (`heavy_stack_task`)** และ **Recursion Demo Task (`recursion_demo_task`)** ใช้ stack มากที่สุด.
        * `heavy_stack_task`: เพราะมีการประกาศ local arrays ขนาดใหญ่หลายตัว (`large_buffer[1024]`, `large_numbers[200]`, `another_buffer[512]`) ภายใน loop ซึ่งจะใช้พื้นที่บน stack ทุกครั้งที่ Task ทำงานใน loop นั้น.
        * `recursion_demo_task`: เพราะมีการเรียกฟังก์ชัน `recursive_function` ซ้ำๆ ซ้อนกัน (recursion) แต่ละครั้งที่เรียก ฟังก์ชันจะสร้าง local variables (`local_array[100]`) และเก็บข้อมูลอื่นๆ (เช่น return address) บน stack ทำให้ stack ถูกใช้งานเพิ่มขึ้นเรื่อยๆ ตามความลึกของการเรียก.

2.  **การใช้ heap แทน stack มีข้อดีอย่างไร?**
    * **ลดการใช้ Stack ของ Task**: การย้ายข้อมูลขนาดใหญ่ (เช่น buffer, array) ไปไว้บน heap (โดยใช้ `malloc`/`free`) จะช่วยลดปริมาณ stack ที่ Task นั้นต้องการลงได้อย่างมาก ทำให้ลดความเสี่ยงของ Stack Overflow และอาจกำหนด stack size เริ่มต้นให้เล็กลงได้. โค้ด `optimized_heavy_task` แสดงตัวอย่างนี้.
    * **จัดการหน่วยความจำที่ยืดหยุ่นกว่า**: Heap เหมาะสำหรับข้อมูลที่มีขนาดไม่แน่นอน หรือมี lifetime ที่ยาวนานกว่าขอบเขตของฟังก์ชันปัจจุบัน.
    * **ใช้หน่วยความจำร่วมกันได้**: ข้อมูลบน Heap สามารถเข้าถึงได้จากหลาย Task (ต้องมีการจัดการ synchronization ที่เหมาะสม) ในขณะที่ Stack เป็นของ Task ใคร Task มัน.

3.  **Stack overflow เกิดขึ้นเมื่อไหร่และทำอย่างไรป้องกัน?**
    * **เกิดขึ้นเมื่อ**: Task พยายามใช้ stack memory เกินกว่าขนาดที่จองไว้ให้ตอนสร้าง Task (`xTaskCreate` หรือ `xTaskCreateStatic`). สาเหตุหลักๆ คือ การประกาศ local variable ขนาดใหญ่เกินไป, การเรียกฟังก์ชันซ้อนกันลึกเกินไป (deep recursion), หรือการเขียนข้อมูลเกินขอบเขตของ local buffer.
    * **วิธีป้องกัน**:
        * **คำนวณ Stack Size อย่างรอบคอบ**: ประเมิน worst-case stack usage ของ Task.
        * **ใช้ Heap สำหรับข้อมูลขนาดใหญ่**: ย้าย local array หรือ buffer ขนาดใหญ่ไปใช้ `malloc`/`free` แทน.
        * **หลีกเลี่ยง Deep Recursion**: เปลี่ยนไปใช้วิธีวน loop (iterative) แทน.
        * **ใช้ Stack Monitoring**: ตรวจสอบ stack usage เป็นประจำด้วย `uxTaskGetStackHighWaterMark()` เพื่อดูว่าใกล้จะเต็มหรือไม่.
        * **เปิดใช้งาน Stack Overflow Detection**: ตั้งค่าใน `menuconfig` (`CONFIG_FREERTOS_CHECK_STACKOVERFLOW`) ให้ FreeRTOS ตรวจสอบและแจ้งเตือน (ผ่าน hook function `vApplicationStackOverflowHook`) เมื่อเกิด overflow.

4.  **การตั้งค่า stack size ควรพิจารณาจากอะไร?**
    * **Local Variables**: ขนาดและจำนวนของ local variables ทั้งหมดที่ Task และฟังก์ชันที่ Task เรียกใช้.
    * **Function Call Depth**: ความลึกสูงสุดของการเรียกฟังก์ชันซ้อนกัน (nested function calls) รวมถึง recursion.
    * **Interrupt Context**: หาก Task ถูกขัดจังหวะโดย ISR ต้องมี stack เพียงพอสำหรับเก็บ context ของ Task และการทำงานของ ISR ด้วย (ถ้า ISR ใช้ stack ของ Task นั้น).
    * **Library Functions**: การเรียกใช้ฟังก์ชันจาก library ต่างๆ (รวมถึง C standard library) อาจใช้ stack เพิ่มเติม.
    * **เผื่อ Buffer**: ควรมีขนาดเผื่อ (safety margin) สำหรับกรณีที่ไม่คาดคิด หรือการเปลี่ยนแปลงโค้ดในอนาคต.
    * **เครื่องมือช่วย**: ESP-IDF มีเครื่องมือช่วยวิเคราะห์ stack usage หรือใช้การ monitor ด้วย `uxTaskGetStackHighWaterMark()` ในระหว่างการทดสอบเพื่อหาค่าที่เหมาะสม.

5.  **Recursion ส่งผลต่อ stack usage อย่างไร?**
    * Recursion (การที่ฟังก์ชันเรียกตัวเอง) ส่งผลให้ stack usage **เพิ่มขึ้นอย่างรวดเร็วตามความลึก (depth) ของการเรียก**. ทุกครั้งที่ฟังก์ชันเรียกตัวเองซ้ำ จะมีการสร้าง stack frame ใหม่เพื่อเก็บ local variables, พารามิเตอร์, และ return address ของการเรียกนั้นๆ. หาก recursion ลึกเกินไป จะใช้ stack memory จนหมดและเกิด Stack Overflow ได้ง่ายมาก. โค้ด `recursive_function` ใน Lab 3 แสดงให้เห็นว่า stack remaining ลดลงทุกครั้งที่เรียกซ้ำ.