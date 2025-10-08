# ปฏิบัติการ: Tasks and Scheduling

## ภาพรวม
การปฏิบัติการนี้จะสอนการจัดการ Task และการทำงานของ Scheduler ใน FreeRTOS

## 📋 รายการแลป

### Lab 1: Task Priority และ Scheduling (45 นาที)
- **ไฟล์**: [lab1-task-priority/](lab1-task-priority/)
- **เป้าหมาย**: ทำความเข้าใจ Priority-based Scheduling
- **กิจกรรม**:
  - ทดสอบ Task Priority ต่างๆ
  - สังเกต Preemptive Scheduling
  - วัดเวลาการทำงานของแต่ละ Priority

### Lab 2: Task States Demonstration (45 นาที)
- **ไฟล์**: [lab2-task-states/](lab2-task-states/)
- **เป้าหมาย**: ศึกษา Task States และการเปลี่ยนแปลง
- **กิจกรรม**:
  - สาธิต Task States: Running, Ready, Blocked, Suspended
  - ใช้ APIs สำหรับเปลี่ยน Task States
  - Monitor Task State Transitions

### Lab 3: Stack Monitoring และ Debugging (30 นาที)
- **ไฟล์**: [lab3-stack-monitoring/](lab3-stack-monitoring/)
- **เป้าหมาย**: จัดการและตรวจสอบ Stack Memory
- **กิจกรรม**:
  - Monitor Stack usage
  - ตรวจจับ Stack overflow
  - Optimize Stack size

## 🛠️ เครื่องมือที่ใช้
- ESP32 Development Board
- LED 3 ดวง
- Push Button 2 ตัว
- Logic Analyzer (ถ้ามี)

## 📊 การประเมินผล
- การทำงานของโปรแกรม (50%)
- การวิเคราะห์และอธิบาย Scheduling behavior (30%)
- การใช้ Debugging tools (20%)

## 📝 รายงานการทดลอง
ให้สร้างไฟล์ `report.md` สำหรับบันทึก:
- ผลการทดลองแต่ละ Lab
- การวิเคราะห์ Task behavior
- ปัญหาที่พบและแนวทางแก้ไข
- ข้อสรุปและข้อเสนอแนะ