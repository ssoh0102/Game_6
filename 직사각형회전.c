#include <stdio.h>
#include <math.h>
#include <windows.h>

#define WIDTH 11
#define HEIGHT 5

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct {
    float m[3][3]; // 3x3 행렬
} Matrix3x3;

// 3x3 행렬 초기화
void initIdentityMatrix(Matrix3x3* mat) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (i == j) {
                mat->m[i][j] = 1;
            }
            else {
                mat->m[i][j] = 0;
            }
        }
    }
}

// 이동 행렬 생성
Matrix3x3 createTranslationMatrix(float tx, float ty) {
    Matrix3x3 mat;
    initIdentityMatrix(&mat);
    mat.m[0][2] = tx; // X축 이동
    mat.m[1][2] = ty; // Y축 이동
    return mat;
}

// 회전 행렬 생성
Matrix3x3 createRotationMatrix(float angle) {
    Matrix3x3 mat;
    initIdentityMatrix(&mat);
    float radian = angle * M_PI / 180;
    mat.m[0][0] = cos(radian); // cos(θ)
    mat.m[0][1] = -sin(radian); // -sin(θ)
    mat.m[1][0] = sin(radian); // sin(θ)
    mat.m[1][1] = cos(radian); // cos(θ)
    return mat;
}

// 점을 변환
void transformPoint(float* x, float* y, Matrix3x3 mat) {
    float newX = mat.m[0][0] * *x + mat.m[0][1] * *y + mat.m[0][2];
    float newY = mat.m[1][0] * *x + mat.m[1][1] * *y + mat.m[1][2];
    *x = newX;
    *y = newY;
}

// 콘솔 화면 지우기
void clearScreen() {
    system("cls");
}

// 콘솔 커서 위치를 변경하는 함수
void setCursorPosition(int x, int y) {
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// 사각형의 꼭짓점 위치를 계산하여 그리기
void drawRectangle(int centerX, int centerY, float angle) {
    // 사각형의 반 너비 및 반 높이
    float halfWidth = WIDTH / 2.0f;
    float halfHeight = HEIGHT / 2.0f;

    // 사각형의 각 꼭짓점
    float vertices[4][2] = {
        {-halfWidth, -halfHeight},
        { halfWidth, -halfHeight},
        { halfWidth,  halfHeight},
        {-halfWidth,  halfHeight}
    };

    // 회전 행렬 생성
    Matrix3x3 rotation = createRotationMatrix(angle);
    Matrix3x3 translation = createTranslationMatrix(centerX, centerY);

    // 변환된 꼭짓점 저장
    int transformedVertices[4][2];
    for (int i = 0; i < 4; i++) {
        float x = vertices[i][0];
        float y = vertices[i][1];
        transformPoint(&x, &y, rotation);
        transformPoint(&x, &y, translation);
        transformedVertices[i][0] = (int)(x + 0.5);
        transformedVertices[i][1] = (int)(y + 0.5);
    }

    // 사각형의 경계선을 그리기
    for (int i = 0; i < 4; i++) {
        int x0 = transformedVertices[i][0];
        int y0 = transformedVertices[i][1];
        int x1 = transformedVertices[(i + 1) % 4][0];
        int y1 = transformedVertices[(i + 1) % 4][1];

        // 선을 그리기
        int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
        int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
        int err = dx + dy, e2;

        while (1) {
            setCursorPosition(x0, y0);
            printf("#");
            if (x0 == x1 && y0 == y1) break;
            e2 = 2 * err;
            if (e2 >= dy) { err += dy; x0 += sx; }
            if (e2 <= dx) { err += dx; y0 += sy; }
        }
    }

    // 내부를 채우기
    // bounding box 찾기
    int minX = transformedVertices[0][0], maxX = transformedVertices[0][0];
    int minY = transformedVertices[0][1], maxY = transformedVertices[0][1];
    for (int i = 1; i < 4; i++) {
        if (transformedVertices[i][0] < minX) minX = transformedVertices[i][0];
        if (transformedVertices[i][0] > maxX) maxX = transformedVertices[i][0];
        if (transformedVertices[i][1] < minY) minY = transformedVertices[i][1];
        if (transformedVertices[i][1] > maxY) maxY = transformedVertices[i][1];
    }

    // 내부 픽셀을 채우기
    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            // 현재 픽셀이 사각형 내부에 있는지 확인
            // 여기서는 단순히 x, y가 vertex 사이에 있을 때를 확인하는 방식으로 구현
            // 선분의 방정식을 사용하여 픽셀이 선의 내부에 있는지 확인
            int inside = 1;
            for (int i = 0; i < 4; i++) {
                int x0 = transformedVertices[i][0];
                int y0 = transformedVertices[i][1];
                int x1 = transformedVertices[(i + 1) % 4][0];
                int y1 = transformedVertices[(i + 1) % 4][1];

                // 크로스 프로덕트를 사용하여 점이 선의 왼쪽에 있는지 확인
                float crossProduct = (x1 - x0) * (y - y0) - (y1 - y0) * (x - x0);
                if (crossProduct < 0) {
                    inside = 0; // 점이 선의 왼쪽에 있지 않음
                    break;
                }
            }
            if (inside) {
                setCursorPosition(x, y);
                printf("#");
            }
        }
    }
}

// 화면 그리기
void draw(int angle) {
    clearScreen();

    // P 위치 (콘솔 중앙)
    int pX = (80 / 2);  // 콘솔 가로 크기 (80)에서 중앙 위치 계산
    int pY = (25 / 2);  // 콘솔 세로 크기 (25)에서 중앙 위치 계산

    // 화면 그리기
    for (int y = 0; y < 25; y++) {  // 콘솔 세로 크기 25로 조정
        for (int x = 0; x < 80; x++) {  // 콘솔 가로 크기 80로 조정
            if (x == pX && y == pY) {
                printf(" "); // P 출력
            }
            else {
                printf(" "); // 빈 공간 출력
            }
        }
        printf("\n");
    }

    // 사각형 그리기
    drawRectangle(pX, pY, angle);
}

int main() {
    for (int angle = 0; angle < 360; angle += 5) {
        draw(angle); // 회전 각도에 따라 그리기
        Sleep(42); // 0.1초 대기
    }
    return 0;
}
