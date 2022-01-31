// This file is for testing purpose only, please disregard it.
#include <iostream>
#include "generator.cc"
#include "iofile.cc"
inline uint64_t CurrentTime_microseconds()
{
    return std::chrono::duration_cast<std::chrono::microseconds>
              (std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}
int main()
{
    /*cout << "=>Test generator class(w=10,timespots=20) <=" << endl;
    int num_workers = 10, timeslots = 20;
    generator g = generator(num_workers, timeslots);
    for (int i = 0 ; i < 20; i++) {
        cout << g.gen_interval(100, 150) << ',';
    }
    cout << "end interval gen in [100, 150] \n" << endl;
    
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
    
    cout << "unique init time per worker" << endl;
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
    
    string filename = "data_gen_matrix.txt";
    int w = write_file(filename, &hist_matrix);
    cout << "saved " << w << endl;
    */
    string filename = "data_gen_matrix_w1000_R20.txt";
    unordered_map <int, deque<uint64_t>> read_matrix;
    int r = read_file(filename, &read_matrix);
    cout << "read " << r << endl;
    cout << "read data" << endl;

    for (auto& e: read_matrix) {
        cout << e.first << ':';
        for (auto& l: e.second) {
            cout << l << ',';
        }
        cout << endl;
    }
    cout << "=>test simulator(range=15)<=" << endl;
    cout << "possible same init time per worker" << endl;
    int range = 150;
    predictor s = predictor(range, read_matrix);
    s.predict_iters();
    for (auto& e: s.get_predictions()) {
        cout << e.first << ':';
        for (auto& l: e.second) {
            cout << l << ',';
        }
        cout << endl;
    }
    /*
    cout << "unique init time per worker" << endl;
    simulator s2 = simulator(range, hist_matrix2);
    s2.simulate_iters();
    for (auto& e: s2.get_predictions()) {
        cout << e.first << ':';
        for (auto& l: e.second) {
            cout << l << ',';
        }
        cout << endl;
    }
    */
    //cout << "aggregared points" << endl;
    //vector<point> all = s.aggregated_points();
    cout << "get predictions in matrix formate" << endl;
    unordered_map<int, vector<uint64_t>> predict_matrix = s.get_predictions();
    //for (const auto & e: all)
    //   cout << e << ' ';
    cout << "=> test zipline <=" << endl;
    int num_workers = read_matrix.size();
    uint64_t tic_load = CurrentTime_microseconds();
    //zipline zip = zipline (num_workers, predict_matrix);
    zipline_tk zip = zipline_tk (3, num_workers, predict_matrix);
    uint64_t tac_load = CurrentTime_microseconds();
    //zipline_dist zip = zipline_dist (num_workers, all);
    uint64_t tic_sort = CurrentTime_microseconds(); 
    //zip.sort_timeline();
    zip.sort_timeline();
    uint64_t tac_sort = CurrentTime_microseconds();

    /*cout << "= sorted points on timeline =" << endl;
    for (auto const &e: zip.get_timeline())
        cout << e;
    cout << "= zip the time line =" << endl;*/
    uint64_t tic_zip = CurrentTime_microseconds();
    //zip.zip();
    zip.zip_tk();
    uint64_t tac_zip = CurrentTime_microseconds();
    //zip.zip_dist();
    cout << "= get set from zipline = " << endl;
    for (const auto& e: zip.getSet())
        cout << e;
    cout << "\n d = " << zip.get_d() << endl;

    cout << "= get min set from zipline = " << endl;
    for (const auto& e: zip.get_minSet())
        cout << e;
    cout << "\n min_d = " << zip.get_min_d() << endl;

    cout << "min point -> " << zip.get_min_point() << endl;
    cout << "max point -> " << zip.get_max_point() << endl;
    cout << "min point in min d set -> " << zip.get_min_d_point() << endl;
    cout << "max point in min d set -> " << zip.get_max_d_point() << endl;

    cout << "= other selections and their min_d =\n min_d | set" << endl;
    /*unordered_map <uint64_t, vector<point>> dist_list = zip.get_dist_list();
    vector<uint64_t> dist_index = zip.get_dist_list_index();
    int count = 1;
    for (const auto& e: dist_index) {
	//cout << "|(" << count++ << ")|";
	if (e == zip.get_min_d())
	   cout << "*";
	cout << e << ":";
	for (const auto& l: dist_list[e]) {
	    cout << l << ",";
	}
	cout << endl;
    }*/
    cout << "top k list" << endl;
    const auto& tklist = zip.get_tk_list();
    for (const auto& e: tklist) {
	cout << "d = " << e.first << "::";
	for (const auto& l: e.second)
	    cout << l << ",";
	cout << endl;
    }
    cout << "penalised min d = " << zip.get_opt() << endl;
    cout << "min d set after penalty" << endl;
    const auto& min_pen_set = zip.min_opt_set();
    for (const auto& e: min_pen_set)
	cout << e << ",";
    cout << endl;
    cout << "min point in min d set with penalty -> " << zip.get_min_d_penal_point() << endl; 
    cout << "max point in min d set with penalty -> " << zip.get_max_d_penal_point() << endl; 
    cout << "sort time (ms): " << tac_sort - tic_sort << endl;
    cout << "zip time (ms): " << tac_zip - tic_zip << endl;
    return 0;
}
