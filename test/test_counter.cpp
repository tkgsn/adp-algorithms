#include <gtest/gtest.h>
#include "noisy_counter.h"

TEST(TestCounter, TestloadHist){
	Counter counter = Counter();
	counter.loadHist("test");
    float ex_1 = counter.query("3");
    float ex_2 = counter.query("2");
    float ex_3 = counter.query("1");
    float ex_0 = counter.query("0");
    EXPECT_EQ(ex_1, 1);
    EXPECT_EQ(ex_2, 2);
    EXPECT_EQ(ex_3, 3);
    EXPECT_EQ(ex_0, 0);
}

TEST(TestCounter, TestThresholdQuery){
	Counter counter = Counter();
	counter.loadHist("test");
    float ex_0_0 = counter.threshold_query("1", 3);
    float ex_0_1 = counter.threshold_query("1", 4);
    float ex_1 = counter.threshold_query("1", 2);
    EXPECT_EQ(ex_0_0, 0);
    EXPECT_EQ(ex_0_1, 0);
    EXPECT_EQ(ex_1, 1);
}