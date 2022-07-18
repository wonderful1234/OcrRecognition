#pragma once
#include <OcrLiteOnnx/OcrLiteCApi.h>
#include <QString>
#define DET_MODEL "models/dbnet.onnx"
#define CLS_MODEL "models/angle_net.onnx"
#define REC_MODEL "models/crnn_lite_lstm.onnx"
#define KEY_FILE  "models/keys.txt"
#define THREAD_NUM 3
class OcrManage
{
public:
	static OcrManage* getInstance();
	QString getTexts(const QString& imagePath);
	~OcrManage();

private:
	static OcrManage* instance;
	OcrManage();
	OCR_HANDLE handle;
};
