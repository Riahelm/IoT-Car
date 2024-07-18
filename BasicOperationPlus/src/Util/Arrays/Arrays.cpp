#include "Arrays.h"

template <typename T>

int getArraySize(T* array){
    return (sizeof(array) / sizeof(T));
}