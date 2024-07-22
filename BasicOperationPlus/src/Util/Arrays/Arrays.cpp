#include "Arrays.h"

template<typename T, int N>

constexpr int array_size(T (&)[N]){
    return N;
}