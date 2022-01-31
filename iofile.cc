/*!
 * Copyright (c) 2019 by Xing Zhao
 * \file iofile.cc
 * \brief implement saving function to save hashmap <int, deque<uint64_t>> to .txt file
 */
#include <iostream>
#include <deque>
#include <unordered_map>
#include <inttypes.h>
#include <unistd.h>
#include <cstring>

using namespace std;
int write_file(string fname, unordered_map<int, vector<uint64_t>> * matrix) {
    int count = 0;
    if (matrix->empty())
	return 0;

    FILE *fp = fopen(fname.c_str(), "w");
    if (!fp)
	return -errno;

    for (unordered_map <int, vector<uint64_t>>::iterator it = matrix->begin(); it != matrix->end(); it++) {
        fprintf (fp, "%d", it->first);
        for (vector<uint64_t>::iterator vec = it->second.begin(); vec != it->second.end(); vec++) {
            fprintf (fp, "%" PRIu64 ",", *vec);
	}
	fprintf (fp, "\n");
        count++;
    }

    fclose (fp);
    return count;
}

int write_file(string fname, unordered_map<int, deque<uint64_t>> * matrix) {
    int count = 0;
    if (matrix->empty())
        return 0;

    FILE *fp = fopen(fname.c_str(), "w");
    if (!fp)
        return -errno;

    for (unordered_map <int, deque<uint64_t>>::iterator it = matrix->begin(); it != matrix->end(); it++) {
        fprintf (fp, "%d:" , it->first);
        for (deque<uint64_t>::iterator dq = it->second.begin(); dq != it->second.end(); dq++) {
             fprintf (fp, "%" PRIu64 ",", *dq);
        }
        fprintf(fp, "\n");
        count++;
    }

    fclose (fp);
    return count;
}

int read_file (string fname, unordered_map<int, deque<uint64_t>> * matrix) {
    int count = 0;
    if (access (fname.c_str(), R_OK) < 0)
        return -errno;

    FILE * fp = fopen (fname.c_str(), "r");
    if (!fp)
        return -errno;

    matrix->clear();

    char *buf = 0;
    size_t buflen = 0;

    while (getline(&buf, &buflen, fp) > 0) {
        char * newline = strchr (buf, '\n');
        if (newline == NULL)
           continue;
        * newline = 0;

        char * split = strchr (buf, ':');
        if (split == NULL)
           continue;
        * split = 0;
        split++;

        int split1 = stoi(buf);
        //string split2 = split;
        char * time = split;
        deque<uint64_t> t;
        while ((time = strchr(time, ',')) != NULL) {
            //if (time[0] != '\0')  {// require verify
		*time = 0;
            	time++;
                t.push_back(stoull(split));
		split = time;
        }

        (* matrix)[split1] = t;

        count++;
    }
    if (buf)
           free(buf);

    fclose(fp);
    return count;
}
