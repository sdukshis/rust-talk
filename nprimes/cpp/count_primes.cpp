// Program counts number of primes less then limit
// using multiple threads
// ===========================

#include <future>
#include <atomic>
#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <functional>

using Number = unsigned long long;

bool is_prime(Number n) {
    if (n <= 1) {
        return false;
    }
    if (n <= 3) {
        return true;
    }
    if ((n % 2 == 0) || (n % 3 == 0)) {
        return false;
    }
    for (Number i = 5; i * i <= n; i += 6) {
        if ((n % i == 0) || (n % (i + 2) == 0)) {
            return false;
        }
    }
    return true;
}

Number count_primes(std::atomic<Number> &shared_counter, Number limit) {
    Number i = 0;
    Number counter = 0;
    while (i < limit) {
        i = shared_counter++;
        if (is_prime(i)) {
            ++counter;
        }
    }
    return counter;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: count_primes limit nthreads" << std::endl;
        return EXIT_FAILURE;
    }
    Number limit = std::stoul(argv[1]);
    Number nthreads = std::stoul(argv[2]);

    std::vector<std::future<Number>> tasks;
    std::atomic<Number> shared_counter{2};
    for (Number i = 0; i < nthreads; ++i) {
        tasks.emplace_back(std::async(std::launch::async, count_primes,
                                      std::ref(shared_counter), limit));
    }

    Number nprimes = std::accumulate(
        tasks.begin(), tasks.end(), Number(),
        [](auto sum, std::future<Number> &fut) { return sum += fut.get(); });
    std::cout << nprimes << std::endl;
    return EXIT_SUCCESS;
}