#ifndef LIBSUPERCPP4WIN_ARRAY_H
#define LIBSUPERCPP4WIN_ARRAY_H

#include <memory>

template<class T>
class Array {
public:
    //�б��ʼ��
    Array(const std::initializer_list<T> listT)
            : arrayLen(listT.size()), arraySize(listT.size() * sizeof(T)) {
        if (listT.size() == 0)
            return;
        array = std::make_unique<T[]>(arrayLen);
        for (int i = 0; i < arrayLen; ++i)
            array[i] = *(listT.begin() + i);
    }

    //����ָ���Ԫ�ظ�������
    Array(T *ptr, size_t arrayLen)
            : arrayLen(arrayLen), arraySize(arrayLen * sizeof(T)) {
        if (arrayLen == 0)
            return;
        array = std::make_unique<T[]>(arrayLen);
        for (int i = 0; i < arrayLen; ++i)
            array[i] = ptr[i];
    }

    //��������
    Array(const Array &rSelf)
            : arrayLen(rSelf.arrayLen), arraySize(rSelf.arraySize) {
        this->array = std::make_unique<T[]>(rSelf.arrayLen);
        for (int i = 0; i < rSelf.arrayLen; ++i)
            this->array[i] = rSelf.array[i];
    }

    //������ֵ
    Array &operator=(const Array &rSelf);

    //ȡֵ
    T &operator[](size_t pos) { return array[pos]; }

    size_t resetLen() {}

    //ȡԪ�ظ���
    size_t getLen() const { return arrayLen; }

    //ȡռ���ڴ�
    size_t getSize() const { return arraySize; }

private:
    std::unique_ptr<T[]> array;
    //Ԫ�ظ���
    size_t arrayLen = 0;
    //ռ���ֽ�
    size_t arraySize = 0;
};



#endif //LIBSUPERCPP4WIN_ARRAY_H
