virtual void Model::populateInputOutputNames(
        const std::unique_ptr<Ort::Session> &session,
        std::vector<Ort::AllocatedStringPtr> &inputNames,
        std::vector<Ort::AllocatedStringPtr> &outputNames)
  {
    Ort::AllocatorWithDefaultOptions allocator;

    inputNames.clear();
    outputNames.clear();
    inputNames.push_back(session->GetInputNameAllocated(0, allocator));
    outputNames.push_back(session->GetOutputNameAllocated(0, allocator));
  }