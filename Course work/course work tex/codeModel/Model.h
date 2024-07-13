#ifndef MODEL_H
#define MODEL_H

#include <onnxruntime_cxx_api.h>

#ifdef _WIN32
#include <wchar.h>
#endif

#include <opencv2/imgproc.hpp>
#include <algorithm>

#define UNUSED_PARAMETER(param) (void)param

class Model {
private:

public:
  Model(){};
  virtual ~Model(){};

  const char *name;

  virtual void populateInputOutputNames(
        const std::unique_ptr<Ort::Session> &session,
        std::vector<Ort::AllocatedStringPtr> &inputNames,
        std::vector<Ort::AllocatedStringPtr> &outputNames);

  virtual bool populateInputOutputShapes(
        const std::unique_ptr<Ort::Session> &session,
        std::vector<std::vector<int64_t>> &inputDims,
        std::vector<std::vector<int64_t>> &outputDims);

  virtual void allocateTensorBuffers(
        const std::vector<std::vector<int64_t>> &inputDims,
        const std::vector<std::vector<int64_t>> &outputDims,
        std::vector<std::vector<float>> &outputTensorValues,
        std::vector<std::vector<float>> &inputTensorValues,
        std::vector<Ort::Value> &inputTensor,
        std::vector<Ort::Value> &outputTensor);

  virtual void getNetworkInputSize(
        const std::vector<std::vector<int64_t>> &inputDims,
        uint32_t &inputWidth,
        uint32_t &inputHeight)
  {
    // BHWC
    inputWidth = (int)inputDims[0][2];
    inputHeight = (int)inputDims[0][1];
  }

  virtual void prepareInputToNetwork(
        cv::Mat &resizedImage, 
        cv::Mat &preprocessedImage)
  {
    preprocessedImage = resizedImage / 255.0;
  }

  virtual void postprocessOutput(cv::Mat &output) { UNUSED_PARAMETER(output); }

  virtual void loadInputToTensor(
        const cv::Mat &preprocessedImage,
        uint32_t inputWidth,
        uint32_t inputHeight,
        std::vector<std::vector<float>> &inputTensorValues)
  {
    preprocessedImage.copyTo(
        cv::Mat(inputHeight, inputWidth, 
                CV_32FC3, &(inputTensorValues[0][0])));
  }

  virtual cv::Mat getNetworkOutput(
        const std::vector<std::vector<int64_t>> &outputDims,
        std::vector<std::vector<float>> &outputTensorValues)
  {
    // BHWC
    uint32_t outputWidth = (int)outputDims[0].at(2);
    uint32_t outputHeight = (int)outputDims[0].at(1);
    int32_t outputChannels = CV_MAKE_TYPE(CV_32F, (int)outputDims[0].at(3));

    return cv::Mat(outputHeight, outputWidth, 
                    outputChannels, outputTensorValues[0].data());
  }

  virtual void assignOutputToInput(std::vector<std::vector<float>> &,
                                   std::vector<std::vector<float>> &) {}

  virtual void runNetworkInference(
        const std::unique_ptr<Ort::Session> &session,
        const std::vector<Ort::AllocatedStringPtr> &inputNames,
        const std::vector<Ort::AllocatedStringPtr> &outputNames,
        const std::vector<Ort::Value> &inputTensor,
        std::vector<Ort::Value> &outputTensor);
};

#endif