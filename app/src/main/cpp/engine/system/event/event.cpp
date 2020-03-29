#include "event.hpp"

#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <cassert>
#include <cstring>
#include <stdexcept>

namespace {
    int fd[2]{0, 0};
}

namespace engine::system::event {
    int init() {
        if (pipe(fd) < 0) {
            return -1;
        }
        if (fcntl(fd[0], F_SETFL, O_NONBLOCK) < 0) {
            return -1;
        }

        return 0;
    }

    void quit() {
        close(fd[0]);
        close(fd[1]);
    }

    void push(Message message) {
        assert(fd[1] != 0);

        write(fd[1], &message, sizeof(Message));
    }

    bool poll(Message &message) {
        assert(fd[0] != 0);

        switch (read(fd[0], &message, sizeof(Message))) {
            case -1:
                if (errno == EAGAIN) {
                    return false;
                }
                std::__throw_runtime_error(strerror(errno));
            case 0:
                return false;
            default:
                break;
        }
        return true;
    }
}