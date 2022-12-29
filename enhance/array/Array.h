#ifndef LIBSUPERCPP4WIN_ARRAY_H
#define LIBSUPERCPP4WIN_ARRAY_H

#include <memory>

template<class T>
class Array {
public:
    //列表初始化
    Array(const std::initializer_list<T> listT)
            : arrayLen(listT.size()), arraySize(listT.size() * sizeof(T)) {
        if (listT.size() == 0)
            return;
        array = std::make_unique<T[]>(arrayLen);
        for (int i = 0; i < arrayLen; ++i)
            array[i] = *(listT.begin() + i);
    }

    //以裸指针和元素个数构造
    Array(T *ptr, size_t arrayLen)
            : arrayLen(arrayLen), arraySize(arrayLen * sizeof(T)) {
        if (arrayLen == 0)
            return;
        array = std::make_unique<T[]>(arrayLen);
        for (int i = 0; i < arrayLen; ++i)
            array[i] = ptr[i];
    }

    //拷贝构造
    Array(const Array &rSelf)
            : arrayLen(rSelf.arrayLen), arraySize(rSelf.arraySize) {
        this->array = std::make_unique<T[]>(rSelf.arrayLen);
        for (int i = 0; i < rSelf.arrayLen; ++i)
            this->array[i] = rSelf.array[i];
    }

    //拷贝赋值
    Array &operator=(const Array &rSelf);

    //取值
    T &operator[](size_t pos) { return array[pos]; }

    size_t resetLen() {}

    //取元素个数
    size_t getLen() const { return arrayLen; }

    //取占用内存
    size_t getSize() const { return arraySize; }

private:
    std::unique_ptr<T[]> array;
    //元素个数
    size_t arrayLen = 0;
    //占用字节
    size_t arraySize = 0;
};



#endif //LIBSUPERCPP4WIN_ARRAY_H
