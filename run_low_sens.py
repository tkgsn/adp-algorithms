import os
import sqlite3
import numpy as np
import argparse
import src.map_processor as map_processor
import src.util as util
import json

data_dir = os.path.join("./dataset/peopleflow")
dbname = os.path.join(data_dir, "peopleflow.sqlite")
con = sqlite3.connect(dbname)
cur = con.cursor()

parser = argparse.ArgumentParser()

time = 18
parser.add_argument('--query', type=str, default=f"select * from peopleflow6 where attr1 != 'home' and state = 'STAY' and (placeID = 3 or placeID = 4 or placeID = 5 or placeID = 6 or placeID = 7) and Timestamp like '2013-12-22 {time}%';") 
parser.add_argument("--epsilon", type=float, default=1)
parser.add_argument("--alpha", type=float, default=1e-4)
parser.add_argument("--delta", type=float, default=1e-4)
parser.add_argument("--threshold", type=int, default=5)
parser.add_argument("--geq", action="store_true")

args = parser.parse_args()

if __name__ == "__main__":
    
    min_lon, max_lon, min_lat, max_lat = util.get_min_max_lon_lat(cur)
    mp = util.generate_Tokyo_map(min_lon, max_lon, min_lat, max_lat)
    states_with_location_id = util.get_states_with_location_id(cur, mp)
    
    data = util.get_data_from_query(cur, mp, args.query)
    data = util.data_to_states(data, mp, limit_sens=True)
        
    counts = util.get_counts(data, states_with_location_id)
    perturbed_counts = util.perturb_counts(counts, epsilon=args.epsilon)
    conditional_ratio = util.get_conditional_ratio(counts, args.threshold, geq=args.geq)
    
    results = {}
    
    beta_dp = util.get_dp_beta(args.epsilon, args.alpha)
    beta_adp = util.get_dp_beta(args.epsilon, args.alpha) if args.geq else util.get_adp_beta(args.epsilon, args.alpha)
    beta_approximate_dp = util.get_approximate_dp_beta(args.epsilon, args.delta, args.alpha)
    
    dp_expected_beta = util.get_expected_beta(conditional_ratio, beta_dp, args.threshold, range(args.threshold))
    adp_expected_beta = util.get_expected_beta(conditional_ratio, beta_adp, args.threshold, range(args.threshold))
    approximate_dp_expected_beta = util.get_expected_beta(conditional_ratio, beta_approximate_dp, args.threshold, range(args.threshold))
    
    results["dp"] = dp_expected_beta
    results["adp"] = adp_expected_beta
    results["approximate_dp"] = approximate_dp_expected_beta
    
    with open(f"results/low_sens_epsilon{args.epsilon}_alpha{args.alpha}_delta{args.delta}_threshold{args.threshold}_geq{args.geq}.txt", "w") as f:
        json.dump(results, f)
        
    
    hot_states = util.get_hot_states(counts, args.threshold)
    safe_states = util.get_safe_states(counts, args.threshold)
    gt_n_safe = len(safe_states)
    gt_n_hot = len(hot_states)
    util.plot_three(mp, states_with_location_id, hot_states, safe_states, s=1, filename="ground_truth.png", gt_n_safe=gt_n_safe, gt_n_hot=gt_n_hot)
    
    one_sided_hot_states = util.get_one_sided_hot_states(perturbed_counts, args.epsilon, args.alpha, args.threshold)
    one_sided_safe_states = util.get_one_sided_safe_states(perturbed_counts, args.threshold)
    util.plot_three(mp, states_with_location_id, one_sided_hot_states, one_sided_safe_states, s=1, filename=f"adp_epsilon{args.epsilon}_alpha{args.alpha}_threshold{args.threshold}.png", gt_n_safe=gt_n_safe, gt_n_hot=gt_n_hot)
    
    one_sided_hot_states = util.get_one_sided_hot_states_by_beta(counts, args.epsilon, args.alpha, args.threshold, beta_dp)
    one_sided_safe_states = util.get_one_sided_safe_states_by_beta(counts, args.epsilon, args.alpha, args.threshold, beta_dp)
    util.plot_three(mp, states_with_location_id, one_sided_hot_states, one_sided_safe_states, s=1, filename=f"dp_epsilon{args.epsilon}_alpha{args.alpha}_threshold{args.threshold}.png", gt_n_safe=gt_n_safe, gt_n_hot=gt_n_hot)

    one_sided_hot_states = util.get_one_sided_hot_states_by_beta(counts, args.epsilon, args.alpha, args.threshold, beta_approximate_dp)
    one_sided_safe_states = util.get_one_sided_safe_states_by_beta(counts, args.epsilon, args.alpha, args.threshold, beta_approximate_dp)
    util.plot_three(mp, states_with_location_id, one_sided_hot_states, one_sided_safe_states, s=1, filename=f"apptoximate_dp_epsilon{args.epsilon}_delta{args.delta}_alpha{args.alpha}_threshold{args.threshold}.png", gt_n_safe=gt_n_safe, gt_n_hot=gt_n_hot)