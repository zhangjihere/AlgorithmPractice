#include<stdio.h>

/**
 * get absolute value, 获取绝对值
 * @param f
 * @return
 */
float Q_fabs(float f) {
    int tmp = *(int *) &f;
    tmp &= 0x7FFFFFFF;
    return *(float *) &tmp;
}

int main() {
    float f = -121;
    printf("%f:", Q_fabs(f));
    return 0;
}