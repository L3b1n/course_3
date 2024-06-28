class GMOD : public IGMOD {
public:
  virtual bool getOverlay() override;
  virtual void setOverlay(bool x) override;

  virtual void setCameraProps(const int& camId, 
                              const int& camResX, 
                              const int& camResY, 
                              const int& camFps) override;

  virtual void start(cv::Mat& camera_frame_raw, bool& _load_flag) override;
  virtual void init(const std::string& filename, 
                    cv::VideoCapture& capture, 
                    const std::string& path = "background.jpg", 
                    const int& typeOfEffect = 0, 
                    const int& typeOfModel = 1) override;

private:
  absl::Status _runMPPGraph(cv::Mat& camera_frame_raw, bool& _load_flag);
  absl::Status _initMPPGraph(cv::VideoCapture& capture);

private:
  int camId;
  int camFps;
  int camResX;
  int camResY;
  int typeOfModel;
  int typeOfEffect;
  bool showOverlay;
  cv::Mat background;
  std::string graphFilename;
  std::string backgroundFilename;

  std::unique_ptr<mediapipe::OutputStreamPoller> output_poller;

  const char kInputStream[12] = "input_video";
  const char kOutputStream[13] = "output_video";
  const char kInputBackground[17] = "input_background";

public:
  std::shared_ptr<mediapipe::CalculatorGraph> _graph;
};