#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>

#define WIDTH 60
#define HEIGHT 25

typedef struct {
    float x, y, z;
} Vector3;

typedef struct {
    float m[3][3];
} Matrix3x3;

typedef struct {
    Vector3 Position;
    Vector3 Scale;
    float Rotation;
    Vector3 Line[2];
    char Symbol[3];
} GameObject_Line;

// 단위 행렬 생성
Matrix3x3 identity_matrix() {
    Matrix3x3 matrix = { {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}} };
    return matrix;
}

// 스케일 행렬 생성
Matrix3x3 scale_matrix(float sx, float sy) {
    Matrix3x3 matrix = identity_matrix();
    matrix.m[0][0] = sx;
    matrix.m[1][1] = sy;
    return matrix;
}

// 회전 행렬 생성
Matrix3x3 rotation_matrix(float angle) {
    Matrix3x3 matrix = identity_matrix();
    float radians = angle * (3.14159f / 180.0f);
    matrix.m[0][0] = cos(radians);
    matrix.m[0][1] = -sin(radians);
    matrix.m[1][0] = sin(radians);
    matrix.m[1][1] = cos(radians);
    return matrix;
}

// 이동 행렬 생성
Matrix3x3 translation_matrix(float tx, float ty) {
    Matrix3x3 matrix = identity_matrix();
    matrix.m[0][2] = tx;
    matrix.m[1][2] = ty;
    return matrix;
}

// 행렬 곱셈
Matrix3x3 multiply_matrices(Matrix3x3 a, Matrix3x3 b) {
    Matrix3x3 result = identity_matrix();
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            result.m[i][j] = 0;
            for (int k = 0; k < 3; ++k) {
                result.m[i][j] += a.m[i][k] * b.m[k][j];
            }
        }
    }
    return result;
}

// 벡터와 행렬 곱셈
Vector3 multiply_matrix_vector(Matrix3x3 m, Vector3 v) {
    Vector3 result;
    result.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z;
    result.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z;
    result.z = 1;
    return result;
}

// 화면 초기화
void Elf2DInitScreen() {
    system("cls");
}

// 화면 버퍼 클리어
void Elf2DClearScreen(char* Buf, int width, int height) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            Buf[i * (width + 1) + j] = ' ';
        }
        Buf[i * (width + 1) + width] = '\n';
    }
    Buf[(width + 1) * height - 1] = '\0';
}

// 화면 버퍼 그리기
void Elf2DDrawBuffer(char* Buf) {
    Elf2DInitScreen();
    printf("%s", Buf);
}

// 선 그리기
void Elf2DDrawLine(int x0, int y0, int x1, int y1, char* Buf, int width, int height) {
    if (x0 < 0 || x0 >= width || y0 < 0 || y0 >= height || x1 < 0 || x1 >= width || y1 < 0 || y1 >= height) {
        return;
    }
    int idx = y0 * (width + 1) + x0;
    Buf[idx] = '*';
}

// 시간 초기화
void ElfTimeInitialize() {
    // 빈 함수 (시간 초기화는 여기서는 필요 없음)
}

// 델타 시간 계산
void ElfTimeCalculateDeltaTime() {
    // 빈 함수 (시간 측정은 여기서는 필요 없음)
}

// 델타 시간 가져오기
double ElfTimeGetDeltaTime() {
    return 16.67; // 대략 60 FPS
}

// 잠자기 함수
void Elf2DSleep(double ms) {
    Sleep((DWORD)ms);
}

// 게임 초기화 함수
void Initialize(GameObject_Line* obj, int objNum) {
    for (int i = 0; i < objNum; i++) {
        obj[i].Position.x = 0;
        obj[i].Position.y = 0;
        obj[i].Rotation = 0;
        obj[i].Line[0].x = 0;
        obj[i].Line[0].y = 0;
        obj[i].Line[1].x = 3;
        obj[i].Line[1].y = 0;
    }
    obj[1].Rotation = 30;
    obj[2].Rotation = 30;
    strcpy_s(obj[0].Symbol, sizeof(obj[0].Symbol), "11");
    strcpy_s(obj[1].Symbol, sizeof(obj[1].Symbol), "22");
    strcpy_s(obj[2].Symbol, sizeof(obj[2].Symbol), "33");
    obj[0].Scale.x = 1;
    obj[0].Scale.y = 1;
    obj[1].Scale.x = 2;
    obj[1].Scale.y = 2;
    obj[2].Scale.x = 2;
    obj[2].Scale.y = 2;
}

// 입력 처리 함수
int Input() {
    if (_kbhit()) {
        char ch = _getch();
        if (ch == 27) {  // ESC 키가 눌리면 게임 종료
            return 99;
        }
    }
    return 0;
}

// 업데이트 함수
void Update(GameObject_Line* obj, int objNum, int e) {
    // 빈 함수 (업데이트 로직 필요 시 추가)
}

// 렌더 함수
void Render(GameObject_Line* obj, int objNum, char* Buf, int width, int height) {
    Vector3 lineA = { 0, 0, 1 }, lineB = { 0, 0, 1 };
    Matrix3x3 world = identity_matrix();

    for (int i = 0; i < objNum; i++) {
        obj[i].Position.x = lineB.x;
        obj[i].Position.y = lineB.y;

        lineA.x = obj[i].Line[0].x;
        lineA.y = obj[i].Line[0].y;
        lineA.z = 1;
        lineB.x = obj[i].Line[1].x;
        lineB.y = obj[i].Line[1].y;
        lineB.z = 1;

        Matrix3x3 scale = scale_matrix(obj[i].Scale.x, obj[i].Scale.y);
        Matrix3x3 rotation = rotation_matrix(obj[i].Rotation);
        Matrix3x3 translation = translation_matrix(obj[i].Position.x, obj[i].Position.y);

        world = multiply_matrices(scale, world);
        world = multiply_matrices(rotation, world);
        world = multiply_matrices(translation, world);
        lineA = multiply_matrix_vector(world, lineA);
        lineB = multiply_matrix_vector(world, lineB);

        Elf2DDrawLine((int)lineA.x, (int)lineA.y, (int)lineB.x, (int)lineB.y, Buf, width, height);
    }
}

// 게임 루프
int main() {
    int fps = 60;
    double frameTime = 1000.0 / fps;

    char screenBuffer[(WIDTH + 1) * HEIGHT];
    int screenWidth = WIDTH;
    int screenHeight = HEIGHT;

    GameObject_Line LineObj[3];

    Initialize(LineObj, 3);
    Elf2DInitScreen();
    ElfTimeInitialize();

    int isGameRunning = 1;
    while (isGameRunning) {
        Elf2DClearScreen(screenBuffer, screenWidth, screenHeight);

        int gameEvent = Input();
        if (gameEvent == 99) {
            isGameRunning = 0;
        }

        Update(LineObj, 3, gameEvent);
        Render(LineObj, 3, screenBuffer, screenWidth, screenHeight);
        Elf2DDrawBuffer(screenBuffer);

        ElfTimeCalculateDeltaTime();
        double deltaTime = ElfTimeGetDeltaTime();
        double sleepTime = frameTime - deltaTime;
        if (sleepTime > 0) {
            Elf2DSleep(sleepTime);
        }
    }

    return 0;
}
