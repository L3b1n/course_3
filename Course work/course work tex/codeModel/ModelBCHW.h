class ModelBCHW : public Model {
public:
  ModelBCHW() {}
  ~ModelBCHW() {}
  virtual void prepareInputToNetwork(
        cv::Mat &resizedImage, 
        cv::Mat &preprocessedImage)
  {
    resizedImage = resizedImage / 255.0;
    hwc_to_chw(resizedImage, preprocessedImage);
  }

  virtual void postprocessOutput(cv::Mat &output)
  {
    cv::Mat outputTransposed;
    chw_to_hwc_32f(output, outputTransposed);
    outputTransposed.copyTo(output);
  }

  virtual void getNetworkInputSize(
        const std::vector<std::vector<int64_t>> &inputDims,
        uint32_t &inputWidth, 
        uint32_t &inputHeight)
  {
    // BCHW
    inputWidth = (int)inputDims[0][3];
    inputHeight = (int)inputDims[0][2];
  }

  virtual cv::Mat getNetworkOutput(
        const std::vector<std::vector<int64_t>> &outputDims,
        std::vector<std::vector<float>> &outputTensorValues)
  {
    // BCHW
    uint32_t outputWidth = (int)outputDims[0].at(3);
    uint32_t outputHeight = (int)outputDims[0].at(2);
    int32_t outputChannels = CV_MAKE_TYPE(CV_32F, (int)outputDims[0].at(1));

    return cv::Mat(outputHeight, outputWidth, 
            outputChannels, outputTensorValues[0].data());
  }

  virtual void loadInputToTensor(
        const cv::Mat &preprocessedImage, 
        uint32_t, uint32_t,
        std::vector<std::vector<float>> &inputTensorValues)
  {
    inputTensorValues[0].assign(
            preprocessedImage.begin<float>(), 
            preprocessedImage.end<float>());
  }
};