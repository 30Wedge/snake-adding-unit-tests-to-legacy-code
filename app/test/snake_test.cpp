#include <gtest/gtest.h>
#include <fff.h>
#include <conio.h>

DEFINE_FFF_GLOBALS

TEST(HelloTest, smoke_test_please_compile) {
  // Please compile
  EXPECT_EQ(0,0);
}

TEST(MockVerify, call_api_functions_and_verify_the_mock_counters_increment) {
  // some conio.h api calls
  textattr(DIM);
  gotoxy(20, 40);
  textattr(RESETATTR);

  // expect the fake counters to increment
  EXPECT_EQ(textattr_fake.call_count, 2);
  EXPECT_EQ(textattr_fake.arg0_val, RESETATTR); // Captures the last value that textattr was called with

  EXPECT_EQ(gotoxy_fake.call_count, 1);
  EXPECT_EQ(gotoxy_fake.arg0_val, 20);
  EXPECT_EQ(gotoxy_fake.arg1_val, 40);

  // cleanup fakes after test
  RESET_FAKE(textattr);
  RESET_FAKE(gotoxy);
}
