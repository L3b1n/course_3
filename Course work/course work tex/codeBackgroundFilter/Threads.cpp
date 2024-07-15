void BackgroundFilter::filterUpdateThreads(const int32_t numThreads_ = 1) {
    const uint32_t newNumThreads = numThreads_; 

    if (tf->modelSelection.empty() || tf->numThreads != newNumThreads) {
        // Re-initialize model if it's not already the selected 
        // one or switching inference device
        tf->numThreads = newNumThreads;
        initialized = false;
    }
}