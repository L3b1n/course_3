void createOrtSession(FilterData *tf)
{
  if (tf->model.get() == nullptr) {
    std::cerr << "Error! Model object is not initialized!\n";
    return;
  }

  Ort::SessionOptions sessionOptions;

  sessionOptions.SetGraphOptimizationLevel(
        GraphOptimizationLevel::ORT_ENABLE_ALL);
  if (tf->useGPU != USEGPU_CPU) {
    sessionOptions.DisableMemPattern();
    sessionOptions.SetExecutionMode(ExecutionMode::ORT_SEQUENTIAL);
  } else {
    sessionOptions.SetInterOpNumThreads(tf->numThreads);
    sessionOptions.SetIntraOpNumThreads(tf->numThreads);
  }

  char *modelSelection_rawPtr = (char *)tf->modelSelection.c_str();

  if (modelSelection_rawPtr == nullptr) {
    std::cerr << "Error! Unable to get model filename " 
        << tf->modelSelection.c_str() << " from plugin!\n";
    return;
  }

#ifdef _MODEL_SINET
  if (tf->modelSelection == MODEL_SINET) {
      tf->modelInfo = SINet_Softmax_simple_onnx;
      tf->modelSize = SINet_Softmax_simple_onnx_len;
  }
#endif
#ifdef _MODEL_SELFIE
  if (tf->modelSelection == MODEL_SELFIE) {
      tf->modelInfo = selfie_segmentation_onnx;
      tf->modelSize = selfie_segmentation_onnx_len;
  }
#endif
#if _MODEL_MEDIAPIPE
  if (tf->modelSelection == MODEL_MEDIAPIPE) {
      tf->modelInfo = mediapipe_onnx;
      tf->modelSize = mediapipe_onnx_len;
  }
#endif
#ifdef _MODEL_RVM
  if (tf->modelSelection == MODEL_RVM) {
      tf->modelInfo = rvm_mobilenetv3_fp32_onnx;
      tf->modelSize = rvm_mobilenetv3_fp32_onnx_len;
  }
#endif
#ifdef _MODEL_PPHUMANSEG
  if (tf->modelSelection == MODEL_PPHUMANSEG) {
      tf->modelInfo = pphumanseg_fp32_onnx;
      tf->modelSize = pphumanseg_fp32_onnx_len;
  }
#endif

  try {
#ifdef _WIN32
    if (tf->useGPU == USEGPU_DML) {
      auto &api = Ort::GetApi();
      OrtDmlApi *dmlApi = nullptr;
      Ort::ThrowOnError(
        api.GetExecutionProviderApi("DML", ORT_API_VERSION, 
                (const void **)&dmlApi));
      Ort::ThrowOnError(
            dmlApi->SessionOptionsAppendExecutionProvider_DML(
                    sessionOptions, 0));
    } else if (tf->useGPU == USEGPU_VINO) {
      OrtOpenVINOProviderOptions openvino;
      openvino.num_of_threads = 1;
      openvino.device_type = "CPU_FP32";
      sessionOptions.AppendExecutionProvider_OpenVINO(openvino);
      Ort::ThrowOnError(
            OrtSessionOptionsAppendExecutionProvider_CPU(
                    sessionOptions, 0));
    } else if (tf->useGPU == USEGPU_QNN) {
      Ort::Env env = Ort::Env{ ORT_LOGGING_LEVEL_ERROR, "Default" };
      std::unordered_map<std::string, std::string> qnn_options;
      qnn_options["backend_path"] = "QnnHtp.dll";
      sessionOptions.AppendExecutionProvider("QNN", qnn_options);
    } else if (tf->useGPU == USEGPU_DNN) {
      Ort::Env env = Ort::Env{ ORT_LOGGING_LEVEL_ERROR, "Default" };
      Ort::SessionOptions sf;
      bool enable_cpu_mem_arena = true;
      Ort::ThrowOnError(
        OrtSessionOptionsAppendExecutionProvider_Dnnl(
                sf, enable_cpu_mem_arena));

    }
#endif
    tf->session.reset(
            new Ort::Session(*tf->env, tf->modelInfo, 
                        tf->modelSize, sessionOptions));
  } catch (const std::exception &e) {
    std::cerr << "Error! " << e.what() << "\n";
    return;
  }

  Ort::AllocatorWithDefaultOptions allocator;
  tf->model->populateInputOutputNames(
        tf->session, tf->inputNames, tf->outputNames);

  if (!tf->model->populateInputOutputShapes(
        tf->session, tf->inputDims, tf->outputDims)) {
    std::cerr << "Error! Unable to get model input and output shapes\n";
    return;
  }

  tf->model->allocateTensorBuffers(
        tf->inputDims, tf->outputDims, 
        tf->outputTensorValues, tf->inputTensorValues,
        tf->inputTensor, tf->outputTensor);
}