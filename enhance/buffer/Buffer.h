#ifndef LIBSUPERCPP4WIN_BUFFER_H
#define LIBSUPERCPP4WIN_BUFFER_H


#include <cstddef>

// 实现只读 Buffer
template<typename T>
class Buffer {
public:
    Buffer() = delete;

    constexpr Buffer(T *Ptr, std::size_t Count) : m_buffer(Ptr), m_maxPostion(Count) {

    }

    constexpr T *getCurrentElement() const noexcept {
        return m_buffer + m_currentPostion;
    }

    [[nodiscard]] constexpr std::size_t getCurrnetPos() const noexcept {
        return m_currentPostion;
    }

    [[nodiscard]] constexpr std::size_t getMaxPos() const noexcept {
        return m_maxPostion;
    }

    template<typename GetType>
    constexpr GetType read() noexcept {
        GetType type1;
        type1 = *(GetType *) (m_buffer + m_currentPostion);
        m_currentPostion += sizeof(GetType);
        return type1;
    }

    template<std::size_t GetBytes>
    constexpr void read(T *Ptr) noexcept {
        read(Ptr, GetBytes);
    }

    constexpr void read(T *Ptr, std::size_t Count) noexcept {
        for (std::size_t i = 0; i < Count; ++i) {
            *(Ptr + i) = *(m_buffer + m_currentPostion + i);
        }
        m_currentPostion += Count;
    }

    constexpr void skip(std::size_t Count) noexcept {
        m_currentPostion += Count;
    }

    [[nodiscard]] constexpr bool eof() const noexcept {
        if (m_currentPostion >= m_maxPostion) {
            return true;
        }
        return false;
    }
private:
    T *const m_buffer = nullptr;
    std::size_t m_currentPostion = 0;
    std::size_t m_maxPostion     = 0;
};

#endif //LIBSUPERCPP4WIN_BUFFER_H
