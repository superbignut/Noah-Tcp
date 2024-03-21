#pragma once
#include <string>
#include <string_view>
#include <memory>

// A reference-counted handle to a file descriptor.
class FileDescriptor
{
private:
    class FDWrapper
    {
    public:
        explicit FDWrapper(int fd);

        ~FDWrapper(); // No virtual.

        void close();

        template <typename T>
        T CheckSystemCall(std::string_view s_attempt, T return_value) const;

        FDWrapper(const FDWrapper &other) = delete;

        FDWrapper &operator=(const FDWrapper &other) = delete;

        FDWrapper(FDWrapper &&other) = delete;

        FDWrapper &operator=(FDWrapper &&other) = delete;

    public:
        int fd_;                    // The file descriptor number return by the kernel
        bool eof_ = false;          // Whether fd_ is at EOF
        bool colsed_ = false;       // Whether fd_ has been closed
        bool non_blocking_ = false; // Whether fd_ is non_blocked
        unsigned read_count_ = 0;   // How many times has been read
        unsigned write_count_ = 0;  // How many times has been write
    };

    std::shared_ptr<FDWrapper> internal_fd_;

    // private constructor - can't be used outside.
    explicit FileDescriptor(std::shared_ptr<FDWrapper> other_shared_ptr);

protected:

    static constexpr std::size_t kReadBufferSize = 16384; // 2^14 

    void set_eof(){}

    void register_read(){}

    void register_write(){}

    template<typename T>
    T CheckSystemCall(std::string_view s_attempt, T return_value) const;

public:
    explicit FileDescriptor(int fd);

    ~FileDescriptor() = default;

    FileDescriptor(const FileDescriptor& other) = delete; // copy is forbidden

    FileDescriptor& operator=(const FileDescriptor& other) = delete; // copy is forbidden

    FileDescriptor(FileDescriptor&& other) = default; // move if allowed

    FileDescriptor& operator=(FileDescriptor&& other) = default; // move is allowed

};
