bool BackgroundFilter::setInputImage(const int height, const int width,
		const int type, unsigned char* data);
    cv::Mat temp_mat(height, width, type, data), tempRGB;
    cv::cvtColor(tempMat, tempRGB, cv::COLOR_BGR2RGB);
    {
        std::lock_guard<std::mutex> lock(tf->inputRGBLock);
        tf->inputRGB = std::move(tempRGB);
    }
}