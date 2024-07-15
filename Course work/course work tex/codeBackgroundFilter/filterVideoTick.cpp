void BackgroundFilter::filterVideoTick() {
    if (tf->inputBGRA.empty()) { 
        std::cerr << "Error! Input image is empty!\n"; 
        return; 
    }

    cv::Mat imageRGB;
    {
        std::unique_lock<std::mutex> lock(
                tf->inputRGBLock, std::try_to_lock);
        if (!lock.owns_lock()) {
            return;
        }
        imageRGB = tf->inputRGB.clone();
    }

    if (tf->backgroundMask.empty()) {
        // First frame. Initialize the background mask.
        tf->backgroundMask = 
            cv::Mat(imageRGB.size(), CV_8UC1, cv::Scalar(255));
    }

    try {
        cv::Mat backgroundMask;

        // Process the image to find the mask.
        processImageForBackground(imageRGB, backgroundMask);
        cv::resize(backgroundMask, backgroundMask, imageRGB.size());

        // Save the mask for the next frame
        backgroundMask.copyTo(tf->backgroundMask);
    }
    catch (const Ort::Exception& e) {
        std::cerr << "Error! " << e.what() << "\n";
        // TODO: Fall back to CPU if it makes sense
    }
    catch (const std::exception& e) {
        std::cerr << "Error! " << e.what() << "\n";
    }
}