#include <BackgroundFilter.h>
#include <onnxruntime_cxx_api.h>
#include <cpu_provider_factory.h>

#ifdef _WIN32
#include <dml_provider_factory.h>
#include <dnnl_provider_facrtory.h>
#include <wchar.h>
#include <iostream>
#endif // _WIN32

#include "ort-session-utils.h"
#include "consts.h"
#if _MODEL_MEDIAPIPE
#include "models/models_h/mediapipe.h"
#endif
#ifdef _MODEL_SELFIE
#include "models/models_h/selfie_segmentation.h"
#endif
#ifdef _MODEL_SINET
#include "models/models_h/SINet_Softmax_simple.h"
#endif
#ifdef _MODEL_RVM
#include "models/models_h/rvm_mobilenetv3_fp32.h"
#endif
#ifdef _MODEL_PPHUMANSEG
#include "models/models_h/pphumanseg_fp32.h"
#endif