#include <iostream>
#include <FileHandle.hpp>
#include <AIOWriter.hpp>
#include <AIOReader.hpp>
#include <AIONotifier.hpp>

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include <thread>
#include <functional>

void wait_signal(std::function<void()> &&callable)
{
    std::string s;
    std::cin >> s;
    callable();
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << "Usage: <src_filename> <dst_filename>.";
        return 0;
    }

    FileHandle in(argv[1], O_ASYNC | O_RDONLY);
    if (!in)
        return -1;
    FileHandle out(argv[2], O_ASYNC | O_TRUNC | O_WRONLY);
    if (!out)
        return -2;

    AIOWriter writer(out);
    AIOReader reader(in, 0, 0, [&writer](AIOWrapper::byte *buf, size_t sz, bool eof)
                     { writer.write(buf, sz, eof); });
    AIONotifier notifier({&writer, &reader});

    std::thread th_wait(wait_signal, [&notifier]()
                        { notifier.stop_all(); });
    reader.read();

    th_wait.join();

    return 0;
}
