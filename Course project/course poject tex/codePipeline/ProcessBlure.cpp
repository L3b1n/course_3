input_mat.copyTo(background, 1.0 - mask_mat);
cv::GaussianBlur(mask_mat, mask_mat, cv::Size(3, 3), 7);
cv::GaussianBlur(background, background, cv::Size(19, 19), 7);

background.copyTo(input_mat, 1.0 - mask_mat);