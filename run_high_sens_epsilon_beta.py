import os
import sqlite3
import numpy as np
import argparse
import src.map_processor as map_processor
import src.util as util
import random
import matplotlib.pyplot as plt

data_dir = os.path.join("./dataset/peopleflow")
dbname = os.path.join(data_dir, "peopleflow.sqlite")
con = sqlite3.connect(dbname)
cur = con.cursor()

parser = argparse.ArgumentParser()

parser.add_argument('--query', type=str, default=f"select * from peopleflow6 where attr1 != 'home' and state = 'STAY' and (placeID = 3 or placeID = 4 or placeID = 5 or placeID = 6 or placeID = 7) and Timestamp like '2013-12-22%';") 
parser.add_argument("--epsilon", type=float, default=1)
parser.add_argument("--alpha", type=float, default=1e-4)
parser.add_argument("--delta", type=float, default=1e-4)
parser.add_argument("--threshold", type=int, default=5)
parser.add_argument("--way_choose_query", type=str, default="RANDOM")
parser.add_argument("--c", type=int, default=1)

args = parser.parse_args()

if __name__ == "__main__":
    
    min_lon, max_lon, min_lat, max_lat = util.get_min_max_lon_lat(cur)
    mp = util.generate_Tokyo_map(min_lon, max_lon, min_lat, max_lat)
    states_with_location_id = util.get_states_with_location_id(cur, mp)
    
    data = util.get_data_from_query(cur, mp, args.query)
    data = util.data_to_states(data, mp, limit_sens=True)
        
    counts = util.get_counts(data, states_with_location_id)
    conditional_ratio = util.get_conditional_ratio(counts, args.threshold)
    
    epsilons = [0.1, 0.5, 1, 2, 5]
    results = {"epsilons": epsilons, "dp":[], "approximate_dp":[], "adp":[]}
    
    for epsilon in epsilons:
        dp_beta = util.get_dp_beta(epsilon, args.alpha)
        dp_expected_beta = util.get_expected_beta(conditional_ratio, dp_beta, args.threshold)
        results["dp"].append(dp_expected_beta)

        adp_beta = util.get_adp_beta(epsilon, args.alpha)
        adp_expected_beta = util.get_expected_beta(conditional_ratio, adp_beta, args.threshold)
        results["adp"].append(adp_expected_beta)
        
        approximate_dp_beta = util.get_approximate_dp_beta(epsilon, args.delta, args.alpha)
        approximate_dp_expected_beta = util.get_expected_beta(conditional_ratio, approximate_dp_beta, args.threshold)
        results["approximate_dp"].append(approximate_dp_expected_beta)
        
    plt.plot(epsilons, results["dp"], label="DP")
    plt.plot(epsilons, results["approximate_dp"], label="Approximate DP")
    plt.plot(epsilons, results["adp"], label="ADP")
    plt.legend()
    plt.savefig("imgs/temp.png")
    print(results)

#     n_answers = 0
#     n_iters = 100
#     for _ in range(n_iters):
#         count_above = 0
#         perturbed_counts = {}
#         if args.way_choose_query == "RANDOM":
#             while True:
#                 choiced = random.choice(list(states_with_location_id))
#                 perturbed_counts[choiced] = util.perturb(counts[choiced], args.epsilon/args.c)
#                 if perturbed_counts[choiced] > args.threshold:
#                     count_above += 1
#                     if count_above >= args.c:
#                         break
#         n_answers += len(perturbed_counts)
#     print(n_answers/n_iters)
