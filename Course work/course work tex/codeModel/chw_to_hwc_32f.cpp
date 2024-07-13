static void chw_to_hwc_32f(cv::InputArray src, cv::OutputArray dst)
{
  const cv::Mat srcMat = src.getMat();
  const int channels = srcMat.channels();
  const int height = srcMat.rows;
  const int width = srcMat.cols;
  const int dtype = srcMat.type();
  assert(dtype == CV_32F);
  const int channelStride = height * width;
  
  cv::Mat flatMat = srcMat.reshape(1, 1);

  std::vector<cv::Mat> channelsVec(channels);
  for (int i = 0; i < channels; i++) {
    channelsVec[i] =
      cv::Mat(height, width, CV_MAKE_TYPE(dtype, 1), 
            flatMat.ptr<float>(0) + i * channelStride);
  }

  cv::merge(channelsVec, dst);
}