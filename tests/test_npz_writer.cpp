#include "cnpy.h"

#include <cassert>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

int main() {
    const std::string path = "/tmp/cnpy_npz_writer_test.npz";
    std::remove(path.c_str());

    std::vector<unsigned int> a{1, 2, 3, 4};
    std::vector<short> b{10, 11, 12, 13, 14, 15};
    std::vector<size_t> b_shape{2, 3};
    std::vector<unsigned long long> c{100, 200};

    {
        cnpy::NpzWriter writer(path);
        writer.add_array("a", a.data(), {a.size()});
        writer.add_array("b", b.data(), b_shape);
        writer.add_array("c", c.data(), {c.size()});
        writer.add_string("mac", "aa:bb:cc:dd:ee:ff");
        writer.add_string("port", "/dev/test0");
        writer.close();
    }

    auto npz = cnpy::npz_load(path);
    assert(npz.find("a") != npz.end());
    assert(npz.find("b") != npz.end());
    assert(npz.find("c") != npz.end());

    const auto& arr_a = npz["a"];
    assert(arr_a.shape.size() == 1);
    assert(arr_a.shape[0] == 4);
    const auto* a_data = arr_a.data<unsigned int>();
    assert(a_data[0] == 1 && a_data[3] == 4);

    const auto& arr_b = npz["b"];
    assert(arr_b.shape.size() == 2);
    assert(arr_b.shape[0] == 2 && arr_b.shape[1] == 3);
    const auto* b_data = arr_b.data<short>();
    assert(b_data[0] == 10 && b_data[5] == 15);

    const auto& arr_c = npz["c"];
    assert(arr_c.shape.size() == 1);
    assert(arr_c.shape[0] == 2);
    const auto* c_data = arr_c.data<unsigned long long>();
    assert(c_data[0] == 100 && c_data[1] == 200);

    std::cout << "cnpy-tests: ok\n";
    std::remove(path.c_str());
    return 0;
}
