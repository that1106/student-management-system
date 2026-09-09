# SYSTEM TESTING DOCUMENT

**Project:** Student Management System (C++ Console Application + MySQL)
**Group:** Group 06[cite: 2]
**Template Standard:** Elearning 5 Template (5-Column Format)[cite: 2]

---

| Test Case ID | Function | Test Steps | Input Data | Expected Result |
| :--- | :--- | :--- | :--- | :--- |
| **TC01** | Login | 1. Run application<br>2. Select '1. Login'<br>3. Enter valid Username and Password<br>4. Press Enter | Username: admin<br>Password: Admin@123 | User logs in successfully and Admin Main Menu is displayed[cite: 2]. |
| **TC02** | Login | 1. Run application<br>2. Select '1. Login'<br>3. Enter valid Username and wrong Password<br>4. Press Enter | Username: admin<br>Password: wrongpass | Error message 'Incorrect username or password' appears, user remains on Login screen[cite: 2]. |
| **TC03** | Login | 1. On Login screen, enter invalid menu option<br>2. Press Enter | Option: abc or 99 | Error message 'Invalid option' appears, prompting re-entry without crashing[cite: 2]. |
| **TC04** | Logout | 1. Log into the system<br>2. Select '0. Logout' from main menu | Option: 0 | User session is terminated and returned to Login screen[cite: 2]. |
| **TC05** | Change Password | 1. Select 'Change Password' option<br>2. Enter current password<br>3. Enter valid new password<br>4. Confirm new password | Current: Admin@123<br>New: Admin@2026<br>Confirm: Admin@2026 | Password updated successfully and hashed in database[cite: 2]. |
| **TC06** | Change Password | 1. Select 'Change Password' option<br>2. Enter current password<br>3. Enter weak new password (< 8 chars) | Current: Admin@123<br>New: pass123<br>Confirm: pass123 | Error message requires password to have at least 8 characters, 1 uppercase, 1 lowercase, and 1 number[cite: 2]. |
| **TC07** | Account Management | 1. Select 'Add Account'<br>2. Enter an existing Username | Username: admin<br>Role: Admin | System rejects duplicate username and shows error message[cite: 2]. |
| **TC08** | Student Management - View List | 1. Select 'Student Management'<br>2. Select 'View Student List' | Option: 1 | Displays formatted list of all students on console[cite: 2]. |
| **TC09** | Student Management - Add Student | 1. Select 'Add Student'<br>2. Enter valid student details<br>3. Confirm entry | ID: ST000001<br>Name: Nguyen Van A<br>Email: ana@school.edu.vn<br>Phone: 0901234567<br>Class: SE101 | Message 'Student added successfully' displayed, record inserted into MySQL Database[cite: 2]. |
| **TC10** | Student Management - Add Student | 1. Select 'Add Student'<br>2. Enter invalid Student ID format | ID: ST123<br>Name: Nguyen Van A | System rejects input and displays format requirement error ('ST' + 6 digits)[cite: 2]. |
| **TC11** | Student Management - Add Student | 1. Select 'Add Student'<br>2. Enter Name with numeric characters | ID: ST000002<br>Name: Nguyen Van A 123 | Error message 'Full name must not contain numeric characters' appears[cite: 2]. |
| **TC12** | Student Management - Add Student | 1. Select 'Add Student'<br>2. Enter Phone number with invalid length | Phone: 09012345 (8 digits) | System prompts error requiring phone number to be exactly 10 digits[cite: 2]. |
| **TC13** | Student Management - Edit Student | 1. Select 'Update Student'<br>2. Enter Student ID<br>3. Enter new Phone and Email | ID: ST000001<br>Phone: 0987654321<br>Email: newemail@school.edu.vn | Student record is updated in database without changing Student ID[cite: 2]. |
| **TC14** | Student Management - Delete Student | 1. Select 'Delete Student'<br>2. Enter Student ID<br>3. Select 'N' at confirmation prompt | ID: ST000001<br>Confirm: N | Operation cancelled, student record remains in database[cite: 2]. |
| **TC15** | Student Management - Delete Student | 1. Select 'Delete Student'<br>2. Enter Student ID<br>3. Select 'Y' at confirmation prompt | ID: ST000001<br>Confirm: Y | Student deleted successfully, record removed from database[cite: 2]. |
| **TC16** | Student Management - Search | 1. Select 'Search Student'<br>2. Enter search keyword (Name) | Keyword: Nguyen | Displays all students whose full name contains 'Nguyen'[cite: 2]. |
| **TC17** | Class Management | 1. Select 'Add Class'<br>2. Enter duplicate Class Code | Class Code: SE101 | Error message 'Class Code already exists' displayed[cite: 2]. |
| **TC18** | Subject Management | 1. Select 'Add Subject'<br>2. Enter valid Subject details | Code: PRF192<br>Name: Programming Fundamentals<br>Credits: 3 | Subject added successfully to database[cite: 2]. |
| **TC19** | Subject Management | 1. Select 'Add Subject'<br>2. Enter invalid Credits (negative/zero) | Code: PRF192<br>Credits: -1 | Error message 'Credits must be greater than 0' displayed[cite: 2]. |
| **TC20** | Course Registration | 1. Select 'Register Course'<br>2. Select Student ID and Subject ID | Student ID: ST000001<br>Subject ID: PRF192 | Enrollment record created successfully in database[cite: 2]. |
| **TC21** | Course Registration | 1. Select 'Register Course'<br>2. Select already registered Student ID and Subject ID | Student ID: ST000001<br>Subject ID: PRF192 | Error message 'Duplicate enrollment is not allowed' displayed[cite: 2]. |
| **TC22** | Grade Management | 1. Select 'Input Grade'<br>2. Enter Enrollment ID and valid score | Enrollment ID: ENR001<br>Score: 8.5 | Grade saved successfully, database updated with score 8.5[cite: 2]. |
| **TC23** | Grade Management | 1. Select 'Input Grade'<br>2. Enter invalid score (< 0 or > 10) | Enrollment ID: ENR001<br>Score: 10.5 | Error message 'Score must be between 0.0 and 10.0' displayed[cite: 2]. |
| **TC24** | Teaching Assignment | 1. Select 'Assign Teacher'<br>2. Enter duplicate assignment for teacher and class | Teacher ID: TCH01<br>Subject ID: PRF192<br>Class Code: SE101 | System rejects assignment due to existing duplicate assignment constraint[cite: 2]. |
| **TC25** | Performance Testing | 1. Perform search query on student records database | Keyword: Nguyen | Query results displayed on console within 2 seconds[cite: 2]. |
| **TC26** | Security Testing | 1. On Login screen, input SQL injection string into username field | Username: ' OR '1'='1<br>Password: ' OR '1'='1 | System safely handles input with parameterized query, access denied[cite: 2]. |
