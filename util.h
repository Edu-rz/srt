#include <stdio.h>

// Rellenar un arreglo de 1 dimensión con el parametro `value`
void Initialize1DArray(int length, int* array, int value) {
    for (int i = 0; i < length; i++){
        array[i] = value;
    }
}

// Rellenar un arreglo de 2 dimensiones con el parametro `value`
void Initialize2DArray(int length, int height, int* array, int value) {
    for (int i = 0; i < length * height; i++){
        array[i] = value;
    }
}

void ColorBlack() {
    printf("\033[0;30m");
}

void ColorRed() {
    printf("\033[0;31m");
}

void ColorGreen() {
    printf("\033[0;32m");
}

void ColorYellow() {
    printf("\033[0;33m");
}

void ColorBlue() {
    printf("\033[0;34m");
}

void ColorPurple() {
    printf("\033[0;35m");
}

void ColorCyan() {
    printf("\033[0;36m");
}

void ColorWhite() {
    printf("\033[0;37m");
}

void ColorReset () {
    printf("\033[0m");
}