# 🧪 SYSTEM TESTING DOCUMENTATION

<p align="center">
  <img src="https://img.shields.io/badge/Language-C%2B%2B17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white" alt="C++" />
  <img src="https://img.shields.io/badge/Database-MySQL_8.0-4479A1?style=for-the-badge&logo=mysql&logoColor=white" alt="MySQL" />
  <img src="https://img.shields.io/badge/Testing_Status-26%2F26_Passed-2ea44f?style=for-the-badge&logo=github" alt="Status" />
</p>

> [!NOTE]
> * **Project Name:** Student Management System (C++ Console Application + MySQL)
> * **Development Group:** Group 06
> * **Branch:** `feature/testing-document`
> * **Document Standard:** Elearning 5-Column Testing Matrix

---

## 📊 Test Execution Summary

| Metric | Details | Percentage | Status |
| :--- | :---: | :---: | :---: |
| **Total Test Cases** | **26** | 100% | 📋 Total |
| **Passed Cases** | **26** | 100% | ✅ Passed |
| **Failed Cases** | **0** | 0% | ❌ Failed |
| **Pass Rate** | **100%** | -- | 🚀 Excellent |

---

## 📑 Table of Contents
- [🔐 Module 1: Authentication & Account Management](#-module-1-authentication--account-management)
- [👨‍🎓 Module 2: Student Management](#-module-2-student-management)
- [📚 Module 3: Class, Subject & Course Registration](#-module-3-class-subject--course-registration)
- [📝 Module 4: Grade & Teaching Assignment](#-module-4-grade--teaching-assignment)
- [🛡️ Module 5: Performance & Security Testing](#-module-5-performance--security-testing)

---

## 🔐 Module 1: Authentication & Account Management

| ID | Function | Test Steps | Input Data | Expected Result | Status |
| :---: | :--- | :--- | :--- | :--- | :---: |
| **TC01** | Login | 1. Run application<br>2. Select '1. Login'<br>3. Enter valid Username and Password<br>4. Press Enter | **Username:** `admin`<br>**Password:** `Admin@123` | User logs in successfully and Admin Main Menu is displayed. | ✅ PASSED |
| **TC02** | Login | 1. Run application<br>2. Select '1. Login'<br>3. Enter valid Username and wrong Password<br>4. Press Enter | **Username:** `admin`<br>**Password:** `wrongpass` | Error message *'Incorrect username or password'* appears, user remains on Login screen. | ✅ PASSED |
| **TC03** | Login | 1. On Login screen, enter invalid menu option<br>2. Press Enter | **Option:** `abc` or `99` | Error message *'Invalid option'* appears, prompting re-entry without crashing. | ✅ PASSED |
| **TC04** | Logout | 1. Log into the system<br>2. Select '0. Logout' from main menu | **Option:** `0` | User session is terminated and returned to Login screen. | ✅ PASSED |
| **TC05** | Change Password | 1. Select 'Change Password'<br>2. Enter current password<br>3. Enter valid new password<br>4. Confirm new password | **Current:** `Admin@123`<br>**New:** `Admin@2026`<br>**Confirm:** `Admin@2026` | Password updated successfully and hashed in database. | ✅ PASSED |
| **TC06** | Change Password | 1. Select 'Change Password'<br>2. Enter current password<br>3. Enter weak new password (< 8 chars) | **Current:** `Admin@123`<br>**New:** `pass123`<br>**Confirm:** `pass123` | Error message requires password to have at least 8 characters, 1 uppercase, 1 lowercase, and 1 number. | ✅ PASSED |
| **TC07** | Account Management | 1. Select 'Add Account'<br>2. Enter an existing Username | **Username:** `admin`<br>**Role:** `Admin` | System rejects duplicate username and shows error message. | ✅ PASSED |

---

## 👨‍🎓 Module 2: Student Management

| ID | Function | Test Steps | Input Data | Expected Result | Status |
| :---: | :--- | :--- | :--- | :--- | :---: |
| **TC08** | View List | 1. Select 'Student Management'<br>2. Select 'View Student List' | **Option:** `1` | Displays formatted list of all students on console. | ✅ PASSED |
| **TC09** | Add Student | 1. Select 'Add Student'<br>2. Enter valid student details<br>3. Confirm entry | **ID:** `ST000001`<br>**Name:** `Nguyen Van A`<br>**Email:** `ana@school.edu.vn`<br>**Phone:** `0901234567`<br>**Class:** `SE101` | Message *'Student added successfully'* displayed, record inserted into MySQL Database. | ✅ PASSED |
| **TC10** | Add Student | 1. Select 'Add Student'<br>2. Enter invalid Student ID format | **ID:** `ST123`<br>**Name:** `Nguyen Van A` | System rejects input and displays format requirement error (*'ST'* + 6 digits). | ✅ PASSED |
| **TC11** | Add Student | 1. Select 'Add Student'<br>2. Enter Name with numeric characters | **ID:** `ST000002`<br>**Name:** `Nguyen Van A 123` | Error message *'Full name must not contain numeric characters'* appears. | ✅ PASSED |
| **TC12** | Add Student | 1. Select 'Add Student'<br>2. Enter Phone number with invalid length | **Phone:** `09012345` (8 digits) | System prompts error requiring phone number to be exactly 10 digits. | ✅ PASSED |
| **TC13** | Edit Student | 1. Select 'Update Student'<br>2. Enter Student ID<br>3. Enter new Phone and Email | **ID:** `ST000001`<br>**Phone:** `0987654321`<br>**Email:** `newemail@school.edu.vn` | Student record is updated in database without changing Student ID. | ✅ PASSED |
| **TC14** | Delete Student | 1. Select 'Delete Student'<br>2. Enter Student ID<br>3. Select 'N' at confirmation prompt | **ID:** `ST000001`<br>**Confirm:** `N` | Operation cancelled, student record remains in database. | ✅ PASSED |
| **TC15** | Delete Student | 1. Select 'Delete Student'<br>2. Enter Student ID<br>3. Select 'Y' at confirmation prompt | **ID:** `ST000001`<br>**Confirm:** `Y` | Student deleted successfully, record removed from database. | ✅ PASSED |
| **TC16** | Search Student | 1. Select 'Search Student'<br>2. Enter search keyword (Name) | **Keyword:** `Nguyen` | Displays all students whose full name contains 'Nguyen'. | ✅ PASSED |

---

## 📚 Module 3: Class, Subject & Course Registration

| ID | Function | Test Steps | Input Data | Expected Result | Status |
| :---: | :--- | :--- | :--- | :--- | :---: |
| **TC17** | Class Management | 1. Select 'Add Class'<br>2. Enter duplicate Class Code | **Class Code:** `SE101` | Error message *'Class Code already exists'* displayed. | ✅ PASSED |
| **TC18** | Subject Management | 1. Select 'Add Subject'<br>2. Enter valid Subject details | **Code:** `PRF192`<br>**Name:** `Programming Fundamentals`<br>**Credits:** `3` | Subject added successfully to database. | ✅ PASSED |
| **TC19** | Subject Management | 1. Select 'Add Subject'<br>2. Enter invalid Credits (negative/zero) | **Code:** `PRF192`<br>**Credits:** `-1` | Error message *'Credits must be greater than 0'* displayed. | ✅ PASSED |
| **TC20** | Course Registration | 1. Select 'Register Course'<br>2. Select Student ID and Subject ID | **Student ID:** `ST000001`<br>**Subject ID:** `PRF192` | Enrollment record created successfully in database. | ✅ PASSED |
| **TC21** | Course Registration | 1. Select 'Register Course'<br>2. Select already registered Student ID and Subject ID | **Student ID:** `ST000001`<br>**Subject ID:** `PRF192` | Error message *'Duplicate enrollment is not allowed'* displayed. | ✅ PASSED |

---

## 📝 Module 4: Grade & Teaching Assignment

| ID | Function | Test Steps | Input Data | Expected Result | Status |
| :---: | :--- | :--- | :--- | :--- | :---: |
| **TC22** | Grade Management | 1. Select 'Input Grade'<br>2. Enter Enrollment ID and valid score | **Enrollment ID:** `ENR001`<br>**Score:** `8.5` | Grade saved successfully, database updated with score 8.5. | ✅ PASSED |
| **TC23** | Grade Management | 1. Select 'Input Grade'<br>2. Enter invalid score (< 0 or > 10) | **Enrollment ID:** `ENR001`<br>**Score:** `10.5` | Error message *'Score must be between 0.0 and 10.0'* displayed. | ✅ PASSED |
| **TC24** | Teaching Assignment | 1. Select 'Assign Teacher'<br>2. Enter duplicate assignment for teacher and class | **Teacher ID:** `TCH01`<br>**Subject ID:** `PRF192`<br>**Class Code:** `SE101` | System rejects assignment due to existing duplicate assignment constraint. | ✅ PASSED |

---

## 🛡️ Module 5: Performance & Security Testing

| ID | Function | Test Steps | Input Data | Expected Result | Status |
| :---: | :--- | :--- | :--- | :--- | :---: |
| **TC25** | Performance Testing | 1. Perform search query on student records database | **Keyword:** `Nguyen` | Query results displayed on console within 2 seconds. | ✅ PASSED |
| **TC26** | Security Testing | 1. On Login screen, input SQL injection string into username field | **Username:** `' OR '1'='1`<br>**Password:** `' OR '1'='1` | System safely handles input with parameterized query, access denied. | ✅ PASSED |

---

> [!TIP]
> **Sign-off Approval:**
> * **Lead QA/Tester:** Group 06 Test Team
> * **Last Updated:** September 2026
				
				
				

