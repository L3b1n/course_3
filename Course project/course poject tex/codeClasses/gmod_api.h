#pragma once
#include<string>
#include<functional>

#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

DLLEXPORT class IGMOD {
public:
  virtual bool getOverlay() = 0;
  virtual void setOverlay(bool x) = 0;
    
  virtual void setCameraProps(const int& camId, 
                              const int& camResX, 
                              const int& camResY, 
                              const int& camFps) = 0;
    
  virtual void start(cv::Mat& camera_frame_raw, bool& _load_flag) = 0;
  virtual void init(const std::string& filename, 
                    cv::VideoCapture& capture, 
                    const std::string& path = "background.jpg", 
                    const int& typeOfEffect = 0, 
                    const int& typeOfModel = 1) = 0;
  virtual void stop() = 0;
};

DLLEXPORT IGMOD* CreateGMOD();