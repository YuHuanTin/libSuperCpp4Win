#ifndef LIBSUPERCPP4WIN_ARRAY_H
#define LIBSUPERCPP4WIN_ARRAY_H

#include <algorithm>
#include <cstddef>
#include <memory>


template<typename T>
class Array {
public: // ctor functions
    Array() = delete;

    /**
     * @example
     * Array a1{1, 2, 3, 4, 5}; \n
     * Array a2(a1);
     * @param CopyCtor
     */
    Array(const Array &CopyCtor)
            : arrayLen(CopyCtor.arrayLen), arrayCapacity(CopyCtor.arrayCapacity) {
        if (!CopyCtor.array) return;
        array = std::make_unique<T[]>(arrayLen);
        std::copy(CopyCtor.array.get(), CopyCtor.array.get() + CopyCtor.arrayLen, array.get());
    }

    /**
     * @example
     * Array a1{1, 2, 3, 4, 5}; \n
     * Array a2(std::move(a1));
     * @param MoveCtor
     */
    Array(Array &&MoveCtor) noexcept
            : arrayLen(MoveCtor.arrayLen), arrayCapacity(MoveCtor.arrayCapacity) {
        if (!MoveCtor.array) return;
        array = std::move(MoveCtor.array);
    }

    /**
     * @example
     * Array a(100,-1);
     * @param ArraySize
     * @param DefaultValue
     */
    explicit Array(const std::size_t ArraySize, const T DefaultValue = 0)
            : arrayLen(ArraySize), arrayCapacity(ArraySize) {
        if (arrayLen == 0) return;
        array = std::make_unique<T[]>(arrayLen);
        std::fill(array.get(), array.get() + arrayLen, DefaultValue);
    }

    /**
     * @example
     * Array a = {1,2,3,4,5,6}; \n
     * Array a{1,2,3,4,5,6};
     * @param InitializerList
     */
    Array(const std::initializer_list<T> &InitializerList)
            : arrayLen(InitializerList.size()), arrayCapacity(InitializerList.size()) {
        if (arrayLen == 0) return;
        array = std::make_unique<T[]>(arrayLen);
        std::copy(InitializerList.begin(), InitializerList.end(), array.get());
    }

    /**
     * @example
     * auto str = new char[6]{"hello"}; \n
     * Array a(str, 6); \<- copy '\0'
     * @param RawPtr
     * @param ArraySize
     */
    Array(const T *RawPtr, std::size_t ArraySize)
            : arrayLen(ArraySize), arrayCapacity(ArraySize) {
        if (RawPtr == nullptr || arrayLen == 0) return;
        array = std::make_unique<T[]>(arrayLen);
        std::copy(RawPtr, RawPtr + arrayLen, array.get());
    }

    /**
     * @example
     * std::vector\<int> v{1, 2, 3, 4, 5, 6, 7}; \n
     * Array\<int> a(v.begin(), v.end());
     * @param Begin
     * @param End
     */
    template<typename Iter, typename std::enable_if_t<!std::is_same<typename std::iterator_traits<Iter>::value_type, void>::value, int> = 0>
    Array(const Iter &Begin, const Iter &End) {
        if (Begin == End) return;
        std::size_t size = End - Begin;
        arrayLen = size;
        arrayCapacity = size;
        array = std::make_unique<T[]>(arrayLen);
        std::copy(Begin, End, array.get());
    }

    /**
     * @example
     * Array a1{1, 2, 3, 4, 5}; \n
     * Array a2 = a1;
     * @param CopyAssignment
     */
    Array &operator=(const Array &CopyAssignment) {
        if (this == &CopyAssignment || CopyAssignment.arrayLen == 0) {
            return *this;
        }

        arrayLen = CopyAssignment.arrayLen;
        arrayCapacity = CopyAssignment.arrayCapacity;
        array = std::make_unique<T[]>(arrayLen);
        std::copy(CopyAssignment.array.get(), CopyAssignment.array.get() + arrayLen, array.get());
        return *this;
    }

    /**
     * @example
     * Array a1{1, 2, 3, 4, 5}; \n
     * Array a2 = std::move(a1);
     * @param MoveAssignment
     */
    Array &operator=(Array &&MoveAssignment) noexcept {
        if (this == &MoveAssignment || MoveAssignment.arrayLen == 0) {
            return *this;
        }

        arrayLen = MoveAssignment.arrayLen;
        arrayCapacity = MoveAssignment.arrayCapacity;
        array = std::move(MoveAssignment.array);

        MoveAssignment.arrayLen = 0;
        MoveAssignment.arrayCapacity = 0;
        return *this;
    }
public: // element changer
    T &operator[](std::size_t Pos) const {
        checkIsOutRange(Pos);
        return array[Pos];
    }

    Array &push_front() {

    }

    Array &push_back() {

    }

    Array &insert(std::size_t Pos, const T *InsertBegin, std::size_t InsertLen) {
        checkInsertPos(Pos);
        const auto &size = InsertLen;
        if (size == 0) return *this;

        if (arrayLen + size > arrayCapacity) {
            std::unique_ptr<T[]> newArray = capacityGrow(size);
            std::move(array.get(), array.get() + Pos, newArray.get());
            std::copy(InsertBegin, InsertBegin + InsertLen, newArray.get() + Pos);
            std::move(array.get() + Pos, array.get() + arrayLen, newArray.get() + size + Pos);
            array = std::move(newArray);
        } else {
            shiftElement(Pos, size);
            std::copy(InsertBegin, InsertBegin + InsertLen, array.get() + Pos);
        }

        arrayLen += size;
        return *this;
    }

    Array &insert(std::size_t Pos, const std::initializer_list<T> &InitializerList) {
        checkInsertPos(Pos);
        const auto &size = InitializerList.size();
        if (size == 0) return *this;

        if (arrayLen + size > arrayCapacity) {
            std::unique_ptr<T[]> newArray = capacityGrow(size);
            std::move(array.get(), array.get() + Pos, newArray.get());
            std::copy(InitializerList.begin(), InitializerList.end(), newArray.get() + Pos);
            std::move(array.get() + Pos, array.get() + arrayLen, newArray.get() + size + Pos);
            array = std::move(newArray);
        } else {
            shiftElement(Pos, size);
            std::copy(InitializerList.begin(), InitializerList.end(), array.get() + Pos);
        }
        arrayLen += size;
        return *this;
    }

    template<typename Iter, typename std::enable_if_t<!std::is_same<typename std::iterator_traits<Iter>::value_type, void>::value, int> = 0>
    Array &insert(std::size_t Pos, const Iter &Begin, const Iter &End) {
        checkInsertPos(Pos);
        std::size_t size = End - Begin;
        if (size == 0) return *this;

        if (arrayLen + size > arrayCapacity) {
            std::unique_ptr<T[]> newArray = capacityGrow(size);
            std::move(array.get(), array.get() + Pos, newArray.get());
            std::copy(Begin, End, newArray.get() + Pos);
            std::move(array.get() + Pos, array.get() + arrayLen, newArray.get() + size + Pos);
            array = std::move(newArray);
        } else {
            shiftElement(Pos, size);
            std::copy(Begin, End, array.get() + Pos);
        }

        arrayLen += size;
        return *this;
    }

public:
    std::size_t size() const { return arrayLen; }
private:
    // check index pos
    void checkIsOutRange(std::size_t Pos) const {
        // index must be [Begin, End)
        if (Pos >= arrayLen) throw std::runtime_error("index out of range");
    }
    // check insert pos
    void checkInsertPos(std::size_t Pos) const {
        // insert must be [Begin, End]
        if (Pos > arrayLen) throw std::runtime_error("insert pos out of range");
    }
    // grow array capacity
    // new capacity is greater than the old capacity
    std::unique_ptr<T[]> capacityGrow(std::size_t NeedCapacity) {
        // Determine whether to use 1.5 old capacity or old capacity + need to increase space
        arrayCapacity = std::max(arrayCapacity + NeedCapacity, (std::size_t) (arrayCapacity * 1.5));
        return std::make_unique<T[]>(arrayCapacity);
    }

    // move all elements starting at Pos by Offset distance
    void shiftElement(std::size_t Pos, std::size_t Offset) {
        const auto backElementNum = arrayLen - Pos;
        for (std::size_t i = 0; i < backElementNum; ++i) {
            array[arrayLen - 1 + Offset - i] = array[arrayLen - 1 - i];
        }
    }
private:
    std::unique_ptr<T[]> array;
    std::size_t arrayLen = 0;
    std::size_t arrayCapacity = 0;
};


#endif //LIBSUPERCPP4WIN_ARRAY_H
