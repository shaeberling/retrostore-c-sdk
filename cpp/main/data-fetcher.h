#ifndef retro_store_data_fetcher_h
#define retro_store_data_fetcher_h

#include <string>

namespace retrostore {

// API to fetch URLs. Depending on the environment, feel free to implement
// in a way that works for you, e.g. using ESP32's IDF HTTP fetch API.
class DataFetcher
{
  public:
    DataFetcher(const std::string& host, int port) :
            host_(host),
            port_(port){}
    void Fetch(const std::string& path, char** data) {/* No-op*/ };
  protected:
    const std::string host_;
    const int port_;
};
}  // namespace retrostore

#endif