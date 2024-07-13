virtual bool Model::populateInputOutputShapes(
        const std::unique_ptr<Ort::Session> &session,
        std::vector<std::vector<int64_t>> &inputDims,
        std::vector<std::vector<int64_t>> &outputDims)
  {
    // Assuming model only has one input and one output image
    inputDims.clear();
    outputDims.clear();
    inputDims.push_back(std::vector<int64_t>());
    outputDims.push_back(std::vector<int64_t>());

    // Get output shape
    const Ort::TypeInfo outputTypeInfo = session->GetOutputTypeInfo(0);
    const auto outputTensorInfo = outputTypeInfo.GetTensorTypeAndShapeInfo();
    outputDims[0] = outputTensorInfo.GetShape();

    // fix any -1 values in outputDims to 1
    for (auto &i : outputDims[0]) {
      if (i == -1) {
        i = 1;
      }
    }

    // Get input shape
    const Ort::TypeInfo inputTypeInfo = session->GetInputTypeInfo(0);
    const auto inputTensorInfo = inputTypeInfo.GetTensorTypeAndShapeInfo();
    inputDims[0] = inputTensorInfo.GetShape();

    // fix any -1 values in inputDims to 1
    for (auto &i : inputDims[0]) {
      if (i == -1) {
        i = 1;
      }
    }

    if (inputDims[0].size() < 3 || outputDims[0].size() < 3) {
      std::cerr << "Input or output tensor dims are < 3. input = "
            << (int)inputDims.size() << ", output = "
            << (int)outputDims.size() << "\n";
      return false;
    }

    return true;
  }