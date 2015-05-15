#ifndef RSYS_RSYS_TEST_H
#define RSYS_RSYS_TEST_H

#include <cpptest-suite.h>
#include <cpptest-assert.h>
#include "rsys/models/svd.h"

namespace rsys_tests {

    using namespace rsys;

    class rsys_test_suite : public Test::Suite {
    public:
        rsys_test_suite() {
            TEST_ADD(rsys_test_suite::test_create);
            TEST_ADD(rsys_test_suite::test_setting_value);
        }

    protected:
        virtual void setup();
        virtual void tear_down();

    private:
        void test_create();
        void test_setting_value();


    private:

    };

    void rsys_test_suite::setup() {

    }

    void rsys_test_suite::tear_down() {

    }

    void rsys_test_suite::test_create() {

    }

    void rsys_test_suite::test_setting_value() {

    }

}

#endif //RSYS_RSYS_TEST_H
