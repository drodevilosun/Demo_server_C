#include <iostream>
#include <string>
#include <mutex>

class Logger {
public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    template <typename T>
    void log(const T& value) {
        std::lock_guard<std::mutex> lock(mutex);
        std::cout << value << std::endl;
    }

private:
    Logger() {}
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::mutex mutex;
};
