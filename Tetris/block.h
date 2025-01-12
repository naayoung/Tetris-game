#ifndef BLOCK_H
#define BLOCK_H

#define BLOCK_SIZE 4
#define WIDTH 14
#define HEIGHT 16

#define BLOCK_INIT 5

extern int currentBlock[BLOCK_SIZE][BLOCK_SIZE];
extern int blockX, blockY;
extern int blockType;

//블록 색상 관련
#define RESET_COLOR "\033[0m"  // 색상 초기화
extern const char *blockColors[7];  // 블록 색상 배열 선언

void spawnBlock();
void newBlock();

void moveLeft();
void moveRight();
void moveDown();
void rotateBlock();

void autoMoveDown();

#endif