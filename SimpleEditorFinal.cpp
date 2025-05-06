#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 320  
#define ACTIVEMAXLEN 100 // 活区行数
#define PAGE_SIZE 20  // 每页20行

typedef struct {
    char line[MAX_LINE_LENGTH];
} Line;

typedef struct {
    Line lines[ACTIVEMAXLEN];
    int lineCount;  // 当前活区中的行数
    int firstLineNumber;  // 活区起始行号
} ActiveArea;

// 打开文件并返回文件指针
FILE* openFile(const char *filename, const char *mode) {
    FILE *file = fopen(filename, mode);
    if (!file) {
        printf("无法打开文件: %s\n", filename);
        exit(1);
    }
    return file;
}

// 打印活区内容，每20行一页
void printActiveArea(ActiveArea *area) {
    int i, pageCount = 0;
    while (pageCount * PAGE_SIZE < area->lineCount) {
        for (i = pageCount * PAGE_SIZE; i < (pageCount + 1) * PAGE_SIZE && i < area->lineCount; i++) {
            printf("%4d %s\n", area->firstLineNumber + i, area->lines[i].line);
        }
        pageCount++;
        if (pageCount * PAGE_SIZE < area->lineCount) {
            char choice;
            printf("按任意键翻页显示，或按'q'退出该功能: ");
            choice = getchar();
            getchar();  // 防止换行符引起输入问题
            if (choice == 'q' || choice == 'Q') {
                break;
            }
        }
    }
}

// 插入新行到活区指定位置
void insertLine(ActiveArea *area, int lineNumber, char *newLine) {
    if (lineNumber < area->firstLineNumber || lineNumber > area->firstLineNumber + area->lineCount) {
        printf("插入位置非法\n");
        return;
    }

    // 如果活区已满，在output中存储第一行
    if (area->lineCount == ACTIVEMAXLEN) {
        FILE *outFile = openFile("output.txt", "a");
        fprintf(outFile, "%4d %s\n", area->firstLineNumber, area->lines[0].line);
        fclose(outFile);

        // 后续行向上移动
        for (int i = 1; i < ACTIVEMAXLEN; i++) {
            area->lines[i - 1] = area->lines[i];
        }
        area->lineCount--;
    }

    // 在指定位置插入新行
    int insertIndex = lineNumber - area->firstLineNumber;
    for (int i = area->lineCount; i > insertIndex; i--) {
        area->lines[i] = area->lines[i - 1];
    }

    strncpy(area->lines[insertIndex].line, newLine, MAX_LINE_LENGTH);
    area->lineCount++;

    // 执行完插入后自动显示活区
    printActiveArea(area);
}

// 删除活区指定行
void deleteLine(ActiveArea *area, int lineNumberStart, int lineNumberEnd) {
    if (lineNumberStart < area->firstLineNumber || lineNumberEnd >= area->firstLineNumber + area->lineCount) {
        printf("删除行号超出范围\n");
        return;
    }

    int startIndex = lineNumberStart - area->firstLineNumber;
    int endIndex = lineNumberEnd - area->firstLineNumber;

    for (int i = startIndex; i <= endIndex; i++) {
        for (int j = i; j < area->lineCount - 1; j++) {
            area->lines[j] = area->lines[j + 1];
        }
        area->lineCount--;
    }

    // 执行完删除后自动显示活区
    printActiveArea(area);
}

// 切换活区，保存当前活区并读取下一段内容
void switchActiveArea(ActiveArea *area, FILE *inputFile) {
    FILE *outFile = openFile("output.txt", "a");

    // 将当前活区写入文件
    for (int i = 0; i < area->lineCount; i++) {
        fprintf(outFile, "%4d %s\n", area->firstLineNumber + i, area->lines[i].line);
    }
    fclose(outFile);

    // 清空当前活区
    area->lineCount = 0;
    area->firstLineNumber += ACTIVEMAXLEN;

    // 从输入文件中读取下一段活区，避免无限读取空内容
    int linesRead = 0;
    for (int i = 0; i < ACTIVEMAXLEN; i++) {
        if (fgets(area->lines[i].line, MAX_LINE_LENGTH, inputFile) != NULL) {
            area->lineCount++;
            linesRead++;
        } else {
            break;  // 到达文件末尾，停止读取
        }
    }

    // 如果没有读取到任何行，说明文件已结束
    if (linesRead == 0) {
         printActiveArea(area);
        printf("文件已读取完毕，无法继续切换活区。\n");
    } else {
        // 执行切换后自动显示活区
        printActiveArea(area);
    }
}

// 检查插入行号是否合法
int validateInsertCommand(int lineNumber, ActiveArea *area) {
    if (lineNumber < area->firstLineNumber || lineNumber > area->firstLineNumber + area->lineCount) {
        printf("插入行号非法！应在[%d, %d]之间。\n", area->firstLineNumber, area->firstLineNumber + area->lineCount);
        return 0;  // 无效
    }
    return 1;  // 有效
}

// 检查删除行号是否合法
int validateDeleteCommand(int lineNumStart, int lineNumEnd, ActiveArea *area) {
    if (lineNumStart < area->firstLineNumber || lineNumEnd >= area->firstLineNumber + area->lineCount) {
        printf("删除行号超出范围！\n");
        return 0;  // 无效
    }
    if (lineNumStart > lineNumEnd) {
        printf("删除命令格式错误，起始行号应小于结束行号。\n");
        return 0;  // 无效
    }
    return 1;  // 有效
}

// 串替换函数（存在问题，尚未考虑超出行中字符最大数量）
void replaceStringInLine(ActiveArea *area, int lineNum, const char *oldStr, const char *newStr) {
    if (lineNum < area->firstLineNumber || lineNum >= area->firstLineNumber + area->lineCount) {
        printf("行号超出范围！\n");
        return;
    }
    
    // 查找并替换
    char *pos = strstr(area->lines[lineNum - area->firstLineNumber].line, oldStr);// 查找是否为子串
    if (pos) {
        // 替换内容
        char temp[MAX_LINE_LENGTH];
        strncpy(temp, area->lines[lineNum - area->firstLineNumber].line, pos - area->lines[lineNum - area->firstLineNumber].line);
        temp[pos - area->lines[lineNum - area->firstLineNumber].line] = '\0';
        strcat(temp, newStr);
        strcat(temp, pos + strlen(oldStr));
        strncpy(area->lines[lineNum - area->firstLineNumber].line, temp, MAX_LINE_LENGTH);
        printf("替换成功。\n");
    } else {
        printf("未找到要替换的内容。\n");
    }

    // 执行完替换后自动显示活区
    printActiveArea(area);
}

// 在活区中查找模式
void matchPatternInActiveArea(ActiveArea *area, const char *pattern) {
    printf("匹配包含 \"%s\" 的行：\n", pattern);
    int found = 0;
    for (int i = 0; i < area->lineCount; i++) {
        if (strstr(area->lines[i].line, pattern)) {
            printf("行 %d: %s\n", area->firstLineNumber + i, area->lines[i].line);
            found = 1;
        }
    }
    if (!found) {
        printf("未找到匹配的内容。\n");
    }
}

void mainMenu(ActiveArea *area, FILE *inputFile) {
    char command;
    char line[MAX_LINE_LENGTH];
    int lineNumStart, lineNumEnd;

    while (1) {
        printf("\n请输入命令:\n");
        printf("i  插入行\n");
        printf("d  删除行\n");
        printf("n  切换活区\n");
        printf("p  显示活区\n");
        printf("S  串替换\n");
        printf("m  模式匹配\n");
        printf("q  退出\n");
        printf("请输入命令: ");
        scanf(" %c", &command);
        getchar();  // 读取掉换行符

        switch (command) {
            case 'i':
                printf("请输入插入行号：");
                scanf("%d", &lineNumStart);
                getchar();  // 读取掉换行符
                printf("请输入要插入的内容：");
                fgets(line, MAX_LINE_LENGTH, stdin);
                if (validateInsertCommand(lineNumStart, area)) {
                    insertLine(area, lineNumStart, line);
                }
                break;
            case 'd':
                printf("请输入删除起始行号：");
                scanf("%d", &lineNumStart);
                getchar();  // 读取掉换行符
                printf("请输入删除结束行号：");
                scanf("%d", &lineNumEnd);
                if (validateDeleteCommand(lineNumStart, lineNumEnd, area)) {
                    deleteLine(area, lineNumStart, lineNumEnd);
                }
                break;
            case 'n':
                switchActiveArea(area, inputFile);
                break;
            case 'p':
                printActiveArea(area);
                break;
            case 'S':
                printf("请依次输入要替换目标和替换内容（以回车键相隔）:");
                char oldStr[MAX_LINE_LENGTH], newStr[MAX_LINE_LENGTH];
                scanf("%s%s", oldStr, newStr);
                printf("请输入行号：");
                scanf("%d", &lineNumStart);
                replaceStringInLine(area, lineNumStart, oldStr, newStr);
                break;
            case 'm':
                printf("请输入查找的目标：");
                scanf("%s", line);
                matchPatternInActiveArea(area, line);
                break;
            case 'q':
                return;
            default:
                printf("无效命令\n");
                break;
        }
    }
}


int main() {
    FILE *inputFile = openFile("input.txt", "r");  // 此处可改为output.txt实现同名文件多次修改

    ActiveArea area;
    area.firstLineNumber = 1;
    area.lineCount = 0;

    // 读取初始活区
    for (int i = 0; i < ACTIVEMAXLEN && fgets(area.lines[i].line, MAX_LINE_LENGTH, inputFile) != NULL; i++) {
        area.lineCount++;
    }

    // 启动编辑程序
    mainMenu(&area, inputFile);

    fclose(inputFile);
    return 0;
}
