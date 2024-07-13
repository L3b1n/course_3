virtual void Model::allocateTensorBuffers(
        const std::vector<std::vector<int64_t>> &inputDims,
        const std::vector<std::vector<int64_t>> &outputDims,
        std::vector<std::vector<float>> &outputTensorValues,
        std::vector<std::vector<float>> &inputTensorValues,
        std::vector<Ort::Value> &inputTensor,
        std::vector<Ort::Value> &outputTensor)
  {
    outputTensorValues.clear();
    outputTensor.clear();
    inputTensorValues.clear();
    inputTensor.clear();

    Ort::MemoryInfo memoryInfo = 
        Ort::MemoryInfo::CreateCpu(
            OrtAllocatorType::OrtDeviceAllocator,
            OrtMemType::OrtMemTypeDefault);

    for (size_t i = 0; i < inputDims.size(); i++) {
      inputTensorValues.push_back(
            std::vector<float>(vectorProduct(inputDims[i]), 0.0f));
      inputTensor.push_back(
            Ort::Value::CreateTensor<float>(
                memoryInfo, inputTensorValues[i].data(), 
                inputTensorValues[i].size(), inputDims[i].data(),
                inputDims[i].size()));
    }

    for (size_t i = 0; i < outputDims.size(); i++) {
      outputTensorValues.push_back(
        std::vector<float>(vectorProduct(outputDims[i]), 0.0f));
      outputTensor.push_back(
            Ort::Value::CreateTensor<float>(
                memoryInfo, outputTensorValues[i].data(), 
                outputTensorValues[i].size(), outputDims[i].data(), 
                outputDims[i].size()));
    }
  }