#include <gtest/gtest.h>
#include "noisy_counter.h"

class TestCounter : public ::testing::Test{
protected:
    Counter counter = Counter();
    
    virtual void SetUp(){
        counter.loadHist("test");
    }
};

TEST_F(TestCounter, TestloadHist){
    float ex_1 = counter.query("3");
    float ex_2 = counter.query("2");
    float ex_3 = counter.query("1");
    float ex_0 = counter.query("0");
    EXPECT_EQ(ex_1, 1);
    EXPECT_EQ(ex_2, 2);
    EXPECT_EQ(ex_3, 3);
    EXPECT_EQ(ex_0, 0);
}

TEST_F(TestCounter, TestThresholdQuery){
    float ex_0_0 = counter.threshold_query("1", 3);
    float ex_0_1 = counter.threshold_query("1", 4);
    float ex_1 = counter.threshold_query("1", 2);
    EXPECT_EQ(ex_0_0, 0);
    EXPECT_EQ(ex_0_1, 0);
    EXPECT_EQ(ex_1, 1);
}

TEST_F(TestCounter, TestRandomQuery){
    vector<int> queries = counter.random_query();
    EXPECT_EQ(queries.size(), max_index(counter.get_hist()));
}