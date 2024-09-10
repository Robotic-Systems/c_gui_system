#include "CppUTest/TestHarness.h"


extern "C"
{
    #include <string.h>
    #include "../spies/logger_spy.h"
}

TEST_GROUP(LoggerSpyTestGroup)
{
    void setup()
    {
        logger_spy_init();
    }

    void teardown()
    {
    }
};

// Spy empty when nothing done.
TEST(LoggerSpyTestGroup, spy_empty_when_nothing_done)
{
    // Init the logger
    logger_spy_init();

    // Length of string in spy is 0
    uint32_t len = strlen(logger_spy_get_string());

    // Check length of string is 0
    CHECK(len == 0);
}

// spy contains string that is last sent
TEST(LoggerSpyTestGroup, spy_contains_string_that_is_last_sent)
{
    // test out
    char write_out[MAX_STR_LEN] = {0};
    sprintf(write_out, "memes");

    // write little
    logger_spy_write(write_out);

    // get stored string
    char * read_out;
    read_out = logger_spy_get_string();

    // make sure both strings equal
    CHECK(strcmp(write_out, read_out) == 0);
}

// big string overwritten by little string works as expected
TEST(LoggerSpyTestGroup, big_string_overwritten_by_little_string_works_as_expected)
{
    char write_big[MAX_STR_LEN] = {0};
    sprintf(write_big, "hellllloo");
    // test out
    char write_little[MAX_STR_LEN] = {0};
    sprintf(write_little, "memes");

    logger_spy_write(write_big);
    logger_spy_write(write_little);

    // get stored string
    char * read_out;
    read_out = logger_spy_get_string();

    // make sure both strings equal
    CHECK(strcmp(write_little, read_out) == 0);
}

// Check that we cannot overflow the buffer.
TEST(LoggerSpyTestGroup, test_that_the_spy_string_does_not_overflow)
{
    char str[MAX_STR_LEN+11] = {0};
    memset(str,'a', MAX_STR_LEN+10);
    logger_spy_write(str);
    
    uint32_t buffer_len = strlen(logger_spy_get_string());

    CHECK(buffer_len == MAX_STR_LEN);
}

/* when no time is set logger_spy_get_time returns 00:00 time */
TEST(LoggerSpyTestGroup, when_no_time_set_logger_spy_get_time_returns_0000)
{
    char str[MAX_STR_LEN+11] = {0};
    logger_spy_get_time(str);
    STRCMP_EQUAL("1970-1-1T00:00:00",str);
}

/* logger_spy_set_time can be used to set the current date and time  */
TEST(LoggerSpyTestGroup, logger_spy_set_time_can_set_time)
{
    char str[50] = {0};
    sprintf(str, "2023-1-1T10:1:30");
    logger_spy_set_time(str);

    char str2[MAX_STR_LEN+11] = {0};
    logger_spy_get_time(str2);
    STRCMP_EQUAL("2023-1-1T10:1:30",str2);
}


