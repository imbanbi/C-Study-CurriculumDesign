#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR 256
#define MAX_CODE_LENGTH 256
#define MAX_BITS_PER_LINE 50  

// 哈夫曼树节点结构体
typedef struct HuffmanNode {
    char character;
    int frequency;
    struct HuffmanNode *left, *right;
} HuffmanNode;

HuffmanNode* root = NULL;  // 哈夫曼树根节点

// 哈夫曼编码表
char* huffmanCode[MAX_CHAR];

// 创建哈夫曼树节点
HuffmanNode* createNode(char character, int frequency) {
    HuffmanNode *newNode = (HuffmanNode *)malloc(sizeof(HuffmanNode));
    newNode->character = character;
    newNode->frequency = frequency;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// 合并两个哈夫曼节点
HuffmanNode* mergeNodes(HuffmanNode *left, HuffmanNode *right) {
    HuffmanNode *merged = createNode('\0', left->frequency + right->frequency);
    merged->left = left;
    merged->right = right;
    return merged;
}

// 根据字符集生成哈夫曼树
void buildHuffmanTree(char characters[], int frequencies[], int n) {
    HuffmanNode* nodes[MAX_CHAR];
    int nodeCount = 0;
    
    // 按频率排序
    for(int j = 0; j < n - 1; j++){
        for(int i = j; i < n && frequencies[i] > frequencies[i + 1]; i++){
            int tempfrequencies = frequencies[i];
            frequencies[i] = frequencies[i + 1];
            frequencies[i + 1] = tempfrequencies;
            char tempcharacters = characters[i];
            characters[i] = characters[i + 1];
            characters[i + 1] = tempcharacters;
        }
    }

    // 创建哈夫曼树节点并放入数组
    for (int i = 0; i < n; i++) {
        nodes[nodeCount++] = createNode(characters[i], frequencies[i]);
    }

    // 构建哈夫曼树
    while (nodeCount > 1) {
        // 选择频率最小的两个节点
        HuffmanNode *left = nodes[0];
        HuffmanNode *right = nodes[1];

        // 合并并更新节点数组
        nodes[0] = mergeNodes(left, right);
        for (int i = 1; i < nodeCount - 1; i++) {
            nodes[i] = nodes[i + 1];
        }
        nodeCount--;

        // 按频率排序节点数组
        for (int i = nodeCount - 1; i > 0 && nodes[i]->frequency < nodes[i - 1]->frequency; i--) {
            HuffmanNode *temp = nodes[i];
            nodes[i] = nodes[i - 1];
            nodes[i - 1] = temp;
        }
    }

    // 返回哈夫曼树根节点
    root = nodes[0];
}

// 生成哈夫曼编码
void generateHuffmanCodes(HuffmanNode* node, char* code, int length) {
    if (node == NULL) return;

    // 如果是叶子节点，存储编码
    if (node->character != '\0') {
        code[length] = '\0';
        // 多次报错，查询后需无符号字符更为方便
        huffmanCode[(unsigned char)node->character] = strdup(code);
        return;
    }

    // 向左走
    code[length] = '0';
    generateHuffmanCodes(node->left, code, length + 1);

    // 向右走
    code[length] = '1';
    generateHuffmanCodes(node->right, code, length + 1);
}

// 打印哈夫曼树（使用凹入式）
void printHuffmanTreeIndented(HuffmanNode* node, int level) {
    if (node == NULL) return;

    // 每级缩进Tab
    for (int i = 0; i < level; i++) {
        printf("\t");
    }

    // 打印节点信息
    if (node->character != '\0') {
        printf("字符: '%c', 频率: %d\n", node->character, node->frequency);
    } else {
        printf("生成节点, 频率: %d\n", node->frequency);
    }

    // 递归打印左子树和右子树
    printHuffmanTreeIndented(node->left, level + 1);
    printHuffmanTreeIndented(node->right, level + 1);
}


// 保存哈夫曼树到文件
void saveHuffmanTreeRecursive(HuffmanNode* node, FILE* file) {
    if (node == NULL) return;
    if (node->character != '\0') {
        fwrite(&node->character, sizeof(char), 1, file);
        fwrite(&node->frequency, sizeof(int), 1, file);
    }
    saveHuffmanTreeRecursive(node->left, file);
    saveHuffmanTreeRecursive(node->right, file);
}

void saveHuffmanTreeToFile(FILE *file) {
    if (root == NULL) return;
    saveHuffmanTreeRecursive(root, file);
}


// 编码文件
void encodeFile(const char *inputFile, const char *outputFile) {
    FILE *inFile = fopen(inputFile, "r");
    FILE *outFile = fopen(outputFile, "w");

    if (!inFile || !outFile) {
        printf("无法打开文件\n");
        return;
    }

    char ch;
    while ((ch = fgetc(inFile)) != EOF) {
        fputs(huffmanCode[(unsigned char)ch], outFile);
    }

    fclose(inFile);
    fclose(outFile);
}

// 译码文件
void decodeFile(const char *inputFile, const char *outputFile) {
    FILE *inFile = fopen(inputFile, "r");
    FILE *outFile = fopen(outputFile, "w");

    if (!inFile || !outFile) {
        printf("无法打开文件\n");
        return;
    }

    HuffmanNode *current = root;
    char ch;
    while ((ch = fgetc(inFile)) != EOF) {
        if (ch == '0') {
            current = current->left;
        } else {
            current = current->right;
        }

        if (current->left == NULL && current->right == NULL) {
            fputc(current->character, outFile);
            current = root;
        }
    }

    fclose(inFile);
    fclose(outFile);
}

// 打印编码文件
void printEncodedFile(const char *inputFile) {
    FILE *inFile = fopen(inputFile, "r");
    if (!inFile) {
        printf("无法打开文件 %s\n", inputFile);
        return;
    }

    char ch;
    int count = 0;

    // 逐个字符读取文件
    while ((ch = fgetc(inFile)) != EOF) {
        if (ch >= '0' && ch <= '9') { 
            printf("%c", ch);
            count++;

            // 每50个字符换行
            if (count % MAX_BITS_PER_LINE == 0) {
                printf("\n");
            }
        }
    }

    // 文件读取完后检查是否有剩余的字符，打印剩下的字符
    if (count % MAX_BITS_PER_LINE != 0) {
        printf("\n");
    }

    fclose(inFile);
}

// 打印哈夫曼树
void printHuffmanTree() {
    if (root != NULL) {
        printf("哈夫曼树:\n");
        printHuffmanTreeIndented(root, 0);
    }
}

// 统计字符频率并生成哈夫曼树
void generateHuffmanTreeFromFile(const char *inputFile) {
    FILE *file = fopen("C:\\Code\\.vscode\\DataStructure.c\\CurriculumDesign\\count.txt", "r");
    if (file == NULL) {
    perror("打开文件失败");
    exit(1);  // 退出程序
    }


    int frequencies[MAX_CHAR] = {0};
    char characters[MAX_CHAR];
    int n = 0;
    char ch;
    // 读取文件并统计字符频率
    while ((ch = fgetc(file)) != EOF) {
        int found = 0;
        
        // 查找字符是否已经记录过
        for (int i = 0; i < n; i++) {
            if (characters[i] == ch) {
                frequencies[i]++;
                found = 1;
                break;
            }
        }

        // 如果是新的字符，添加到 characters 数组中，并初始化频率
        if (!found && n < MAX_CHAR) {
            characters[n] = ch;
            frequencies[n] = 1;
            n++;
        }
    }
    /*
    // 统计字符频率,这里通过搜索获知unsigned char格式可以用于便利地利用数组统计字符出现频率，这里进行尝试，树格式正确但输出频率全0，还未想明白为什么
    while ((ch = fgetc(file)) != EOF) {
        if (frequencies[(unsigned char)ch] == 0) {
            characters[n++] = ch;
        }
        frequencies[(unsigned char)ch]++;
    }
    */
    fclose(file);

    // 构建哈夫曼树
    buildHuffmanTree(characters, frequencies, n);
    char code[MAX_CODE_LENGTH];
    generateHuffmanCodes(root, code, 0);

    printf("根据 %s 文件生成哈夫曼树成功！\n", inputFile);
}

// 菜单
void menu() {
    while (1) {
        printf("\n选择功能:\n");
        printf("I: 初始化\n");
        printf("E: 编码\n");
        printf("D: 译码\n");
        printf("P: 打印编码文件\n");
        printf("T: 打印哈夫曼树\n");
        printf("Z: 根据文件统计字符频率并生成哈夫曼树\n");
        printf("Q: 退出\n");

        char choice;
        scanf(" %c", &choice);  // 此处数据测试时出现多次问题，实在没想明白故询问gpt——空格用于跳过前一个输入的换行符

        switch (choice) {
            case 'I': {
                // 读取字符和频率初始化哈夫曼树
                int n;
                printf("请输入字符集大小: ");
                scanf("%d", &n);

                char characters[n];
                int frequencies[n];

                printf("请输入字符及其频率:\n");
                for (int i = 0; i < n; i++) {
                    printf("字符 %d: ", i + 1);
                    scanf(" %c", &characters[i]);
                    printf("频率: ");
                    scanf("%d", &frequencies[i]);
                }

                buildHuffmanTree(characters, frequencies, n);
                char code[MAX_CODE_LENGTH];
                generateHuffmanCodes(root, code, 0);
                printf("哈夫曼树初始化成功！\n");
                break;
            }
            case 'E':
                encodeFile("C:\\Code\\.vscode\\DataStructure.c\\CurriculumDesign\\ToBeTran.txt", "CodeFile.txt");
                printf("编码完成，保存在 CodeFile.txt\n");
                break;
            case 'D':
                decodeFile("CodeFile.txt", "TextFile.txt");
                printf("译码完成，保存在 TextFile.txt\n");
                break;
            case 'P':
                printEncodedFile("CodeFile.txt");
                printf("编码文件打印完成。\n");
                break;
            case 'T':
                printHuffmanTree();
                printf("哈夫曼树打印完成。\n");
                break;
            case 'Z':
                generateHuffmanTreeFromFile("count.txt");
                break;
            case 'Q':
                printf("退出程序。\n");
                return;
            default:
                printf("无效选项，重新输入。\n");
        }
    }
}

int main() {
    menu();
    return 0;
}
