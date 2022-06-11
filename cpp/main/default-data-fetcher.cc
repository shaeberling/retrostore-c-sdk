#include "default-data-fetcher.h"

#include <string>

namespace retrostore {

DefaultDataFetcher::DefaultDataFetcher() :
    DataFetcher("retrostore.org", 80) {
}

void DefaultDataFetcher::Fetch(const std::string& path, char** data) {
  // FIXME: Implement.
}

}  // namespace retrostore
