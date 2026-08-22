# Database Schema

## 1. Overview

The Student Management System uses a relational database to store information about users, administrators, teachers, students, classes, subjects, teaching assignments, enrollments, and grades.

The database schema is based on the Class Diagram and Class Description of the system.

The main tables are:

* User
* Admin
* Teacher
* Student
* ClassRoom
* Subject
* TeachingAssignment
* Enrollment
* Grade

---

## 2. User

The `User` table stores common account information used by Admin, Teacher, and Student.

| Column   | Data Type    | Constraint       | Description             |
| -------- | ------------ | ---------------- | ----------------------- |
| userId   | INT          | PK               | ID of the user          |
| username | VARCHAR(50)  | UNIQUE, NOT NULL | Username used to log in |
| password | VARCHAR(255) | NOT NULL         | Password of the account |

---

## 3. Admin

The `Admin` table stores information about administrators.

| Column   | Data Type    | Constraint           | Description                    |
| -------- | ------------ | -------------------- | ------------------------------ |
| adminId  | INT          | PK                   | ID of the administrator        |
| userId   | INT          | FK, UNIQUE, NOT NULL | Related User                   |
| fullName | VARCHAR(100) | NOT NULL             | Full name of the administrator |
| email    | VARCHAR(100) | UNIQUE               | Email of the administrator     |

`Admin` inherits from `User`.

---

## 4. Teacher

The `Teacher` table stores information about teachers.

| Column      | Data Type    | Constraint           | Description              |
| ----------- | ------------ | -------------------- | ------------------------ |
| teacherId   | INT          | PK                   | ID of the teacher        |
| userId      | INT          | FK, UNIQUE, NOT NULL | Related User             |
| fullName    | VARCHAR(100) | NOT NULL             | Full name of the teacher |
| dateOfBirth | DATE         |                      | Date of birth            |
| gender      | VARCHAR(10)  |                      | Gender of the teacher    |
| phone       | VARCHAR(20)  |                      | Phone number             |
| email       | VARCHAR(100) | UNIQUE               | Email address            |

`Teacher` inherits from `User`.

---

## 5. Student

The `Student` table stores information about students.

| Column      | Data Type    | Constraint           | Description              |
| ----------- | ------------ | -------------------- | ------------------------ |
| studentId   | INT          | PK                   | ID of the student        |
| userId      | INT          | FK, UNIQUE, NOT NULL | Related User             |
| fullName    | VARCHAR(100) | NOT NULL             | Full name of the student |
| dateOfBirth | DATE         | NOT NULL             | Date of birth            |
| gender      | VARCHAR(10)  |                      | Gender of the student    |
| address     | VARCHAR(255) |                      | Address of the student   |
| phone       | VARCHAR(20)  |                      | Phone number             |
| email       | VARCHAR(100) | UNIQUE               | Email address            |

`Student` inherits from `User`.

---

## 6. ClassRoom

The `ClassRoom` table stores information about school classes.

| Column     | Data Type    | Constraint | Description              |
| ---------- | ------------ | ---------- | ------------------------ |
| classId    | INT          | PK         | ID of the class          |
| className  | VARCHAR(100) | NOT NULL   | Name of the class        |
| grade      | INT          | NOT NULL   | Grade of the class       |
| schoolYear | VARCHAR(20)  | NOT NULL   | School year of the class |

---

## 7. Subject

The `Subject` table stores information about subjects.

| Column      | Data Type    | Constraint       | Description         |
| ----------- | ------------ | ---------------- | ------------------- |
| subjectId   | INT          | PK               | ID of the subject   |
| subjectName | VARCHAR(100) | UNIQUE, NOT NULL | Name of the subject |
| credits     | INT          | NOT NULL         | Number of credits   |

---

## 8. TeachingAssignment

The `TeachingAssignment` table stores the assignment of a teacher to a class and a subject in a specific semester.

| Column       | Data Type | Constraint   | Description                   |
| ------------ | --------- | ------------ | ----------------------------- |
| assignmentId | INT       | PK           | ID of the teaching assignment |
| teacherId    | INT       | FK, NOT NULL | ID of the teacher             |
| classId      | INT       | FK, NOT NULL | ID of the class               |
| subjectId    | INT       | FK, NOT NULL | ID of the subject             |
| semester     | INT       | NOT NULL     | Semester of the assignment    |

---

## 9. Enrollment

The `Enrollment` table stores the registration of a student for a subject.

| Column         | Data Type | Constraint   | Description                     |
| -------------- | --------- | ------------ | ------------------------------- |
| enrollmentId   | INT       | PK           | ID of the enrollment            |
| studentId      | INT       | FK, NOT NULL | ID of the student               |
| subjectId      | INT       | FK, NOT NULL | ID of the subject               |
| semester       | INT       | NOT NULL     | Semester of the enrollment      |
| enrollmentDate | DATE      | NOT NULL     | Date when the student registers |

---

## 10. Grade

The `Grade` table stores the grade information of a student for a subject.

| Column       | Data Type        | Constraint   | Description                |
| ------------ | ---------------- | ------------ | -------------------------- |
| gradeId      | INT              | PK           | ID of the grade            |
| studentId    | INT              | FK, NOT NULL | ID of the student          |
| subjectId    | INT              | FK, NOT NULL | ID of the subject          |
| score        | DOUBLE PRECISION | NOT NULL     | Score of the student       |
| semester     | INT              | NOT NULL     | Semester of the grade      |
| academicYear | VARCHAR(20)      | NOT NULL     | Academic year of the grade |

---

# 11. Relationships Between Tables

## User, Admin, Teacher and Student

`Admin`, `Teacher`, and `Student` inherit from `User`.

```text
             User
           /  |  \
          /   |   \
       Admin Teacher Student
```

The database represents these relationships using `userId`.

```text
User 1 -------- 0..1 Admin
User 1 -------- 0..1 Teacher
User 1 -------- 0..1 Student
```

## Teacher and TeachingAssignment

A teacher can have many teaching assignments.

```text
Teacher 1 -------- 0..* TeachingAssignment
```

## ClassRoom and TeachingAssignment

A class can have many teaching assignments.

```text
ClassRoom 1 -------- 0..* TeachingAssignment
```

## Subject and TeachingAssignment

A subject can be used in many teaching assignments.

```text
Subject 1 -------- 0..* TeachingAssignment
```

## ClassRoom and Student

One class can contain many students.

```text
ClassRoom 1 -------- 0..* Student
```

The `Student` table therefore needs a `classId` foreign key.

---

## Student and Enrollment

A student can have many enrollments.

```text
Student 1 -------- 0..* Enrollment
```

---

## Subject and Enrollment

A subject can have many enrollments.

```text
Subject 1 -------- 0..* Enrollment
```

---

## Enrollment and Grade

An enrollment can have grade information.

```text
Enrollment 1 -------- 0..* Grade
```

The `Grade` table keeps `studentId` and `subjectId` because these attributes are defined in the `Grade` class.

---

# 12. Primary Keys

| Table              | Primary Key  |
| ------------------ | ------------ |
| User               | userId       |
| Admin              | adminId      |
| Teacher            | teacherId    |
| Student            | studentId    |
| ClassRoom          | classId      |
| Subject            | subjectId    |
| TeachingAssignment | assignmentId |
| Enrollment         | enrollmentId |
| Grade              | gradeId      |

---

# 13. Foreign Keys

| Table              | Foreign Key | References         |
| ------------------ | ----------- | ------------------ |
| Admin              | userId      | User(userId)       |
| Teacher            | userId      | User(userId)       |
| Student            | userId      | User(userId)       |
| Student            | classId     | ClassRoom(classId) |
| TeachingAssignment | teacherId   | Teacher(teacherId) |
| TeachingAssignment | classId     | ClassRoom(classId) |
| TeachingAssignment | subjectId   | Subject(subjectId) |
| Enrollment         | studentId   | Student(studentId) |
| Enrollment         | subjectId   | Subject(subjectId) |
| Grade              | studentId   | Student(studentId) |
| Grade              | subjectId   | Subject(subjectId) |

---

# 14. Summary

The database schema is designed based on the Class Description of the Student Management System.

The `User` table stores common account information. `Admin`, `Teacher`, and `Student` contain their own specific information.

`ClassRoom` stores class information and is related to students and teaching assignments.

`Subject` stores subject information and is related to teaching assignments and enrollments.

`TeachingAssignment` connects teachers, classes, and subjects.

`Enrollment` manages student subject registration.

`Grade` stores student scores and academic information.
