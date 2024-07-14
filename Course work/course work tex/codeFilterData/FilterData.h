#ifndef FILTERDATA_H
#define FILTERDATA_H

#include "models/Model.h"
#include "OrtUtils/ORTModelData.h"

struct FilterData : public ORTModelData {
	std::string useGPU;
	uint32_t numThreads;
	std::string modelSelection;
	std::unique_ptr<Model> model;

	cv::Mat inputRGB;

	std::mutex inputRGBLock;

	const unsigned char* modelInfo = nullptr;
    unsigned int modelSize = 0;
};

#endif /* FILTERDATA_H */