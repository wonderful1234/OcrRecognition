#include "OcrManage.h"
#include <QFileInfo>
#include <mutex>
std::unique_ptr<OcrManage>OcrManage::m_instance;
OcrManage::OcrManage()
{
	handle= OcrInit(DET_MODEL, CLS_MODEL, REC_MODEL, KEY_FILE, THREAD_NUM);
}

OcrManage& OcrManage::getInstance()
{
	static std::once_flag s_flag;
	std::call_once(s_flag, [&]() {
		m_instance.reset(new OcrManage);
		});
	return *m_instance;
}
QString OcrManage::getTexts(const QString& imagePath)
{
	QString text = "";
	QFileInfo info(imagePath);
	if (info.exists())
	{
		std::string path = info.absolutePath().toLocal8Bit().data();
		path += "\\";
		std::string name = info.fileName().toLocal8Bit().data();
		if (handle)
		{
			OCR_PARAM param = { 0 };
			OCR_BOOL bRet = OcrDetect(handle, path.c_str(), name.c_str(), &param);
			if (bRet)
			{
				int nLen = OcrGetLen(handle);
				if (nLen > 0)
				{
					char* szInfo = new char[nLen];
					if (szInfo)
					{
						if (OcrGetResult(handle, szInfo, nLen))
						{
							text = QString::fromUtf8(szInfo);
						}
					}
				}
			}
		}
	}
	return text;
	
}
void OcrManage::destoryOcr()
{
	if(handle)
		OcrDestroy(handle);
}

OcrManage::~OcrManage()
{
}
