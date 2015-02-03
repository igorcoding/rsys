#ifndef MATRIX_TEST_H
#define MATRIX_TEST_H

#include "rsys/data_sources/matrix.h"

#include <cpptest.h>

namespace rsys_tests {

    using namespace rsys;

    class matrix_test_suite : public Test::Suite {
    public:
        matrix_test_suite() {
            TEST_ADD(matrix_test_suite::test_create);
            TEST_ADD(matrix_test_suite::test_setting_value);
        }

    protected:
        virtual void setup();
        virtual void tear_down();

    private:
        void test_create();
        void test_setting_value();


    private:
        ds::matrix<double>* _m;
    };

    void matrix_test_suite::setup() {
        size_t rows = 5, cols = 5;
        _m = new ds::matrix<double>(rows, cols, -1);

        _m->set(0, 0, 4);   _m->set(0, 1, 5);   _m->set(0, 2, 2);  _m->set(0, 3, -1);  _m->set(0, 4, -1);
        _m->set(1, 0, -1);  _m->set(1, 1, 4);   _m->set(1, 2, 4);  _m->set(1, 3, 3);   _m->set(1, 4, -1);
        _m->set(2, 0, -1);  _m->set(2, 1, 2);   _m->set(2, 2, -1); _m->set(2, 3, 5);  _m->set(2, 4, -1);
        _m->set(3, 0, 5);   _m->set(3, 1, 4);   _m->set(3, 2, -1); _m->set(3, 3, 4);  _m->set(3, 4, -1);
        _m->set(4, 0, -1);  _m->set(4, 1, -1);  _m->set(4, 2, -1); _m->set(4, 3, -1);  _m->set(4, 4, -1);
    }

    void matrix_test_suite::tear_down() {
        delete _m;
    }

    void matrix_test_suite::test_create() {
        size_t rows = 10, cols = 10;
        ds::matrix<double>* mp = new ds::matrix<double>(rows, cols, -1);
        ds::matrix<double>& m = *mp;
        TEST_ASSERT(m.rows() == rows);
        TEST_ASSERT(m.cols() == cols);
        TEST_ASSERT(m.get_def_value() == -1);
        TEST_ASSERT(m.capacity() >= rows);

        for (auto& row : m) {
            for (auto& v : *row) {
                TEST_ASSERT(v == -1);
            }
        }

        delete mp;
    }

    void matrix_test_suite::test_setting_value() {
        _m->set(0, 1, 42);
        TEST_ASSERT(_m->at(0, 1) == 42);

        _m->at(0, 1) = 34;
        TEST_ASSERT(_m->at(0, 1) == 34);
        
        (*_m)[0][1] = 55;
        TEST_ASSERT(_m->at(0, 1) == 55);

        ds::mvector<double> mvec(_m->cols(), 2);

        _m->set(0, mvec);
        TEST_ASSERT((*_m)[0] == mvec);
    }

}

#endif // MATRIX_TEST_H