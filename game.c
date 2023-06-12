#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ��� ����
#define BOARD_SIZE 10
#define MONSTER_COUNT 20
#define GOLD_COUNT 5
#define MAX_GAMES 10

// �÷��̾� ����ü
typedef struct {
    int x;
    int y;
} Player;

// ���� ����
char board[BOARD_SIZE][BOARD_SIZE];

// ���Ϳ� �÷��̾� ��ġ �ʱ�ȭ
void initialize(Player* player, Player monsters[], int* gold_count) {
    // ���� �ʱ�ȭ
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = '-';
        }
    }

    // �÷��̾� ��ġ �ʱ�ȭ
    player->x = rand() % BOARD_SIZE;
    player->y = rand() % BOARD_SIZE;
    board[player->x][player->y] = 'B';

    // ���� ��ġ �ʱ�ȭ
    for (int i = 0; i < MONSTER_COUNT; i++) {
        monsters[i].x = rand() % BOARD_SIZE;
        monsters[i].y = rand() % BOARD_SIZE;
        board[monsters[i].x][monsters[i].y] = 'M';
    }

    // �� ��ġ �ʱ�ȭ
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

// ���� ���
void printBoard() {
    printf("======= ���� ======\n");
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
    printf("===================\n");
}

// �÷��̾� �̵�
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
        printf("�߸��� �Է��Դϴ�. ����Ű(w, s, a, d)�� �Է��ϼ���.\n");
        return;
    }

    if (new_x < 0 || new_x >= BOARD_SIZE || new_y < 0 || new_y >= BOARD_SIZE) {
        printf("�̵��� �� ���� ��ġ�Դϴ�.\n");
        return;
    }

    if (board[new_x][new_y] == 'M') {
        printf("���Ϳ��� �������ϴ�! -5���Ф�\n");
        *score -= 5;
    }
    else if (board[new_x][new_y] == 'G') {
        printf("���� ȹ���Ͽ����ϴ�! +10�� ><\n");
        *gold_count -= 1;
        *score += 10;
    }

    board[player->x][player->y] = '-';//�ٰ� ���� �˾ƺ��Ⱑ ������� ����� ���Ǹ� ���� ����
    board[new_x][new_y] = 'B';
    player->x = new_x;
    player->y = new_y;
}

// ���� ����
void playGame(int game_num, int total_score[], int total_moves[], double total_time[]) {
    Player player;
    Player monsters[MONSTER_COUNT];
    int gold_count;
    int score = 0;
    int moves = 0;
    time_t start_time, end_time;

    initialize(&player, monsters, &gold_count);
    printBoard();

    printf("���� %d�� �����մϴ�!\n", game_num);

    start_time = time(NULL);

    while (gold_count > 0) {
        char direction;
        printf("�̵��� ������ �Է��ϼ���. (w: ��, s: �Ʒ�, a: ����, d: ������): ");
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

    printf("��� ���� ȹ���Ͽ����ϴ�! ���� Ŭ����!\n");
    printf("���� ��ȣ: %d\n", game_num);
    printf("���ھ�: %d\n", score);
    printf("�ҿ� �ð�: %.2lf��\n", game_time);
    printf("�̵� Ƚ��: %d\n", moves);
}

// ��ŷ ���
void printRanking(int total_score[], int total_moves[], double total_time[], int num_games) {
    printf("===== ��ŷ =====\n");
    printf("���� ��ȣ |  ���ھ�  |  �̵� Ƚ��  |  �ҿ� �ð�\n");

    // ������ ���� �ӽ� �迭 ����
    int ranking[MAX_GAMES];
    for (int i = 0; i < num_games; i++) {
        ranking[i] = i;
    }

    //��ŷ ����(���� ���� ���!)
    for (int i = 0; i < num_games - 1; i++) {
        for (int j = 0; j < num_games - i - 1; j++) {
            if (total_score[ranking[j]] < total_score[ranking[j + 1]]) {
                // ���ھ ū ������� ����
                int temp = ranking[j];
                ranking[j] = ranking[j + 1];
                ranking[j + 1] = temp;
            }
        }
    }

    // ��ŷ ���
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
    printf("<<<���ѷα�>>>\n");
    printf("������ 2923��... ���� �������� ��������� �ر��ϰ�\n");
    printf("ȭ���� ����� �ϴ� ������ ���� ���� '��'��!!!!\n");
    printf("�׸��� ���� ���� �����ϸ鼭 ���� Ŀ�Ǹ� ����� �� ����� ���...�Ф�\n");
    printf("������ �Ϸ��� ���� Ŀ�ǰ� �� �ʿ��ѵ�...\n");
    printf("Ŀ�Ǹ� �� �� �ִ� ���� ���� ������ ����� �߾ӻ꿡 ���� \n");
    printf("!!!����!!! ���� �ֿ����� �� ���ε�...\n");
    printf("ȯ��������� �赵 ���� ���� ���͵��� ����� �߾ӻ�..!!\n");
    printf("���͵��� ���� ���� ȹ���ϼ���!!\n");

    do {
        printf("\n^��^ Welcom to ���ݱ� ���� ^��^\n\n");
        printf("===== ���� �޴� =====\n");
        printf("m -> ���� ��� ����\n");
        printf("s -> ���� ����\n");
        printf("r -> ��ŷ ����\n");
        printf("x -> ���� ����\n");
        printf("�޴��� �����ϼ���: ");
        scanf_s(" %c", &choice);

        switch (choice) {
        case 's':
            num_games++;
            playGame(num_games, total_score, total_moves, total_time);
            printf("������ ����Ͻðڽ��ϱ�? (y/n): ");
            scanf_s(" %c", &choice);
            break;
        case 'r':
            printRanking(total_score, total_moves, total_time, num_games);
            break;
        case 'x':
            printf("������ �����մϴ�.\n");
            return 0;
        case 'm':
            printf("====== ���ݱ� ���� �޴��� ======\n");
            printf("!!!���͸� ���� ���� �ֿ���!!!\n");
            printf("����(M)�� ���� ��(G)�� ȹ���ϼ���!\n");
            printf("�ڽ��� ĳ����(B)�� W(��),A(����),S(�Ʒ�),D(������) Ű�� ��Ʈ�� �� �� �ֽ��ϴ�.\n");
            printf("������ �����ϰ� ������ x��, �޴����� ������ 'm'�� �����ϼ���.\n");
            break;
        default:
            printf("�߸��� �Է��Դϴ�. �ٽ� �����ϼ���.\n");
            break;
        }
    } while (choice != 'x');

    return 0;
}
