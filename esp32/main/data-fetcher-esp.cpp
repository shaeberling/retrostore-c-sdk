#include "data-fetcher-esp.h"

#include <string>

namespace retrostore {

DataFetcherEsp::DataFetcherEsp() :
    DataFetcher("retrostore.org", 80) {
}

void DataFetcherEsp::Fetch(const std::string& path, char** data) {
  // FIXME: Implement.
}

}  // namespace retrostore
