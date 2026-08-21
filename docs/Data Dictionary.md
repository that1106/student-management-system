# Data Dictionary

## 1. Overview

This document describes the attributes used in the Student Management System database.

The attributes are based directly on the Class Description.

---

# 2. User

| Attribute | Data Type    | Constraint       | Description             |
| --------- | ------------ | ---------------- | ----------------------- |
| userId    | INT          | PK               | ID of the user          |
| username  | VARCHAR(50)  | UNIQUE, NOT NULL | Username used to log in |
| password  | VARCHAR(255) | NOT NULL         | Password of the account |

### Relationship

`Admin`, `Teacher`, and `Student` inherit from `User`.

---

# 3. Admin

| Attribute | Data Type    | Constraint           | Description                    |
| --------- | ------------ | -------------------- | ------------------------------ |
| adminId   | INT          | PK                   | ID of the administrator        |
| userId    | INT          | FK, UNIQUE, NOT NULL | ID of the related User         |
| fullName  | VARCHAR(100) | NOT NULL             | Full name of the administrator |
| email     | VARCHAR(100) | UNIQUE               | Email of the administrator     |

### Responsibility

The `Admin` table stores administrator information used for managing students, teachers, classes, subjects, accounts, and reports.

---

# 4. Teacher

| Attribute   | Data Type    | Constraint           | Description                  |
| ----------- | ------------ | -------------------- | ---------------------------- |
| teacherId   | INT          | PK                   | ID of the teacher            |
| userId      | INT          | FK, UNIQUE, NOT NULL | ID of the related User       |
| fullName    | VARCHAR(100) | NOT NULL             | Full name of the teacher     |
| dateOfBirth | DATE         |                      | Date of birth of the teacher |
| gender      | VARCHAR(10)  |                      | Gender of the teacher        |
| phone       | VARCHAR(20)  |                      | Phone number of the teacher  |
| email       | VARCHAR(100) | UNIQUE               | Email address of the teacher |

### Responsibility

The `Teacher` table stores information about teachers who can view classes, view students, enter grades, update grades, and view schedules.

---

# 5. Student

| Attribute   | Data Type    | Constraint           | Description                  |
| ----------- | ------------ | -------------------- | ---------------------------- |
| studentId   | INT          | PK                   | ID of the student            |
| userId      | INT          | FK, UNIQUE, NOT NULL | ID of the related User       |
| fullName    | VARCHAR(100) | NOT NULL             | Full name of the student     |
| dateOfBirth | DATE         | NOT NULL             | Date of birth of the student |
| gender      | VARCHAR(10)  |                      | Gender of the student        |
| address     | VARCHAR(255) |                      | Address of the student       |
| phone       | VARCHAR(20)  |                      | Phone number of the student  |
| email       | VARCHAR(100) | UNIQUE               | Email address of the student |

### Responsibility

The `Student` table stores personal and academic-related information of students.

---

# 6. TeachingAssignment

| Attribute    | Data Type | Constraint   | Description                   |
| ------------ | --------- | ------------ | ----------------------------- |
| assignmentId | INT       | PK           | ID of the teaching assignment |
| teacherId    | INT       | FK, NOT NULL | ID of the teacher             |
| classId      | INT       | FK, NOT NULL | ID of the class               |
| subjectId    | INT       | FK, NOT NULL | ID of the subject             |
| semester     | INT       | NOT NULL     | Semester of the assignment    |

### Responsibility

The `TeachingAssignment` table keeps track of which teacher teaches which subject and class.

---

# 7. ClassRoom

| Attribute  | Data Type    | Constraint | Description              |
| ---------- | ------------ | ---------- | ------------------------ |
| classId    | INT          | PK         | ID of the class          |
| className  | VARCHAR(100) | NOT NULL   | Name of the class        |
| grade      | INT          | NOT NULL   | Grade of the class       |
| schoolYear | VARCHAR(20)  | NOT NULL   | School year of the class |

### Responsibility

The `ClassRoom` table stores class information and supports the management of students and teachers related to the class.

---

# 8. Subject

| Attribute   | Data Type    | Constraint       | Description                      |
| ----------- | ------------ | ---------------- | -------------------------------- |
| subjectId   | INT          | PK               | ID of the subject                |
| subjectName | VARCHAR(100) | UNIQUE, NOT NULL | Name of the subject              |
| credits     | INT          | NOT NULL         | Number of credits of the subject |

### Responsibility

The `Subject` table stores and manages information about subjects.

---

# 9. Enrollment

| Attribute      | Data Type | Constraint   | Description                     |
| -------------- | --------- | ------------ | ------------------------------- |
| enrollmentId   | INT       | PK           | ID of the enrollment            |
| studentId      | INT       | FK, NOT NULL | ID of the student               |
| subjectId      | INT       | FK, NOT NULL | ID of the subject               |
| semester       | INT       | NOT NULL     | Semester of the enrollment      |
| enrollmentDate | DATE      | NOT NULL     | Date when the student registers |

### Responsibility

The `Enrollment` table manages student subject registration.

---

# 10. Grade

| Attribute    | Data Type        | Constraint   | Description                |
| ------------ | ---------------- | ------------ | -------------------------- |
| gradeId      | INT              | PK           | ID of the grade            |
| studentId    | INT              | FK, NOT NULL | ID of the student          |
| subjectId    | INT              | FK, NOT NULL | ID of the subject          |
| score        | DOUBLE PRECISION | NOT NULL     | Score of the student       |
| semester     | INT              | NOT NULL     | Semester of the grade      |
| academicYear | VARCHAR(20)      | NOT NULL     | Academic year of the grade |

### Responsibility

The `Grade` table stores and processes student scores and academic results.

---

# 11. Attribute Mapping

This section maps the attributes in the Class Description to the database fields.

| Class              | Attribute      | Database Field                  |
| ------------------ | -------------- | ------------------------------- |
| User               | userId         | User.userId                     |
| User               | username       | User.username                   |
| User               | password       | User.password                   |
| Admin              | adminId        | Admin.adminId                   |
| Admin              | fullName       | Admin.fullName                  |
| Admin              | email          | Admin.email                     |
| Teacher            | teacherId      | Teacher.teacherId               |
| Teacher            | fullName       | Teacher.fullName                |
| Teacher            | dateOfBirth    | Teacher.dateOfBirth             |
| Teacher            | gender         | Teacher.gender                  |
| Teacher            | phone          | Teacher.phone                   |
| Teacher            | email          | Teacher.email                   |
| Student            | studentId      | Student.studentId               |
| Student            | fullName       | Student.fullName                |
| Student            | dateOfBirth    | Student.dateOfBirth             |
| Student            | gender         | Student.gender                  |
| Student            | address        | Student.address                 |
| Student            | phone          | Student.phone                   |
| Student            | email          | Student.email                   |
| TeachingAssignment | teacherId      | TeachingAssignment.teacherId    |
| TeachingAssignment | classId        | TeachingAssignment.classId      |
| TeachingAssignment | subjectId      | TeachingAssignment.subjectId    |
| TeachingAssignment | assignmentId   | TeachingAssignment.assignmentId |
| TeachingAssignment | semester       | TeachingAssignment.semester     |
| ClassRoom          | classId        | ClassRoom.classId               |
| ClassRoom          | className      | ClassRoom.className             |
| ClassRoom          | grade          | ClassRoom.grade                 |
| ClassRoom          | schoolYear     | ClassRoom.schoolYear            |
| Subject            | subjectId      | Subject.subjectId               |
| Subject            | subjectName    | Subject.subjectName             |
| Subject            | credits        | Subject.credits                 |
| Enrollment         | studentId      | Enrollment.studentId            |
| Enrollment         | subjectId      | Enrollment.subjectId            |
| Enrollment         | enrollmentId   | Enrollment.enrollmentId         |
| Enrollment         | semester       | Enrollment.semester             |
| Enrollment         | enrollmentDate | Enrollment.enrollmentDate       |
| Grade              | studentId      | Grade.studentId                 |
| Grade              | subjectId      | Grade.subjectId                 |
| Grade              | gradeId        | Grade.gradeId                   |
| Grade              | score          | Grade.score                     |
| Grade              | semester       | Grade.semester                  |
| Grade              | academicYear   | Grade.academicYear              |

---

# 12. Relationship Summary

| Class      | Relationship | Related Class      | Multiplicity |
| ---------- | ------------ | ------------------ | ------------ |
| User       | Inheritance  | Admin              | 1 - 0..1     |
| User       | Inheritance  | Teacher            | 1 - 0..1     |
| User       | Inheritance  | Student            | 1 - 0..1     |
| Teacher    | Has          | TeachingAssignment | 1 - 0..*     |
| ClassRoom  | Has          | TeachingAssignment | 1 - 0..*     |
| Subject    | Has          | TeachingAssignment | 1 - 0..*     |
| ClassRoom  | Contains     | Student            | 1 - 0..*     |
| Student    | Has          | Enrollment         | 1 - 0..*     |
| Subject    | Has          | Enrollment         | 1 - 0..*     |
| Enrollment | Has          | Grade              | 1 - 0..*     |

---

# 13. Constraint Summary

| Constraint | Description                               |
| ---------- | ----------------------------------------- |
| PK         | Primary Key. Uniquely identifies a record |
| FK         | Foreign Key. References another table     |
| UNIQUE     | Prevents duplicate values                 |
| NOT NULL   | The field must have a value               |

---

# 14. Foreign Key Summary

| Table              | Foreign Key | References        |
| ------------------ | ----------- | ----------------- |
| Admin              | userId      | User.userId       |
| Teacher            | userId      | User.userId       |
| Student            | userId      | User.userId       |
| TeachingAssignment | teacherId   | Teacher.teacherId |
| TeachingAssignment | classId     | ClassRoom.classId |
| TeachingAssignment | subjectId   | Subject.subjectId |
| Enrollment         | studentId   | Student.studentId |
| Enrollment         | subjectId   | Subject.subjectId |
| Grade              | studentId   | Student.studentId |
| Grade              | subjectId   | Subject.subjectId |

---

# 15. Summary

The data dictionary follows the Class Description of the Student Management System.

The database fields use the same attribute names as the Class Diagram, such as `userId`, `studentId`, `teacherId`, `classId`, `subjectId`, `assignmentId`, `enrollmentId`, and `gradeId`.

The database structure also follows the relationships and multiplicities defined between the classes.
