void BackgroundFilter::filterUpdateModel(const std::string model_) {
    const std::string newModel = model_ /*MODEL_***/;
    if (tf->modelSelection.empty() || tf->modelSelection != newModel) {
        // Re-initialize model if it's not already the selected 
        // one or switching inference device
        tf->modelSelection = newModel;

        if (tf->modelSelection == MODEL_SINET) {
            tf->model.reset(new ModelSINET);
        }
        if (tf->modelSelection == MODEL_SELFIE) {
            tf->model.reset(new ModelSelfie);
        }
        if (tf->modelSelection == MODEL_MEDIAPIPE) {
            tf->model.reset(new ModelMediaPipe);
        }
        if (tf->modelSelection == MODEL_RVM) {
            tf->model.reset(new ModelRVM);
        }
        if (tf->modelSelection == MODEL_PPHUMANSEG) {
            tf->model.reset(new ModelPPHumanSeg);
        }
        initialized = false;
    }
}