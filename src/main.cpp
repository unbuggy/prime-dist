/** @file main.cpp A program to analyze prime number distribution. */

#include "std.hpp"

/** Sets each bit in `*result` true if its index is prime, and false otherwise.
  */
void identify_primes(std::vector<bool>* result)
{
    auto& r = *result;
    auto  n = r.size();
    if (n < 3) {
        r.assign(n, false);
    } else {
        r.assign(n, true);
        r[0] = r[1] = false;
        for (std::size_t i = 4; i < n; i += 2)
            r[i] = false;
        for (std::size_t i = 3, q = std::sqrt(n); i <= q; i += 2) {
            for (std::size_t j = i * 2; j < n; j += i)
                r[j] = false;
        }
    }
}

/** Sets `*result` elements to counts of passing values from integer ranges.
  * Each value `v` is considered *passing* if `filter[v]` is true.  The count
  * at each index `i` in `*result` corresponds to the range of integers
  * beginning at `i * weight` and containing `weight` distinct values.
  * The behavior is undefined unless `filter` contains at least `weight *
  * result->size()` elements.
  */
void fill_buckets(
        std::vector<std::size_t>* result,
        std::vector<bool> const&  filter,
        std::size_t               weight)
{
    assert(filter.size() >= weight * result->size());
    auto& r = *result;
    auto  n = r.size();
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = i * weight, e = j + weight; j < e; ++j) {
            if (filter[j])
                ++r[i];
        }
    }

}

int main(int argc, char** argv) try
{
    if (argc != 4)
        throw "usage; main <column-weight> <column-count> <row-count>";

    std::size_t m = std::stol(argv[1]); // integers per column
    std::size_t w = std::stol(argv[2]); // total output width
    std::size_t h = std::stol(argv[3]); // total output height

    if (m == 0) throw "The column weight must be positive.";
    if (w == 0) throw "The column count must be positive.";
    if (h == 0) throw "The row count must be positive.";

    std::vector<bool> prime(m * w);
    identify_primes(&prime);

    std::vector<std::size_t> buckets(w);
    fill_buckets(&buckets, prime, m);

    if (auto x = *std::max_element(buckets.begin(), buckets.end())) {
        for (std::size_t row = h; --row;) {
            for (std::size_t col = 0; col < w; ++col)
                std::cout << (buckets[col] * h / x >= row ? 'o' : ' ');
            std::cout << '\n';
        }
    }

} catch (char const* x) {
    std::clog << "Error: " << x << '\n';
    return -1;
} catch (std::exception const& x) {
    std::clog << "Error: " << x.what() << '\n';
    return -2;
}

//         Copyright Unbuggy Software, LLC 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
