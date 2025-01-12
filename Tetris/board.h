#ifndef BOARD_H
#define BOARD_H

#define BLOCK_SIZE 4

#define WIDTH 14
#define HEIGHT 16
#define WALL 2

extern int board[HEIGHT][WIDTH];
extern const char* colorBoard[HEIGHT][WIDTH];  // 보드 색상 정보 저장

void clearScreen();

void initBoard(); //보드판 초기화
void drawBoard(); //보드판 그림
void drawFinalBoard(); //최종 보드판 그림

int isGameOver();  // 게임 종료 함수 선언

void setBlock(int blockType); //블록 초기화

void clearFullLines(); //라인지우기
int canMove(); //이동 가능 확인

#endif