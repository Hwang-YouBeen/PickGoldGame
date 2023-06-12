#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 상수 정의
#define BOARD_SIZE 10
#define MONSTER_COUNT 20
#define GOLD_COUNT 5
#define MAX_GAMES 10

// 플레이어 구조체
typedef struct {
    int x;
    int y;
} Player;

// 게임 보드
char board[BOARD_SIZE][BOARD_SIZE];

// 몬스터와 플레이어 위치 초기화
void initialize(Player* player, Player monsters[], int* gold_count) {
    // 보드 초기화
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = '-';
        }
    }

    // 플레이어 위치 초기화
    player->x = rand() % BOARD_SIZE;
    player->y = rand() % BOARD_SIZE;
    board[player->x][player->y] = 'B';

    // 몬스터 위치 초기화
    for (int i = 0; i < MONSTER_COUNT; i++) {
        monsters[i].x = rand() % BOARD_SIZE;
        monsters[i].y = rand() % BOARD_SIZE;
        board[monsters[i].x][monsters[i].y] = 'M';
    }

    // 금 위치 초기화
    *gold_count = 0;
    while (*gold_count < GOLD_COUNT) {
        int x = rand() % BOARD_SIZE;
        int y = rand() % BOARD_SIZE;
        if (board[x][y] == '-') {
            board[x][y] = 'G';
            *gold_count += 1;
        }
    }
}

// 보드 출력
void printBoard() {
    printf("======= 보드 ======\n");
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
    printf("===================\n");
}

// 플레이어 이동
void movePlayer(Player* player, char direction, int* gold_count, int* score) {
    int new_x = player->x;
    int new_y = player->y;

    switch (direction) {
    case 'w':
        new_x -= 1;
        break;
    case 's':
        new_x += 1;
        break;
    case 'a':
        new_y -= 1;
        break;
    case 'd':
        new_y += 1;
        break;
    default:
        printf("잘못된 입력입니다. 방향키(w, s, a, d)를 입력하세요.\n");
        return;
    }

    if (new_x < 0 || new_x >= BOARD_SIZE || new_y < 0 || new_y >= BOARD_SIZE) {
        printf("이동할 수 없는 위치입니다.\n");
        return;
    }

    if (board[new_x][new_y] == 'M') {
        printf("몬스터에게 잡혔습니다! -5점ㅠㅠ\n");
        *score -= 5;
    }
    else if (board[new_x][new_y] == 'G') {
        printf("금을 획득하였습니다! +10점 ><\n");
        *gold_count -= 1;
        *score += 10;
    }

    board[player->x][player->y] = '-';//줄과 열을 알아보기가 어려워서 사용자 편의를 위해 넣음
    board[new_x][new_y] = 'B';
    player->x = new_x;
    player->y = new_y;
}

// 게임 실행
void playGame(int game_num, int total_score[], int total_moves[], double total_time[]) {
    Player player;
    Player monsters[MONSTER_COUNT];
    int gold_count;
    int score = 0;
    int moves = 0;
    time_t start_time, end_time;

    initialize(&player, monsters, &gold_count);
    printBoard();

    printf("게임 %d을 시작합니다!\n", game_num);

    start_time = time(NULL);

    while (gold_count > 0) {
        char direction;
        printf("이동할 방향을 입력하세요. (w: 위, s: 아래, a: 왼쪽, d: 오른쪽): ");
        scanf_s(" %c",&direction);

        movePlayer(&player, direction, &gold_count, &score);
        printBoard();

        moves++;
    }

    end_time = time(NULL);
    double game_time = difftime(end_time, start_time);
    total_score[game_num - 1] = score;
    total_moves[game_num - 1] = moves;
    total_time[game_num - 1] = game_time;

    printf("모든 금을 획득하였습니다! 게임 클리어!\n");
    printf("게임 번호: %d\n", game_num);
    printf("스코어: %d\n", score);
    printf("소요 시간: %.2lf초\n", game_time);
    printf("이동 횟수: %d\n", moves);
}

// 랭킹 출력
void printRanking(int total_score[], int total_moves[], double total_time[], int num_games) {
    printf("===== 랭킹 =====\n");
    printf("게임 번호 |  스코어  |  이동 횟수  |  소요 시간\n");

    // 정렬을 위해 임시 배열 생성
    int ranking[MAX_GAMES];
    for (int i = 0; i < num_games; i++) {
        ranking[i] = i;
    }

    //랭킹 정렬(버블 정렬 사용!)
    for (int i = 0; i < num_games - 1; i++) {
        for (int j = 0; j < num_games - i - 1; j++) {
            if (total_score[ranking[j]] < total_score[ranking[j + 1]]) {
                // 스코어가 큰 순서대로 정렬
                int temp = ranking[j];
                ranking[j] = ranking[j + 1];
                ranking[j + 1] = temp;
            }
        }
    }

    // 랭킹 출력
    for (int i = 0; i < num_games; i++) {
        int game_num = ranking[i] + 1;
        int score = total_score[ranking[i]];
        int moves = total_moves[ranking[i]];
        double time = total_time[ranking[i]];

        printf("%9d | %8d | %10d | %9.2lf\n", game_num, score, moves, time);
    }
}

int main() {
    srand(time(NULL));
    int total_score[MAX_GAMES];
    int total_moves[MAX_GAMES];
    double total_time[MAX_GAMES];
    int num_games = 0;
    char choice;
    printf("<<<프롤로그>>>\n");
    printf("지금은 2923년... 숱한 전쟁으로 세계경제가 붕괴하고\n");
    printf("화폐의 기능을 하는 유일한 것은 오직 '금'뿐!!!!\n");
    printf("그리고 가진 금을 과제하면서 마실 커피를 사느라 다 써버린 당신...ㅠㅠ\n");
    printf("과제를 하려면 아직 커피가 더 필요한데...\n");
    printf("커피를 살 수 있는 금을 얻을 유일한 방법은 중앙산에 가서 \n");
    printf("!!!직접!!! 금을 주워오는 것 뿐인데...\n");
    printf("환경오염으로 듣도 보도 못한 몬스터들이 들끓는 중앙산..!!\n");
    printf("몬스터들을 피해 금을 획득하세요!!\n");

    do {
        printf("\n^ㅇ^ Welcom to 금줍기 게임 ^ㅇ^\n\n");
        printf("===== 게임 메뉴 =====\n");
        printf("m -> 게임 방법 보기\n");
        printf("s -> 게임 시작\n");
        printf("r -> 랭킹 보기\n");
        printf("x -> 게임 종료\n");
        printf("메뉴를 선택하세요: ");
        scanf_s(" %c", &choice);

        switch (choice) {
        case 's':
            num_games++;
            playGame(num_games, total_score, total_moves, total_time);
            printf("게임을 계속하시겠습니까? (y/n): ");
            scanf_s(" %c", &choice);
            break;
        case 'r':
            printRanking(total_score, total_moves, total_time, num_games);
            break;
        case 'x':
            printf("게임을 종료합니다.\n");
            return 0;
        case 'm':
            printf("====== 금줍기 게임 메뉴얼 ======\n");
            printf("!!!몬스터를 피해 금을 주워라!!!\n");
            printf("몬스터(M)을 피해 금(G)을 획득하세요!\n");
            printf("자신의 캐릭터(B)는 W(위),A(왼쪽),S(아래),D(오른쪽) 키로 컨트롤 할 수 있습니다.\n");
            printf("게임을 종료하고 싶으면 x를, 메뉴얼을 보려면 'm'을 선택하세요.\n");
            break;
        default:
            printf("잘못된 입력입니다. 다시 선택하세요.\n");
            break;
        }
    } while (choice != 'x');

    return 0;
}
