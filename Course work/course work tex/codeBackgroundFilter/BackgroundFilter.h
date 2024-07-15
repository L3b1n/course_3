#pragma once
#include <onnxruntime_cxx_api.h>

#include <opencv2/imgproc.hpp>

#include <numeric>
#include <memory>
#include <exception>
#include <new>
#include <mutex>
#include <iostream>

#include "models/ModelSINET.h"
#include "models/ModelMediapipe.h"
#include "models/ModelSelfie.h"
#include "models/ModelRVM.h"
#include "models/ModelPPHumanSeg.h"
#include "FilterData.h"
#include "consts.h"

class BackgroundFilter
{
public:	
	BackgroundFilter() {
		tf = new FilterData();
		std::string instanceName{"background-removal-inference"};
		tf->env.reset(
            new Ort::Env(OrtLoggingLevel::ORT_LOGGING_LEVEL_ERROR, 
                    instanceName.c_str()));
		tf->modelSelection = MODEL_MEDIAPIPE;
	}

    void filterUpdateThreads(const uint32_t numThreads_ = 1);

	void filterUpdateModel(const std::string moadel_ = MODEL_MEDIAPIPE);

	void filterUpdateProvider(const std::string useGPU_ = USEGPU_CPU);

    void filterActivateChanges() { 
        setDefines(); 
        if (!initialized) {
            createOrtSession(tf);
            initialized = true;
        }
    }

    void filterVideoTick();

    bool setInputImage(const int height, const int width,
		const int type, unsigned char* data);

	cv::Mat getMask() {
		return tf->backgroundMask;
	}

private:
    FilterData *tf;
    bool initialized = false;

    void setDefines() {
		if (tf->modelSelection == MODEL_SINET) {
			#define _MODEL_SINET 1
		}
		if (tf->modelSelection == MODEL_SELFIE) {
			#define _MODEL_SELFIE 1
		}
		if (tf->modelSelection == MODEL_MEDIAPIPE) {
			#define _MODEL_MEDIAPIPE 1
		}
		if (tf->modelSelection == MODEL_RVM) {
			#define _MODEL_RVM 1
		}
		if (tf->modelSelection == MODEL_PPHUMANSEG) {
			#define _MODEL_PPHUMANSEG 1
		}
	}

	void processImageForBackground(
            const cv::Mat& imageBGRA, cv::Mat& backgroundMask);
};