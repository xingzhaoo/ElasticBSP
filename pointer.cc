/*!
 * Copyright (c) 2019 by Xing Zhao
 * \pointer.cc
 * \brief implement pointer (timestamp) of ZipLine algorithm
 */
#ifndef POINTER_H
#define POINTER_H

#include <iostream>
#include <cstdint>

class point {
  public:
    uint64_t timestamp;
    int worker_id;
    int worker_index;

    point(int id, int index, uint64_t time):
	    worker_id(id), worker_index(index), timestamp(time)
	  {}

    bool operator< (const point & msgObj) const {
      uint64_t right = msgObj.timestamp;
      uint64_t left = this->timestamp;
      return (left < right);
    }

    uint64_t operator- (const point & msgObj) const {
      uint64_t right = msgObj.timestamp;
      uint64_t left = this->timestamp;
      return (left - right);
    }

    friend std::ostream& operator<< (std::ostream& os, const point& obj);
};

inline std::ostream& operator<< (std::ostream& os, const point& obj) {
  os << obj.worker_id << "|" << obj.worker_index << "|" << obj.timestamp;
  return os;
}

#endif /* POINTER_H */
