C++ Console Interface Design

1. Overview

The Student Management System uses a text-based console interface developed in C++17. Users interact with the system by entering numbered menu options and form values from the keyboard.

The interface supports three user roles:

Admin

Teacher

Student

After a successful login, the system identifies the user's role and displays the corresponding menu.

2. General Navigation Flow

Start
→ Login Screen
→ Validate Credentials
→ Identify User Role
→ Admin Menu / Teacher Menu / Student Menu
→ Select Function
→ Display Result or Message
→ Return to Current Menu
→ Log out
→ Login Screen

Navigation rules:

Enter a number to select a function.

Enter 0 in a submenu to return to the previous menu.

Enter 0 in a role menu to log out and return to the Login Screen.

Invalid options do not close the application.

After an error, the system displays a message and requests the input again.

3. Login Screen

================================
 STUDENT MANAGEMENT SYSTEM
================================

 Username: ____________
 Password: ____________

 [1] Login
 [0] Exit

 Choose an option: _

Inputs

Username

Password

Menu option

Processing

Check that the username and password are not empty.

Compare the credentials with the stored user account.

Identify whether the user is an Admin, Teacher, or Student.

Results

Valid Admin account: display Admin Menu.

Valid Teacher account: display Teacher Menu.

Valid Student account: display Student Menu.

Invalid credentials: remain on the Login Screen.

Option 0: exit the application.

Messages

Login successful.
Invalid username or password. Please try again.
Username and password must not be empty.

4. Admin Menu

========== ADMIN MENU ==========

 1. Manage Accounts
 2. Manage Students
 3. Manage Teachers
 4. Manage Classes
 5. Manage Subjects
 6. Manage Teaching Assignments
 7. Manage Enrollments
 8. Generate Reports and Statistics
 0. Log out

 Choose an option: _

Admin Navigation

Option

Destination

1

Manage Accounts

2

Manage Students

3

Manage Teachers

4

Manage Classes

5

Manage Subjects

6

Manage Teaching Assignments

7

Manage Enrollments

8

Reports and Statistics

0

Login Screen

5. Manage Students Screen

======= MANAGE STUDENTS ========

 1. Add Student
 2. Update Student
 3. Delete Student
 4. Search Student
 5. View Student List
 0. Back to Admin Menu

 Choose an option: _

Add Student Form

========== ADD STUDENT ==========

 Student ID:     __________
 Full Name:      __________
 Date of Birth:  __________
 Gender:         __________
 Address:        __________
 Phone:          __________
 Email:          __________
 Class ID:       __________

 [1] Save student
 [0] Cancel

Student Messages

Student added successfully.
Student updated successfully.
Student deleted successfully.
Student ID already exists.
Student not found.
Required fields must not be empty.

6. Teacher Menu

========= TEACHER MENU =========

 1. View Assigned Classes
 2. View Student List
 3. Enter Grade
 4. Update Grade
 5. View Schedule
 0. Log out

 Choose an option: _

Teacher Navigation

Option

Destination

1

Assigned Classes Screen

2

Student List Screen

3

Enter Grade Screen

4

Update Grade Screen

5

Teacher Schedule Screen

0

Login Screen

7. Enter Grade Screen

=========== ENTER GRADE =========

 Enrollment ID: __________
 Student Name:  __________
 Subject:       __________
 Score (0-10): __________

 [1] Save grade
 [0] Back to Teacher Menu

Grade Validation

Enrollment ID must exist.

Score must be a number.

Score must be between 0 and 10.

A final grade must not be duplicated for the same enrollment.

Grade Messages

Grade saved successfully.
Grade updated successfully.
Enrollment not found.
Invalid score. Score must be between 0 and 10.
A grade already exists for this enrollment.

8. Student Menu

========= STUDENT MENU =========

 1. View Profile
 2. Register Subject
 3. Drop Subject
 4. View Grades
 5. View Schedule
 6. View Academic Results
 0. Log out

 Choose an option: _

Student Navigation

Option

Destination

1

Student Profile Screen

2

Subject Registration Screen

3

Drop Subject Screen

4

Grade List Screen

5

Student Schedule Screen

6

Academic Results Screen

0

Login Screen

9. Academic Results Screen

====== ACADEMIC RESULTS ========

 Student ID:     __________
 Semester:       __________
 Academic Year:  __________

 Subject       Score       Result
 --------------------------------
 Mathematics   8.5         Passed
 Programming   9.0         Passed

 [0] Back to Student Menu

If no result exists, the system displays:

No academic result was found for the selected semester.

10. Common Success Messages

Operation completed successfully.
Data saved successfully.
Data updated successfully.
Data deleted successfully.
Registration completed successfully.

11. Common Error Messages

Invalid option. Please choose again.
Invalid input. Please try again.
Required fields must not be empty.
The requested record was not found.
Duplicate data is not allowed.
Database connection failed.
The operation could not be completed.

12. Input Validation Rules

Input

Validation rule

Menu option

Must be an integer shown in the current menu

Username

Must not be empty

Password

Must not be empty

Student, class, subject, or enrollment ID

Must be a positive integer and must exist when referenced

Full name

Must not be empty

Email

Must use a valid email format

Phone

Must contain valid phone-number characters

Date of birth

Must be a valid date

Semester

Must be a supported semester number

Score

Must be a number between 0 and 10

13. Interface Design Principles

Every screen has a clear title.

Menu options are numbered consistently.

Option 0 always means Back, Log out, Cancel, or Exit depending on the current screen.

Success and error messages are displayed immediately after an operation.

Sensitive password values are not printed in reports or normal output.

The system returns users to a safe menu after completing an operation.

14. Required Diagram Files

The editable menu-navigation diagram and exported images are stored in:

docs/ui-ux/console-interface.drawio
docs/ui-ux/login-screen.png
docs/ui-ux/admin-menu.png
docs/ui-ux/teacher-menu.png
docs/ui-ux/student-menu.png
docs/ui-ux/validation-messages.png
