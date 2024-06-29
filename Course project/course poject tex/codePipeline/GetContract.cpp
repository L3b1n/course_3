static ::mediapipe::Status GetContract(CalculatorContract* cc) {
    RET_CHECK(cc->Inputs().HasTag("IMAGE_CPU"));
    RET_CHECK(cc->Inputs().HasTag("MASK_CPU"));
    RET_CHECK(cc->Outputs().HasTag("OUTPUT_VIDEO"));
    RET_CHECK(cc->Inputs().HasTag("BACKGROUND_CPU"));
    cc->Inputs().Tag("IMAGE_CPU").Set<ImageFrame>();
    cc->Inputs().Tag("MASK_CPU").Set<ImageFrame>();
    cc->Outputs().Tag("OUTPUT_VIDEO").Set<ImageFrame>();
    cc->Inputs().Tag("BACKGROUND_CPU").Set<ImageFrame>();

    return ::mediapipe::OkStatus();
  }