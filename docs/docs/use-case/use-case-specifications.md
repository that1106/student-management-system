# Use Case Specifications

## 1. Actors

### Admin

Manages accounts, students, teachers, classes, subjects, teaching assignments, enrollments, reports, and statistics.

### Teacher

Views assigned classes and students, manages grades, and views the teaching schedule.

### Student

Views personal and academic information, registers or drops subjects, and views the study schedule.

---

## 2. UC-01: Log in

- Primary Actors: Admin, Teacher, Student
- Preconditions: The user has an active account.
- Postconditions: The user enters the correct role menu.

### Main Flow

1. The user selects Log in.
2. The system requests username and password.
3. The user enters the credentials.
4. The system validates the credentials.
5. The system identifies the user role.
6. The system displays the corresponding role menu.

### Alternative Flows

- If a required field is empty, the system requests the user to enter it again.
- If the credentials are invalid, the system displays an error message.
- If the account is locked, the system denies access.

---

## 3. UC-02: Manage Students

- Primary Actor: Admin
- Preconditions: The Admin is logged in.
- Postconditions: Student information is updated in the database.

### Main Functions

- View the student list.
- Add a student.
- Update student information.
- Delete a student.
- Search for students.
- Assign a student to a class.

### Alternative Flows

- Duplicate student IDs are rejected.
- Invalid student information is rejected.
- The system asks for confirmation before deleting a student.

---

## 4. UC-03: Manage Classes and Subjects

- Primary Actor: Admin
- Preconditions: The Admin is logged in.
- Postconditions: Class or subject information is updated.

### Main Functions

- Add, update, delete, and search for classes.
- Add, update, delete, and search for subjects.
- View the students in a class.

### Alternative Flows

- Duplicate class or subject IDs are rejected.
- A class or subject currently in use cannot be deleted without confirmation.

---

## 5. UC-04: Manage Teaching Assignments

- Primary Actor: Admin
- Supporting Actor: Teacher
- Preconditions: Teacher, class, and subject records exist.
- Postconditions: A teaching assignment is stored.

### Main Flow

1. The Admin selects a teacher.
2. The Admin selects a class and subject.
3. The Admin enters the semester and academic year.
4. The system validates the information.
5. The system stores the teaching assignment.
6. The Teacher can view the assigned class and schedule.

### Alternative Flow

- A duplicate teaching assignment is rejected.

---

## 6. UC-05: Manage Enrollments

- Primary Actors: Admin, Student
- Preconditions: Student and subject records exist.
- Postconditions: Enrollment information is updated.

### Main Functions

- Register a student for a subject.
- Drop a registered subject.
- View registered subjects.

### Alternative Flows

- Duplicate enrollment is rejected.
- The system rejects registration for an unavailable subject.

---

## 7. UC-06: Manage Grades

- Primary Actor: Teacher
- Supporting Actor: Student
- Preconditions: The Teacher is logged in and an enrollment record exists.
- Postconditions: The grade is stored or updated.

### Main Flow

1. The Teacher selects an assigned class and subject.
2. The system displays the student list.
3. The Teacher selects a student.
4. The Teacher enters or updates the score.
5. The system validates the score.
6. The system stores the grade.
7. The Student can view the grade.

### Alternative Flow

- Scores outside the range from 0 to 10 are rejected.

---

## 8. UC-07: View Academic Information

- Primary Actor: Student
- Preconditions: The Student is logged in.
- Postconditions: No system data is changed.

### Main Functions

- View personal profile.
- View grades.
- View schedule.
- View academic results.
- View registered subjects.

---

## 9. UC-08: Generate Reports and Statistics

- Primary Actor: Admin
- Preconditions: The Admin is logged in.
- Postconditions: A report is displayed or exported.

### Main Functions

- Generate student reports.
- Generate class and subject statistics.
- Generate grade statistics.
- View academic result summaries.

---

## 10. Common Functions

All actors can:

- Log in.
- Log out.
- Change their password.
