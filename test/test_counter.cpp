#include <gtest/gtest.h>
#include "noisy_counter.h"

class TestCounter : public ::testing::Test{
protected:
    Counter counter = Counter(2);
    
    virtual void SetUp(){
        counter.loadHist("test");
    }
};

class TestCounter2 : public ::testing::Test{
protected:
    Counter counter = Counter(2);
    
    virtual void SetUp(){
        counter.loadHist("test2");
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
    EXPECT_EQ(ex_0_0, -1);
    EXPECT_EQ(ex_0_1, -1);
    EXPECT_EQ(ex_1, 1);
}

TEST_F(TestCounter, TestRandomQuery){
    vector<int> queries = counter.random_query();
    EXPECT_EQ(queries.size(), max_index(counter.get_hist()));
}


TEST_F(TestCounter2, TestRandomThreshold){
    auto in_range = [](int i) {
        if (i >= 4 && i <= 16){
            return true;
        }else{
            return false;
        }
        
    };
    for(int i=0; i<100; i++){
        int threshold = counter.random_threshold();
        EXPECT_PRED1(in_range, threshold);
    };
}

TEST_F(TestCounter2, TestRunSparseVector){
    map<int, float> res = counter.run_sparse_vector();
    EXPECT_FLOAT_EQ(res[2], -1);
    EXPECT_FLOAT_EQ(res[3], -1);
    EXPECT_FLOAT_EQ(res[15], 2);
    EXPECT_FLOAT_EQ(res[20], 7);
}

TEST_F(TestCounter2, TestEvaluatePrecision){
    map<int, float> res = counter.run_sparse_vector();
    float result = counter.evaluate_precision(res);
    EXPECT_FLOAT_EQ(result, 1);

    res[2] = 5;
    float result2 = counter.evaluate_precision(res);
    EXPECT_FLOAT_EQ(result2, 7./8.);
}

TEST_F(TestCounter2, TestEvaluateRecall){
    map<int, float> res = counter.run_sparse_vector();
    float result = counter.evaluate_recall(res);
    EXPECT_FLOAT_EQ(result, 1);

    res[2] = 5;
    float result2 = counter.evaluate_recall(res);
    EXPECT_FLOAT_EQ(result2, 1);


    res[19] = -1;
    float result3 = counter.evaluate_recall(res);
    EXPECT_FLOAT_EQ(result3, 6./7.);
}

TEST_F(TestCounter2, TestEvaluateFValue){
    map<int, float> res = counter.run_sparse_vector();
    float result = counter.evaluate_f_value(res);
    EXPECT_FLOAT_EQ(result, 1 );
}