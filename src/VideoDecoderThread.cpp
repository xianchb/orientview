// Copyright � 2014 Mikko Ronkainen <firstname@mikkoronkainen.com>
// License: GPLv3, see the LICENSE file.

#include "VideoDecoderThread.h"
#include "VideoDecoder.h"

using namespace OrientView;

VideoDecoderThread::VideoDecoderThread()
{
}

bool VideoDecoderThread::initialize(VideoDecoder* videoDecoder)
{
	qDebug("Initializing VideoDecoderThread");

	this->videoDecoder = videoDecoder;

	frameReadSemaphore = new QSemaphore();
	frameAvailableSemaphore = new QSemaphore();
	decodedFrameData = FrameData();
	decodedFrameDataGrayscale = FrameData();

	return true;
}

void VideoDecoderThread::shutdown()
{
	qDebug("Shutting down VideoDecoderThread");

	if (frameReadSemaphore != nullptr)
	{
		delete frameReadSemaphore;
		frameReadSemaphore = nullptr;
	}

	if (frameAvailableSemaphore != nullptr)
	{
		delete frameAvailableSemaphore;
		frameAvailableSemaphore = nullptr;
	}
}

void VideoDecoderThread::run()
{
	frameReadSemaphore->release(1);

	while (!isInterruptionRequested())
	{
		while (!frameReadSemaphore->tryAcquire(1, 20) && !isInterruptionRequested()) {}

		if (isInterruptionRequested())
			break;

		if (videoDecoder->getNextFrame(&decodedFrameData, &decodedFrameDataGrayscale))
			frameAvailableSemaphore->release(1);
		else
			QThread::msleep(20);
	}
}

bool VideoDecoderThread::getNextFrame(FrameData* frameData, FrameData* frameDataGrayscale)
{
	if (frameAvailableSemaphore->tryAcquire(1, 20))
	{
		frameData->data = decodedFrameData.data;
		frameData->dataLength = decodedFrameData.dataLength;
		frameData->rowLength = decodedFrameData.rowLength;
		frameData->width = decodedFrameData.width;
		frameData->height = decodedFrameData.height;
		frameData->duration = decodedFrameData.duration;
		frameData->number = decodedFrameData.number;

		frameDataGrayscale->data = decodedFrameDataGrayscale.data;
		frameDataGrayscale->dataLength = decodedFrameDataGrayscale.dataLength;
		frameDataGrayscale->rowLength = decodedFrameDataGrayscale.rowLength;
		frameDataGrayscale->width = decodedFrameDataGrayscale.width;
		frameDataGrayscale->height = decodedFrameDataGrayscale.height;
		frameDataGrayscale->duration = decodedFrameDataGrayscale.duration;
		frameDataGrayscale->number = decodedFrameDataGrayscale.number;

		return true;
	}
	else
		return false;
}

void VideoDecoderThread::signalFrameRead()
{
	frameReadSemaphore->release(1);
}
