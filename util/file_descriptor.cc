#include "file_descriptor.hh"

FileDescriptor::FDWrapper::FDWrapper(int fd) : fd_(fd)
{
}

// public constructor.
FileDescriptor::FileDescriptor(int fd)
    : internal_fd_(std::make_shared<FDWrapper>(fd)) {}

// private constructor. increase the shared_ptr count.
FileDescriptor::FileDescriptor(std::shared_ptr<FDWrapper> other_shared_ptr)
    : internal_fd_(std::move(other_shared_ptr)) {}