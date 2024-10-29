#include <stdio.h>
#include <math.h>
#include <windows.h>

#define WIDTH 40
#define HEIGHT 20

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct {
    float x, y, w; // 2D 좌표 (x, y)와 동차 좌표 (w)
} vec3;

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

// 행렬 곱셈
Matrix3x3 multiplyMatrices(Matrix3x3 a, Matrix3x3 b) {
    Matrix3x3 result;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result.m[i][j] = a.m[i][0] * b.m[0][j] + a.m[i][1] * b.m[1][j] + a.m[i][2] * b.m[2][j];
        }
    }
    return result;
}

// vec3 점 변환
vec3 Mul(vec3 a, Matrix3x3* b) {
    vec3 result;
    result.x = a.x * b->m[0][0] + a.y * b->m[0][1] + a.w * b->m[0][2];
    result.y = a.x * b->m[1][0] + a.y * b->m[1][1] + a.w * b->m[1][2];
    result.w = a.x * b->m[2][0] + a.y * b->m[2][1] + a.w * b->m[2][2];
    return result;
}

// 콘솔 화면 지우기
void clearScreen() {
    system("cls");
}

// 화면 그리기
void draw(int angle) {
    clearScreen();

    // 태양 위치
    int sunX = WIDTH / 2;
    int sunY = HEIGHT / 2;

    // 지구의 초기 위치
    float earthDistance = 7.0f; // 태양으로부터의 거리
    float earthAngle = angle; // 지구의 회전 각도

    // 달의 초기 위치
    float moonDistance = 2.2f; // 지구로부터의 거리
    float moonAngle = angle * 2; // 달 회전 (지구보다 빠르게)

    // 지구의 좌표 계산
    vec3 earthPos = { earthDistance, 0.0f, 1.0f };
    Matrix3x3 earthRotation = createRotationMatrix(earthAngle);
    earthPos = Mul(earthPos, &earthRotation);

    int earthX = (int)(sunX + earthPos.x);
    int earthY = (int)(sunY + earthPos.y);

    // 달의 좌표 계산
    vec3 moonPos = { moonDistance, 0.0f, 1.0f };
    Matrix3x3 moonRotation = createRotationMatrix(moonAngle);
    moonPos = Mul(moonPos, &moonRotation);

    int moonX = (int)(earthX + moonPos.x);
    int moonY = (int)(earthY + moonPos.y);

    // 화면 그리기
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == sunX && y == sunY) {
                printf(" S "); // 태양 출력
            }
            else if (x == earthX && y == earthY) {
                printf(" E "); // 지구 출력
            }
            else if (x == moonX && y == moonY) {
                printf(" M "); // 달 출력
            }
            else {
                printf(" . "); // 빈 공간 출력
            }
        }
        printf("\n");
    }
}

int main() {
    while (1) { // 계속 회전하게
        for (int angle = 0; angle < 360; angle += 5) {
            draw(angle); // 회전 각도에 따라 그리기
            Sleep(100); // 0.1초 대기
        }
    }
    return 0;
}
