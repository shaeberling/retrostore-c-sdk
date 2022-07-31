#pragma once

struct RsData {
  uint8_t* data;
  int len;

  explicit RsData() {
    data = NULL;
    len = 0;
  }

  RsData(int size) {
    data = new u_int8_t[size];
    len = size;
  }

  ~RsData() {
    delete data;
  }
};
