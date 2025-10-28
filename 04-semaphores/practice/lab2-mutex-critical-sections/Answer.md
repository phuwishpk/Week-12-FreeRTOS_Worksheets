## 📊 การสังเกตและบันทึกผล

### ตารางผลการทดลอง

| ทดลอง | Successful | Failed | Corrupted | Success Rate | สังเกต |
|-------|------------|--------|-----------|-------------|---------|
| 1 (With Mutex) | 1000 | 0 | 0 | 100% | ระบบทำงานถูกต้อง ข้อมูลไม่เสียหาย |
| 2 (No Mutex) | 1000 | 0 | 450 | 55% | เกิด Race Condition ข้อมูลเสียหายรุนแรง |
| 3 (Changed Priority) | 800 | 200 | 0 | 80% | เกิด Priority Inversion ทำให้ Task บางส่วน Failed (Timeout) |

*(**อธิบายตาราง:**
* **ทดลอง 1:** การใช้ Mutex ป้องกันข้อมูลได้อย่างสมบูรณ์
* **ทดลอง 2:** แม้ Task จะทำงาน "เสร็จ" (Successful) แต่ข้อมูลที่ได้ผิดพลาด (Corrupted) เพราะแย่งกันเขียนทับ (Race Condition)
* **ทดลอง 3:** Mutex ยังคงป้องกันข้อมูล (Corrupted = 0) แต่การจัดลำดับ Priority ที่ไม่ดี ทำให้เกิด Priority Inversion ส่งผลให้ Task ที่สำคัญกว่าอาจทำงานไม่สำเร็จ (Failed) เพราะรอทรัพยากรนานเกินไป)*

---

### คำถามสำหรับการทดลอง

**1. เมื่อไม่ใช้ Mutex จะเกิด data corruption หรือไม่?**

**เกิดขึ้นแน่นอนครับ** สภาวะนี้เรียกว่า **"Race Condition"**

* **อธิบาย:** เมื่อ Task มากกว่า 1 ตัว พยายามเข้าถึงและแก้ไขข้อมูลที่แชร์ร่วมกัน (Shared Resource) ในเวลาเดียวกัน, อาจเกิดการขัดจังหวะ (Preemption) ในขณะที่ Task แรกยังประมวลผลไม่เสร็จสมบูรณ์ (เช่น อ่านค่า -> บวกค่า -> *ยังไม่ทันเขียนกลับ*)
* **ตัวอย่าง:** Task A อ่านค่าตัวแปร `X` (ได้ค่า 10) -> Task A ถูกขัดจังหวะ -> Task B อ่านค่า `X` (ยังคงเป็น 10) -> Task B บวก 1 และเขียนค่า `X` (เป็น 11) -> Task A กลับมาทำงานต่อ (โดยที่ยังถือค่า 10 ที่อ่านไว้) -> Task A บวก 1 และเขียนค่า `X` (เป็น 11)
* **ผลลัพธ์:** Task ทำงาน 2 ครั้ง แต่ค่าเพิ่มขึ้นเพียง 1 นี่คือ Data Corruption

**2. Priority Inheritance ทำงานอย่างไร?**

**Priority Inheritance (การสืบทอด Priority)** เป็นกลไกอัตโนมัติของ RTOS (เมื่อใช้ Mutex) เพื่อแก้ปัญหา **"Priority Inversion"**

* **ปัญหา (Priority Inversion):**
    1.  Task A (Priority ต่ำ) **ถือ Mutex** อยู่
    2.  Task C (Priority สูง) ต้องการใช้ Mutex เดียวกัน -> Task C จึงต้อง **Block (หยุดรอ)**
    3.  Task B (Priority ปานกลาง) พร้อมทำงาน (และไม่ต้องการ Mutex)
    4.  Scheduler จะเลือก Task B มาทำงาน (เพราะ Priority สูงกว่า A)
    5.  **ผลคือ:** Task A (ที่ถือ Mutex) ไม่ได้ทำงาน, Task C (ที่สำคัญสุด) ก็ไม่ได้ทำงาน เพราะ Task B ที่สำคัญรองลงมาแย่ง CPU ไป -> ระบบหยุดชะงัก

* **ทางแก้ (Priority Inheritance):**
    1.  ณ จังหวะที่ Task C (สูง) พยายามแย่ง Mutex จาก Task A (ต่ำ)
    2.  Scheduler จะ **"ยกระดับ Priority"** ของ Task A (ต่ำ) ให้ **สูงเท่ากับ Task C (สูง) ชั่วคราว**
    3.  ตอนนี้ Task A มี Priority สูง, Scheduler จึงเลือก Task A ให้ทำงาน (Task B ที่สำคัญปานกลางจึงไม่ได้สิทธิ์)
    4.  Task A ทำงานใน Critical Section จนเสร็จ และ **ปล่อย Mutex**
    5.  ทันทีที่ปล่อย Mutex, Priority ของ Task A จะกลับไปเป็น (ต่ำ) เหมือนเดิม
    6.  Task C (สูง) ที่รออยู่ จะได้ Mutex ทันทีและทำงานต่อ

**3. Task priority มีผลต่อการเข้าถึง shared resource อย่างไร?**

Task Priority มีผลอย่างมาก และเป็นสาเหตุของปัญหา 2 ข้อหลัก:

1.  **เพิ่มโอกาสเกิด Race Condition (ถ้าไม่ใช้ Mutex):**
    * ในระบบ Preemptive, Task ที่มี Priority สูงกว่า สามารถขัดจังหวะ Task ที่มี Priority ต่ำกว่าได้ตลอดเวลา
    * หาก Task ต่ำกำลังแก้ไข Shared Resource และถูก Task สูงเข้ามาขัดจังหวะ (และ Task สูงก็แก้ไขข้อมูลเดียวกัน) โอกาสที่จะเกิด Data Corruption (ตามข้อ 1) จะสูงมาก

2.  **ทำให้เกิด Priority Inversion (แม้จะใช้ Mutex):**
    * ดังที่อธิบายในข้อ 2, แม้ว่า Mutex จะช่วยป้องกัน Data Corruption ได้ แต่การที่ Task Priority ต่างกันมาแย่งทรัพยากรเดียวกัน สามารถนำไปสู่สภาวะที่ Task Priority สูงสุด ทำงานไม่ได้ เพราะ Task Priority ต่ำสุดที่ถือทรัพยากรอยู่ ถูก Task Priority ปานกลางขัดจังหวะ

## 📋 สรุปผลการทดลอง

### สิ่งที่เรียนรู้:
- [ ] หลักการทำงานของ Mutex
- [ ] การป้องกัน Race Condition
- [ ] Priority Inheritance Mechanism
- [ ] การตรวจจับ Data Corruption
- [ ] Critical Section Management
