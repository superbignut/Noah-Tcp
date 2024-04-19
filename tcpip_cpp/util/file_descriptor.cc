#include "file_descriptor.hh"
#include <stdexcept>
#include <exception.hh>

FileDescriptor::FDWrapper::FDWrapper(int fd) : fd_(fd) {}

template <typename T>
T FileDescriptor::FDWrapper::CheckSystemCall(std::string_view s_attempt, T return_value) const
{
    if (return_value > 0)
        return return_value;

    if (non_blocking_ and (errno == EAGAIN or errno == EINPROGRESS))
    {
        return 0;
    }

    throw unix_error{s_attempt};
}
// public constructor.
FileDescriptor::FileDescriptor(int fd)
    : internal_fd_(std::make_shared<FDWrapper>(fd)) {}

// private constructor. increase the shared_ptr count.
FileDescriptor::FileDescriptor(std::shared_ptr<FDWrapper> other_shared_ptr)
    : internal_fd_(std::move(other_shared_ptr)) {}

template <typename T>
T FileDescriptor::CheckSystemCall(std::string_view s_attempt, T return_value) const
{
    if (not internal_fd_)
    {
        throw runtime_error("internal error: missing internal_fd_");
    }
    return internal_fd_->CheckSystemCall(s_attempt, return_value);
}