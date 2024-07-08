bool runFilterModelInference(
        FilterData *tf, 
        const cv::Mat &imageRGB, 
        cv::Mat &output)
{
  if (tf->session.get() == nullptr) {
    // Onnx runtime session is not initialized. Problem in initialization
    std::cerr << "Error! Session isn't initialized!\n";
    return false;
  }
  if (tf->model.get() == nullptr) {
    // Model object is not initialized
    std::cerr << "Error! Model isn't initialized!\n";
    return false;
  }

  // Resize to network input size
  uint32_t inputWidth, inputHeight;
  tf->model->getNetworkInputSize(
        tf->inputDims, inputWidth, inputHeight);

  cv::Mat resizedImageRGB;
  cv::resize(imageRGB, resizedImageRGB, cv::Size(inputWidth, inputHeight));

  // Prepare input to nework
  cv::Mat resizedImage, preprocessedImage;
  resizedImageRGB.convertTo(resizedImage, CV_32F);

  tf->model->prepareInputToNetwork(resizedImage, preprocessedImage);

  tf->model->loadInputToTensor(
            preprocessedImage, inputWidth, 
            inputHeight, tf->inputTensorValues);

  // Run network inference
  tf->model->runNetworkInference(
        tf->session, tf->inputNames, 
        tf->outputNames, tf->inputTensor, tf->outputTensor);

  // Get output
  // Map network output to cv::Mat
  cv::Mat outputImage = 
        tf->model->getNetworkOutput(
                    tf->outputDims, 
                    tf->outputTensorValues);

  // Assign output to input in some models that have temporal information
  tf->model->assignOutputToInput(
        tf->outputTensorValues, tf->inputTensorValues);

  // Post-process output. The image will now be in [0,1] float, BHWC format
  tf->model->postprocessOutput(outputImage);

  // Convert [0,1] float to CV_8U [0,255]
  outputImage.convertTo(output, CV_8U, 255.0);
  return true;
}