#include "liblzma-node.hpp"

namespace lzma {

#ifdef LZMA_NATIVE_THREAD_SUPPORT
AddonContext::AddonContext(Isolate* isolate, Local<Object> exports) {
  // Link the existence of this object instance to the existence of exports.
  exports_.Reset(isolate, exports);
  exports_.SetWeak(this, DeleteMe, WeakCallbackType::kParameter);
  node::AddEnvironmentCleanupHook(isolate, CleanUp, this);
}

AddonContext::~AddonContext() {
  node::RemoveEnvironmentCleanupHook(Isolate::GetCurrent(), CleanUp, this);
  if (!exports_.IsEmpty()) {
    // Reset the reference to avoid leaking data.
    exports_.ClearWeak();
    exports_.Reset();
  }
}

// Method to call when "exports" is about to be garbage-collected.
void AddonContext::DeleteMe(const WeakCallbackInfo<AddonContext>& info) {
  delete info.GetParameter();
}

// Method to call before a Node.js instance shuts down.
void AddonContext::CleanUp(void* arg) {
  AddonContext* self = reinterpret_cast<AddonContext*>(arg);
  delete self;
}

#endif

#ifdef LZMA_NATIVE_THREAD_SUPPORT
void AddonContext::moduleInit(Local<Object> exports, Local<External> external) {
  LZMAStream::Init(exports, external, lzma_stream_constructor_);
  IndexParser::Init(exports, external, index_parser_constructor_);
#else
void moduleInit(Local<Object> exports) {
  LZMAStream::Init(exports);
  IndexParser::Init(exports);
#endif
  Nan::Set(exports, NewString("versionNumber"),            Nan::GetFunction(Nan::New<FunctionTemplate>(lzmaVersionNumber)).ToLocalChecked());
  Nan::Set(exports, NewString("versionString"),            Nan::GetFunction(Nan::New<FunctionTemplate>(lzmaVersionString)).ToLocalChecked());
  Nan::Set(exports, NewString("checkIsSupported"),         Nan::GetFunction(Nan::New<FunctionTemplate>(lzmaCheckIsSupported)).ToLocalChecked());
  Nan::Set(exports, NewString("checkSize"),                Nan::GetFunction(Nan::New<FunctionTemplate>(lzmaCheckSize)).ToLocalChecked());
  Nan::Set(exports, NewString("crc32_"),                   Nan::GetFunction(Nan::New<FunctionTemplate>(lzmaCRC32)).ToLocalChecked());
  Nan::Set(exports, NewString("filterEncoderIsSupported"), Nan::GetFunction(Nan::New<FunctionTemplate>(lzmaFilterEncoderIsSupported)).ToLocalChecked());
  Nan::Set(exports, NewString("filterDecoderIsSupported"), Nan::GetFunction(Nan::New<FunctionTemplate>(lzmaFilterDecoderIsSupported)).ToLocalChecked());
  Nan::Set(exports, NewString("rawEncoderMemusage"),       Nan::GetFunction(Nan::New<FunctionTemplate>(lzmaRawEncoderMemusage)).ToLocalChecked());
  Nan::Set(exports, NewString("rawDecoderMemusage"),       Nan::GetFunction(Nan::New<FunctionTemplate>(lzmaRawDecoderMemusage)).ToLocalChecked());
  Nan::Set(exports, NewString("mfIsSupported"),            Nan::GetFunction(Nan::New<FunctionTemplate>(lzmaMfIsSupported)).ToLocalChecked());
  Nan::Set(exports, NewString("modeIsSupported"),          Nan::GetFunction(Nan::New<FunctionTemplate>(lzmaModeIsSupported)).ToLocalChecked());
  Nan::Set(exports, NewString("easyEncoderMemusage"),      Nan::GetFunction(Nan::New<FunctionTemplate>(lzmaEasyEncoderMemusage)).ToLocalChecked());
  Nan::Set(exports, NewString("easyDecoderMemusage"),      Nan::GetFunction(Nan::New<FunctionTemplate>(lzmaEasyDecoderMemusage)).ToLocalChecked());

  // enum lzma_ret
  Nan::Set(exports, NewString("OK"),                Nan::New<Number>(LZMA_OK));
  Nan::Set(exports, NewString("STREAM_END"),        Nan::New<Number>(LZMA_STREAM_END));
  Nan::Set(exports, NewString("NO_CHECK"),          Nan::New<Number>(LZMA_NO_CHECK));
  Nan::Set(exports, NewString("UNSUPPORTED_CHECK"), Nan::New<Number>(LZMA_UNSUPPORTED_CHECK));
  Nan::Set(exports, NewString("GET_CHECK"),         Nan::New<Number>(LZMA_GET_CHECK));
  Nan::Set(exports, NewString("MEM_ERROR"),         Nan::New<Number>(LZMA_MEM_ERROR));
  Nan::Set(exports, NewString("MEMLIMIT_ERROR"),    Nan::New<Number>(LZMA_MEMLIMIT_ERROR));
  Nan::Set(exports, NewString("FORMAT_ERROR"),      Nan::New<Number>(LZMA_FORMAT_ERROR));
  Nan::Set(exports, NewString("OPTIONS_ERROR"),     Nan::New<Number>(LZMA_OPTIONS_ERROR));
  Nan::Set(exports, NewString("DATA_ERROR"),        Nan::New<Number>(LZMA_DATA_ERROR));
  Nan::Set(exports, NewString("BUF_ERROR"),         Nan::New<Number>(LZMA_BUF_ERROR));
  Nan::Set(exports, NewString("PROG_ERROR"),        Nan::New<Number>(LZMA_PROG_ERROR));

  // enum lzma_action
  Nan::Set(exports, NewString("RUN"),        Nan::New<Number>(LZMA_RUN));
  Nan::Set(exports, NewString("SYNC_FLUSH"), Nan::New<Number>(LZMA_SYNC_FLUSH));
  Nan::Set(exports, NewString("FULL_FLUSH"), Nan::New<Number>(LZMA_FULL_FLUSH));
  Nan::Set(exports, NewString("FINISH"),     Nan::New<Number>(LZMA_FINISH));

  // enum lzma_check
  Nan::Set(exports, NewString("CHECK_NONE"),   Nan::New<Number>(LZMA_CHECK_NONE));
  Nan::Set(exports, NewString("CHECK_CRC32"),  Nan::New<Number>(LZMA_CHECK_CRC32));
  Nan::Set(exports, NewString("CHECK_CRC64"),  Nan::New<Number>(LZMA_CHECK_CRC64));
  Nan::Set(exports, NewString("CHECK_SHA256"), Nan::New<Number>(LZMA_CHECK_SHA256));

  // lzma_match_finder
  Nan::Set(exports, NewString("MF_HC3"), Nan::New<Number>(LZMA_MF_HC3));
  Nan::Set(exports, NewString("MF_HC4"), Nan::New<Number>(LZMA_MF_HC4));
  Nan::Set(exports, NewString("MF_BT2"), Nan::New<Number>(LZMA_MF_BT2));
  Nan::Set(exports, NewString("MF_BT3"), Nan::New<Number>(LZMA_MF_BT3));
  Nan::Set(exports, NewString("MF_BT4"), Nan::New<Number>(LZMA_MF_BT4));

  // lzma_mode
  Nan::Set(exports, NewString("MODE_FAST"),   Nan::New<Number>(LZMA_MODE_FAST));
  Nan::Set(exports, NewString("MODE_NORMAL"), Nan::New<Number>(LZMA_MODE_NORMAL));

  // defines
  Nan::Set(exports, NewString("FILTER_X86"),               NewString("LZMA_FILTER_X86"));
  Nan::Set(exports, NewString("FILTER_POWERPC"),           NewString("LZMA_FILTER_POWERPC"));
  Nan::Set(exports, NewString("FILTER_IA64"),              NewString("LZMA_FILTER_IA64"));
  Nan::Set(exports, NewString("FILTER_ARM"),               NewString("LZMA_FILTER_ARM"));
  Nan::Set(exports, NewString("FILTER_ARMTHUMB"),          NewString("LZMA_FILTER_ARMTHUMB"));
  Nan::Set(exports, NewString("FILTER_SPARC"),             NewString("LZMA_FILTER_SPARC"));
  Nan::Set(exports, NewString("FILTER_DELTA"),             NewString("LZMA_FILTER_DELTA"));
  Nan::Set(exports, NewString("FILTERS_MAX"),              NewString("LZMA_FILTERS_MAX"));
  Nan::Set(exports, NewString("FILTER_LZMA1"),             NewString("LZMA_FILTER_LZMA1"));
  Nan::Set(exports, NewString("FILTER_LZMA2"),             NewString("LZMA_FILTER_LZMA2"));
  Nan::Set(exports, NewString("VLI_UNKNOWN"),              NewString("LZMA_VLI_UNKNOWN"));

  Nan::Set(exports, NewString("VLI_BYTES_MAX"),            Nan::New<Number>(LZMA_VLI_BYTES_MAX));
  Nan::Set(exports, NewString("CHECK_ID_MAX"),             Nan::New<Number>(LZMA_CHECK_ID_MAX));
  Nan::Set(exports, NewString("CHECK_SIZE_MAX"),           Nan::New<Number>(LZMA_CHECK_SIZE_MAX));
  Nan::Set(exports, NewString("PRESET_DEFAULT"),           Nan::New<Number>(LZMA_PRESET_DEFAULT));
  Nan::Set(exports, NewString("PRESET_LEVEL_MASK"),        Nan::New<Number>(LZMA_PRESET_LEVEL_MASK));
  Nan::Set(exports, NewString("PRESET_EXTREME"),           Nan::New<Number>(LZMA_PRESET_EXTREME));
  Nan::Set(exports, NewString("TELL_NO_CHECK"),            Nan::New<Number>(LZMA_TELL_NO_CHECK));
  Nan::Set(exports, NewString("TELL_UNSUPPORTED_CHECK"),   Nan::New<Number>(LZMA_TELL_UNSUPPORTED_CHECK));
  Nan::Set(exports, NewString("TELL_ANY_CHECK"),           Nan::New<Number>(LZMA_TELL_ANY_CHECK));
  Nan::Set(exports, NewString("CONCATENATED"),             Nan::New<Number>(LZMA_CONCATENATED));
  Nan::Set(exports, NewString("STREAM_HEADER_SIZE"),       Nan::New<Number>(LZMA_STREAM_HEADER_SIZE));
  Nan::Set(exports, NewString("VERSION_MAJOR"),            Nan::New<Number>(LZMA_VERSION_MAJOR));
  Nan::Set(exports, NewString("VERSION_MINOR"),            Nan::New<Number>(LZMA_VERSION_MINOR));
  Nan::Set(exports, NewString("VERSION_PATCH"),            Nan::New<Number>(LZMA_VERSION_PATCH));
  Nan::Set(exports, NewString("VERSION_STABILITY"),        Nan::New<Number>(LZMA_VERSION_STABILITY));
  Nan::Set(exports, NewString("VERSION_STABILITY_ALPHA"),  Nan::New<Number>(LZMA_VERSION_STABILITY_ALPHA));
  Nan::Set(exports, NewString("VERSION_STABILITY_BETA"),   Nan::New<Number>(LZMA_VERSION_STABILITY_BETA));
  Nan::Set(exports, NewString("VERSION_STABILITY_STABLE"), Nan::New<Number>(LZMA_VERSION_STABILITY_STABLE));
  Nan::Set(exports, NewString("VERSION"),                  Nan::New<Number>(LZMA_VERSION));
  Nan::Set(exports, NewString("VERSION_STRING"),           NewString(LZMA_VERSION_STRING));

  Nan::Set(exports, NewString("asyncCodeAvailable"),       Nan::New<Boolean>(LZMAStream::asyncCodeAvailable));
}

}

#ifdef LZMA_NATIVE_THREAD_SUPPORT
NODE_MODULE_INIT(/* exports, module, context */) {
  v8::Isolate* isolate = context->GetIsolate();

  // Create a new instance of AddonContext for this instance of the addon.
  lzma::AddonContext* data = new lzma::AddonContext(isolate, exports);

  // Wrap the data in a v8::External so we can pass it to the method we expose.
  v8::Local<v8::External> external = v8::External::New(isolate, data);

  data->moduleInit(exports, external);
}
#else
NODE_MODULE(lzma_native, lzma::moduleInit)
#endif
