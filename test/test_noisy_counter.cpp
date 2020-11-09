#include <gtest/gtest.h>
#include "noisy_counter.h"

class TestNoisyCounter : public ::testing::Test{
protected:
    NoisyCounter noisy_counter = NoisyCounter(1, 1);
    boost::random::laplace_distribution<> laplace_query = boost::random::laplace_distribution<>(0, 2);
    boost::random::laplace_distribution<> laplace_threshold = boost::random::laplace_distribution<>(0, 2);
    boost::random::mt19937 gen = boost::random::mt19937(0);
	NoisyCounter counter = NoisyCounter(1, 1);
    
    virtual void SetUp(){
        counter.loadHist("test");
    }
};

TEST_F(TestNoisyCounter, TestQuery){
    float ex_1 = counter.query("3");
    float ex_2 = counter.query("2");
    float ex_3 = counter.query("1");
    float ex_0 = counter.query("0");
    EXPECT_FLOAT_EQ(ex_1, 1 + laplace_query(gen));
    EXPECT_FLOAT_EQ(ex_2, 2 + laplace_query(gen));
    EXPECT_FLOAT_EQ(ex_3, 3 + laplace_query(gen));
    EXPECT_FLOAT_EQ(ex_0, 0 + laplace_query(gen));
}

TEST_F(TestNoisyCounter, TestThresholdQuery){
    float ex_0 = counter.threshold_query("3", 3);
    float ex_04 = counter.threshold_query("2", 3);
    laplace_threshold(gen);
    laplace_query(gen);
    float thre_noise = laplace_threshold(gen);
    float query_noise = laplace_query(gen);
    EXPECT_NEAR(ex_0, 0, 1e-5);
    EXPECT_NEAR(ex_04, 2 + query_noise - 3 - thre_noise, 1e-5);
}