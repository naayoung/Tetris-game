#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <conio.h>  // 윈도우 환경용
    #include <windows.h>
#else
    #include <termios.h>  // 맥/리눅스용
    #include <unistd.h>
    #include <fcntl.h>
#endif

#include "input.h"
#include "block.h"
#include "board.h"

#ifdef _WIN32
int kbhit() {
    return _kbhit();  // 윈도우의 conio.h 사용
}
#else
int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);  // 현재 터미널 속성 저장
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);  // 캐논 모드 및 에코 비활성화
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);  // 새 터미널 속성 적용
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();  // 키 입력 확인
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);  // 터미널 속성 복원
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);  // 읽은 문자 반환
        return 1;
    }
    return 0;
}

char getch() {
    struct termios oldt, newt;
    char ch;

    tcgetattr(STDIN_FILENO, &oldt);  // 현재 터미널 속성 저장
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);  // 캐논 모드 및 에코 비활성화
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);  // 새 터미널 속성 적용

    ch = getchar();  // 키 입력 읽기

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);  // 터미널 속성 복원
    return ch;
}
#endif

//////////////////////////////////////////
/*     키 입력에 따른 블록 이동 처리      */
//////////////////////////////////////////
void handleInput() {
    char input;

    if (kbhit()) {  // 키 입력이 있을 경우
        input = getch();  // 입력값 받기

        switch (input) {
            case 'a': // 왼쪽
                moveLeft();
                break;
            case 'd': // 오른쪽
                moveRight();
                break;
            case 's': // 아래
                moveDown();
                break;
            case 'w': // 회전
                rotateBlock();
                break;
            case 'q':  // 종료
                printf("Exiting...\n");
                fflush(stdout);  // 출력 버퍼를 즉시 비워서 화면에 표시되도록 합니다.

                #ifdef _WIN32
                Sleep(1000); // 1000ms = 1초
                #else
                usleep(1000000); // 1초
                #endif

                drawFinalBoard(); // 게임 종료 시 최종 보드 출력
                exit(0); // 게임 종료
                break;
            default:
                break;
        }
    } else {
        // 입력값이 없을 경우 자동으로 블록을 아래로 이동
        autoMoveDown();
    }
}