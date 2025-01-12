#include <stdio.h>
#include <stdlib.h>
#include "block.h"
#include "board.h"

// 이동 관련 함수 프로토타입
int canMoveLeft();
int canMoveRight();
int canMoveDown();
// 회전 관련 함수 프로토타입
int canRotate(int tempBlock[BLOCK_SIZE][BLOCK_SIZE]);

int blockType = -1; //초기 블록 타입

// 블록 색상 배열
const char *blockColors[7] = {
    "\033[38;5;51m",  // CYAN
    "\033[38;5;226m", // YELLOW
    "\033[38;5;171m", // MAGENTA
    "\033[38;5;32m",  // BLUE
    "\033[38;5;196m", // RED
    "\033[38;5;46m",  // GREEN
    "\033[38;5;214m"  // ORANGE
};

/////////////////////
/*  블록 종류 정의  */
/////////////////////
int blocks[7][4][4] = {
    // I 블록
    {{0, 0, 0, 0},
     {0, 0, 0, 0},
     {1, 1, 1, 1},
     {0, 0, 0, 0}},
    
    // O 블록
    {{0, 0, 0, 0},
     {0, 1, 1, 0},
     {0, 1, 1, 0},
     {0, 0, 0, 0}},
    
    // T 블록
    {{0, 0, 0, 0},
     {0, 1, 1, 0},
     {0, 1, 0, 0},
     {0, 0, 0, 0}},
    
    // L 블록
    {{0, 0, 0, 0},
     {0, 0, 1, 0},
     {1, 1, 1, 0},
     {0, 0, 0, 0}},
    
    // J 블록
    {{0, 0, 0, 0},
     {1, 0, 0, 0},
     {1, 1, 1, 0},
     {0, 0, 0, 0}},
    
    // Z 블록
    {{0, 0, 0, 0},
     {0, 1, 1, 0},
     {1, 1, 0, 0},
     {0, 0, 0, 0}},
    
    // S 블록
    {{0, 0, 0, 0},
     {1, 1, 0, 0},
     {0, 1, 1, 0},
     {0, 0, 0, 0}}
};

int currentBlock[BLOCK_SIZE][BLOCK_SIZE];  // 현재 블록
int blockX = BLOCK_INIT, blockY = 0;  // 블록의 시작 위치

/////////////////
/*  블록 초기화  */
////////////////
void setBlock(int blockType) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            currentBlock[i][j] = blocks[blockType][i][j];
        }
    }
}

/////////////////////
/*  랜덤 블록 생성  */
/////////////////////

// 랜덤으로 블록 생성
int randomBlockType() {
    blockType = rand() % 7;
    return blockType;  // 0부터 6까지 랜덤 값 반환
}
// 랜덤 블록을 생성하고 설정
void spawnBlock() {
    setBlock(randomBlockType());
}

//왼쪽 이동
void moveLeft() {
    if (blockX > 0 && canMoveLeft()) {
        blockX--;
    }
    //printf("moveLeft함수 : blockX: %d, blockY: %d\n", blockX, blockY);
}
//왼족 이동 가능 확인
int canMoveLeft() {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            //블록이 있는 부분을 검사
            if (currentBlock[i][j] != 0) {
                //게임판 왼쪽 경계를 넘어가지 않고, 이미 다른 블록이 있는지 확인
                if (blockX + j - 1 < 0 || board[blockY + i][blockX + j - 1] != 0) {
                    return 0;  //이동할 수 없으면 0 반환
                }
            }
        }
    }
    //printf("왼쪽 이동가능확인 : blockX: %d, blockY: %d\n", blockX, blockY);
    return 1;  //이동할 수 있으면 1 반환
}

//오른쪽 이동
void moveRight() {
    if (blockX < WIDTH - BLOCK_SIZE && canMoveRight()) {
        blockX++;
    }
    //printf("moveRight함수 : blockX: %d, blockY: %d\n", blockX, blockY);
}
//오른쪽 이동 가능 확인
int canMoveRight() {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            //블록이 있는 부분을 검사
            if (currentBlock[i][j] != 0) {
                //게임판 오른쪽 경계를 넘어가지 않고, 이미 다른 블록이 있는지 확인
                if (blockX + j + 1 >= WIDTH || board[blockY + i][blockX + j + 1] != 0) {
                    return 0;  //이동할 수 없으면 0 반환
                }
            }
        }
    }
    //printf("오른쪽 이동 확인 : blockX: %d, blockY: %d\n", blockX, blockY);
    return 1;  //이동할 수 있으면 1 반환
}

//아래 이동
void moveDown() {
    if (blockY < HEIGHT - BLOCK_SIZE && canMoveDown()) {
        blockY++;
    }
    //printf("moveDown함수 : blockX: %d, blockY: %d\n", blockX, blockY);
}
//아래 이동 가능 확인
int canMoveDown() {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            //블록이 있는 부분을 검사
            if (currentBlock[i][j] != 0) {
                //게임판 아래 경계를 넘어가지 않고, 이미 다른 블록이 있는지 확인
                if (blockY + i + 1 >= HEIGHT || board[blockY + i + 1][blockX + j] != 0) {
                    return 0;  //이동할 수 없으면 0 반환
                }
            }
        }
    }
    return 1;  //이동할 수 있으면 1 반환
}



//90도 회전
void rotateBlock() {
    int tempBlock[BLOCK_SIZE][BLOCK_SIZE];
    for (int i = 0; i < BLOCK_SIZE; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            tempBlock[i][j] = currentBlock[BLOCK_SIZE - 1 - j][i];
        }
    }
    
    // 회전 후 유효성 체크 (충돌 여부)
    if (canRotate(tempBlock)) {
        for (int i = 0; i < BLOCK_SIZE; i++) {
            for (int j = 0; j < BLOCK_SIZE; j++) {
                currentBlock[i][j] = tempBlock[i][j];
            }
        }
    }
}
// 회전 후 블록이 유효한지 확인
int canRotate(int tempBlock[BLOCK_SIZE][BLOCK_SIZE]) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            if (tempBlock[i][j] != 0 && (blockX + j < 0 || blockX + j >= WIDTH || blockY + i >= HEIGHT || board[blockY + i][blockX + j] != 0)) {
                return 0;  // 회전할 수 없으면 0 반환
            }
        }
    }
    return 1;  // 회전할 수 있으면 1 반환
}


/////////////////////////
/* 블록 자동 하강 함수 */
/////////////////////////

void autoMoveDown() {
    // 블록이 더 이상 내려갈 수 없으면 return
    if (blockY < HEIGHT - BLOCK_SIZE && canMoveDown()) {
        blockY++;  // 블록 한 칸 아래로 이동
    } else {
        // 블록이 떨어졌으면 보드에 추가
        for (int i = 0; i < BLOCK_SIZE; i++) {
            for (int j = 0; j < BLOCK_SIZE; j++) {
                if (currentBlock[i][j] != 0) {
                    board[blockY + i][blockX + j] = currentBlock[i][j];
                    colorBoard[blockY + i][blockX + j] = blockColors[blockType];  // 색상 정보 저장
                }
            }
        }
        clearFullLines();
        
        // 새로운 블록 생성
        spawnBlock();
        blockX = BLOCK_INIT;  // 블록 시작 위치 X
        blockY = 0;  // 블록 시작 위치 Y
    }
    // 보드판 새로 그리기
    drawBoard();
}