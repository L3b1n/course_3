void BackgroundFilter::processImageForBackground(
        const cv::Mat& imageRGB, 
        cv::Mat& backgroundMask) 
{
    cv::Mat outputImage;
    if (!runFilterModelInference(tf, imageRGB, outputImage)) {
        return;
    }
    // Assume outputImage is now a single channel, uint8 image with values between 0 and 255

    const uint8_t threshold_value = (uint8_t)(0.05f * 255.0f);
    backgroundMask = outputImage < threshold_value;
}