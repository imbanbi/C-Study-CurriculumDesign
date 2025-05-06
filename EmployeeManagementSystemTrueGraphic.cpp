#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <wchar.h>
#include <windows.h>
#include <tchar.h>
#include <conio.h>


#define MAX_EMPLOYEES 100

typedef struct {
    int id;
    wchar_t name[50];
    wchar_t gender[10];
    wchar_t birth[15];
    wchar_t education[30];
    wchar_t position[30];
    wchar_t phone[20];
    wchar_t address[100];
} Employee;

Employee employees[MAX_EMPLOYEES];
int employeeCount = 0;

// 绘制主界面
void drawMainMenu() {
    setbkmode(TRANSPARENT);
    setbkcolor(WHITE);  // 设置背景为白色，初始背景颜色变黑至今没想明白为什么

    settextcolor(BLACK);  // 设置字体颜色为黑色
    TCHAR s1[] = _T("ManageSyetem");
    outtextxy(250, 50, s1);
     
    // 绘制按钮
    setfillcolor(RGB(102, 204, 255));  // 设置按钮颜色
    bar(200, 150, 600, 200);  // 绘制按钮背景
    rectangle(200, 150, 600, 200);  // 绘制按钮边框
    TCHAR s2[] = _T("insert");
    outtextxy(310, 160, s2);

    bar(200, 220, 600, 270); 
    rectangle(200, 220, 600, 270);
    TCHAR s8[] = _T("research");
    outtextxy(310, 230, s8);

    bar(200, 290, 600, 340);
    rectangle(200, 290, 600, 340); 
    TCHAR s7[] = _T("delete");
    outtextxy(310, 300, s7);

    bar(200, 360, 600, 410); 
    rectangle(200, 360, 600, 410);
    TCHAR s3[] = _T("arrangem");
    outtextxy(310, 370, s3);

    bar(200, 430, 600, 480); 
    rectangle(200, 430, 600, 480);
    TCHAR s5[] = _T("show");
    outtextxy(310, 440, s5);

    bar(200, 500, 600, 550); 
    rectangle(200, 500, 600, 550); 
    TCHAR s6[] = _T("update");
    outtextxy(310, 510, s6);

    bar(200, 570, 600, 620);  // 更新按钮
    rectangle(200, 570, 600, 620); 
    TCHAR s99[] = _T("exit");
    outtextxy(310, 580, s99);
}


// 鼠标点击检测
int detectButtonClick(int x, int y) {
    if (x >= 200 && x <= 600) {
        if (y >= 150 && y <= 200) return 1;  // 插入
        if (y >= 220 && y <= 270) return 2;  // 查询
        if (y >= 290 && y <= 340) return 3;  // 删除
        if (y >= 360 && y <= 410) return 4;  // 排序
        if (y >= 430 && y <= 480) return 5;  // 显示全部
        if (y >= 500 && y <= 550) return 6;  // 更新
        if (y >= 570 && y <= 620) return 7;  // 退出
    }
    return 0;
}



// 员工信息插入
void insertEmployeeGUI() {
    cleardevice();
    settextstyle(20, 0, _T("Consolas"));

    TCHAR prompt[] = _T("Please input the information of the employee");
    outtextxy(100, 50, prompt);

    // 输入框提示文本
    outtextxy(100, 120, _T("ID:"));
    outtextxy(100, 170, _T("NAME:"));
    outtextxy(100, 220, _T("GENDER:"));
    outtextxy(100, 270, _T("BIRTH:"));
    outtextxy(100, 320, _T("EDUCATION:"));
    outtextxy(100, 370, _T("POSITION:"));
    outtextxy(100, 420, _T("PHONE:"));
    outtextxy(100, 470, _T("ADDERSS:"));

    // 获取用户输入
    settextcolor(BLACK);
    wchar_t idStr[20];
    InputBox(idStr, 20, L"Please input the ID:");
    employees[employeeCount].id = _wtoi(idStr);

    InputBox(employees[employeeCount].name, sizeof(employees[employeeCount].name) / sizeof(wchar_t), L"Please input the name:");
    InputBox(employees[employeeCount].gender, sizeof(employees[employeeCount].gender) / sizeof(wchar_t), L"Please input the gender:");
    InputBox(employees[employeeCount].birth, sizeof(employees[employeeCount].birth) / sizeof(wchar_t), L"Please input the birth:");
    InputBox(employees[employeeCount].education, sizeof(employees[employeeCount].education) / sizeof(wchar_t), L"Please input the education:");
    InputBox(employees[employeeCount].position, sizeof(employees[employeeCount].position) / sizeof(wchar_t), L"Please input the position:");
    InputBox(employees[employeeCount].phone, sizeof(employees[employeeCount].phone) / sizeof(wchar_t), L"Please input the phone:");
    InputBox(employees[employeeCount].address, sizeof(employees[employeeCount].address) / sizeof(wchar_t), L"Please input the address:");

    employeeCount++;
    MessageBoxW(NULL, L"插入成功！", L"提示", MB_OK);
    cleardevice();  // 清除屏幕内容
}

// 删除员工
void deleteEmployeeGUI() {
    cleardevice();  
    wchar_t deleteKey[50];
    int found = 0;

    cleardevice();
    outtextxy(100, 100, _T("Please input the ID of the emplopyee you want to delete:"));
    InputBox(deleteKey, sizeof(deleteKey) / sizeof(wchar_t), L"输入编号:");

    int deleteId = _wtoi(deleteKey);

    for (int i = 0; i < employeeCount; i++) {
        if (employees[i].id == deleteId) {
            found = 1;
            for (int j = i; j < employeeCount - 1; j++) {
                employees[j] = employees[j + 1];
            }
            employeeCount--;
            MessageBoxW(NULL, L"删除成功！", L"提示", MB_OK);
            return;
        }
    }
    if (!found) {
        MessageBoxW(NULL, L"未找到该员工信息", L"提示", MB_OK);
    }
    cleardevice(); // 通过两次清除屏幕内容，避免部分功能结束后存在文本保留
}

// 排序员工
int compareEmployees(const Employee* a, const Employee* b, int key) {
    switch (key) {
    case 1: return a->id - b->id;  // 按编号
    case 2: return wcscmp(a->name, b->name);  // 按姓名
    case 3: return wcscmp(a->gender, b->gender);  // 按性别
    case 4: return wcscmp(a->birth, b->birth);  // 按出生年月
    case 5: return wcscmp(a->education, b->education);  // 按学历
    case 6: return wcscmp(a->position, b->position);  // 按职务
    case 7: return wcscmp(a->phone, b->phone);  // 按电话
    case 8: return wcscmp(a->address, b->address);  // 按住址
    default: return 0;
    }
}

void sortEmployeesByKey(int key) {
    for (int i = 0; i < employeeCount - 1; i++) {
        for (int j = 0; j < employeeCount - i - 1; j++) {
            if (compareEmployees(&employees[j], &employees[j + 1], key) > 0) {
                Employee temp = employees[j];
                employees[j] = employees[j + 1];
                employees[j + 1] = temp;
            }
        }
    }
    MessageBoxW(NULL, L"排序完成！", L"提示", MB_OK);
}

// 排序GUI调用
void sortEmployeesGUI() {
    wchar_t inputBuffer[10];  // 存储输入的字符串
    int sortKey;
    cleardevice();

    outtextxy(100, 100, _T("Select the keywords to sort by: 1 - Number, 2 - Name, 3 - Gender, 4 - Date of Birth, 5 - Education, 6 - Position, 7 - Telephone, 8 - Address"));
    
    // 使用 InputBox 获取用户输入
    InputBox(inputBuffer, sizeof(inputBuffer) / sizeof(wchar_t), L"Please input the keywords you choose:");

    // 将输入的字符串转换为整数
    sortKey = _wtoi(inputBuffer);  // 使用 _wtoi 将 wchar_t 转换为 int

    // 判断输入是否有效
    if (sortKey >= 1 && sortKey <= 8) {
        sortEmployeesByKey(sortKey);
    } else {
        MessageBoxW(NULL, L"无效选项！", L"提示", MB_OK);
    }
    cleardevice(); 
}


// 显示员工信息
#include <windows.h>  // 包含 Sleep 函数头文件

void showAllEmployeesGUI() {
    cleardevice();
    outtextxy(100, 50, _T("The list of employee information:"));

    int y = 100;
    for (int i = 0; i < employeeCount; i++) {
        wchar_t info[300];  // 使用 wchar_t 数组

        // 使用 swprintf 格式化字符串
        swprintf(info, sizeof(info) / sizeof(wchar_t),
            L"ID: %d  NAME: %s  GENDER: %s  BIRTH: %s  EDUCATION: %s  POSITION: %s  PHONE: %s  ADDRESS: %s",
            employees[i].id, employees[i].name, employees[i].gender, employees[i].birth,
            employees[i].education, employees[i].position, employees[i].phone, employees[i].address);

        // 转换为 TCHAR 类型，这里是由于编程时部分使用了仅完全适配于微软编译环境的拓展库，所以会有数据类型造成的报错，通过询问gpt获取了下方的判定代码
        TCHAR tInfo[300];

#ifdef _UNICODE
        // 如果是 Unicode 编译，TCHAR 和 wchar_t 一样，可以直接复制
        wcscpy_s(tInfo, info);
#else
        // 如果是 ANSI 编译，使用 wcstombs 将 wchar_t 转换为 char
        wcstombs(tInfo, info, sizeof(tInfo) / sizeof(TCHAR));
#endif

        // 使用 outtextxy 显示
        outtextxy(100, y, tInfo);
        y += 50;  // 每行间隔

        // 延迟 500 毫秒，确保每行显示的时间足够
        Sleep(2500);  // 毫秒
    }

    if (employeeCount == 0) {
        MessageBoxW(NULL, L"当前无员工信息！", L"提示", MB_OK);
    }
    cleardevice();
}


// 查询
int matchesCriteria(const Employee* emp, int key, const wchar_t* value) {
    if (value == NULL || value[0] == L'\0') {
        return 0;  // 如果查询值为空，则返回不匹配
    }

    wprintf(L"Comparing %d with %s\n", emp->id, value);  // 打印比较信息
    switch (key) {
    case 1: return emp->id == _wtoi(value);  // 编号
    case 2: return wcscmp(emp->name, value) == 0;  // 姓名
    case 3: return wcscmp(emp->gender, value) == 0;  // 性别
    case 4: return wcscmp(emp->birth, value) == 0;  // 出生年月
    case 5: return wcscmp(emp->education, value) == 0;  // 学历
    case 6: return wcscmp(emp->position, value) == 0;  // 职务
    case 7: return wcscmp(emp->phone, value) == 0;  // 电话
    case 8: return wcscmp(emp->address, value) == 0;  // 住址
    default: return 0;
    }
}


void searchEmployeesByKey(int key, const wchar_t* value) {
    int found = 0;
    cleardevice();
    settextstyle(20, 0, _T("Consolas"));
    outtextxy(100, 100, L"The result:");

    int y = 150;
    for (int i = 0; i < employeeCount; i++) {
        if (matchesCriteria(&employees[i], key, value)) {
            wchar_t info[300];
            swprintf(info, sizeof(info) / sizeof(wchar_t),
                L"ID: %d  NAME: %s  GENDER: %s  BIRTH: %s  EDUCATION: %s  POSITION: %s  PHONE: %s  ADDRESS: %s",
                employees[i].id, employees[i].name, employees[i].gender, employees[i].birth,
                employees[i].education, employees[i].position, employees[i].phone, employees[i].address);

            outtextxy(100, y, info);
            y += 50;
            found = 1;
            Sleep(3000);
            cleardevice();  // 清除屏幕内容
        }
    }

    if (!found) {
        MessageBoxW(NULL, L"未找到符合条件的员工", L"提示", MB_OK);
    }
}


void searchEmployeeGUI() {
    int searchKey;
    wchar_t searchKeyStr[10];  // 存储输入（宽字符）
    wchar_t searchValue[50];

    cleardevice();
    outtextxy(100, 100, _T("Select the keywords to sort by: 1 - ID, 2 - Name, 3 - Gender, 4 - Date of Birth, 5 - Education, 6 - Position, 7 - Telephone, 8 - Address"));

    // 获取输入为宽字符串
    InputBox(searchKeyStr, sizeof(searchKeyStr) / sizeof(wchar_t), L"Please input the keyword you choose:");

    // 转换为整数
    if (swscanf_s(searchKeyStr, L"%d", &searchKey) == 1 && searchKey >= 1 && searchKey <= 8) {
        InputBox(searchValue, sizeof(searchValue) / sizeof(wchar_t), L"Please input the value you want to search:");
        searchEmployeesByKey(searchKey, searchValue);
    }
    else {
        MessageBoxW(NULL, L"无效选项！", L"提示", MB_OK);
    }
    cleardevice(); 
}

//更新信息
void updateEmployeeGUI() {
    cleardevice();
    wchar_t updateKey[20];
    int found = 0;

    outtextxy(100, 100, _T("Please input the ID of the employee you want to update:"));
    InputBox(updateKey, sizeof(updateKey) / sizeof(wchar_t), L"请输入要更新的员工编号：");

    int updateId = _wtoi(updateKey);

    for (int i = 0; i < employeeCount; i++) {
        if (employees[i].id == updateId) {
            found = 1;

            // 显示当前员工信息
            wchar_t info[300];
            swprintf(info, sizeof(info) / sizeof(wchar_t),
                     L"当前信息: ID: %d, NAME: %s, GENDER: %s, BIRTH: %s, EDUCATION: %s, POSITION: %s, PHONE: %s, ADDRESS: %s",
                     employees[i].id, employees[i].name, employees[i].gender, employees[i].birth,
                     employees[i].education, employees[i].position, employees[i].phone, employees[i].address);
            MessageBoxW(NULL, info, L"当前员工信息", MB_OK);

            // 更新字段
            InputBox(employees[i].name, sizeof(employees[i].name) / sizeof(wchar_t), L"请输入新的姓名 ：");
            InputBox(employees[i].gender, sizeof(employees[i].gender) / sizeof(wchar_t), L"请输入新的性别 ：");
            InputBox(employees[i].birth, sizeof(employees[i].birth) / sizeof(wchar_t), L"请输入新的出生日期 ：");
            InputBox(employees[i].education, sizeof(employees[i].education) / sizeof(wchar_t), L"请输入新的学历 ：");
            InputBox(employees[i].position, sizeof(employees[i].position) / sizeof(wchar_t), L"请输入新的职务 ：");
            InputBox(employees[i].phone, sizeof(employees[i].phone) / sizeof(wchar_t), L"请输入新的电话 ：");
            InputBox(employees[i].address, sizeof(employees[i].address) / sizeof(wchar_t), L"请输入新的地址 ：");

            MessageBoxW(NULL, L"更新成功！", L"提示", MB_OK);
            cleardevice();
            return;
        }
    }

    if (!found) {
        MessageBoxW(NULL, L"未找到该员工信息", L"提示", MB_OK);
    }
    cleardevice();
}


int main() {
    initgraph(1400, 800);  // 初始化窗口
    int choice;
    cleardevice(); 
    while (1) {
        drawMainMenu();  // 绘制主界面

        // 检测鼠标点击事件
        MOUSEMSG msg = GetMouseMsg();
        if (msg.uMsg == WM_LBUTTONDOWN) {
            choice = detectButtonClick(msg.x, msg.y);

            switch (choice) {
            case 1:
                insertEmployeeGUI();  // 插入员工
                break;
            case 2:
                searchEmployeeGUI();  // 查询员工
                break;
            case 3:
                deleteEmployeeGUI();  // 删除员工
                break;
            case 4:
                sortEmployeesGUI();  // 排序员工
                break;
            case 5:
                showAllEmployeesGUI();  // 显示全部员工
                break;
            case 6:
                updateEmployeeGUI();  // 更新员工
                break;
            case 7:
                closegraph();  // 退出系统
                return 0;
            default:
                break;
            }
        }

    }
}
