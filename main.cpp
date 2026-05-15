#include <atomic>
#include <charconv>
#include <cstring>
#include <iostream>
#include <getopt.h>
#include <thread>
#include <csignal>
#include "include/all.hpp"

std::atomic finish_required{false};
std::atomic<size_t> renew_counter{0};
std::atomic exception_was_thrown{false};
int return_code = 0;

void sigterm_process(int signal) {
    if (signal != SIGTERM) return;
    finish_required.store(true, std::memory_order_release);
}

int main(int argc, char *argv[]) {

    std::signal(SIGTERM, sigterm_process);

    std::filesystem::path path_to_dir = std::getenv("HOME"); // home by def
    size_t time_dealy_in_milliseconds = 300000; // 5 mins by def

    static option long_options[] = {
        {"path", required_argument, 0, 'p'},
        {"time", required_argument, 0, 't'},
        {0, 0, 0, 0}
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "p:t:", long_options, nullptr)) > -1) {
        switch (opt) {
        case 'p': path_to_dir = optarg;
            break;
        case 't': std::from_chars(optarg, optarg + std::strlen(optarg), time_dealy_in_milliseconds);
            break;
        default:
            std::cerr << "Undefined argument: " << optarg << std::endl;
            return 1;
        }
    }

    auto task = [time_dealy_in_milliseconds, path_to_dir = std::move(path_to_dir)]() {
        std::chrono::milliseconds delay(time_dealy_in_milliseconds);
        while (!finish_required.load(std::memory_order_acquire) && !exception_was_thrown.load(std::memory_order_acquire)) {
            try {
                task::finder::find(path_to_dir);
            } catch (const std::exception &ex) {
                exception_was_thrown.store(true, std::memory_order_release);
                std::cout << "An exception was thrown wile iterating in given path." << std::endl;
                std::cerr << "Exception what(): " << ex.what() << std::endl;
                return_code = 1;
            }
            renew_counter.fetch_add(1, std::memory_order_release);
            if (!exception_was_thrown.load(std::memory_order_acquire)) std::this_thread::sleep_for(delay); // вот тут можно было бы вместо сна сделать стоп токен
        }
    };
    std::jthread thread(task);

    size_t current_stamp = 0;
    while (!finish_required.load(std::memory_order_acquire) && !exception_was_thrown.load(std::memory_order_acquire)) {
        size_t temp_stamp = renew_counter.load(std::memory_order_acquire);
        if (temp_stamp == current_stamp) continue;
        current_stamp = temp_stamp;
        auto time_now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::cout << "File generated. Time stamp: " <<  std::ctime(&time_now) << std::endl;
    }
    return return_code;
}