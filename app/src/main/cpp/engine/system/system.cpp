#include "system.hpp"
#include <array>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>

namespace engine::system::event {
    namespace {
        int fd[2] {0, 0};
    }

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
        perror("push");
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

