#include <gtest/gtest.h>
#include "noisy_counter.h"

class TestNoisyCounter : public ::testing::Test{
protected:
    int k = 2;
    boost::random::laplace_distribution<> laplace_query = boost::random::laplace_distribution<>(0, 2 * k);
    boost::random::laplace_distribution<> laplace_threshold = boost::random::laplace_distribution<>(0, 2);
    default_random_engine gen = std::default_random_engine(0);
	NoisyCounter counter = NoisyCounter(1, 1, k);
    
    virtual void SetUp(){
        counter.loadHist("test");
    }
};

class TestNoisyCounter2 : public ::testing::Test{
protected:
    int k = 2;
    boost::random::laplace_distribution<> laplace_query = boost::random::laplace_distribution<>(0, 2 * k);
    boost::random::laplace_distribution<> laplace_threshold = boost::random::laplace_distribution<>(0, 2);
    default_random_engine gen = std::default_random_engine(0);
	NoisyCounter counter = NoisyCounter(1, 1, k);
    
    virtual void SetUp(){
        counter.loadHist("test2");
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
    int threshold = 6;
    float ex_0 = counter.threshold_query("1", threshold);
    float ex_04 = counter.threshold_query("3", threshold);
    laplace_threshold(gen);
    laplace_query(gen);
    float thre_noise = laplace_threshold(gen);
    float query_noise = laplace_query(gen);
    EXPECT_NEAR(ex_0, -1, 1e-5);
    EXPECT_NEAR(ex_04, 1 + query_noise - threshold - thre_noise, 1e-5);
}

TEST_F(TestNoisyCounter2, TestThresholdQuery){
    map<int, float> res = counter.run_sparse_vector();
    map<int, float> measured = counter.measure(res, 0.35);
}