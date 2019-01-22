#include <TemperatureWatcher.h>
#include <unity.h>

void test_pass(void){
    TEST_ASSERT_EQUAL(1, 1);
}

void test_fail(void){
    TEST_ASSERT_EQUAL(0, 1);
}

int main(int argc, char **argv){
    UNITY_BEGIN();
    RUN_TEST(test_pass);
    RUN_TEST(test_fail);
    UNITY_END();

    return 0;
}


