/*!
 * Copyright (c) 2019 by Xing Zhao
 * \file workers_iteration_intervals_gen.cc 
 * \brief generate a simulated n x m matrix of timestamps based on 
 * the given number of worker n and the number of iterations (timeslots) m.
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
    if (argc > 3 || argc == 1)
       cout << "Please enter the number of workers and interations, and output file name.\n"
	    << "Format example: workers_iteration_intervals_gen.exe 10 20 data_gen_matrix.txt" << endl;
    int num_workers = stoi(argv[1]);
    int timeslots = stoi(argv[2]);
    string filename (argv[3]);

    cout << "=> generator class(workers=" << num_workers << 
	    ",timespots="<< timeslots << ") <=" << endl;
    cout << "Output file: " << filename << endl;
    //int num_workers = 10, timeslots = 20;
    generator g = generator(num_workers, timeslots);
    for (int i = 0 ; i < timeslots; i++) {
        cout << g.gen_interval(100, 150) << ',';
    }
    cout << "end interval gen in [100, 150] in time unit: ms \n" << endl;

    cout << "possible same init time per worker" << endl;
    g.generate();
    auto hist_matrix = g.get_gen();
    for (auto& e: hist_matrix) {
        cout << e.first << ':';
        for (auto& l: e.second) {
            cout << l << ',';
        }
        cout << endl;
    }

    cout << "\nunique init time per worker" << endl;
    generator gs = generator(num_workers, timeslots);
    gs.generate_uniq();
    auto hist_matrix2 = gs.get_gen();
    for (auto& e: hist_matrix2) {
        cout << e.first << ':';
        for (auto& l: e.second) {
            cout << l << ',';
        }
        cout << endl;
    }

    //string filename = "data_gen_matrix.txt";
    int w = write_file(filename, &hist_matrix);
    cout << "\nSaved results on the file: " << filename <<  "\ncontaining " << w << " workers/lines" << endl;

    return 0;
} 
