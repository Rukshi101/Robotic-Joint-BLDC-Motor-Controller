#include "unity.h"
#include "velocity_control.h"

void setUp(void) {}
void tearDown(void) {}

/* Test RPM calculation with known values */
void test_rpm_at_1000rpm(void) {
    /* At 1000 RPM with 24 edges per rev and 170MHz timer:
       delta = (170,000,000 / 24) / (1000/60) = 425,000 ticks */
    uint32_t delta = 425000;
    float rpm = compute_rpm(delta);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 1000.0f, rpm);
}

void test_rpm_at_4000rpm(void) {
    /* At 4000 RPM: delta = 425000 / 4 = 106250 ticks */
    uint32_t delta = 106250;
    float rpm = compute_rpm(delta);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 4000.0f, rpm);
}

void test_rpm_zero_delta_returns_zero(void) {
    /* Division by zero guard */
    float rpm = compute_rpm(0);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, rpm);
}



void test_duty_correction_negative_error(void) {
    float correction = compute_duty_correction(1000.0f, 1200.0f, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, -0.1f, correction);
}

void test_duty_correction_positive_error(void) {
    float correction = compute_duty_correction(1000.0f, 800.0f, 0.5f);
    /* 1000 - 800 = 200, 0.5 * 200 / 1000 = 0.1 */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.1f, correction);
}

void test_duty_correction_zero_target_returns_zero(void) {
    float correction = compute_duty_correction(0.0f, 100.0f, 0.5f);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, correction);
}
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_rpm_at_1000rpm);
    RUN_TEST(test_rpm_at_4000rpm);
    RUN_TEST(test_rpm_zero_delta_returns_zero);
    RUN_TEST(test_duty_correction_positive_error);
    RUN_TEST(test_duty_correction_negative_error);
    RUN_TEST(test_duty_correction_zero_target_returns_zero);
    return UNITY_END();
}
