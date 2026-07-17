#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <limits>

using namespace std;

// ---------- Data Structures ----------

struct Course {
    string code;
    string name;
    int credits;
    string teacher;
};

struct Student {
    string roll;
    string name;
    string department;
    int semester;
    vector<string> courses;
    vector<float> semesterGPAs;
    float cgpa;
    bool feePaid;
    double fineAmount;
    Student* next;

    Student() {
        semester = 0;
        cgpa = 0.0f;
        feePaid = false;
        fineAmount = 0.0;
        next = NULL;
    }
};

// Global lists
Student* head = NULL;
vector<Course> courses;
const int MAX_CREDITS = 18; // Max credits per semester

// ---------- Helper / Utility Functions ----------

void pauseAndClear() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

Student* findStudentByRoll(const string& roll) {
    Student* cur = head;
    while (cur) {
        if (cur->roll == roll) return cur;
        cur = cur->next;
    }
    return NULL;
}

int totalCreditsForStudent(Student* s) {
    int total = 0;
    for (size_t i = 0; i < s->courses.size(); ++i) {
        for (size_t j = 0; j < courses.size(); ++j) {
            if (courses[j].code == s->courses[i]) {
                total += courses[j].credits;
                break;
            }
        }
    }
    return total;
}

Course* findCourseByCode(const string& code) {
    for (size_t i = 0; i < courses.size(); ++i) {
        if (courses[i].code == code) return &courses[i];
    }
    return NULL;
}

void calculateCGPA(Student* s) {
    if (s->semesterGPAs.empty()) {
        s->cgpa = 0.0;
        return;
    }
    float sum = 0.0;
    for (size_t i = 0; i < s->semesterGPAs.size(); ++i) {
        sum += s->semesterGPAs[i];
    }
    s->cgpa = sum / s->semesterGPAs.size();
}

// ---------- File Handling ----------

void loadCoursesFromFile(const string& fname = "courses.txt") {
    courses.clear();
    ifstream fin(fname.c_str());
    if (!fin.is_open()) return;
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string code, name, creditsStr, teacher;
        if (!getline(ss, code, '|')) continue;
        if (!getline(ss, name, '|')) continue;
        if (!getline(ss, creditsStr, '|')) continue;
        if (!getline(ss, teacher)) teacher = "";
        Course c;
        c.code = code;
        c.name = name;
        c.credits = atoi(creditsStr.c_str());
        c.teacher = teacher;
        courses.push_back(c);
    }
    fin.close();
}

void saveCoursesToFile(const string& fname = "courses.txt") {
    ofstream fout(fname.c_str());
    for (size_t i = 0; i < courses.size(); ++i) {
        fout << courses[i].code << "|" << courses[i].name << "|" 
             << courses[i].credits << "|" << courses[i].teacher << "\n";
    }
    fout.close();
}

void loadStudentsFromFile(const string& fname = "students.txt") {
    while (head) {
        Student* tmp = head;
        head = head->next;
        delete tmp;
    }
    ifstream fin(fname.c_str());
    if (!fin.is_open()) return;
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string roll, name, dept, semStr, feeStr, fineStr, cgpaStr, gpasStr, coursesStr;
        if (!getline(ss, roll, '|')) continue;
        getline(ss, name, '|');
        getline(ss, dept, '|');
        getline(ss, semStr, '|');
        getline(ss, feeStr, '|');
        getline(ss, fineStr, '|');
        getline(ss, cgpaStr, '|');
        getline(ss, gpasStr, '|');
        getline(ss, coursesStr);

        Student* s = new Student();
        s->roll = roll;
        s->name = name;
        s->department = dept;
        s->semester = atoi(semStr.c_str());
        s->feePaid = (feeStr == "1");
        s->fineAmount = atof(fineStr.c_str());
        s->cgpa = atof(cgpaStr.c_str());

        s->semesterGPAs.clear();
        if (!gpasStr.empty()) {
            stringstream gss(gpasStr);
            string g;
            while (getline(gss, g, ',')) {
                if (!g.empty()) s->semesterGPAs.push_back(atof(g.c_str()));
            }
        }

        s->courses.clear();
        if (!coursesStr.empty()) {
            stringstream css(coursesStr);
            string cc;
            while (getline(css, cc, ',')) {
                if (!cc.empty()) s->courses.push_back(cc);
            }
        }

        s->next = head;
        head = s;
    }
    fin.close();
}

void saveStudentsToFile(const string& fname = "students.txt") {
    ofstream fout(fname.c_str());
    Student* cur = head;
    while (cur) {
        fout << cur->roll << "|" << cur->name << "|" << cur->department << "|"
             << cur->semester << "|" << (cur->feePaid ? "1" : "0") << "|"
             << cur->fineAmount << "|" << cur->cgpa << "|";
        for (size_t i = 0; i < cur->semesterGPAs.size(); ++i) {
            fout << cur->semesterGPAs[i];
            if (i + 1 < cur->semesterGPAs.size()) fout << ",";
        }
        fout << "|";
        for (size_t i = 0; i < cur->courses.size(); ++i) {
            fout << cur->courses[i];
            if (i + 1 < cur->courses.size()) fout << ",";
        }
        fout << "\n";
        cur = cur->next;
    }
    fout.close();
}

// ---------- Student Operations ----------

void addStudent() {
    cin.ignore();
    Student* s = new Student();
    cout << "Enter Roll Number: "; getline(cin, s->roll);
    if (findStudentByRoll(s->roll)) {
        cout << "Student with this roll already exists.\n";
        delete s;
        pauseAndClear();
        return;
    }
    cout << "Enter Name: "; getline(cin, s->name);
    cout << "Enter Department: "; getline(cin, s->department);
    cout << "Enter Current Semester (number): "; 
    string semStr;
    getline(cin, semStr);
    s->semester = atoi(semStr.c_str());
    s->feePaid = false;
    s->fineAmount = 0.0;
    s->cgpa = 0.0;
    s->semesterGPAs.clear();
    s->courses.clear();
    s->next = head; 
    head = s;
    cout << "Student added successfully.\n";
    pauseAndClear();
}

void displayAllStudents() {
    if (!head) {
        cout << "No students in the system.\n";
        pauseAndClear();
        return;
    }
    cout << "\n" << left << setw(12) << "Roll" << setw(22) << "Name" << setw(15) << "Dept"
         << setw(6) << "Sem" << setw(8) << "CGPA" << setw(10) << "Fee" << setw(10) << "Fine" << "\n";
    cout << string(85,'-') << "\n";
    Student* cur = head;
    while (cur) {
        cout << left << setw(12) << cur->roll << setw(22) << cur->name 
             << setw(15) << cur->department << setw(6) << cur->semester 
             << setw(8) << fixed << setprecision(2) << cur->cgpa
             << setw(10) << (cur->feePaid ? "Paid" : "Unpaid") 
             << setw(10) << cur->fineAmount << "\n";
        cur = cur->next;
    }
    pauseAndClear();
}

void deleteStudent() {
    cin.ignore();
    string roll;
    cout << "Enter Roll Number to delete: ";
    getline(cin, roll);
    
    if (!head) {
        cout << "No students in the system.\n";
        pauseAndClear();
        return;
    }
    
    if (head->roll == roll) {
        Student* tmp = head;
        head = head->next;
        delete tmp;
        cout << "Student deleted successfully.\n";
        pauseAndClear();
        return;
    }
    
    Student* cur = head;
    while (cur->next) {
        if (cur->next->roll == roll) {
            Student* tmp = cur->next;
            cur->next = tmp->next;
            delete tmp;
            cout << "Student deleted successfully.\n";
            pauseAndClear();
            return;
        }
        cur = cur->next;
    }
    cout << "Student not found.\n";
    pauseAndClear();
}

void searchStudent() {
    cin.ignore();
    string roll;
    cout << "Enter Roll Number: ";
    getline(cin, roll);
    Student* s = findStudentByRoll(roll);
    if (!s) {
        cout << "Student not found.\n";
        pauseAndClear();
        return;
    }
    cout << "\n--- Student Details ---\n";
    cout << "Roll: " << s->roll << "\n";
    cout << "Name: " << s->name << "\n";
    cout << "Department: " << s->department << "\n";
    cout << "Semester: " << s->semester << "\n";
    cout << "CGPA: " << fixed << setprecision(2) << s->cgpa << "\n";
    cout << "Fee Status: " << (s->feePaid ? "Paid" : "Unpaid") << "\n";
    cout << "Fine Amount: " << s->fineAmount << "\n";
    cout << "Enrolled Courses: ";
    if (s->courses.empty()) cout << "None";
    else {
        for (size_t i = 0; i < s->courses.size(); ++i) {
            cout << s->courses[i];
            if (i + 1 < s->courses.size()) cout << ", ";
        }
    }
    cout << "\n";
    pauseAndClear();
}

// ---------- Course Operations ----------

void addCourse() {
    cin.ignore();
    Course c;
    cout << "Enter Course Code: "; getline(cin, c.code);
    if (findCourseByCode(c.code)) {
        cout << "Course with this code already exists.\n";
        pauseAndClear();
        return;
    }
    cout << "Enter Course Name: "; getline(cin, c.name);
    cout << "Enter Credits: ";
    string credStr;
    getline(cin, credStr);
    c.credits = atoi(credStr.c_str());
    cout << "Enter Teacher Name: "; getline(cin, c.teacher);
    courses.push_back(c);
    cout << "Course added successfully.\n";
    pauseAndClear();
}

void displayAllCourses() {
    if (courses.empty()) {
        cout << "No courses in the system.\n";
        pauseAndClear();
        return;
    }
    cout << "\n" << left << setw(12) << "Code" << setw(30) << "Name" 
         << setw(10) << "Credits" << setw(25) << "Teacher" << "\n";
    cout << string(77,'-') << "\n";
    for (size_t i = 0; i < courses.size(); ++i) {
        cout << left << setw(12) << courses[i].code << setw(30) << courses[i].name 
             << setw(10) << courses[i].credits << setw(25) << courses[i].teacher << "\n";
    }
    pauseAndClear();
}

void allocateCourse() {
    cin.ignore();
    string roll, code;
    cout << "Enter Student Roll Number: "; getline(cin, roll);
    Student* s = findStudentByRoll(roll);
    if (!s) {
        cout << "Student not found.\n";
        pauseAndClear();
        return;
    }
    cout << "Enter Course Code: "; getline(cin, code);
    Course* c = findCourseByCode(code);
    if (!c) {
        cout << "Course not found.\n";
        pauseAndClear();
        return;
    }
    
    // Check if already enrolled
    for (size_t i = 0; i < s->courses.size(); ++i) {
        if (s->courses[i] == code) {
            cout << "Student already enrolled in this course.\n";
            pauseAndClear();
            return;
        }
    }
    
    // Check credit limit
    int currentCredits = totalCreditsForStudent(s);
    if (currentCredits + c->credits > MAX_CREDITS) {
        cout << "Cannot allocate. Credit limit (" << MAX_CREDITS << ") exceeded.\n";
        cout << "Current credits: " << currentCredits << ", Course credits: " << c->credits << "\n";
        pauseAndClear();
        return;
    }
    
    s->courses.push_back(code);
    cout << "Course allocated successfully.\n";
    pauseAndClear();
}

// ---------- Fee & Fine Management ----------

void manageFees() {
    cin.ignore();
    string roll;
    cout << "Enter Student Roll Number: "; getline(cin, roll);
    Student* s = findStudentByRoll(roll);
    if (!s) {
        cout << "Student not found.\n";
        pauseAndClear();
        return;
    }
    
    cout << "\n--- Fee Management ---\n";
    cout << "1. Mark Fee as Paid\n";
    cout << "2. Mark Fee as Unpaid\n";
    cout << "3. Add Fine\n";
    cout << "4. Clear Fine\n";
    cout << "Choose: ";
    int ch;
    cin >> ch;
    
    if (ch == 1) {
        s->feePaid = true;
        cout << "Fee marked as paid.\n";
    } else if (ch == 2) {
        s->feePaid = false;
        cout << "Fee marked as unpaid.\n";
    } else if (ch == 3) {
        cout << "Enter fine amount: ";
        double fine;
        cin >> fine;
        s->fineAmount += fine;
        cout << "Fine added. Total fine: " << s->fineAmount << "\n";
    } else if (ch == 4) {
        s->fineAmount = 0.0;
        cout << "Fine cleared.\n";
    }
    pauseAndClear();
}

// ---------- GPA Management ----------

void enterSemesterGPA() {
    cin.ignore();
    string roll;
    cout << "Enter Student Roll Number: "; getline(cin, roll);
    Student* s = findStudentByRoll(roll);
    if (!s) {
        cout << "Student not found.\n";
        pauseAndClear();
        return;
    }
    
    cout << "Enter GPA for semester " << (s->semesterGPAs.size() + 1) << ": ";
    float gpa;
    cin >> gpa;
    s->semesterGPAs.push_back(gpa);
    calculateCGPA(s);
    cout << "GPA added. New CGPA: " << fixed << setprecision(2) << s->cgpa << "\n";
    pauseAndClear();
}

// ---------- Admin Menu ----------

void adminMenu() {
    string username, password;
    cin.ignore();
    cout << "Enter Admin Username: "; getline(cin, username);
    cout << "Enter Admin Password: "; getline(cin, password);
    
    if (username != "admin" || password != "admin123") {
        cout << "Invalid credentials.\n";
        pauseAndClear();
        return;
    }
    
    while (true) {
        cout << "\n==== Admin Menu ====\n";
        cout << "1. Add Student\n";
        cout << "2. Delete Student\n";
        cout << "3. Search Student\n";
        cout << "4. Display All Students\n";
        cout << "5. Add Course\n";
        cout << "6. Display All Courses\n";
        cout << "7. Allocate Course to Student\n";
        cout << "8. Manage Fees/Fines\n";
        cout << "9. Enter Semester GPA\n";
        cout << "10. Logout\n";
        cout << "Choose: ";
        int ch;
        cin >> ch;
        
        if (ch == 1) addStudent();
        else if (ch == 2) deleteStudent();
        else if (ch == 3) searchStudent();
        else if (ch == 4) displayAllStudents();
        else if (ch == 5) addCourse();
        else if (ch == 6) displayAllCourses();
        else if (ch == 7) allocateCourse();
        else if (ch == 8) manageFees();
        else if (ch == 9) enterSemesterGPA();
        else if (ch == 10) break;
        else cout << "Invalid choice.\n";
    }
}

// ---------- Student Login ----------

void studentLogin() {
    cin.ignore();
    string roll;
    cout << "Enter Roll Number: "; getline(cin, roll);
    Student* s = findStudentByRoll(roll);
    if (!s) {
        cout << "Student not found.\n";
        pauseAndClear();
        return;
    }
    
    while (true) {
        cout << "\n==== Student Menu (" << s->name << ") ====\n";
        cout << "1. View My Details\n";
        cout << "2. View My Courses\n";
        cout << "3. View Fee Status\n";
        cout << "4. Logout\n";
        cout << "Choose: ";
        int ch;
        cin >> ch;
        
        if (ch == 1) {
            cout << "\n--- My Details ---\n";
            cout << "Roll: " << s->roll << "\n";
            cout << "Name: " << s->name << "\n";
            cout << "Department: " << s->department << "\n";
            cout << "Semester: " << s->semester << "\n";
            cout << "CGPA: " << fixed << setprecision(2) << s->cgpa << "\n";
            pauseAndClear();
        } else if (ch == 2) {
            cout << "\n--- My Courses ---\n";
            if (s->courses.empty()) {
                cout << "No courses enrolled.\n";
            } else {
                cout << left << setw(12) << "Code" << setw(30) << "Name" 
                     << setw(10) << "Credits" << setw(25) << "Teacher" << "\n";
                cout << string(77,'-') << "\n";
                for (size_t i = 0; i < s->courses.size(); ++i) {
                    Course* c = findCourseByCode(s->courses[i]);
                    if (c) {
                        cout << left << setw(12) << c->code << setw(30) << c->name 
                             << setw(10) << c->credits << setw(25) << c->teacher << "\n";
                    }
                }
                cout << "Total Credits: " << totalCreditsForStudent(s) << "\n";
            }
            pauseAndClear();
        } else if (ch == 3) {
            cout << "\n--- Fee Status ---\n";
            cout << "Fee Status: " << (s->feePaid ? "Paid" : "Unpaid") << "\n";
            cout << "Fine Amount: " << s->fineAmount << "\n";
            pauseAndClear();
        } else if (ch == 4) {
            break;
        } else {
            cout << "Invalid choice.\n";
        }
    }
}

// ---------- Main Program ----------

int main() {
    loadCoursesFromFile();
    loadStudentsFromFile();

    cout << "==== Student Management System ====\n";
    while (true) {
        cout << "\n=== Main Menu ===\n";
        cout << "1. Admin Login\n";
        cout << "2. Student Login\n";
        cout << "3. Exit\n";
        cout << "Choose: ";
        int ch; 
        cin >> ch;
        
        if (ch == 1) {
            adminMenu();
        } else if (ch == 2) {
            studentLogin();
        } else if (ch == 3) {
            saveStudentsToFile();
            saveCoursesToFile();
            cout << "Data saved. Exiting...\n";
            break;
        } else {
            cout << "Invalid choice.\n";
        }
    }

    // Free linked list
    while (head) { 
        Student* t = head; 
        head = head->next; 
        delete t; 
    }
    return 0;
}
