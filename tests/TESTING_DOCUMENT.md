# SYSTEM TESTING DOCUMENT
**Project:** Student Management System (C++ Console Application + MySQL)[cite: 2]
**Group:** Group 06[cite: 2]
**Architecture:** 4-Layer Architecture (Presentation, Business Logic, Repository/DAO, Database)[cite: 2]
**Target Branch:** `develop`[cite: 2]

---

## I. TỔNG QUAN & PHẠM VI KIỂM THỬ (TESTING SCOPE & STRATEGY)

* **Mục tiêu:** Kiểm thử toàn bộ các chức năng từ FR-01 đến FR-18 và các yêu cầu phi chức năng từ NFR-01 đến NFR-05[cite: 2].
* **Môi trường kiểm thử:** C++17 Console Application, MySQL Server, MySQL Connector/C++[cite: 2].
* **Phương pháp:** Black-box Testing, Boundary Value Analysis, Exception Handling & Security Verification[cite: 2].

---

## II. MA TRẬN KỊCH BẢN KIỂM THỬ (TEST CASES MATRIX)

### 1. Phân hệ Xác thực & Quản lý Tài khoản (Auth & Account Management)

| Test Case ID | Requirement | Kịch bản kiểm thử | Các bước thực hiện | Kết quả kỳ vọng | Kết quả thực tế | Trạng thái |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **TC_LOG_01** | FR-12[cite: 2] | Đăng nhập Admin thành công | 1. Chạy app.<br>2. Nhập Username Admin.<br>3. Nhập Password đúng. | Đăng nhập thành công, điều hướng đến ADMIN MENU[cite: 2]. | *Chưa test* | Pending |
| **TC_LOG_02** | FR-12[cite: 2] | Đăng nhập thất bại do sai Password | 1. Nhập Username đúng.<br>2. Nhập Password sai. | Báo lỗi *"Incorrect username or password"*, giữ nguyên màn hình Login[cite: 2]. | *Chưa test* | Pending |
| **TC_LOG_03** | FR-12[cite: 2] | Nhập Lựa chọn Menu không hợp lệ | 1. Tại Login Screen, nhập chuỗi `abc` hoặc số `99`. | Báo lỗi *"Invalid input"*, yêu cầu nhập lại, không văng app[cite: 2]. | *Chưa test* | Pending |
| **TC_LGO_01** | FR-13[cite: 2] | Đăng xuất khỏi hệ thống | 1. Chọn menu `0. Log out`. | Trở về màn hình Login Screen, hủy phiên làm việc hiện tại[cite: 2]. | *Chưa test* | Pending |
| **TC_PWD_01** | FR-14[cite: 2] | Đổi mật khẩu hợp lệ | 1. Nhập Mật khẩu cũ đúng.<br>2. Nhập Mật khẩu mới: `Admin@2026`. | Cập nhật thành công, mật khẩu mới được lưu dưới dạng Hash trong DB[cite: 2]. | *Chưa test* | Pending |
| **TC_PWD_02** | FR-14[cite: 2] | Đổi mật khẩu không đủ độ mạnh | 1. Nhập Mật khẩu mới: `admin123`. | Báo lỗi mật khẩu phải >= 8 ký tự, gồm chữ hoa, chữ thường và số[cite: 2]. | *Chưa test* | Pending |
| **TC_ACC_01** | FR-15[cite: 2] | Tạo tài khoản trùng Username | 1. Thêm tài khoản mới với Username đã tồn tại trong DB. | Hệ thống từ chối và báo lỗi Username bị trùng lặp[cite: 2]. | *Chưa test* | Pending |

---

### 2. Phân hệ Quản lý Sinh viên (Student Management)

| Test Case ID | Requirement | Kịch bản kiểm thử | Các bước thực hiện | Kết quả kỳ vọng | Kết quả thực tế | Trạng thái |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **TC_STU_01** | FR-01[cite: 2] | Xem danh sách sinh viên | 1. Chọn Student Management -> View List. | Hiển thị bảng danh sách dạng Console chuẩn xác[cite: 2]. | *Chưa test* | Pending |
| **TC_STU_02** | FR-02[cite: 2] | Thêm sinh viên thành công | 1. ID: `ST000001`<br>2. Name: `Nguyen Van A`<br>3. Phone: `0901234567`<br>4. Class Code hợp lệ. | Báo *"Student added successfully"*, dữ liệu ghi vào MySQL[cite: 2]. | *Chưa test* | Pending |
| **TC_STU_03** | FR-02 & FR-06[cite: 2] | Thêm Sinh viên sai định dạng ID | 1. Nhập ID: `ST123` (thiếu 6 chữ số) hoặc `12345678`. | Báo lỗi ID phải khớp định dạng `ST` + 6 chữ số[cite: 2]. | *Chưa test* | Pending |
| **TC_STU_04** | FR-02 & FR-06[cite: 2] | Thêm Sinh viên tên chứa số | 1. Nhập Name: `Nguyen Van A 123`. | Báo lỗi *"Full name must not contain numeric characters"*[cite: 2]. | *Chưa test* | Pending |
| **TC_STU_05** | FR-02 & FR-06[cite: 2] | Thêm Sinh viên SĐT sai độ dài | 1. Nhập Phone: `09012345` (8 số) hoặc `090123456789`. | Từ chối lưu, yêu cầu SĐT phải đủ 10 chữ số[cite: 2]. | *Chưa test* | Pending |
| **TC_STU_06** | FR-03[cite: 2] | Cập nhật thông tin sinh viên | 1. Nhập ID `ST000001`.<br>2. Cập nhật Email mới. | Cập nhật thông tin thành công, giữ nguyên Mã sinh viên[cite: 2]. | *Chưa test* | Pending |
| **TC_STU_07** | FR-04[cite: 2] | Hủy thao tác Xóa sinh viên | 1. Chọn Delete ID `ST000001`.<br>2. Tại prompt `(Y/N)`, chọn `N`. | Hủy thao tác, giữ nguyên dữ liệu sinh viên trong DB[cite: 2]. | *Chưa test* | Pending |
| **TC_STU_08** | FR-04[cite: 2] | Xác nhận Xóa sinh viên | 1. Chọn Delete ID `ST000001`.<br>2. Tại prompt `(Y/N)`, chọn `Y`. | Báo xóa thành công, bản ghi bị xóa khỏi DB[cite: 2]. | *Chưa test* | Pending |
| **TC_STU_09** | FR-05[cite: 2] | Tìm kiếm sinh viên theo Tên | 1. Nhập từ khóa tìm kiếm: `"Nguyen"`. | Hiển thị tất cả sinh viên có tên chứa chữ `"Nguyen"`[cite: 2]. | *Chưa test* | Pending |

---

### 3. Phân hệ Quản lý Lớp, Môn học & Đăng ký (Class, Subject & Enrollment)

| Test Case ID | Requirement | Kịch bản kiểm thử | Các bước thực hiện | Kết quả kỳ vọng | Kết quả thực tế | Trạng thái |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **TC_CLS_01** | FR-07[cite: 2] | Thêm Lớp học trùng Mã lớp | 1. Nhập Class Code đã tồn tại trong DB. | Báo lỗi trùng Class Code và ngăn chặn ghi DB[cite: 2]. | *Chưa test* | Pending |
| **TC_SUB_01** | FR-08[cite: 2] | Thêm Môn học hợp lệ | 1. Nhập Mã môn, Tên môn, Số tín chỉ = `3`. | Thêm thành công môn học vào DB[cite: 2]. | *Chưa test* | Pending |
| **TC_SUB_02** | FR-08[cite: 2] | Thêm Môn học số tín chỉ âm | 1. Nhập Số tín chỉ = `-1` hoặc `0`. | Báo lỗi số tín chỉ phải lớn hơn 0[cite: 2]. | *Chưa test* | Pending |
| **TC_ENR_01** | FR-09[cite: 2] | Đăng ký môn học thành công | 1. Chọn Student ID và Subject ID hợp lệ. | Bản ghi lưu thành công vào bảng `Enrollment`[cite: 2]. | *Chưa test* | Pending |
| **TC_ENR_02** | FR-09[cite: 2] | Đăng ký trùng môn học | 1. Chọn Student ID và Subject ID đã đăng ký rồi. | Báo lỗi *"Duplicate enrollment is not allowed"*[cite: 2]. | *Chưa test* | Pending |

---

### 4. Phân hệ Điểm số & Phân công (Grades & Teaching Assignment)

| Test Case ID | Requirement | Kịch bản kiểm thử | Các bước thực hiện | Kết quả kỳ vọng | Kết quả thực tế | Trạng thái |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **TC_GRD_01** | FR-10[cite: 2] | GV Nhập điểm hợp lệ (0.0 - 10.0) | 1. Nhập Enrollment ID.<br>2. Nhập Score: `8.5`. | Báo *"Grade saved successfully"*, DB cập nhật `8.5`[cite: 2]. | *Chưa test* | Pending |
| **TC_GRD_02** | FR-10[cite: 2] | GV Nhập điểm vượt giới hạn | 1. Nhập Score: `10.5` hoặc `-1.0`. | Báo lỗi *"Score must be between 0 and 10"*[cite: 2]. | *Chưa test* | Pending |
| **TC_GRD_03** | FR-10[cite: 2] | GV Nhập điểm ngoài phân công | 1. GV chọn nhập điểm lớp/môn chưa được phân công. | Từ chối thao tác, báo vi phạm quyền phân công[cite: 2]. | *Chưa test* | Pending |
| **TC_TCH_01** | FR-17[cite: 2] | Phân công Giảng viên trùng lặp | 1. Phân công GV A dạy Môn B tại Lớp C lần thứ 2. | Hệ thống từ chối do vi phạm ràng buộc phân công[cite: 2]. | *Chưa test* | Pending |

---

### 5. Kiểm thử Phi chức năng (Non-Functional Requirements)

| Test Case ID | Requirement | Kịch bản kiểm thử | Các bước thực hiện | Kết quả kỳ vọng | Kết quả thực tế | Trạng thái |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **TC_NFR_01** | NFR-03[cite: 2] | Hiệu năng truy vấn | 1. Thực hiện tìm kiếm/xem danh sách. | Kết quả hiển thị ra console trong thời gian **≤ 2 giây**[cite: 2]. | *Chưa test* | Pending |
| **TC_NFR_02** | NFR-04[cite: 2] | Chống SQL Injection | 1. Nhập `' OR '1'='1` vào các ô Input. | Xử lý an toàn bằng Parameterized Query, không lộ DB[cite: 2]. | *Chưa test* | Pending |
| **TC_NFR_03** | NFR-02[cite: 2] | Tính toàn vẹn giao dịch | 1. Gián đoạn khi nhập liệu lỗi giữa chừng. | Đã Rollback giao dịch, không ghi dữ liệu rác vào MySQL[cite: 2]. | *Chưa test* | Pending |
