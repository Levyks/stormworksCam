#include <napi.h>
#include "napi-thread-safe-callback.hpp"
#include <thread>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

char* imageStringfier(char* output, Mat frame, int width, int height);

void startProcessing(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    if (info.Length() < 4) {
      Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
      return;
    }

    if (!info[0].IsString() || !info[1].IsNumber() || !info[2].IsNumber()) {
      Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
      return;
    }

    string url = info[0].As<Napi::String>().Utf8Value();
    int width = info[1].As<Napi::Number>().Int32Value();
    int height = info[2].As<Napi::Number>().Int32Value();

    // Capture callback in main thread
    auto callback = std::make_shared<ThreadSafeCallback>(info[3].As<Napi::Function>());

    // Pass callback to other thread
    std::thread([callback, url, width, height]
    { 
      Mat frame;

      VideoCapture capture(url);

      if (!capture.isOpened()) {
          cerr << "ERROR! Unable to open camera\n";
          return;
      }

      char *pixelData = (char *) malloc( ( (width * height * 6) + 1 ) * sizeof(char) ); 

      for (;;)
      {
          // wait for a new frame from camera and store it into 'frame'
          capture.read(frame);
          // check if we succeeded
          if (frame.empty()) {
              cerr << "ERROR! blank frame grabbed\n";
              break;
          }

          imageStringfier(pixelData, frame, width, height);

          callback->call([pixelData](Napi::Env env, std::vector<napi_value>& args)
          {
              args = { env.Undefined(), Napi::String::New(env, pixelData) };
          });

      }
    
    }).detach();
}

char* imageStringfier(char* output, Mat frame, int width, int height) {

    resize(frame, frame, Size(width, height));

    const char* hexDigits = "0123456789abcdef";

    size_t idx = 0;

    for (int i = 0; i < frame.rows; ++i) {
        Vec3b* buf = frame.ptr<Vec3b>(i, 0);
        const Vec3b* buf_end = buf + frame.cols;
        while (buf != buf_end) {
            for (int k = 0; k < 3; ++k) {
                uchar ch = (*buf)[k];
                output[idx++] = hexDigits[(ch >> 4) & 0x0F];
                output[idx++] = hexDigits[ch & 0x0F];
            }
            ++buf;
        }
    }

    output[idx++] = '\0';

    return output;
}

Napi::Object Init( Napi::Env env, Napi::Object exports )
{
  exports.Set(Napi::String::New(env, "startProcessing"), Napi::Function::New(env, startProcessing));
  return exports;
}

NODE_API_MODULE(module, Init)
