#ifndef MATRIX_TEST_H
#define MATRIX_TEST_H

#include "rsys/data_sources/matrix.h"

#include <cpptest.h>

namespace rsys_tests {

    using namespace rsys;

    template <typename T>
    class matrix_test_suite : public Test::Suite {
    public:
        matrix_test_suite() {
            TEST_ADD(matrix_test_suite::creation_test);
            TEST_ADD(matrix_test_suite::second_test);
        }

    protected:
        virtual void setup();
        virtual void tear_down();

    private:
        void creation_test();
        void second_test();


    private:
        ds::matrix<T>* _m;
    };

    template <typename T>
    void matrix_test_suite<T>::setup() {

    }

    template <typename T>
    void matrix_test_suite<T>::tear_down() {

    }

    template <typename T>
    void matrix_test_suite<T>::creation_test() {
        size_t rows = 10, cols = 10;
        ds::matrix<T>* mp = new ds::matrix<T>(rows, cols, T());
        ds::matrix<T>& m = *mp;
        TEST_ASSERT(m.rows() == rows);
        TEST_ASSERT(m.cols() == cols);
        TEST_ASSERT(m.get_def_value() == T());
        TEST_ASSERT(m.capacity() >= rows);

        for (auto& row : m) {
            for (auto& v : *row) {
                TEST_ASSERT(v == T());
            }
        }

        delete mp;
    }

    template <typename T>
    void matrix_test_suite<T>::second_test() {

    }

}

#endif // MATRIX_TEST_H