# student-management-system-dsa
Student Management System using C++ and Data Structures A console-based DSA project implementing linked lists, vectors, file handling, and menu-driven authentication for managing students, courses, GPA, fees, and records.
# Student Management System (DSA Project)

A **console-based Student Management System** developed in **C++** as a **Data Structures & Algorithms (DSA) project**.  
The system efficiently manages students, courses, GPA, fees, and records using **linked lists, vectors, and file handling**.

---

## 👤 Developer Information

- **Name:** Muhammad Furqan  
- **Roll No:** SU92-BSCSM-F24-020  
- **Degree Program:** BSCS  
- **Project Type:** DSA Semester Project  
- **Language:** C++

---

## 🎯 Project Objectives

- Apply **Data Structures concepts** in a real-world system  
- Manage student academic records efficiently  
- Implement persistent storage using files  
- Provide separate **Admin** and **Student** access  

---

## 🧠 Data Structures Used

- **Singly Linked List**
  - Used to store and manage student records dynamically
- **Vectors**
  - Course list
  - Student enrolled courses
  - Semester-wise GPA storage
- **Strings & Structures**
  - Structured data handling
- **File Handling**
  - Persistent storage using `.txt` files

---

## ⚙️ Core Features

### 🔐 Authentication System
- Admin login (secured)
- Student login (roll number based)

---

### 👨‍🎓 Student Management
- Add new students  
- Delete students  
- Search student by roll number  
- Display all students  
- View individual student details  

---

### 📚 Course Management
- Add new courses  
- Display all courses  
- Allocate courses to students  
- Credit limit enforcement (Max 18 credits per semester)

---

### 📊 GPA & CGPA Management
- Enter semester GPA  
- Automatic CGPA calculation  
- Semester-wise GPA tracking  

---

### 💰 Fee & Fine Management
- Mark fee as paid/unpaid  
- Add fine  
- Clear fine  
- Student can view fee status  

---

### 💾 File Handling
- `students.txt` → Stores student records  
- `courses.txt` → Stores course details  
- Data automatically loads on startup  
- Data saved safely on exit  

---

## 🖥️ How to Compile & Run

### Compile
```bash
g++ main.cpp -o sms
./sms

Default Admin Credentials

Username: admin

Password: admin123

(You may change these inside the code)
🚀 Learning Outcomes

Practical use of Linked Lists

File handling with structured data

Menu-driven program design

Real-world application of DSA concepts

Improved problem-solving and logic building
