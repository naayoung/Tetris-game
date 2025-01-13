#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "block.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

int board[HEIGHT][WIDTH] = {0};
const char* colorBoard[HEIGHT][WIDTH] = {{0}};  // 색상 배열 초기화

////////////////////////
/*    보드판 초기화    */
////////////////////////
void initBoard() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (j < WALL || j >= WIDTH-WALL || i == HEIGHT - 1) { // 0~4열, 15~19열, 마지막 행
                board[i][j] = -1; // 벽
            } else {
                board[i][j] = 0;  // 빈 공간
            }
        }
    }
}

////////////////////////
/*    보드판 그리기    */
////////////////////////
const char* gameInstructions[] = {
    "####################",
    "    [ 게임 설명 ]",
    "--------------------",
    "    a: 왼쪽 이동",
    "    d: 오른쪽 이동",
    "    s: 아래로 이동",
    "    w: 회전",
    "    q: 종료",
    "--------------------",
    "####################",
};

void clearScreen() {
    #ifdef _WIN32
        system("cls");  // Windows 환경
    #else
        system("clear");  // macOS, Linux 환경
    #endif
}

void drawBoard() {
    clearScreen();  // 화면 초기화

    printf("\n");

    // 보드와 게임 설명을 나란히 출력
    for (int j = 0; j < HEIGHT; j++) {
        // 보드 출력 (왼쪽 부분)
        for (int i = 0; i < WIDTH; i++) {
            if (j >= blockY && j < blockY + BLOCK_SIZE &&
                i >= blockX && i < blockX + BLOCK_SIZE &&
                currentBlock[j - blockY][i - blockX] != 0) {
                printf("%s□ %s", blockColors[blockType], RESET_COLOR);  // 해당 색상으로 블록 출력
            } else if (board[j][i] == -1) {
                printf("▨ ");
            } else if (board[j][i] == 1) {
                int blockType = board[j][i] - 1;
                if (blockType >= 0 && blockType < 7) {
                    printf("%s■ %s", colorBoard[j][i], RESET_COLOR);
                }
            } else if (j == 1) {
                printf(". "); //게임 오버 라인
            } else {
                printf("  ");
            }
        }

        // 오른쪽 게임 설명 출력
        printf("    ");  // 일정 간격을 두어 보드와 설명을 구분
        if (j < 10) {
            printf("%s", gameInstructions[j]);  // 해당 줄에 게임 설명 출력
        }
        printf("\n");
    }
    printf("\n");
}

//최종 보드판
void finalDrawBoard() {
    clearScreen();  // 화면 초기화

    printf("\n");
    for (int j = 0; j < HEIGHT; j++) {
        for (int i = 0; i < WIDTH; i++) {
            if (board[j][i] == -1) {
                printf("▨ ");
            } else if (board[j][i] == 1) {
                int blockType = board[j][i] - 1;
                if (blockType >= 0 && blockType < 7) {
                    printf("%s■ %s", colorBoard[j][i], RESET_COLOR);
                }
            } else if (j == 1) {
                printf(". "); //게임 오버 라인
            } else {
                printf("  ");
            }
        }
        printf("\n");
    }
}

////////////////////////
/*    게임 플레이     */
///////////////////////

//종료 확인
int isGameOver() {
    for (int j = WALL; j < WIDTH-WALL; j++) { // 블록이 생성될 수 있는 열만 확인
        if (board[1][j] != 0) { // 첫 번째 행에 블록이 있다면 게임 종료
            return 1;
        }
    }
    return 0;
}

//움직임 확인
int canMove(int newX, int newY) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            if (currentBlock[i][j] != 0) {
                int nx = newX + i;
                int ny = newY + j;
                if (nx < 0 || ny < 0 || nx >= HEIGHT || ny >= WIDTH || board[nx][ny] != 0) {
                    return 0; // 이동 불가
                }
            }
        }
    }
    //printf("canMove함수 : blockX: %d, blockY: %d\n", blockX, blockY);
    return 1; // 이동 가능
}

// 한 줄이 꽉 찬 경우 해당 줄을 삭제하고, 위의 줄들을 내린다.
void clearFullLines() {
    for (int i = HEIGHT - 2; i >= 0; i--) {
        int fullLine = 1; // 현재 줄이 꽉 찼는지 체크

        //벽 제외 부분 확인
        for (int j = WALL; j < WIDTH - WALL; j++) {
            if (board[i][j] == 0) {
                fullLine = 0;
                break;
            }
        }
        
        if (fullLine) {
            // 꽉 찬 줄을 지우고, 그 위의 모든 줄을 내린다.
            for (int k = i; k > 0; k--) {
                for (int j = 0; j < WIDTH; j++) {
                    board[k][j] = board[k - 1][j];
                }
            }
            // 첫 번째 줄을 비운다
            for (int j = 0; j < WIDTH; j++) {
                if (j < WALL || j >= WIDTH-WALL) { // 0~4열, 15~19열, 마지막 행
                    board[0][j] = -1; // 벽
                } else {
                    board[0][j] = 0;  // 빈 공간
                }
            }
        }
    }
}

////////////////////////
/*    게임 종료 시    */
////////////////////////
void drawFinalBoard() {
    clearScreen(); // 게임 종료 시 화면 초기화
    finalDrawBoard();   // 최종 보드 상태 출력
    printf("\n");
    printf("Game Over! Final Board \n\n");
    fflush(stdout);
    
    #ifdef _WIN32
    Sleep(1000); // 1000ms = 1초
    #else
    usleep(1000000); // 1초
    #endif
}