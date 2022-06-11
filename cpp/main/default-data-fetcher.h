#ifndef retro_store_default_data_fetcher_h
#define retro_store_default_data_fetcher_h

#include "data-fetcher.h"

#include <string>

namespace retrostore {

// Default implementation of the data fetcher.
class DefaultDataFetcher : public DataFetcher
{
  public:
    DefaultDataFetcher();
    void Fetch(const std::string& path, char** data);
};

}  // namespace retrostore

#endif