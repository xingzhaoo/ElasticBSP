/*!
 * Copyright (c) 2019 by Xing Zhao
 * \file future_iteration_intervals_gen.cc
 * \brief generate a predicted matrix timestamps based on the given input, 
 * such as a simulated matrix and a pre-defined range of future iterations.
 */
#include <iostream>
#include "generator.cc"
#include "iofile.cc"

inline uint64_t CurrentTime_microseconds()
{
    return std::chrono::duration_cast<std::chrono::microseconds>
              (std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

int main(int argc, char *argv[])
{
    if (argc > 2 || argc == 1)
       cout << "Please enter the input filename (e.g., data_gen_matrix_w100_R20.txt), and the range of future predicted iterations.\n"
	    << "Format example: future_iteration_intervals_gen.exe data_gen_matrix_w100_R20.txt 150" << endl;
    string filename (argv[1]);
    int range = stoi(argv[2]);
    cout << "Reading file " << filename << endl;

    unordered_map <int, deque<uint64_t>> read_matrix;
    int r = read_file(filename, &read_matrix);
    cout << "Found " << r << " workers" << endl;
    cout << "Reading data" << endl;

    for (auto& e: read_matrix) {
        cout << e.first << ':';
        for (auto& l: e.second) {
            cout << l << ',';
        }
        cout << endl;
    }
    cout << "\n=>test simulator(range=" << range << ")<=" << endl;
    cout << "Case for possible same init time per worker" << endl;
    //int range = 150;
    predictor s = predictor(range, read_matrix);
    s.predict_iters();
    for (auto& e: s.get_predictions()) {
        cout << e.first << ':';
        for (auto& l: e.second) {
            cout << l << ',';
        }
        cout << endl;
    }

    cout << "\nComputed predictions of future iterations of workers in matrix format" << endl;
    unordered_map<int, vector<uint64_t>> predict_matrix = s.get_predictions();

    int worker_n = predict_matrix.size();
    string filename_gen = "data_gen_future_iteration_workers_n" + to_string(worker_n) + "_R" + to_string(range) + ".txt";
    int w = write_file(filename_gen, &predict_matrix);
    cout << "Saved to file " << filename_gen << endl;
    cout << "containing workers " << w << ", with range " << range << endl;

    return 0;
}
