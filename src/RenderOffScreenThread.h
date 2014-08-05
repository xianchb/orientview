// Copyright � 2014 Mikko Ronkainen <firstname@mikkoronkainen.com>
// License: GPLv3, see the LICENSE file.

#pragma once

#include <QThread>
#include <QSemaphore>
#include <QOpenGLFramebufferObject>

#include "FrameData.h"

namespace OrientView
{
	class MainWindow;
	class EncodeWindow;
	class VideoDecoder;
	class VideoDecoderThread;
	class VideoStabilizer;
	class Renderer;
	class VideoEncoder;
	struct Settings;

	class RenderOffScreenThread : public QThread
	{
		Q_OBJECT

	public:

		bool initialize(MainWindow* mainWindow, EncodeWindow* encodeWindow, VideoDecoder* videoDecoder, VideoDecoderThread* videoDecoderThread, VideoStabilizer* videoStabilizer, Renderer* renderer, VideoEncoder* videoEncoder, Settings* settings);
		void shutdown();

		bool tryGetNextFrame(FrameData* frameData, int timeout);
		void signalFrameRead();

	protected:

		void run();

	private:

		void readDataFromFramebuffer(QOpenGLFramebufferObject* sourceFbo);

		MainWindow* mainWindow = nullptr;
		EncodeWindow* encodeWindow = nullptr;
		VideoDecoder* videoDecoder = nullptr;
		VideoDecoderThread* videoDecoderThread = nullptr;
		VideoStabilizer* videoStabilizer = nullptr;
		Renderer* renderer = nullptr;
		VideoEncoder* videoEncoder = nullptr;

		QOpenGLFramebufferObject* mainFramebuffer = nullptr;
		QOpenGLFramebufferObject* secondaryFramebuffer = nullptr;

		QSemaphore* frameReadSemaphore = nullptr;
		QSemaphore* frameAvailableSemaphore = nullptr;

		int framebufferWidth = 0;
		int framebufferHeight = 0;

		FrameData renderedFrameData;
	};
}
