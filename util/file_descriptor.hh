#pragma once
#include <string>
#include <string_view>
#include <memory>
#include <vector>

// A reference-counted handle to a file descriptor.
class FileDescriptor
{
private:
    // FDWrapper : a handle on a kernel file descriptor.
    // FileDescriptor has a std::shared_ptr to FDWrapper.
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

    // private means can't be used outside.
    // explicit means can't be used as FileDescriptor a = make_shared<>(xxx)
    explicit FileDescriptor(std::shared_ptr<FDWrapper> other_shared_ptr);

protected:
    static constexpr std::size_t kReadBufferSize = 16384; // 2^14

    void set_eof() {}

    void register_read() {}

    void register_write() {}

    template <typename T>
    T CheckSystemCall(std::string_view s_attempt, T return_value) const;

public:
    // Construct from a file descriptor number returned by kernel.
    explicit FileDescriptor(int fd);

    ~FileDescriptor() = default;

    FileDescriptor(const FileDescriptor &other) = delete; // copy is forbidden

    FileDescriptor &operator=(const FileDescriptor &other) = delete; // copy is forbidden

    FileDescriptor(FileDescriptor &&other) = default; // move if allowed

    FileDescriptor &operator=(FileDescriptor &&other) = default; // move is allowed

    void read(std::string &buffer);
    void read(std::vector<std::string> &buffers);

    std::size_t write(std::string_view buffer);
    std::size_t write(const std::vector<std::string_view> &buffer);
    std::size_t write(const std::vector<std::string> &buffer);

    void close() { internal_fd_->close(); }

    FileDescriptor duplicate() const;

    void set_blocking(bool blocking);

    off_t size() const;

    int fd_num() const;

    bool eof() const;

    bool closed() const;

    unsigned int read_count() const;

    unsigned int write_count() const;
};
