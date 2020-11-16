#include <gtest/gtest.h>
#include "adaptive_noisy_counter.h"
#include <boost/foreach.hpp>

class TestAdaptiveNoisyCounter : public ::testing::Test{
protected:
    int k = 2;
    boost::random::laplace_distribution<> laplace_query = boost::random::laplace_distribution<>(0, 2 * k);
    boost::random::laplace_distribution<> bigger_laplace_query = boost::random::laplace_distribution<>(0, 2 * 2 * k);
    boost::random::laplace_distribution<> laplace_threshold = boost::random::laplace_distribution<>(0, 2);
    default_random_engine gen = std::default_random_engine(0);
    AdaptiveNoisyCounter counter = AdaptiveNoisyCounter(1, 1, k);

    virtual void SetUp(){
        counter.loadHist("test");
    }
};

class TestAdaptiveNoisyCounter2 : public ::testing::Test{
protected:
    int k = 2;
    boost::random::laplace_distribution<> laplace_query = boost::random::laplace_distribution<>(0, 2 * k);
    boost::random::laplace_distribution<> bigger_laplace_query = boost::random::laplace_distribution<>(0, 2 * 2 * k);
    boost::random::laplace_distribution<> laplace_threshold = boost::random::laplace_distribution<>(0, 2);
    default_random_engine gen = std::default_random_engine(0);
    AdaptiveNoisyCounter counter = AdaptiveNoisyCounter(1, 1, k);

    virtual void SetUp(){
        counter.loadHist("test2");
    }
};

TEST_F(TestAdaptiveNoisyCounter, TestBiggerQuery){
    float ex_1 = counter.bigger_query("3");
    float ex_2 = counter.bigger_query("2");
    float ex_3 = counter.bigger_query("1");
    float ex_0 = counter.bigger_query("0");
    EXPECT_FLOAT_EQ(ex_1, 1 + bigger_laplace_query(gen));
    EXPECT_FLOAT_EQ(ex_2, 2 + bigger_laplace_query(gen));
    EXPECT_FLOAT_EQ(ex_3, 3 + bigger_laplace_query(gen));
    EXPECT_FLOAT_EQ(ex_0, 0 + bigger_laplace_query(gen));
}

TEST_F(TestAdaptiveNoisyCounter, TestThresholdQuery){
    int threshold = -2;
    float ex_0 = counter.threshold_query("3", 2);
    bigger_laplace_query(gen);
    laplace_threshold(gen);
    laplace_query(gen);
    float ex_04 = counter.threshold_query("4", -2);
    float query_noise = bigger_laplace_query(gen);
    float thre_noise = laplace_threshold(gen);
    float query_noise2 = laplace_query(gen);
    EXPECT_NEAR(ex_0, -1, 1e-5);
    EXPECT_NEAR(ex_04, 4 + query_noise2 - threshold - thre_noise, 1e-5);
}


TEST_F(TestAdaptiveNoisyCounter2, TestRunSparseVector){
    map<int, float> res = counter.run_sparse_vector();
    EXPECT_FLOAT_EQ(res[3], -1);
    EXPECT_FLOAT_EQ(res[8], -1);
}