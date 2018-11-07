#include "liblzma-node.hpp"

namespace lzma {

MTOptions::MTOptions(Value val) {
  Object opt = val.ToObject();
  opts_.flags = 0;
  opts_.filters = nullptr;

  opts_.block_size = opt["blockSize"].ToNumber().Int64Value();
  opts_.timeout = opt["timeout"].ToNumber().Uint32Value();
  opts_.preset = opt["preset"].ToNumber().Uint32Value();
  opts_.preset = (lzma_check)opt["check"].ToNumber().Int32Value();
  opts_.threads = opt["threads"].ToNumber().Uint32Value();

  if (opts_.threads == 0) {
    opts_.threads = lzma_cputhreads();
  }

  Value filters = opts["filters"];
  if (filters.IsArray()) {
    filters_.reset(new FilterArray(filters));
    opts_.filters = filters_->array();
  }
}

}
