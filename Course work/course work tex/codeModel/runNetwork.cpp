virtual void Model::runNetworkInference(
        const std::unique_ptr<Ort::Session> &session,
        const std::vector<Ort::AllocatedStringPtr> &inputNames,
        const std::vector<Ort::AllocatedStringPtr> &outputNames,
        const std::vector<Ort::Value> &inputTensor,
        std::vector<Ort::Value> &outputTensor)
  {
    if (inputNames.size() == 0 || outputNames.size() == 0 
        || inputTensor.size() == 0 || outputTensor.size() == 0) {
      std::cerr 
            << "Error! Skip network inference. Inputs or outputs are null.\n";
      return;
    }

    std::vector<const char *> rawInputNames;
    for (auto &inputName : inputNames) {
      rawInputNames.push_back(inputName.get());
    }

    std::vector<const char *> rawOutputNames;
    for (auto &outputName : outputNames) {
      rawOutputNames.push_back(outputName.get());
    }

    session->Run(Ort::RunOptions{nullptr}, rawInputNames.data(), 
                inputTensor.data(), inputNames.size(), 
                rawOutputNames.data(), outputTensor.data(), 
                outputNames.size());
  }