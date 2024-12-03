#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
typedef struct {
    int x;
    int y;
} Position;

char board[3][3];
const char player = 'X';
const char computer = 'O';

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

void initializeBoard();
void printBoard();
int checkWinner();
int checkWinnerPlayer(char player);
int evaluate();
int minimax(int depth, int isMaximizingPlayer);
Position bestMove();
void playerMove();
void computerMove();
int freepos();
void dumbComputerMove();

int main(void) {
    srand(time(NULL));
    int random = rand() % 2;
    char current_player;
    int difficulty;

    printf("Choose difficulty:\n");
    printf("0. Easy\n");
    printf("1. Impossible\n");
    while (scanf("%d", &difficulty) != 1 || (difficulty != 0 && difficulty != 1)) {
        printf("Invalid input. Enter 0 or 1.\n");
        while (getchar() != '\n');
    }

    if (random) {
        current_player = 'c';
        printf("Computer starts!\n");
    } else {
        current_player = 'h';
        printf("You start!\n");
    }

    initializeBoard();
    if(difficulty){
        while (1) {
        printBoard();
        if (checkWinner()) {
            break; 
        }
        if (current_player == 'h') {
            playerMove();
            system("clear");
        } else {
            computerMove();
        }
        current_player = (current_player == 'h') ? 'c' : 'h';
    }}
    else{
        while (1) {
        printBoard();
        if (checkWinner()) {
            break; 
        }
        if (current_player == 'h') {
            playerMove();
        } else {
            dumbComputerMove();
        }
        current_player = (current_player == 'h') ? 'c' : 'h';
    }
    }
    return 0;
}

void initializeBoard() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = ' ';
        }
    }
}

void printBoard() {
    printf("\n");
    printf(" %c | %c | %c \n", board[0][0], board[0][1], board[0][2]);
    printf("---|---|--- \n");
    printf(" %c | %c | %c \n", board[1][0], board[1][1], board[1][2]);
    printf("---|---|--- \n");
    printf(" %c | %c | %c \n", board[2][0], board[2][1], board[2][2]);
}

int evaluate() {
    if (checkWinnerPlayer(computer)) return 10;
    if (checkWinnerPlayer(player)) return -10;
    return 0;
}

int minimax(int depth, int isMaximizingPlayer) {
    int score = evaluate();
    if (score == 10 || score == -10 || freepos() == 0)
        return score;

    if (isMaximizingPlayer) {
        int best = -1000;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = computer;
                    best = max(best, minimax(depth + 1, 0));
                    board[i][j] = ' ';
                }
            }
        }
        return best;
    } else {
        int best = 1000;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = player;
                    best = min(best, minimax(depth + 1, 1));
                    board[i][j] = ' ';
                }
            }
        }
        return best;
    }
}

Position bestMove() {
    int bestVal = -1000;
    Position move = {-1, -1};

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = computer;
                int moveVal = minimax(0, 0);
                board[i][j] = ' ';
                if (moveVal > bestVal) {
                    move.x = i;
                    move.y = j;
                    bestVal = moveVal;
                }
            }
        }
    }
    return move;
}

void computerMove() {
    Position move = bestMove();
    board[move.x][move.y] = computer;
    printf("Computer is thinking...\n");
    sleep(4);
}
void dumbComputerMove() {
    Position cplayer;
    printf("Computer is thinking...\n");
    sleep(4);
    while (1) {
        cplayer.x = rand() % 3;
        cplayer.y = rand() % 3;
        if (board[cplayer.x][cplayer.y] == ' ') {
            board[cplayer.x][cplayer.y] = computer;
            break;
        }
    }
}
void playerMove() {
    Position hplayer;
    while (1) {
        printf("Enter position of X *row, col*: ");
        if (scanf("%d,%d", &hplayer.x, &hplayer.y) != 2) {
            printf("Invalid input. Enter two integers separated by a comma.\n");
            while (getchar() != '\n');
            continue;
        }
        hplayer.x--;
        hplayer.y--;
        if (hplayer.x < 0 || hplayer.x > 2 || hplayer.y < 0 || hplayer.y > 2) {
            printf("Invalid position. Enter row and column between 1 and 3.\n");
            continue;
        }
        if (board[hplayer.x][hplayer.y] != ' ') {
            printf("Cell already occupied, choose another\n");
            continue;
        }
        board[hplayer.x][hplayer.y] = player;
        break;
    }
}

int freepos() {
    int count = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') count++;
        }
    }
    return count;
}

int checkWinnerPlayer(char player) {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player) return 1;
        if (board[0][i] == player && board[1][i] == player && board[2][i] == player) return 1;
    }
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player) return 1;
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player) return 1;
    return 0;
}
int checkWinner() {
    if (checkWinnerPlayer(player)) {
        printf("You win!\n");
        return 1;
    }
    if (checkWinnerPlayer(computer)) {
        printf("Computer wins!\n");
        return 1;
    }
    if (freepos() == 0) {
        printf("It's a draw!\n");
        return 1;
    }
    return 0;
}