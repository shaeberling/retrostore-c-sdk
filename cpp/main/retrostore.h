#ifndef retro_store_h
#define retro_store_h

#include <string>

#include "data-fetcher.h"

namespace retrostore {

// API to communicate with the RetroStore server.
class RetroStore
{
  public:
    // Construct with default settings such as server.
    explicit RetroStore();
    // Use custom fetcher, will transfer ownrship of the object.
    RetroStore(DataFetcher* data_fetcher);
    // Print version of the library.
    void PrintVersion();
    // Fetch metadata of retrostore apps from catalog.
    bool FetchApps(int start, int num);
    // Fetch media images for the app with the given ID.
    void FetchMediaImages(const std::string& appId);
    // Upload system state
    // int UploadState(const SystemState& state);
    // Download system state
    // SystemState* downloadState(int token);
  private:
    const DataFetcher* data_fetcher_;
};

}  // namespace retrostore

#endif