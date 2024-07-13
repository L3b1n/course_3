static void hwc_to_chw(cv::InputArray src, cv::OutputArray dst)
{
  std::vector<cv::Mat> channels;
  cv::split(src, channels);
  for (auto &img : channels) { img = img.reshape(1, 1);}
  cv::hconcat(channels, dst);
}