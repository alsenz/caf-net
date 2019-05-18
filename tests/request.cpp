#include "src/caf-net/request.hpp"

#include "gtest/gtest.h"

TEST(RequestTests, TestURLParse) {
    as::net::request my_req;
    my_req.url("https://martechtoday.com/conductor-ceo-besmertnik-discusses-vision-of-ethical-marketing-225249?utm_source=marketo&utm_medium=email&utm_campaign=newsletter&utm_content=mttoday");
    EXPECT_EQ("martechtoday.com", my_req.host());
    EXPECT_EQ("utm_source=marketo&utm_medium=email&utm_campaign=newsletter&utm_content=mttoday", my_req.query());
    EXPECT_EQ("/conductor-ceo-besmertnik-discusses-vision-of-ethical-marketing-225249", my_req.path());
}