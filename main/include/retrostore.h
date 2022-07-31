#ifndef retro_store_h
#define retro_store_h

#include <memory>
#include <string>
#include <vector>

#include "data-fetcher-esp.h"

namespace retrostore {

typedef enum  {
    RsTrs80Model_UNKNOWN_MODEL = 0,
    RsTrs80Model_MODEL_I = 1,
    RsTrs80Model_MODEL_III = 2,
    RsTrs80Model_MODEL_4 = 3,
    RsTrs80Model_MODEL_4P = 4
} RsTrs80Model;

typedef struct {
  int ix = 0;
  int iy = 0;
  int pc = 0;
  int sp = 0;
  int af = 0;
  int bc = 0;
  int de = 0;
  int hl = 0;
  int af_prime = 0;
  int bc_prime = 0;
  int de_prime = 0;
  int hl_prime = 0;
  int i = 0;
  int r_1 = 0;
  int r_2 = 0;
} RsSystemStateRegisters;

typedef struct _RsMemoryRegion {
  int start = 0;
  int length = 0;
  std::unique_ptr<uint8_t> data;

} RsMemoryRegion;

typedef struct {
  RsTrs80Model model = RsTrs80Model_UNKNOWN_MODEL;
  RsSystemStateRegisters registers;
  std::vector<RsMemoryRegion> regions;
} RsSystemState;

// API to communicate with the RetroStore server.
class RetroStore
{
  public:
    // Construct with default settings such as server.
    explicit RetroStore();
    // Use custom fetcher, will transfer ownrship of the object.
    RetroStore(DataFetcherEsp* data_fetcher);
    // Print version of the library.
    void PrintVersion();
    // Fetch metadata of retrostore apps from catalog.
    bool FetchApps(int start, int num);
    // Fetch media images for the app with the given ID.
    void FetchMediaImages(const std::string& appId);
    // Upload system state
    // int UploadState(const RsSystemState& state);
    // Download system state
    void downloadState(int token, RsSystemState* state);
  private:
    const DataFetcherEsp* data_fetcher_;
};

}  // namespace retrostore

#endif