#include "matrix_test.h"

#include <cstdlib>
#include <memory>


int main() {
    using namespace rsys_tests;
    Test::TextOutput output(Test::TextOutput::Verbose);

    Test::Suite ts;
    ts.add(std::make_unique<matrix_test_suite>());
    return ts.run(output) ? EXIT_SUCCESS : EXIT_FAILURE;
}