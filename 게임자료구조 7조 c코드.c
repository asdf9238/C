#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#pragma warning(disable:4996) //비주얼스튜디오 scanf 오류 무시코드
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
    printf("끝말잇기 기록: ");
    do {
        printf("%s -> ", current->data);
        current = current->link;
    } while (current != head);
    printf("%s\n", word);

}

int judge(ListNode* head, element data, int lang) {
    element lastword;
    strcpy(lastword, head->data);
    if (lang == 1) { //영어라면
        char lastcharacter = lastword[strlen(lastword) - 1];
        if (lastcharacter == data[0]) return 0; // 같다면
        else return 1; // 같지 않다면
    }
    else {
        char lastcharacter1 = lastword[strlen(lastword) - 2];
        char lastcharacter2 = lastword[strlen(lastword) - 1];
        if ((lastcharacter1 == data[0]) && (lastcharacter2 == data[1])) return 0; // 같다면
        else return 1; // 같지 않다면
    }

}

void playWordChainGame() {
    ListNode* head = NULL;
    int count = 0;
    element word;
    char lastcharacter;
    char choice;
    int lang = 0;

    printf("끝말잇기 게임을 시작합니다. 영어와 한글을 선택하세요.(영어=1/한글=2) : ");
    scanf("%d", &lang);
    if (!(lang == 1 || lang == 2))
    {
        printf("1과 2 외의 다른 글자를 입력하였습니다. 다시 입력해주세요.\n");
        printf("\n");
        playWordChainGame();
    }
    printf("\n");
    while (1) {
        printf("%d번 플레이어의 차례입니다. 단어를 입력하세요: ", (count % 2) + 1);

        scanf("%s", word);

        if (count == 0) {
            head = insert(head, word);
        }
        else {
            if (search(head, word) == 1) {
                printf("\n");
                printf("%d번 플레이어가 중복된 단어 %s를 입력하여 패배했습니다.\n", (count % 2) + 1, word);
                printf("\n");
                printList(head, word);
                printf("\n");
                break;
            }
            if (judge(head, word, lang) == 1) {
                printf("\n");
                printf("%d번 플레이어가 앞단어의 마지막 글자와 다른 글자로 시작하는 단어를 입력하여 패배했습니다.\n", (count % 2) + 1);
                printf("\n");
                printList(head, word);
                printf("\n");
                break;
            }
            head = insert(head, word);

        }
        count++;
    }

    printf("끝말잇기 게임을 다시 플레이하시겠습니까? (Y/N) : ");
    scanf(" %c", &choice);
    printf("\n");
    if (choice == 'Y' || choice == 'y')
        playWordChainGame();  // 게임을 다시 시작
    else
        printf("게임을 종료합니다. 감사합니다.\n");
}

// 게임 실행시 xy 좌표의 구조체 생성
typedef struct xy {
    int x;
    int y;
} xy;

//두더지에 대한 구조체 생성
typedef struct dodugi {
    xy position;
    struct dodugi* next;
} dodugi;

dodugi* dodugiStack = NULL;

// 게임판 두더지에 대한 좌표 지정
xy g_point[9] = {
    {10, 15}, {20, 15}, {30, 15},
    {10, 10}, {20, 10}, {30, 10},
    {10, 5}, {20, 5}, {30, 5}
};

// 게임 시작 화면
void game_screen() {
    printf(" ---------------\n");
    printf("|               |\n");
    printf("|               |\n");
    printf("|  두더지 게임  |\n");
    printf("|               |\n");
    printf("|               |\n");
    printf(" ---------------\n");
    printf("게임 키 : 키패드 (1~9)\n");
    printf("    - 게임 종료 ESC\n\n");
    printf("게임을 시작하려면 아무 키나 눌러주세요!\n");
}

// 게임 초기 화면
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

// 두더지 게임 화면 출력
void update_map(char map[MAP_HEIGHT][MAP_WIDTH]) {
    init_map(map);
    dodugi* current = dodugiStack;
    while (current != NULL) {
        map[current->position.y][current->position.x] = 'X';
        current = current->next;
    }

    // 게임 판 출력
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            printf("%c", map[i][j]);
        }
        printf("\n");
    }
}

//일정 점수 도달하면 속도 1씩 증가
int speed() {
    if (score != 0 && score % 50 == 0)
        speedMultiplier++;
}

//랜덤 위치 두더지 생성
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

// 주어진 위치에 두더지가 있는지 확인
int is_position_available(xy position) {
    dodugi* current = dodugiStack;
    while (current != NULL) {
        if (current->position.x == position.x && current->position.y == position.y)
            return 0;
        current = current->next;
    }
    return 1;
}

// 게임 종료
void end_game() {
    game_over(score);
    _getch();
    exit(0); // 프로그램 종료
}

// 게임 시작
void GameStart() {
    char map[MAP_HEIGHT][MAP_WIDTH];

    init_map(map);
    update_map(map);

    while (1) {
        generate_dodugi(); // 두더지 생성

        if (_kbhit()) { // 키보드 입력 버퍼를 확인하는 함수
            int key = _getch();
            if (key == 27) { // ESC key
                break;
            }
            else if (key >= 49 && key <= 57) { // Keypad 1~9
                int enteredNumber = key - 48; // 아스키 코드 값을 이용하여 해당하는 숫자 값을 구하는 코드
                if (remove_dodugi(enteredNumber)) { // 두더지 제거
                    score += 10;
                    printf("점수: %d\n", score);
                }
            }
        }

        Sleep(3000 / speed());  // 속도 점점 증가
        system("cls");
        update_map(map); // 게임 판 업데이트
        if (dodugi_count() >= MAX_DODUGI) { // 두더지가 9마리 다 차면 게임 종료
            end_game();
            return;
        }
    }
}

// 게임 종료 화면
int game_over(int finalScore) {
    system("cls");
    printf(" ---------------\n");
    printf("|               |\n");
    printf("|               |\n");
    printf("|   GAME OVER   |\n");
    printf("|               |\n");
    printf("|               |\n");
    printf(" ---------------\n");
    printf("게임 종료! 최종 점수: %d\n", finalScore);
    printf("    - 게임 종료 ESC\n\n");
}

// 두더지의 개수 세기
int dodugi_count() {
    int count = 0;
    dodugi* current = dodugiStack;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

// 게임맵에 두더지 제거
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

typedef struct QuizListNode { // 노드 타입을 구조체로 정의한다.
    char askQuiz[100];
    char quizAnswer[100];
    char quizHint[100];
    int rightOrWrong;      // 생성당시 0, 맞추면1, 틀리면2
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
        printf("정답은 무엇인가요? (공백을 포함하지 않는 하나의 한글 단어입니다) : ");
        scanf("%s", playerAnswer);

        if (!strcmp(currunt->quizAnswer, playerAnswer)) {
            printf("정답!!\n");
            currunt->rightOrWrong = 1;
        }
        else {
            printf("오답입니다\n");
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
        printf("틀린 문제이므로 Hint가 제공됩니다 \nHint : [ %s ]\n", currunt->quizHint);
        printf("정답은 무엇인가요? (공백을 포함하지 않는 하나의 한글 단어입니다) : ");
        scanf("%s", playerAnswer);

        if (!strcmp(currunt->quizAnswer, playerAnswer)) {
            printf("정답!!\n\n");
            currunt->rightOrWrong = 1;
        }
        else {
            printf("오답입니다\n\n");
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
        printf("게임 클리어");
        next = getch();
        return 111;
    }
    else {
        return 222;
    }
}

int Game(void) {

    const char Quiz1[100] = { "퀴즈1: 별중에 가장 슬픈 별은 ? " };
    const char Quiz2[100] = { "퀴즈2: 차도가 없는 나라는?" };
    const char Quiz3[100] = { "퀴즈3: 많이 맞을수록 좋은 것은?" };
    const char Quiz4[100] = { "퀴즈4: 세상에서 제일 큰 코는?" };
    const char Quiz5[100] = { "퀴즈5: 돼지가 방귀를 뀌면?" };

    const char Answer1[100] = { "이별" };
    const char Answer2[100] = { "인도" };
    const char Answer3[100] = { "시험문제" };
    const char Answer4[100] = { "맥시코" };
    const char Answer5[100] = { "돈가스" };

    const char Hint1[100] = { "* 별" };
    const char Hint2[100] = { "* 도" };
    const char Hint3[100] = { "ㅅㅎㅁㅈ" };
    const char Hint4[100] = { "* * 코" };
    const char Hint5[100] = { "돈 * *" };

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
    printf("첫 번쨰 시도입니다. 3번쨰 시도 전까지 모든 문제를 맞추지 못하면 GAME OVER\n");
    printf("첫 번쨰 시도를 시작하시려면 아무 키나 누르세요\n");
    next = getch();

    printf("\n ");
    quizSolve1(head);

    a1 = gameFinish(head);
    if (a1 == 222) {
        printf("모든 문제를 맞추지 못했습니다 다시 도전하시겠습니까? (남은기회2)\n");
        printf("다시 도전하시려면 아무 키나 누르세요\n");
        next = getch();
        system("cls");

        printf("\n ");
        printf("두 번쨰 시도입니다. 3번쨰 시도 전까지 모든 문제를 맞추지 못하면 GAME OVER\n");
        printf("두 번쨰 시도를 시작하시려면 아무 키나 누르세요\n");
        next = getch();

        printf("\n ");
        quizSolve2(head);

        a2 = gameFinish(head);
        if (a2 == 222) {
            printf("모든 문제를 맞추지 못했습니다 다시 도전하시겠습니까? (남은기회1)\n");
            printf("다시 도전하시려면 아무 키나 누르세요\n");
            next = getch();
            system("cls");

            printf("\n ");
            printf("세 번쨰 시도입니다. 3번쨰 시도 전까지 모든 문제를 맞추지 못하면 GAME OVER\n");
            printf("세 번쨰 시도를 시작하시려면 아무 키나 누르세요\n");
            next = getch();
            printf("\n ");
            quizSolve2(head);

            for (QuizListNode* currunt = head; currunt != NULL; currunt = currunt->link) {
                if (currunt->rightOrWrong == 1) {
                    finish = finish + 1;
                }
            }
            if (finish == 5) {
                printf("게임 클리어");
                next = getch();
            }
            else {
                printf("게임 오버");
                next = getch();
            }
        }
    }
}

void game_explanation()
{
    system("cls");
    printf("게임설명\n");
    printf("1.끝말잇기 게임\n");
    printf("  1번 플레이어와 2번 플레이어가 번갈아 가며 끝말잇기를 진행\n");
    printf("  중복된 단어를 입력하는 사람이 패배\n\n\n");
    printf("2. 두더지 게임\n");
    printf("  9개의 두더지를 키패드를 이용하여 잡는 방식\n");
    printf("  두더지 9개가 모두 차면 게임 오버\n\n\n");
    printf("3. 퀴즈 게임\n");
    printf("  퀴즈 5개 맞추기를 시작\n");
    printf("  해당 퀴즈를 맞추면 해당 퀴즈는 삭제\n");
    printf("  못맞춘 퀴즈는 힌트가 제공되고 5개의 퀴즈를 전부 맞추면 클리어\n");
    printf("  3번의 시도 안에 5개의 퀴즈를 전부 맞추지 못하면 게임 오버\n\n");
    printf("게임을 시작할려면 1, 2, 3번중 하나를 입력하시오");
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
            printf("  숫자 1,2,3 중 하나를 선택하세요\n");
        }
    }
}


int main(void) {
    printf("\n\n\n\n\n");
    printf("                            ■■■■■■  ■■■  ■    ■          ■    ■■■■■■■■     ■ ■    ■■■■■   ■   \n");
    printf("                            ■       ■  ■    ■          ■           ■     ■ ■   ■     ■  ■   \n");
    printf("                            ■       ■  ■    ■          ■           ■   ■■■ ■    ■■■■■   ■   \n");
    printf("                            ■       ■  ■    ■          ■           ■     ■ ■     ■■■■■■■■ \n");
    printf("                            ■       ■  ■    ■          ■           ■     ■ ■     ■      ■ \n");
    printf("                            ■■■■■■■■■  ■    ■■■■■■■■   ■           ■     ■ ■     ■■■■■■■■ \n\n\n");
    printf("\n");
    printf("1. 끝말잇기 게임\n");
    printf("2. 두더지 게임\n");
    printf("3. 퀴즈게임\n");
    printf("4. 게임설명읽기\n");
    printf("어떤 게임을 하시겠습니까? 숫자를 선택하세요. 게임 설명이 필요하면 4번을 누르세요 : \n");

    game_start();
}
