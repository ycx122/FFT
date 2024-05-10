#include <stdio.h>

#define M_PI 3.14159265

double my_fmod(double x, double y) {
    if (y == 0.0) {
        return 0; // 如果除数为零，则返回错误或零（标准库的 fmod 在这种情况下返回 NaN）
    }
    
    double result = x;
    while (result >= y) {
        result -= y;
    }
    while (result < 0) {
        result += y;
    }

    return result;
}

double factorial(int n) {
    double result = 1.0;
    for (int i = 1; i <= n; i++) {
        result *= i;
    }
    return result;
}

double power(double base, int exp) {
    double result = 1.0;
    while (exp > 0) {
        result *= base;
        exp--;
    }
    return result;
}

// 规范化角度到 [-π, π]
double normalize_angle(double x) {
    x = my_fmod(x, 2 * M_PI); // fmod 函数用于计算浮点数除法的余数
    if (x < -M_PI) {
        x += 2 * M_PI;
    } else if (x > M_PI) {
        x -= 2 * M_PI;
    }
    return x;
}

// 手动实现sin函数（泰勒级数）
double my_sin(double x) {
    const int terms = 10; // 使用级数的项数
    double sum = 0.0;

    x = normalize_angle(x);

    for (int n = 0; n < terms; ++n) {
        double term = power(-1, n) * power(x, 2 * n + 1) / factorial(2 * n + 1);
        sum += term;
    }

    return sum;
}

// 手动实现cos函数（泰勒级数）
double my_cos(double x) {
    const int terms = 10; // 使用级数的项数
    double sum = 0.0;

    x = normalize_angle(x);

    for (int n = 0; n < terms; ++n) {
        double term = power(-1, n) * power(x, 2 * n) / factorial(2 * n);
        sum += term;
    }

    return sum;
}


// 定义复数结构体
typedef struct Complex {
    double real;
    double imag;
} Complex;

// 原地计算FFT
void fft(Complex *x, int N) {
    // 如果只有一个元素，就直接返回
    if (N <= 1) return;

    // 将偶数索引和奇数索引的元素分别放入不同的数组
    Complex even[N/2];
    Complex odd[N/2];
    for (int i = 0; i < N/2; ++i) {
        even[i] = x[i*2];
        odd[i] = x[i*2 + 1];
    }

    // 递归计算
    fft(even, N/2);
    fft(odd, N/2);

    // 合并结果
    for (int k = 0; k < N/2; ++k) {
        double angle = -2 * M_PI * k / N;
        Complex t = {my_cos(angle) * odd[k].real - my_sin(angle) * odd[k].imag,
                     my_sin(angle) * odd[k].real + my_cos(angle) * odd[k].imag};
        x[k].real = even[k].real + t.real;
        x[k].imag = even[k].imag + t.imag;
        x[k+N/2].real = even[k].real - t.real;
        x[k+N/2].imag = even[k].imag - t.imag;
    }
}

// 打印数组
void print_array(Complex *x, int N) {
    for (int i = 0; i < N; i++)
        printf("(%f, %f)\n", x[i].real, x[i].imag);
}

// 主函数
int main() {
    // 输入数据集合
    Complex signal[] = {
        {1.0, 0.0},
        {1, 0.0},
        {1.0, 0.0},
        {0, 0.0},
        {1.0, 0.0},
        {0.0, 0.0},
        {1.0, 0.0},
        {0.0, 0.0}
    };

    // FFT的长度必须为2的幂
    int N = sizeof(signal) / sizeof(signal[0]);

    // 计算FFT
    fft(signal, N);

    // 输出结果
    print_array(signal, N);

    return 0;
}