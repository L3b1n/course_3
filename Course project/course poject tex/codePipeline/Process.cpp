::mediapipe::Status Process(CalculatorContext* cc) final {
    const auto& input_img = 
        cc->Inputs().Tag("IMAGE_CPU").Get<ImageFrame>();
    cv::Mat input_mat = formats::MatView(&input_img);

    const auto& background_img = 
        cc->Inputs().Tag("BACKGROUND_CPU").Get<ImageFrame>();
    cv::Mat background = formats::MatView(&background_img);

    const auto& mask_img = 
        cc->Inputs().Tag("MASK_CPU").Get<ImageFrame>();
    cv::Mat mask_mat = formats::MatView(&mask_img);

    cv::cvtColor(background, background, cv::COLOR_RGB2BGR);
    cv::resize(background, background, input_mat.size());
    
    mask_mat.convertTo(mask_mat, CV_8UC3);
    cv::blur(mask_mat, mask_mat, cv::Size(15, 15));
    cv::threshold(mask_mat, mask_mat, 0.5, 255, cv::THRESH_BINARY);

    background.copyTo(input_mat, 1.0 - mask_mat);
    cv::Mat output_image = input_mat;

    auto output_frame = absl::make_unique<ImageFrame>(
        input_img.Format(), input_img.Width(), input_img.Height());

    cv::Mat output_mat = 
        mediapipe::formats::MatView(output_frame.get());
    output_image.copyTo(output_mat);

    cc->Outputs()
       .Tag("OUTPUT_VIDEO")
       .Add(output_frame.release(), cc->InputTimestamp());

    return ::mediapipe::OkStatus();
  }