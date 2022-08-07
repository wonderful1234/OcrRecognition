#pragma once
#include <OcrLiteOnnx/OcrLiteCApi.h>
#include <QString>
#include <memory>
#define DET_MODEL "models/dbnet.onnx"
#define CLS_MODEL "models/angle_net.onnx"
#define REC_MODEL "models/crnn_lite_lstm.onnx"
#define KEY_FILE  "models/keys.txt"
#define THREAD_NUM 3
class OcrManage
{
public:
	static OcrManage& getInstance();
	QString getTexts(const QString& imagePath);
	void destoryOcr();
	~OcrManage();

private:
	static std::unique_ptr<OcrManage>m_instance;
	OcrManage();
	OcrManage(const OcrManage&) = delete;
	OcrManage& operator=(const OcrManage&) = delete;
	OCR_HANDLE handle;
};
