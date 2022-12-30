
#include "Array.h"

template<class T>
Array<T> &Array<T>::operator=(const Array<T> &rSelf) {
    if (this == &rSelf || this->array == rSelf.array)
        return *this;
    this->array.reset(rSelf.array.get());
    this->arrayLen = rSelf.arrayLen;
    this->arraySize = rSelf.arraySize;
    return *this;
}