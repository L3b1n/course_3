void GMOD::init(const std::string& filename,
                cv::VideoCapture& capture,
                const std::string& path,
                const int& typeOfEffect,
                const int& typeOfModel) {
  this->graphFilename = filename;
  this->backgroundFilename = path;
  this->typeOfEffect = (typeOfEffect != 0 && typeOfEffect != 1 ? 1 : typeOfEffect);
  this->typeOfModel = (typeOfModel != 0 && typeOfModel != 1 ? 1 : typeOfModel);
  auto status = this->_initMPPGraph(capture);
  if (!status.ok()) {
    std::string msg(status.message());
    LOG(ERROR) << (msg);
  }
}