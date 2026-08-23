# Database Schema

## 1. Overview

The Student Management System uses a relational database to store information about users, administrators, teachers, students, classes, subjects, teaching assignments, enrollments, and grades.

The database schema is based on the Class Diagram, Class Description, and ERD of the system.

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

| Column       | Data Type    | Constraint       | Description                                      |
| ------------ | ------------ | ---------------- | ------------------------------------------------ |
| userId       | INT          | PK               | ID of the user                                   |
| username     | VARCHAR(50)  | UNIQUE, NOT NULL | Username used to log in                          |
| passwordHash | VARCHAR(255) | NOT NULL         | Hashed password used for account authentication. |

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
| phone       | VARCHAR(15)  |                      | Phone number             |
| email       | VARCHAR(100) | UNIQUE               | Email address            |

`Teacher` inherits from `User`.

---

## 5. Student

The `Student` table stores information about students and their associated classroom.

| Column      | Data Type    | Constraint           | Description              |
| ----------- | ------------ | -------------------- | ------------------------ |
| studentId   | INT          | PK                   | ID of the student        |
| userId      | INT          | FK, UNIQUE, NOT NULL | Related User             |
| classId     | INT          | FK, NOT NULL         | ID of the class          |
| fullName    | VARCHAR(100) | NOT NULL             | Full name of the student |
| dateOfBirth | DATE         | NOT NULL             | Date of birth            |
| gender      | VARCHAR(10)  |                      | Gender of the student    |
| address     | VARCHAR(255) |                      | Address of the student   |
| phone       | VARCHAR(15)  |                      | Phone number             |
| email       | VARCHAR(100) | UNIQUE               | Email address            |

`Student` inherits from `User`.

---

## 6. ClassRoom

The `ClassRoom` table stores information about school classes.

| Column     | Data Type   | Constraint | Description              |
| ---------- | ----------- | ---------- | ------------------------ |
| classId    | INT         | PK         | ID of the class          |
| className  | VARCHAR(50) | NOT NULL   | Name of the class        |
| grade      | INT         | NOT NULL   | Grade of the class       |
| schoolYear | VARCHAR(20) | NOT NULL   | School year of the class |

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

The `TeachingAssignment` table stores the assignment of a teacher to a class and a subject in a specific semester and academic year.

| Column       | Data Type   | Constraint   | Description                    |
| ------------ | ----------- | ------------ | ------------------------------ |
| assignmentId | INT         | PK           | ID of the teaching assignment  |
| teacherId    | INT         | FK, NOT NULL | ID of the teacher              |
| classId      | INT         | FK, NOT NULL | ID of the class                |
| subjectId    | INT         | FK, NOT NULL | ID of the subject              |
| semester     | INT         | NOT NULL     | Semester of the assignment     |
| academicYear | VARCHAR(20) | NOT NULL     | Academic year of the assignment |

*Constraint:* UNIQUE(`teacherId`, `classId`, `subjectId`, `semester`, `academicYear`) to prevent duplicate assignments.

---

## 9. Enrollment

The `Enrollment` table stores the registration of a student for a subject in a specific semester and academic year.

| Column         | Data Type   | Constraint   | Description                     |
| -------------- | ----------- | ------------ | ------------------------------- |
| enrollmentId   | INT         | PK           | ID of the enrollment            |
| studentId      | INT         | FK, NOT NULL | ID of the student               |
| subjectId      | INT         | FK, NOT NULL | ID of the subject               |
| semester       | INT         | NOT NULL     | Semester of the enrollment      |
| academicYear   | VARCHAR(20) | NOT NULL     | Academic year of the enrollment |
| enrollmentDate | DATE        | NOT NULL     | Date when the student registers |

*Constraint:* UNIQUE(`studentId`, `subjectId`, `semester`, `academicYear`) to prevent duplicate enrollments.

---

## 10. Grade

The `Grade` table stores the final score corresponding to a student's enrollment record.

| Column       | Data Type | Constraint                              | Description                |
| ------------ | --------- | --------------------------------------- | -------------------------- |
| gradeId      | INT       | PK                                      | ID of the grade            |
| enrollmentId | INT       | FK, UNIQUE, NOT NULL                    | Related Enrollment record  |
| score        | DOUBLE    | NOT NULL, CHECK (score BETWEEN 0 AND 10) | Final score of the student |

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

The database represents these relationships using `userId` with 1-to-0..1 cardinality:

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

## Student and Enrollment

A student can have many enrollments.

```text
Student 1 -------- 0..* Enrollment

```

## Subject and Enrollment

A subject can have many enrollments.

```text
Subject 1 -------- 0..* Enrollment

```

## Enrollment and Grade

An enrollment record has at most one final grade record.

```text
Enrollment 1 -------- 0..1 Grade

```

---

# 12. Primary Keys

| Table | Primary Key |
| --- | --- |
| User | userId |
| Admin | adminId |
| Teacher | teacherId |
| Student | studentId |
| ClassRoom | classId |
| Subject | subjectId |
| TeachingAssignment | assignmentId |
| Enrollment | enrollmentId |
| Grade | gradeId |

---

# 13. Foreign Keys

| Table | Foreign Key | References |
| --- | --- | --- |
| Admin | userId | User(userId) |
| Teacher | userId | User(userId) |
| Student | userId | User(userId) |
| Student | classId | ClassRoom(classId) |
| TeachingAssignment | teacherId | Teacher(teacherId) |
| TeachingAssignment | classId | ClassRoom(classId) |
| TeachingAssignment | subjectId | Subject(subjectId) |
| Enrollment | studentId | Student(studentId) |
| Enrollment | subjectId | Subject(subjectId) |
| Grade | enrollmentId | Enrollment(enrollmentId) |

---

# 14. Summary

The database schema is designed based on the ERD and Class Description of the Student Management System.

The `User` table stores common account credentials (`passwordHash`). `Admin`, `Teacher`, and `Student` inherit from `User`.

`ClassRoom` stores classroom information and links directly to `Student`.

`Subject` stores subject details and is linked with teaching assignments and student enrollments.

`TeachingAssignment` connects teachers, classes, and subjects for specific semesters and academic years.

`Enrollment` manages student subject registrations across semesters and academic years.

`Grade` references `Enrollment` directly to record student final scores with valid constraints (0 to 10).
