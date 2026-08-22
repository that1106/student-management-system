# Class Description

## 1. User

### Description

`User` is the parent class of the system. It contains the basic information and functions used for user accounts.

### Attributes

* `userId`: int - ID of the user.
* `username`: String - Username used to log in.
* `password`: String - Password of the account.

### Methods

* `login(): boolean` - Checks the user's login information.
* `logout(): void` - Logs the user out of the system.
* `changePassword(newPassword: String): void` - Changes the current password with the new provided password.

### Responsibility

The User class is mainly used for account and login management. It also provides common functions for Admin, Teacher, and Student.

---

## 2. Admin

### Description

`Admin` represents the administrator of the student management system. Admin has functions for managing the main data of the system.

### Attributes

* `adminId`: int - ID of the administrator.
* `fullName`: String - Full name of the administrator.
* `email`: String - Email of the administrator.

### Methods

* `manageStudent(): void` - Manages student information.
* `manageTeacher(): void` - Manages teacher information.
* `manageClass(): void` - Manages class information.
* `manageSubject(): void` - Manages subject information.
* `manageAccount(): void` - Manages user accounts.
* `generateReport(): void` - Generates reports from the system.

### Responsibility

The Admin class is responsible for managing students, teachers, classes, subjects, accounts, and reports.

---

## 3. Teacher

### Description

`Teacher` represents a teacher who works in the system. A teacher can view assigned classes and students and manage their grades.

### Attributes

* `teacherId`: int - ID of the teacher.
* `fullName`: String - Full name of the teacher.
* `dateOfBirth`: Date - Date of birth.
* `gender`: String - Gender of the teacher.
* `phone`: String - Phone number.
* `email`: String - Email address.

### Methods

* `viewClass(): void` - Views the classes assigned to the teacher.
* `viewStudent(): void` - Views students in the assigned classes.
* `enterGrade(enrollmentId: int, score: double): void` - Enters a grade for a specific student enrollment.
* `updateGrade(enrollmentId: int, score: double): void` - Updates a grade for a specific student enrollment.
* `viewSchedule(): void` - Views the teaching schedule.

### Responsibility

The Teacher class is used to manage teaching-related information and student grades.

---

## 4. Student

### Description

`Student` represents a student in the system. The student can view personal information and academic information.

### Attributes

* `studentId`: int - ID of the student.
* `fullName`: String - Full name of the student.
* `dateOfBirth`: Date - Date of birth.
* `gender`: String - Gender of the student.
* `address`: String - Address of the student.
* `phone`: String - Phone number.
* `email`: String - Email address.

### Methods

* `viewProfile(): void` - Views personal information.
* `viewGrade(): void` - Views grades.
* `viewSchedule(): void` - Views the class schedule.
* `viewAcademicResult(): void` - Views academic results.

### Responsibility

The Student class is used for students to view their personal information, grades, schedules, and academic results.

---

## 5. TeachingAssignment

### Description

`TeachingAssignment` stores the assignment of a teacher to a class and a subject in a specific semester and academic year.

### Attributes

* `teacherId`: int - ID of the teacher.
* `classId`: int - ID of the class.
* `subjectId`: int - ID of the subject.
* `assignmentId`: int - ID of the teaching assignment.
* `semester`: int - Semester of the assignment.
* `academicYear`: String - Academic year of the assignment.

### Methods

* `assignTeacher(): void` - Assigns a teacher to a class and subject.
* `removeAssignment(): void` - Removes an existing teaching assignment.

### Responsibility

This class is responsible for managing teacher assignments for specific subjects, classes, semesters, and academic years.

---

## 6. ClassRoom

### Description

`ClassRoom` represents a class in the school. It stores basic class information and manages students in the class.

### Attributes

* `classId`: int - ID of the class.
* `className`: String - Name of the class.
* `grade`: int - Grade of the class.
* `schoolYear`: String - School year of the class.

### Methods

* `addStudent(student: Student): void` - Adds a student to the class.
* `removeStudent(studentId: int): void` - Removes a student from the class using student ID.
* `viewStudents(): void` - Displays the students in the class.

### Responsibility

The ClassRoom class is responsible for storing class information and managing the students enrolled in the class.

---

## 7. Subject

### Description

`Subject` represents a subject taught in the school. It contains the subject name and the number of credits.

### Attributes

* `subjectId`: int - ID of the subject.
* `subjectName`: String - Name of the subject.
* `credits`: int - Number of credits of the subject.

### Methods

* `addSubject(): void` - Adds a new subject.
* `updateSubject(): void` - Updates subject information.
* `deleteSubject(): void` - Deletes a subject.
* `viewSubject(): void` - Displays subject information.

### Responsibility

The Subject class is used to store and manage information about subjects.

---

## 8. Enrollment

### Description

`Enrollment` represents the registration of a student for a subject in a specific semester and academic year.

### Attributes

* `enrollmentId`: int - ID of the enrollment.
* `studentId`: int - ID of the student.
* `subjectId`: int - ID of the subject.
* `semester`: int - Semester of the enrollment.
* `academicYear`: String - Academic year of the enrollment.
* `enrollmentDate`: Date - Date when the student registers for the subject.

### Methods

* `enroll(): void` - Registers a student for a subject.
* `drop(): void` - Cancels the student's enrollment.

### Responsibility

The Enrollment class is used to manage student subject registration across semesters and academic years.

---

## 9. Grade

### Description

`Grade` stores the grade information corresponding to a specific student enrollment.

### Attributes

* `gradeId`: int - ID of the grade record.
* `enrollmentId`: int - ID of the associated enrollment record.
* `score`: double - Score of the student.

### Methods

* `enterGrade(score: double): void` - Enters a grade for an enrollment.
* `updateGrade(score: double): void` - Updates the grade score.
* `getResult(): String` - Returns the academic result.

### Responsibility

The Grade class stores and updates score details for each enrollment.

---

# 10. Relationships Between Classes

### User, Admin, Teacher and Student

`Admin`, `Teacher`, and `Student` inherit from `User`.

```text
             User
           /  |  \
          /   |   \
       Admin Teacher Student

```

The `User` class contains common account functions, while the three subclasses have their own specific functions.

### Teacher and TeachingAssignment

A teacher can have many teaching assignments.

```text
Teacher 1 -------- 0..* TeachingAssignment

```

### ClassRoom and TeachingAssignment

A class can have many teaching assignments.

```text
ClassRoom 1 -------- 0..* TeachingAssignment

```

### Subject and TeachingAssignment

A subject can be used in many teaching assignments.

```text
Subject 1 -------- 0..* TeachingAssignment

```

### ClassRoom and Student

One class can contain many students.

```text
ClassRoom 1 -------- 0..* Student

```

### Student and Enrollment

A student can have many enrollments.

```text
Student 1 -------- 0..* Enrollment

```

### Subject and Enrollment

A subject can have many enrollments.

```text
Subject 1 -------- 0..* Enrollment

```

### Enrollment and Grade

An enrollment has at most one final grade record.

```text
Enrollment 1 -------- 0..1 Grade

```

---

# 11. Summary

The classes in the Student Management System have distinct responsibilities. `User` provides common account functions, while `Admin`, `Teacher`, and `Student` represent different user roles.

`ClassRoom` and `Subject` store structural information about classes and subjects. `TeachingAssignment` connects teachers with classes and subjects for specific semesters and academic years. `Enrollment` manages subject registration, and `Grade` references an enrollment record to store final academic evaluation results.

```

```
