import os
import sqlite3
import numpy as np
import argparse
import src.map_processor as map_processor
import src.util as util
import random
import matplotlib.pyplot as plt
import json

data_dir = os.path.join("./dataset/peopleflow")
dbname = os.path.join(data_dir, "peopleflow.sqlite")
con = sqlite3.connect(dbname)
cur = con.cursor()

parser = argparse.ArgumentParser()

parser.add_argument('--query', type=str, default=f"select * from peopleflow6 where attr1 != 'home' and state = 'STAY' and (placeID = 3 or placeID = 4 or placeID = 5 or placeID = 6 or placeID = 7) and Timestamp like '2013-12-22%';") 
parser.add_argument("--epsilon", type=float, default=1)
parser.add_argument("--alpha", type=float, default=1e-3)
parser.add_argument("--delta", type=float, default=1e-4)
parser.add_argument("--threshold", type=int, default=5)
parser.add_argument("--way_choose_query", type=str, default="RANDOM")
parser.add_argument("--c", type=int, default=1)

args = parser.parse_args()

if __name__ == "__main__":
    
    print(args)
    
    min_lon, max_lon, min_lat, max_lat = util.get_min_max_lon_lat(cur)
    mp = util.generate_Tokyo_map(min_lon, max_lon, min_lat, max_lat)
    states_with_location_id = util.get_states_with_location_id(cur, mp)
    
    data = util.get_data_from_query(cur, mp, args.query)
    data = util.data_to_states(data, mp, limit_sens=True)
        
    counts = util.get_counts(data, states_with_location_id)
    conditional_ratio = util.get_conditional_ratio(counts, args.threshold)
    
    results = {}
    
#     if args.optimal_c:
#         args.c = int(args.epsilon / 1)
#         if args.c == 0:
#             args.c = 1
            
#     print("c", args.c)
    
    result = []
    n_iters = 10000
    for _ in range(n_iters):
        count_above = 0
        perturbed_counts = {}
        if args.way_choose_query == "RANDOM":
            while True:
                choiced = random.choice(list(states_with_location_id))
                perturbed_counts[choiced] = util.perturb(counts[choiced], args.epsilon/args.c)
                if perturbed_counts[choiced] > args.threshold:
                    count_above += 1
                    if count_above >= args.c:
                        break
        result.append(len(perturbed_counts))
    expected_nquery = np.average(result)
    max_value = max(result)
    result = {i:result.count(i)/n_iters for i in range(1,max_value+1)}
    values = list(result.values())   
    cumulated_result = {i:1-sum(values[:i-1]) for i in range(1,max_value+1)}
    
    expected_n_right_answers_adp = util.get_expected_n_right_answers(counts, args.c, args.epsilon, args.alpha, args.threshold)
    expected_n_right_answers_approximate_dp = util.get_expected_n_right_answers(counts, args.c, args.epsilon, args.alpha, args.threshold, type="Approximate DP", delta=args.delta)
    results["cumulated_result"] = cumulated_result
    # results["expected_nquery"] = expected_nquery
    results["expected_n_right_answers"] = {}
    results["expected_n_right_answers"]["ADP"] = expected_n_right_answers_adp
    results["expected_n_right_answers"]["Approximate DP"] = expected_n_right_answers_approximate_dp
    # print(expected_nquery)
    
    
    dp_beta = util.get_dp_beta(args.epsilon/args.c, args.alpha)
    dp_expected_beta = util.get_expected_beta(conditional_ratio, dp_beta, args.threshold, range(args.threshold))
    
    adp_beta = util.get_adp_beta(args.epsilon/args.c, args.alpha)
    adp_expected_beta = util.get_expected_beta(conditional_ratio, adp_beta, args.threshold, range(args.threshold))

    approximate_dp_beta = util.get_approximate_dp_beta(args.epsilon/args.c, args.delta, args.alpha)
    approximate_dp_expected_beta = util.get_expected_beta(conditional_ratio, approximate_dp_beta, args.threshold, range(args.threshold))

    results["beta"] = {}
    results["beta"]["dp"] = dp_expected_beta
    results["beta"]["adp"] = adp_expected_beta
    results["beta"]["approximate_dp"] = approximate_dp_expected_beta
    print(dp_expected_beta, adp_expected_beta, approximate_dp_expected_beta)
    
    with open(f"results/epsilon{args.epsilon}_alpha{args.alpha}_delta{args.delta}_threshold{args.threshold}_c{args.c}.txt", "w") as f:
        json.dump(results, f)