absl::Status GMOD::_initMPPGraph(cv::VideoCapture& capture) {
  std::string calculator_graph_config_contents;
  MP_RETURN_IF_ERROR(mediapipe::file::GetContents(
      graphFilename,
      &calculator_graph_config_contents));

  LOG(INFO) << "Get calculator graph config contents: "
            << calculator_graph_config_contents;

  mediapipe::CalculatorGraphConfig config =
    mediapipe::ParseTextProtoOrDie<mediapipe::CalculatorGraphConfig>(
      calculator_graph_config_contents);

  LOG(INFO) << "Initialize the calculator graph.";
  _graph.reset(new mediapipe::CalculatorGraph());
  MP_RETURN_IF_ERROR(_graph->Initialize(config));

  LOG(INFO) << "Initialize the camera";
  capture.open(camId);
  RET_CHECK(capture.isOpened());
  capture.set(cv::CAP_PROP_FRAME_WIDTH, camResX);
  capture.set(cv::CAP_PROP_FRAME_HEIGHT, camResY);
  capture.set(cv::CAP_PROP_FPS, camFps);

  std::map<std::string, mediapipe::Packet> input_side_packets;
  input_side_packets["type_of_effect"] = mediapipe::MakePacket<int>(this->typeOfEffect);
  input_side_packets["model_selection"] = mediapipe::MakePacket<int>(this->typeOfModel);

  LOG(INFO) << "Start running the calculator graph.";

  auto output_poller_sop = _graph->AddOutputStreamPoller(kOutputStream);
  RET_CHECK(output_poller_sop.ok());
  output_poller = 
    std::make_unique<mediapipe::OutputStreamPoller>(
      std::move(output_poller_sop.value()));

  MP_RETURN_IF_ERROR(_graph->StartRun(input_side_packets));
  LOG(INFO) << "Start grabbing and processing frames.";
  this->background = cv::imread("testData/" + this->backgroundFilename);
  return ::mediapipe::OkStatus();
}