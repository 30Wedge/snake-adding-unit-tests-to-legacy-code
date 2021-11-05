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

class EatGoldTest : public ::testing::Test
{
protected:
  snake_t m_snake;
  screen_t m_screen;
public:

  const int SNAKE_POS_X = 10;
  const int SNAKE_POS_Y = 10;
  const int SNAKE_LEN = 1;

  const char SCREEN_FILLER = 'X';
  const char SCREEN_BLANK = ' ';
  const int SCREEN_GOLD = 5;
  const int SCREEN_SCORE = 100;
  const int SCREEN_HI_SCORE = 500;
  const int SCREEN_OBSTACLES = 10;

  EatGoldTest() {
    /// set up snake
    m_snake.len = SNAKE_LEN;
    m_snake.body[0] = {SNAKE_POS_X, SNAKE_POS_Y};

    /// set up screen
    // fill screen up with filler to detect no-change in other spaces
    for(int row = 0; row < MAXROW; row++){
      for(int col = 0; col < MAXCOL; col++){
        m_screen.grid[row][col] = SCREEN_FILLER;
      }
    }
    m_screen.gold = SCREEN_GOLD;
    m_screen.score = SCREEN_SCORE;
    m_screen.high_score = SCREEN_HI_SCORE;
    m_screen.obstacles = SCREEN_OBSTACLES;
  }
};

TEST_F(EatGoldTest, test_eat_gold_return_value_is_correct)
{
  /* Given: default screen & snake*/
  /* When */
  int ret = eat_gold(&m_snake, &m_screen);
  /* Then */
  // decrement gold count
  ASSERT_EQ(ret, m_screen.gold);
}

TEST_F(EatGoldTest, test_eat_gold_modifies_screen_correctly)
{
  /* Given: default screen & snake*/
  /* When */
  eat_gold(&m_snake, &m_screen);
  /* Then */
  // blank out the head of the snake on the screen
  for(int row = 0; row < MAXROW; row++){
    for(int col = 0; col < MAXCOL; col++){
      if(row == SNAKE_POS_X - 1 && col == SNAKE_POS_Y - 1)
      {
        ASSERT_EQ(m_screen.grid[row][col], SCREEN_BLANK);
      }
      else
      {
        ASSERT_EQ(m_screen.grid[row][col], SCREEN_FILLER);
      }
    }
  }

  // decrement gold count
  ASSERT_EQ(m_screen.gold, SCREEN_GOLD - 1);

  // increase score but not hi score
  int score_increment = SNAKE_LEN * SCREEN_OBSTACLES;
  ASSERT_EQ(m_screen.score, SCREEN_SCORE + score_increment);
  ASSERT_EQ(m_screen.high_score, SCREEN_HI_SCORE);
}

TEST_F(EatGoldTest, test_eat_gold_modifies_screen_correctly_on_high_score_update)
{
  /* Given: set score to just under the high score threshold*/
  int NEW_STARTING_SCORE = SCREEN_HI_SCORE - 1;
  m_screen.score = NEW_STARTING_SCORE;
  /* When */
  eat_gold(&m_snake, &m_screen);
  /* Then */
  // blank out the head of the snake on the screen
  for(int row = 0; row < MAXROW; row++){
    for(int col = 0; col < MAXCOL; col++){
      if(row == SNAKE_POS_X - 1 && col == SNAKE_POS_Y - 1)
      {
        ASSERT_EQ(m_screen.grid[row][col], SCREEN_BLANK);
      }
      else
      {
        ASSERT_EQ(m_screen.grid[row][col], SCREEN_FILLER);
      }
    }
  }

  // decrement gold count
  ASSERT_EQ(m_screen.gold, SCREEN_GOLD - 1);

  // increase score AND hi score
  int score_increment = SNAKE_LEN * SCREEN_OBSTACLES;
  ASSERT_EQ(m_screen.score, NEW_STARTING_SCORE + score_increment);
  ASSERT_EQ(m_screen.high_score, m_screen.score);
}

TEST_F(EatGoldTest, test_eat_gold_modifies_snake_correctly)
{
  /* Given: default screen & snake*/
  /* When */
  eat_gold(&m_snake, &m_screen);
  /* Then */
  ASSERT_EQ(m_snake.len, SNAKE_LEN + 1);
}