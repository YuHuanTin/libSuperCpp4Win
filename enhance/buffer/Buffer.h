

// 实现只读 Buffer
template<typename T>
class Buffer {
public:
    Buffer() = delete;

    Buffer(T *Ptr, std::size_t Count) : m_buffer(Ptr), m_maxPostion(Count) {

    }

    template<typename GetType>
    GetType read() {
        GetType type1;
        type1 = *(GetType *)(m_buffer + m_currentPostion);
        m_currentPostion += sizeof(GetType);
        return type1;
    }

    template<std::size_t GetBytes>
    void read(T *Ptr) {
        for (std::size_t i = 0; i < GetBytes; ++i) {
            *(Ptr + i) = *(m_buffer + m_currentPostion + i);
        }
        m_currentPostion += GetBytes;
    }

    bool eof() {
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