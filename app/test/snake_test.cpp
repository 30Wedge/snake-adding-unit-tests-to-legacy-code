#include <gtest/gtest.h>
#include <fff.h>
#include <conio.h>
#include <snake.h>

DEFINE_FFF_GLOBALS

/* Expose snake.c functions for test */
extern "C" {
extern int eat_gold(snake_t *snake, screen_t *screen);
extern void show_score(screen_t *screen);
}

TEST(SmokeTest, smoke_test_please_compile) {
  // Please compile
  screen_t my_screen;
  snake_t my_snake;
  eat_gold(&my_snake, &my_screen);
  EXPECT_EQ(0,0);
}

TEST(MockVerify, call_api_functions_and_verify_the_mock_counters_increment) {
  /*
   * Directly call some conio.h api calls
   * These calls are going to be handled by the mock functions defined in mock/conio.h
   */
  textattr(DIM);
  gotoxy(20, 40);
  textattr(RESETATTR);

  /*
   * These demonstrate how the API calls above affect the underlying _fake object.
   */
  // verify textattr() was called twice
  EXPECT_EQ(textattr_fake.call_count, 2);
  // Captures the last value that textattr was called with
  EXPECT_EQ(textattr_fake.arg0_val, RESETATTR);

  // Verify gotoxy was called once
  EXPECT_EQ(gotoxy_fake.call_count, 1);
  // Verify gotoxy's last args were 20 and 40 respectively
  EXPECT_EQ(gotoxy_fake.arg0_val, 20); //
  EXPECT_EQ(gotoxy_fake.arg1_val, 40);

  // Cleanup fake structures after test.
  // These macros reset all the fields of gotoxy_fake and textattr_fake to their default values
  RESET_FAKE(textattr);
  RESET_FAKE(gotoxy);
}

TEST(MockVerify, show_score_calls_some_API_calls) {
  /*
   * Set up input to the show_scores() function
   */
  screen_t my_screen;
  my_screen.level = 1;
  my_screen.gold = 2;
  my_screen.score = 3;
  my_screen.high_score =4;
  // Call show_scores() which calls some conio.h API functions in its body
  show_score(&my_screen);

  // We're not going to specify *what* the API functions called were,
  //   but these EXPECT statements show that at textcolor() and gotoxy() were called
  //   some non-zero number of times
  EXPECT_NE(textcolor_fake.call_count, 0);
  EXPECT_NE(gotoxy_fake.call_count, 0);

  // cleanup fakes after test
  RESET_FAKE(textcolor);
  RESET_FAKE(gotoxy);
}