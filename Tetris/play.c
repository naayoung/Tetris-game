#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "play.h"
#include "input.h"
#include "board.h"
#include "block.h"
#include "intro.h"

#define SCREEN_WIDTH 105  // 화면 너비를 80으로 가정

// 텍스트를 가운데 정렬하여 출력하는 함수
void printCentered(const char* text) {
    int textLength = strlen(text);
    int padding = (SCREEN_WIDTH - textLength) / 2;
    for (int i = 0; i < padding; i++) {
        putchar(' ');
    }
    printf("%s", text);
}

///////////////////////
/*     게임 프로세스     */
//////////////////////
void startGame() {
    char input;

    intro();

    printf("\n");
    printCentered("게임을 진행하시겠습니까? (y/n): ");
    
    input = getchar();
    while ((getchar()) != '\n');  //중복 출력 방지

    if (input == 'y' || input == 'Y') {
        initBoard();       //보드판 초기화
        spawnBlock();      //블록 생성

        while (!isGameOver()) {
            drawBoard();
            handleInput();
            usleep(500000); //0.5초 대기
        }
    } else if (input == 'n' || input == 'N') {
        printCentered("게임을 종료합니다.\n");
        exit(0);  //게임 종료
    } else {
        printCentered("잘못된 입력입니다. 다시 입력해주세요.\n");
        startGame();
    }
}