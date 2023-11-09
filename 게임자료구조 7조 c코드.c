#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#pragma warning(disable:4996) //���־�Ʃ��� scanf ���� �����ڵ�
#define MAX_SIZE 100
#define MAP_WIDTH 40
#define MAP_HEIGHT 20
#define MAX_DODUGI 9

int score = 0;
int heart = 3;
int speedMultiplier = 1;


typedef char element[MAX_SIZE];
typedef struct ListNode {
    element data;
    struct ListNode* link;
} ListNode;

ListNode* insert(ListNode* head, element data) {
    ListNode* node = (ListNode*)malloc(sizeof(ListNode));
    strcpy(node->data, data);
    node->link = NULL;
    if (head == NULL) {
        head = node;
        head->link = head;
    }
    else {
        node->link = head->link;
        head->link = node;
        head = node;
    }
    return head;
}


int search(ListNode* head, element data) {
    if (head == NULL) {
        return 0;
    }

    ListNode* current = head;
    do {
        if (strcmp(current->data, data) == 0) {
            return 1;
        }
        current = current->link;
    } while (current != head);

    return 0;
}

void printList(ListNode* head, element word) {
    ListNode* current = head;
    printf("�����ձ� ���: ");
    do {
        printf("%s -> ", current->data);
        current = current->link;
    } while (current != head);
    printf("%s\n", word);

}

int judge(ListNode* head, element data, int lang) {
    element lastword;
    strcpy(lastword, head->data);
    if (lang == 1) { //������
        char lastcharacter = lastword[strlen(lastword) - 1];
        if (lastcharacter == data[0]) return 0; // ���ٸ�
        else return 1; // ���� �ʴٸ�
    }
    else {
        char lastcharacter1 = lastword[strlen(lastword) - 2];
        char lastcharacter2 = lastword[strlen(lastword) - 1];
        if ((lastcharacter1 == data[0]) && (lastcharacter2 == data[1])) return 0; // ���ٸ�
        else return 1; // ���� �ʴٸ�
    }

}

void playWordChainGame() {
    ListNode* head = NULL;
    int count = 0;
    element word;
    char lastcharacter;
    char choice;
    int lang = 0;

    printf("�����ձ� ������ �����մϴ�. ����� �ѱ��� �����ϼ���.(����=1/�ѱ�=2) : ");
    scanf("%d", &lang);
    if (!(lang == 1 || lang == 2))
    {
        printf("1�� 2 ���� �ٸ� ���ڸ� �Է��Ͽ����ϴ�. �ٽ� �Է����ּ���.\n");
        printf("\n");
        playWordChainGame();
    }
    printf("\n");
    while (1) {
        printf("%d�� �÷��̾��� �����Դϴ�. �ܾ �Է��ϼ���: ", (count % 2) + 1);

        scanf("%s", word);

        if (count == 0) {
            head = insert(head, word);
        }
        else {
            if (search(head, word) == 1) {
                printf("\n");
                printf("%d�� �÷��̾ �ߺ��� �ܾ� %s�� �Է��Ͽ� �й��߽��ϴ�.\n", (count % 2) + 1, word);
                printf("\n");
                printList(head, word);
                printf("\n");
                break;
            }
            if (judge(head, word, lang) == 1) {
                printf("\n");
                printf("%d�� �÷��̾ �մܾ��� ������ ���ڿ� �ٸ� ���ڷ� �����ϴ� �ܾ �Է��Ͽ� �й��߽��ϴ�.\n", (count % 2) + 1);
                printf("\n");
                printList(head, word);
                printf("\n");
                break;
            }
            head = insert(head, word);

        }
        count++;
    }

    printf("�����ձ� ������ �ٽ� �÷����Ͻðڽ��ϱ�? (Y/N) : ");
    scanf(" %c", &choice);
    printf("\n");
    if (choice == 'Y' || choice == 'y')
        playWordChainGame();  // ������ �ٽ� ����
    else
        printf("������ �����մϴ�. �����մϴ�.\n");
}

// ���� ����� xy ��ǥ�� ����ü ����
typedef struct xy {
    int x;
    int y;
} xy;

//�δ����� ���� ����ü ����
typedef struct dodugi {
    xy position;
    struct dodugi* next;
} dodugi;

dodugi* dodugiStack = NULL;

// ������ �δ����� ���� ��ǥ ����
xy g_point[9] = {
    {10, 15}, {20, 15}, {30, 15},
    {10, 10}, {20, 10}, {30, 10},
    {10, 5}, {20, 5}, {30, 5}
};

// ���� ���� ȭ��
void game_screen() {
    printf(" ---------------\n");
    printf("|               |\n");
    printf("|               |\n");
    printf("|  �δ��� ����  |\n");
    printf("|               |\n");
    printf("|               |\n");
    printf(" ---------------\n");
    printf("���� Ű : Ű�е� (1~9)\n");
    printf("    - ���� ���� ESC\n\n");
    printf("������ �����Ϸ��� �ƹ� Ű�� �����ּ���!\n");
}

// ���� �ʱ� ȭ��
void init_map(char map[MAP_HEIGHT][MAP_WIDTH]) {
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            if (i == 0 || i == MAP_HEIGHT - 1 || j == 0 || j == MAP_WIDTH - 1) {
                map[i][j] = '=';
            }
            else {
                map[i][j] = ' ';
            }
        }
    }
}

// �δ��� ���� ȭ�� ���
void update_map(char map[MAP_HEIGHT][MAP_WIDTH]) {
    init_map(map);
    dodugi* current = dodugiStack;
    while (current != NULL) {
        map[current->position.y][current->position.x] = 'X';
        current = current->next;
    }

    // ���� �� ���
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            printf("%c", map[i][j]);
        }
        printf("\n");
    }
}

//���� ���� �����ϸ� �ӵ� 1�� ����
int speed() {
    if (score != 0 && score % 50 == 0)
        speedMultiplier++;
}

//���� ��ġ �δ��� ����
void generate_dodugi() {
    int numDodugi = 0;
    dodugi* current = dodugiStack;
    while (current != NULL) {
        numDodugi++;
        current = current->next;
    }
    if (numDodugi >= MAX_DODUGI)
        return;

    int randIndex;
    xy position;
    do {
        randIndex = rand() % 9;
        position = (xy){ g_point[randIndex].x, g_point[randIndex].y };
    } while (!is_position_available(position));

    dodugi* newDodugi = (dodugi*)malloc(sizeof(dodugi));
    newDodugi->position = position;
    newDodugi->next = dodugiStack;
    dodugiStack = newDodugi;
}

// �־��� ��ġ�� �δ����� �ִ��� Ȯ��
int is_position_available(xy position) {
    dodugi* current = dodugiStack;
    while (current != NULL) {
        if (current->position.x == position.x && current->position.y == position.y)
            return 0;
        current = current->next;
    }
    return 1;
}

// ���� ����
void end_game() {
    game_over(score);
    _getch();
    exit(0); // ���α׷� ����
}

// ���� ����
void GameStart() {
    char map[MAP_HEIGHT][MAP_WIDTH];

    init_map(map);
    update_map(map);

    while (1) {
        generate_dodugi(); // �δ��� ����

        if (_kbhit()) { // Ű���� �Է� ���۸� Ȯ���ϴ� �Լ�
            int key = _getch();
            if (key == 27) { // ESC key
                break;
            }
            else if (key >= 49 && key <= 57) { // Keypad 1~9
                int enteredNumber = key - 48; // �ƽ�Ű �ڵ� ���� �̿��Ͽ� �ش��ϴ� ���� ���� ���ϴ� �ڵ�
                if (remove_dodugi(enteredNumber)) { // �δ��� ����
                    score += 10;
                    printf("����: %d\n", score);
                }
            }
        }

        Sleep(3000 / speed());  // �ӵ� ���� ����
        system("cls");
        update_map(map); // ���� �� ������Ʈ
        if (dodugi_count() >= MAX_DODUGI) { // �δ����� 9���� �� ���� ���� ����
            end_game();
            return;
        }
    }
}

// ���� ���� ȭ��
int game_over(int finalScore) {
    system("cls");
    printf(" ---------------\n");
    printf("|               |\n");
    printf("|               |\n");
    printf("|   GAME OVER   |\n");
    printf("|               |\n");
    printf("|               |\n");
    printf(" ---------------\n");
    printf("���� ����! ���� ����: %d\n", finalScore);
    printf("    - ���� ���� ESC\n\n");
}

// �δ����� ���� ����
int dodugi_count() {
    int count = 0;
    dodugi* current = dodugiStack;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

// ���Ӹʿ� �δ��� ����
int remove_dodugi(int enteredNumber) {
    dodugi* current = dodugiStack;
    dodugi* prev = NULL;

    while (current != NULL) {
        if (current->position.x == g_point[enteredNumber - 1].x && current->position.y == g_point[enteredNumber - 1].y) {
            if (prev == NULL) {
                dodugiStack = current->next;
            }
            else {
                prev->next = current->next;
            }
            free(current);
            return 1;
        }
        prev = current;
        current = current->next;
    }

    return 0;
}

typedef struct QuizListNode { // ��� Ÿ���� ����ü�� �����Ѵ�.
    char askQuiz[100];
    char quizAnswer[100];
    char quizHint[100];
    int rightOrWrong;      // ������� 0, ���߸�1, Ʋ����2
    struct QuizListNode* link;
} QuizListNode;

QuizListNode* insert_first(QuizListNode* head, const char Quiz[], const char Answer[], const char Hint[])
{
    QuizListNode* newnode = (QuizListNode*)malloc(sizeof(QuizListNode));
    if (newnode != NULL) {
        strcpy(newnode->askQuiz, Quiz);
        strcpy(newnode->quizAnswer, Answer);
        strcpy(newnode->quizHint, Hint);
        newnode->rightOrWrong = 0;
        newnode->link = head;
        head = newnode;
        return head;
    }
}

void quizSolve1(QuizListNode* head) {

    char playerAnswer[100] = { 0 };

    for (QuizListNode* currunt = head; currunt != NULL; currunt = currunt->link) {
        printf(" %s \n", currunt->askQuiz);
        printf("������ �����ΰ���? (������ �������� �ʴ� �ϳ��� �ѱ� �ܾ��Դϴ�) : ");
        scanf("%s", playerAnswer);

        if (!strcmp(currunt->quizAnswer, playerAnswer)) {
            printf("����!!\n");
            currunt->rightOrWrong = 1;
        }
        else {
            printf("�����Դϴ�\n");
            currunt->rightOrWrong = 2;
        }
    }
}

void quizSolve2(QuizListNode* head) {

    char playerAnswer[100] = { 0 };

    for (QuizListNode* currunt = head; currunt != NULL; currunt = currunt->link) {
        if (currunt->rightOrWrong == 1) {
            continue;
        }
        printf(" %s \n", currunt->askQuiz);
        printf("Ʋ�� �����̹Ƿ� Hint�� �����˴ϴ� \nHint : [ %s ]\n", currunt->quizHint);
        printf("������ �����ΰ���? (������ �������� �ʴ� �ϳ��� �ѱ� �ܾ��Դϴ�) : ");
        scanf("%s", playerAnswer);

        if (!strcmp(currunt->quizAnswer, playerAnswer)) {
            printf("����!!\n\n");
            currunt->rightOrWrong = 1;
        }
        else {
            printf("�����Դϴ�\n\n");
            currunt->rightOrWrong = 2;
        }
    }
}

int gameFinish(QuizListNode* head) {
    int finish = 0;
    int next = 0;

    for (QuizListNode* currunt = head; currunt != NULL; currunt = currunt->link) {
        if (currunt->rightOrWrong == 1) {
            finish = finish + 1;
        }
    }
    if (finish == 5) {
        printf("���� Ŭ����");
        next = getch();
        return 111;
    }
    else {
        return 222;
    }
}

int Game(void) {

    const char Quiz1[100] = { "����1: ���߿� ���� ���� ���� ? " };
    const char Quiz2[100] = { "����2: ������ ���� �����?" };
    const char Quiz3[100] = { "����3: ���� �������� ���� ����?" };
    const char Quiz4[100] = { "����4: ���󿡼� ���� ū �ڴ�?" };
    const char Quiz5[100] = { "����5: ������ ��͸� ���?" };

    const char Answer1[100] = { "�̺�" };
    const char Answer2[100] = { "�ε�" };
    const char Answer3[100] = { "���蹮��" };
    const char Answer4[100] = { "�ƽ���" };
    const char Answer5[100] = { "������" };

    const char Hint1[100] = { "* ��" };
    const char Hint2[100] = { "* ��" };
    const char Hint3[100] = { "��������" };
    const char Hint4[100] = { "* * ��" };
    const char Hint5[100] = { "�� * *" };

    char playerAnswer[100] = { 0 };

    int finish = 0;
    int next = 0;
    int a1, a2, b = 0;

    QuizListNode* head = NULL;

    head = insert_first(head, Quiz5, Answer5, Hint5);
    head = insert_first(head, Quiz4, Answer4, Hint4);
    head = insert_first(head, Quiz3, Answer3, Hint3);
    head = insert_first(head, Quiz2, Answer2, Hint2);
    head = insert_first(head, Quiz1, Answer1, Hint1);

    printf("\n ");
    printf("ù ���� �õ��Դϴ�. 3���� �õ� ������ ��� ������ ������ ���ϸ� GAME OVER\n");
    printf("ù ���� �õ��� �����Ͻ÷��� �ƹ� Ű�� ��������\n");
    next = getch();

    printf("\n ");
    quizSolve1(head);

    a1 = gameFinish(head);
    if (a1 == 222) {
        printf("��� ������ ������ ���߽��ϴ� �ٽ� �����Ͻðڽ��ϱ�? (������ȸ2)\n");
        printf("�ٽ� �����Ͻ÷��� �ƹ� Ű�� ��������\n");
        next = getch();
        system("cls");

        printf("\n ");
        printf("�� ���� �õ��Դϴ�. 3���� �õ� ������ ��� ������ ������ ���ϸ� GAME OVER\n");
        printf("�� ���� �õ��� �����Ͻ÷��� �ƹ� Ű�� ��������\n");
        next = getch();

        printf("\n ");
        quizSolve2(head);

        a2 = gameFinish(head);
        if (a2 == 222) {
            printf("��� ������ ������ ���߽��ϴ� �ٽ� �����Ͻðڽ��ϱ�? (������ȸ1)\n");
            printf("�ٽ� �����Ͻ÷��� �ƹ� Ű�� ��������\n");
            next = getch();
            system("cls");

            printf("\n ");
            printf("�� ���� �õ��Դϴ�. 3���� �õ� ������ ��� ������ ������ ���ϸ� GAME OVER\n");
            printf("�� ���� �õ��� �����Ͻ÷��� �ƹ� Ű�� ��������\n");
            next = getch();
            printf("\n ");
            quizSolve2(head);

            for (QuizListNode* currunt = head; currunt != NULL; currunt = currunt->link) {
                if (currunt->rightOrWrong == 1) {
                    finish = finish + 1;
                }
            }
            if (finish == 5) {
                printf("���� Ŭ����");
                next = getch();
            }
            else {
                printf("���� ����");
                next = getch();
            }
        }
    }
}

void game_explanation()
{
    system("cls");
    printf("���Ӽ���\n");
    printf("1.�����ձ� ����\n");
    printf("  1�� �÷��̾�� 2�� �÷��̾ ������ ���� �����ձ⸦ ����\n");
    printf("  �ߺ��� �ܾ �Է��ϴ� ����� �й�\n\n\n");
    printf("2. �δ��� ����\n");
    printf("  9���� �δ����� Ű�е带 �̿��Ͽ� ��� ���\n");
    printf("  �δ��� 9���� ��� ���� ���� ����\n\n\n");
    printf("3. ���� ����\n");
    printf("  ���� 5�� ���߱⸦ ����\n");
    printf("  �ش� ��� ���߸� �ش� ����� ����\n");
    printf("  ������ ����� ��Ʈ�� �����ǰ� 5���� ��� ���� ���߸� Ŭ����\n");
    printf("  3���� �õ� �ȿ� 5���� ��� ���� ������ ���ϸ� ���� ����\n\n");
    printf("������ �����ҷ��� 1, 2, 3���� �ϳ��� �Է��Ͻÿ�");
}

void game_start()
{
    int sel = 0;

    while (1) {
        scanf("%d", &sel);
        if (sel == 1) {
            system("cls");
            playWordChainGame();
            break;
        }
        else if (sel == 2) {
            system("cls");
            srand(time(NULL));
            game_screen();
            _getch();
            system("cls");
            GameStart();
            break;
        }
        else if (sel == 3) {
            system("cls");
            Game();
            break;
        }

        else if (sel == 4)
        {
            game_explanation();
            char key = getch();
        }

        else {
            printf("  ���� 1,2,3 �� �ϳ��� �����ϼ���\n");
        }
    }
}


int main(void) {
    printf("\n\n\n\n\n");
    printf("                            �������  ����  ��    ��          ��    ���������     �� ��    ������   ��   \n");
    printf("                            ��       ��  ��    ��          ��           ��     �� ��   ��     ��  ��   \n");
    printf("                            ��       ��  ��    ��          ��           ��   ���� ��    ������   ��   \n");
    printf("                            ��       ��  ��    ��          ��           ��     �� ��     ��������� \n");
    printf("                            ��       ��  ��    ��          ��           ��     �� ��     ��      �� \n");
    printf("                            ����������  ��    ���������   ��           ��     �� ��     ��������� \n\n\n");
    printf("\n");
    printf("1. �����ձ� ����\n");
    printf("2. �δ��� ����\n");
    printf("3. �������\n");
    printf("4. ���Ӽ����б�\n");
    printf("� ������ �Ͻðڽ��ϱ�? ���ڸ� �����ϼ���. ���� ������ �ʿ��ϸ� 4���� �������� : \n");

    game_start();
}
