/*!
 * Copyright (c) 2019 by Xing Zhao
 * \file generator.cc
 * \brief implement generator for simulating iteration intervals of mutiple workers
 */
#ifndef GENERATOR_H
#define GENERATOR_H

#include <iostream>
#include <deque>
#include <vector>
#include <set>
#include <unordered_map>
#include <chrono>
#include <cstdint>
#include <random>
#include <limits>
#include <algorithm>
#include "pointer.cc"

using namespace std;

inline uint64_t CurrentTime_milliseconds()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>
              (std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

class generator {
  public:
  generator (int total_workers, int size_timespots, int lower=1000, int upper=1500) : 
        num_workers(total_workers), size_time_array(size_timespots), interval_lower(lower), interval_upper(upper)      
        {}
  // generate init timestamps for all workers with same timestamps (possible duplication)
  void generate() {
      uint64_t start_base = CurrentTime_milliseconds();
      vector<uint64_t> init_time_array = random_gen(num_workers);
      vector<uint64_t> rand_interval;
      history_matrix.reserve(num_workers);
      uint64_t max_time = 0;
      for (int w=0; w < num_workers; w++ ) {
          uint64_t start = start_base + init_time_array[w];
          uint64_t interval = gen_interval(interval_lower, interval_upper); // interval range
          deque<uint64_t> pushstamps;
          for (int t=0; t < size_time_array; t++){
	      uint64_t pushtime = start + interval * t;
              pushstamps.push_back(pushtime);
	      max_time = max_time > pushtime ? max_time:pushtime; 
          }
          history_matrix.insert(make_pair<int,deque<uint64_t>> (int(w), deque<uint64_t>(pushstamps)));
      }

      for (int w = 0; w < num_workers; w++) {
	  auto& pushstamps = history_matrix[w];
	  uint64_t interval = pushstamps[1] - pushstamps[0];
	  uint64_t last_time = pushstamps[size_time_array - 1];
	  while (last_time < max_time) {
		last_time += interval;
		pushstamps.push_back(last_time);
	  }
      }
  }
  // generate unique init timestamps for all workers given range
  void generate_uniq(int lower=0, int upper=100) {
      uint64_t start_base = CurrentTime_milliseconds();
      vector<uint64_t> init_time_array = srandom_gen(num_workers, lower, upper);
      vector<uint64_t> rand_interval;
      history_matrix.reserve(num_workers);
      uint64_t max_time = 0;
      for (int w=0; w < num_workers; w++ ) {
          uint64_t start = start_base + init_time_array[w];
          uint64_t interval = gen_interval(interval_lower, interval_upper); // interval range
          deque<uint64_t> pushstamps;
          for (int t=0; t < size_time_array; t++){
              uint64_t pushtime = start + interval * t;
              pushstamps.push_back(pushtime);
              max_time = max_time > pushtime ? max_time:pushtime;
          }
          history_matrix.insert(make_pair<int,deque<uint64_t>> (int(w), deque<uint64_t>(pushstamps)));
      }
     
      for (int w = 0; w < num_workers; w++) {
          auto& pushstamps = history_matrix[w];
          uint64_t interval = pushstamps[1] - pushstamps[0];
          uint64_t last_time = pushstamps[size_time_array - 1];
          while (last_time < max_time) {
                last_time += interval;
                pushstamps.push_back(last_time);
          }
      } 
  }
  
  // generate interval time
  uint64_t gen_interval(int lower, int upper) {
      random_device rd;
      mt19937_64 eng(rd());
      uniform_int_distribution<> distr(lower, upper);
      return distr(eng);
  }
  // generate random number
  vector<uint64_t> random_gen(int num) {
      vector<uint64_t> randnum;
      randnum.reserve(num);
      random_device rd;
      mt19937_64 eng(rd());
      uniform_int_distribution<unsigned long long> distr;
      for (int i = 0; i < num; i++) {
          randnum[i] = distr(eng) % num;
      }
      return randnum;
  }
  // generate unique random number given size and range
  vector<uint64_t> srandom_gen(int num, int lower, int upper) {
     set<uint64_t> array;
     while (array.size() < num) {
	 array.insert(gen_interval(lower, upper));    
     }
     return vector<uint64_t> (array.begin(), array.end());    
  }
  // return generate historial push timestamps of all workers (matrix:col, worker and row, push timestamps)
  unordered_map<int, deque<uint64_t>> get_gen() {
      return history_matrix;
  }
  
  private:
  int num_workers;
  int size_time_array;
  int interval_lower;
  int interval_upper;
  unordered_map<int, deque<uint64_t>> history_matrix;
};

class predictor {
  public:
  predictor (int range, const unordered_map<int, deque<uint64_t>>& hist) :
    predict_range(range), history_matrix(hist) {}
  // simulate next R iterations based on last interval per worker
  void predict_iters () {
    int num_workers = history_matrix.size();
    predicted_matrix.reserve(num_workers);
    for (const auto& e: history_matrix) {
        int worker_id = e.first;
        auto& array = e.second;
        auto init_t = array[array.size()-1];
        auto interval = array[array.size()-1] - array[array.size()-2];        
        vector<uint64_t> future_iteration;
        for (int i=0; i < predict_range; i++) {
            future_iteration.push_back(init_t + interval*i);
        }
        predicted_matrix.insert (make_pair<int,vector<uint64_t>> (int(worker_id), vector<uint64_t>(future_iteration)));
    }
  }
  // return simulated future R iterations for all workers in a matrix <col:worker, row:push timestamps>
  unordered_map<int, vector<uint64_t>> get_predictions () {
      return predicted_matrix;
  }
  // return merged arrays of all workers timestamps in point format
  vector<point> aggregated_points () {
      vector<point> agg;
      for (const auto& e : predicted_matrix) {
              int w = e.first;
              const vector<uint64_t>& array = e.second;
              for (size_t i = 0; i < array.size(); i++) {
                  agg.push_back(point(w,i,array[i]));
              }
      }
      return agg;
  } 

  private:
  int predict_range;
  unordered_map<int, deque<uint64_t>> history_matrix;
  unordered_map<int, vector<uint64_t>> predicted_matrix;  
};
#endif /* GENERATOR_H */
