void BackgroundFilter::filterUpdateGpu(const std::string useGPU_) {
    const std::string newUseGpu = useGPU_ /*USEGPU_**/;
    if (tf->modelSelection.empty() || tf->useGPU != newUseGpu) {
        // Re-initialize model if it's not already the selected 
        // one or switching inference device
        tf->useGPU = newUseGpu;
        initialized = false;
    }
}