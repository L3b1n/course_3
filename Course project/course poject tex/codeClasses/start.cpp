void GMOD::start(cv::Mat& camera_frame_raw, 
                 bool& _load_flag) {
  auto status = this->_runMPPGraph(camera_frame_raw, _load_flag);
  if (!status.ok()) {
    std::string msg(status.message());
    LOG(ERROR) << (msg);
    _load_flag = false;
  }
}