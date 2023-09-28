#include <iostream>
#include <FileHandle.hpp>
#include <AIOWriter.hpp>
#include <AIOReader.hpp>

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

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
    reader.read();

    std::string s;
    std::cin >> s;
    return 0;
}
