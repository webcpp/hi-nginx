#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include "utils.hpp"
#include "file_mmap.hpp"

namespace hi
{

    file_mmap::file_mmap() : data()
    {
    }

    file_mmap::~file_mmap()
    {
        for (auto &i : this->data)
        {
            munmap(i.second.first, i.second.second.st_size);
        }
    }

    bool file_mmap::get(const std::string &path, std::pair<char *, struct stat> &ele)
    {
        std::string mmap_key = std::move(hi::md5(path));
        std::unordered_map<std::string, std::pair<char *, struct stat>>::const_iterator iter;
        struct stat st;
        if (stat(path.c_str(), &st) == 0)
        {
            if (S_ISREG(st.st_mode))
            {
                if ((iter = this->data.find(mmap_key)) != this->data.end())
                {
                    if (iter->second.second.st_mtime != st.st_mtime)
                    {
                        munmap(iter->second.first, iter->second.second.st_size);
                        this->data.erase(iter);
                        goto js_read;
                    }
                    ele.first = iter->second.first;
                    ele.second = iter->second.second;
                    return true;
                }
                else
                {
                js_read:
                    int ffd = open(path.c_str(), O_RDONLY | O_NONBLOCK);
                    if (ffd > 0)
                    {
                        char *mmap_ptr = (char *)mmap(0, st.st_size, PROT_READ, MAP_PRIVATE | MAP_POPULATE, ffd, 0);
                        if (mmap_ptr == MAP_FAILED)
                        {
                            close(ffd);
                        }
                        else
                        {
                            close(ffd);
                            if (madvise(mmap_ptr, st.st_size, MADV_SEQUENTIAL) == 0)
                            {
                                ele.first = mmap_ptr;
                                ele.second = st;
                                this->data[mmap_key] = std::move(std::make_pair(mmap_ptr, st));
                                return true;
                            }
                            else
                            {
                                munmap(mmap_ptr, st.st_size);
                            }
                        }
                    }
                }
            }
        }
        else if ((iter = this->data.find(mmap_key)) != this->data.end())
        {
            munmap(iter->second.first, iter->second.second.st_size);
            this->data.erase(iter);
        }
        return false;
    }

    std::pair<char *, struct stat> file_mmap::get(const std::string &path)
    {
        std::pair<char *, struct stat> st;
        if (this->get(path, st))
        {
            return st;
        }
        return st;
    }

} // namespace hi