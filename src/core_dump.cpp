#include "core_dump.hpp"

#include <unistd.h>
#include <linux/prctl.h>  /* Definition of PR_* constants */
#include <sys/prctl.h>
#include <sys/resource.h>
#include <memory>

static bool SetCoreDumpFilter(std::uint8_t value)
{
    auto filepath = std::string{"/proc/"} + std::to_string(getpid()) + std::string{"/coredump_filter"};
    std::unique_ptr<FILE, void(*)(FILE*)> file(std::fopen(filepath.c_str(), "w"), [](auto f){ std::fclose(f); });

    return file && file.get() && std::fprintf(file.get(), "Ox%02X", value) == 4;
}

bool SetCoreDumpState(bool up, std::uint64_t limit, std::uint8_t flags)
{
    if (prctl(PR_SET_DUMPABLE, up) < 0)
    {
        return false;
    }

    struct rlimit64 rlim{};
    if (getrlimit64(RLIMIT_CORE, &rlim) != 0)
    {
        return false;
    }

    rlim.rlim_cur = (up) ? limit : 0;
    rlim.rlim_max = (up) ? limit : 0;
    if (setrlimit64(RLIMIT_CORE, &rlim) != 0)
    {
        return false;
    }

    return SetCoreDumpFilter(flags);
}