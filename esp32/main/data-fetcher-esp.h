#ifndef retro_store_default_data_fetcher_h
#define retro_store_default_data_fetcher_h

#include "data-fetcher.h"

#include <string>

namespace retrostore {

// Default implementation of the data fetcher for the ESP-IDF.
class DataFetcherEsp : public DataFetcher
{
  public:
    DataFetcherEsp();
    void Fetch(const std::string& path, char** data);
};

}  // namespace retrostore

#endif