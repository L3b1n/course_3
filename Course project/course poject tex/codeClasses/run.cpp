absl::Status GMOD::_runMPPGraph(cv::Mat& camera_frame_raw, 
                                bool& _load_flag) {
  cv::Mat camera_frame;
  cv::cvtColor(camera_frame_raw, camera_frame, cv::COLOR_BGR2RGB);

  cv::flip(camera_frame, camera_frame, /*flipcode=HORIZONTAL*/ 1);

  auto input_frame = absl::make_unique<mediapipe::ImageFrame>(
      mediapipe::ImageFormat::SRGB, camera_frame.cols, camera_frame.rows,
      mediapipe::ImageFrame::kDefaultAlignmentBoundary);
  cv::Mat input_frame_mat = mediapipe::formats::MatView(input_frame.get());
  camera_frame.copyTo(input_frame_mat);

  size_t frame_timestamp_us =
      (double)cv::getTickCount() / (double)cv::getTickFrequency() * 1e6;
  MP_RETURN_IF_ERROR(_graph->AddPacketToInputStream(
      kInputStream, mediapipe::Adopt(input_frame.release())
                        .At(mediapipe::Timestamp(frame_timestamp_us))));

  auto background_frame = absl::make_unique<mediapipe::ImageFrame>(
      mediapipe::ImageFormat::SRGB, background.cols, background.rows,
      mediapipe::ImageFrame::kDefaultAlignmentBoundary);
  cv::Mat background_data = mediapipe::formats::MatView(background_frame.get());
  background.copyTo(background_data);

  MP_RETURN_IF_ERROR(_graph->AddPacketToInputStream(
      kInputBackground, mediapipe::Adopt(background_frame.release())
                        .At(mediapipe::Timestamp(frame_timestamp_us))));

  mediapipe::Packet packet;
  if (!output_poller->Next(&packet)){ 
    _load_flag = false; 
    return absl::Status();
  }

  auto& output_frame = packet.Get<mediapipe::ImageFrame>();
  cv::Mat output_frame_mat = mediapipe::formats::MatView(&output_frame);
  cv::cvtColor(output_frame_mat, output_frame_mat, cv::COLOR_RGB2BGR);

  if(showOverlay){
    output_frame_mat.copyTo(camera_frame_raw);
  } 
  
  return ::mediapipe::OkStatus();
}