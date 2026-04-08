#include "Logger.hh"

int main()
{
    auto logTest= [] {
        [](void(int *pType)) {
            LOG_DEBUG("pType");
        }(nullptr);

        [](void (*pFunc)(int *pArgs)) {
            LOG_DEBUG("pFunc");
        }(nullptr);

        LOG_MSG(I, "Hello, MSG!");
        LOG_ALL("Hello, ALL!");
        LOG_TRACE("Hello, TRACE!");
        LOG_DEBUG("Hello, DEBUG!");
        LOG_INFO("Hello, INFO!");
        LOG_WARN("Hello, WARN!");
        LOG_ERROR("Hello, ERROR!");
        LOG_FATAL("Hello, FATAL!");
    };

    logTest();

    return 0;
}
