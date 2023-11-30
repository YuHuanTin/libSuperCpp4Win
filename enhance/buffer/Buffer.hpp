#ifndef LIBSUPERCPP4WIN_BUFFER_HPP
#define LIBSUPERCPP4WIN_BUFFER_HPP


#include <cstddef>
#include <iterator>
#include <stdexcept>


/**
 * 一个存储 T 类型的可读可写 Buffer 类
 * @tparam T 
 */
template<typename T>
class Buffer {
private:

    /**
     * 检测 当前位置 + CheckType * ElementCount 是否超出了最大位置，默认为 T 类型
     * @tparam CheckType 
     * @param ElementCount 
     * @return 
     */
    template<typename CheckType = T>
    constexpr void check_out_of_range(size_t ElementCount) const {
        if (m_currentPostion + ElementCount * sizeof(CheckType) > m_maxPostion)
            throw std::runtime_error("out of range");
    }

    template<typename TransferType>
    constexpr TransferType *transfer_type_ptr(size_t Offset) const {
        /// check if out of range
        check_out_of_range<TransferType>(1);


        /// transfer type ptr
        return reinterpret_cast<std::remove_reference_t<TransferType> *>(m_buffer + Offset);
    }

public:
    Buffer() = delete;

    constexpr Buffer(T *Begin, T *End) : Buffer(Begin, std::distance(Begin, End)) {}

    constexpr Buffer(T *Ptr, size_t Count) : m_buffer(Ptr), m_maxPostion(Count) {}

    Buffer &operator=(const Buffer &Right) {
        if (&Right == this) {
            return *this;
        }

        m_buffer = Right.m_buffer;
        m_currentPostion = Right.m_currentPostion;
        m_maxPostion = Right.m_maxPostion;
        return *this;
    }

    Buffer &operator=(Buffer &&Right) noexcept {
        if (&Right == this) {
            return *this;
        }

        m_buffer = Right.m_buffer;
        m_currentPostion = Right.m_currentPostion;
        m_maxPostion = Right.m_maxPostion;

        Right.m_buffer = nullptr;
        Right.m_currentPostion = 0;
        Right.m_maxPostion = 0;
        return *this;
    }

    [[nodiscard]] constexpr size_t getCurrnetPos() const noexcept {
        return m_currentPostion;
    }

    [[nodiscard]] constexpr size_t getMaxPos() const noexcept {
        return m_maxPostion;
    }

    /**
     * 获取当前位置的 T 元素，而不增加位移
     * @return 
     */
    constexpr T &getCurrentElement() const {
        return *transfer_type_ptr<T>(m_currentPostion);
    }

    /**
     * 从当前位置读取一个 ReadType 元素，并增加位移
     * @tparam ReadType 
     * @return 
     */
    template<typename ReadType>
    constexpr ReadType read() {
        auto ptr = transfer_type_ptr<ReadType>(m_currentPostion);

        next<ReadType>(1);
        return *ptr;
    }

    /**
     * 读取 ElementCount 个元素到 Ptr 为起始地址的空间，并增加位移
     * @tparam ElementCount 
     * @param Ptr 
     * @return 
     */
    template<size_t ElementCount>
    constexpr void read(T *Ptr) {
        read(Ptr, ElementCount);
    }

    /**
     * 读取 ElementCount 个元素到 Ptr 为起始地址的空间，并增加位移
     * @param Ptr 
     * @param ElementCount 
     * @return 
     */
    constexpr void read(T *Ptr, size_t ElementCount) {
        check_out_of_range(ElementCount);

        std::copy(m_buffer + m_currentPostion, m_buffer + m_currentPostion + ElementCount * sizeof(T), Ptr);
        next(ElementCount);
    }

    /**
     * 在当前位置写入 WriteType 类型的值，并增加位移
     * @tparam WriteType 
     * @param WriteValue 
     */
    template<typename WriteType>
    void write(WriteType &&WriteValue) {
        *transfer_type_ptr<WriteType>(m_currentPostion) = WriteValue;

        next<WriteType>(1);
    }

    /**
     * 在当前位置写入 Count 个 WriteType 类型的值，并增加位移
     * @tparam WriteType 
     * @param Ptr 
     * @param Count 
     */
    template<typename WriteType>
    void write(WriteType *Ptr, size_t Count) {
        check_out_of_range<WriteType>(Count);

        std::copy(Ptr, Ptr + Count, m_buffer + m_currentPostion);
        next<WriteType>(Count);
    }

    /**
     * 在当前位置写入 Begin 到 End 的 WriteType 类型的值，并增加位移
     * @tparam WriteType 
     * @param Begin 
     * @param End 
     */
    template<typename WriteType>
    void write(WriteType *Begin, WriteType *End) {
        write(Begin, std::distance(Begin, End));
    }

    /**
     * 向前移动指针，默认移动 Count 个 T 类型的位置，可为 Count 个 PrevType 类型的位置
     * @tparam PrevType 
     * @param Count 
     * @return 
     */
    template<typename PrevType = T>
    constexpr void prev(size_t Count) noexcept {
        m_currentPostion -= Count * sizeof(PrevType);
    }

    /**
     * 向后移动指针，默认移动 Count 个 T 类型的位置，可为 Count 个 NextType 类型的位置
     * @tparam NextType 
     * @param Count 
     * @return 
     */
    template<typename NextType = T>
    constexpr void next(size_t Count) noexcept {
        m_currentPostion += Count * sizeof(NextType);
    }

    /**
     * 移动 T 类型指针到 Pos 位置
     * @param Pos 
     * @return 
     */
    constexpr void moveTo(size_t Pos) noexcept {
        m_currentPostion = Pos;
    }

    /**
     * 重置指针位置到 0
     * @return 
     */
    constexpr void reset() noexcept {
        m_currentPostion = 0;
    }

    /**
     * 检测是否已经处理到结束位置
     * @return 
     */
    [[nodiscard]] constexpr bool eof() const {
        return m_currentPostion >= m_maxPostion;
    }

private:
    T *m_buffer = nullptr;
    size_t m_currentPostion = 0;
    size_t m_maxPostion = 0;
};

#endif //LIBSUPERCPP4WIN_BUFFER_HPP
