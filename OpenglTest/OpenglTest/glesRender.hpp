#pragma once

#include <Windows.h>
#include "glew/glew.h"
#include "glew/wglew.h"
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")

#include "freeImage/FreeImage.h"

#include "CELLMath.hpp"
#include "CELLShader.hpp"

class CglesRender
{
private:
	HGLRC			m_hRC = nullptr;			//OpenGL上下文
	HDC				m_hDC = nullptr;			//绘制设备上下文
	int				m_nFormat = 0;

	//! 窗口句柄
	HWND        _hWnd;
	//! 窗口的高度
	int         _width;
	//! 窗口的宽度
	int         _height;

	//! 增加shader
	PROGRAM_P2_UV_AC4	_shader;
	unsigned			_textureId;

	BYTE*	_videoBuffer;
	int		_videoWidth;
	int		_videoHeight;

	typedef struct _tagLoadingBuffer
	{
		char	pData[1024 * 1024 * 4];
		int		nWidth;
		int		nHeight;
	}LoadingBuffer;
	LoadingBuffer m_pauseCountdownBuffer[4];
	bool m_bShowFullscreenTips = false;
	int m_nPauseCountdown = -1;

public:
	CglesRender() { _videoBuffer = NULL; }
	~CglesRender() { if (_videoBuffer != NULL) delete[] _videoBuffer; }

	LoadingBuffer m_tipsBuffer;
	LoadingBuffer m_bmpBk;

	void setHwnd(HWND hwnd)
	{
		_hWnd = hwnd;
	}

	void setWH(int width, int height)
	{
		_width = width;
		_height = height;
	}

	void setFullscreenTips(bool bShowFullscreenTips)
	{
		m_bShowFullscreenTips = bShowFullscreenTips;
	}

	void setPauseCountdown(int nPauseCountdown)
	{
		m_nPauseCountdown = nPauseCountdown;
	}

	void setSynchronized2Video(bool bSync)
	{
		if (wglSwapIntervalEXT)
		{
			if (bSync)
				wglSwapIntervalEXT(1);
			else
				wglSwapIntervalEXT(0);
		}
	}
	/**
	*   初始化 OpenGLES2.0
	*/
	bool    initOpenGLES20()
	{
		m_hDC = GetDC(_hWnd);
		//PIXELFORMATDESCRIPTOR pfd;
		PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA,
			32,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			24,
			8,
			0,
			PFD_OVERLAY_PLANE/*PFD_MAIN_PLANE*/,
			0,
			0,
			0,
			0
		};
		//memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
		//pfd.nVersion = 1;
		//pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		//pfd.cColorBits = 32;
		//pfd.cDepthBits = 24;
		//pfd.cStencilBits = 8;
		//pfd.iPixelType = PFD_TYPE_RGBA;
		//pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		int pixelFormat = 0;
		if (m_nFormat == 0)
		{
			pixelFormat = ChoosePixelFormat(m_hDC, &pfd);
		}
		else
		{
			pixelFormat = m_nFormat;
		}
		if (!SetPixelFormat(m_hDC, pixelFormat, &pfd))
		{
			return false;
		}
		m_hRC = wglCreateContext(m_hDC);
		if (!wglMakeCurrent(m_hDC, m_hRC))
		{
			return false;
		}
		glewInit();

		return  true;

	}
	/**
	*   销毁OpenGLES2.0
	*/
	void    destroyOpenGLES20()
	{
		if (m_hRC != NULL)
		{
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(m_hRC);
			m_hRC = NULL;
		}
		if (m_hDC != NULL)
		{
			ReleaseDC(_hWnd, m_hDC);
			m_hDC = NULL;
		}
	}

	/**
	*   绘制函数
	*/
	void    render(bool bUniformScale)
	{
		if (_width == 0 || _height == 0)
			return;
		//if (bTest)
		//{
		//	glClearColor(1,0,0,1);
		//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//	glViewport(0,0,_width,_height);
		//} 
		//else
		{
			struct  Vertex
			{
				CELL::float2    pos;
				CELL::float2    uv;
				CELL::Rgba4Byte color;
			};
			//! 清空缓冲区
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
			//! 视口，在Windows窗口指定的位置和大小上绘制OpenGL内容
			glViewport(0, 0, _width, _height);

			//! 创建一个投影矩阵

			CELL::matrix4   screenProj = CELL::ortho<float>(0, float(_width), float(_height), 0, -100.0f, 100);

			float ratioWidth = (float)_width / (float)_videoWidth;
			float ratioHeight = (float)_height / (float)_videoHeight;
			float ratioFinal = ratioWidth < ratioHeight ? ratioWidth : ratioHeight;
			_shader.begin();
			{
				float   x = 0;
				float   y = 0;
				float   w = 0;
				float   h = 0;

				if (bUniformScale)//等比缩放
				{
					if (ratioWidth < ratioHeight)
					{
						x = 0;
						y = (_height - _videoHeight*ratioFinal - 1) / 2;
						w = (float)_width;
						h = (float)_videoHeight*ratioFinal;
					}
					else
					{
						x = (_width - _videoWidth*ratioFinal) / 2;
						y = 0;
						w = (float)_videoWidth*ratioFinal;
						h = (float)_height;
					}
				}
				else
				{
					x = 0;
					y = 0;
					w = (float)_width;
					h = (float)_height;
				}
				/*
				CELL::float2(0,0),
				CELL::float2(1,0),
				CELL::float2(0,1),
				CELL::float2(1,1),
				*/

				/*
				CELL::float2(0,1),
				CELL::float2(1,1),
				CELL::float2(0,0),
				CELL::float2(1,0),
				*/
				Vertex  vertex[] =
				{
					CELL::float2(x,y),          CELL::float2(0,0),      CELL::Rgba4Byte(255,255,255,255),
					CELL::float2(x + w,y),      CELL::float2(1,0),      CELL::Rgba4Byte(255,255,255,255),
					CELL::float2(x,y + h),      CELL::float2(0,1),      CELL::Rgba4Byte(255,255,255,255),
					CELL::float2(x + w, y + h), CELL::float2(1,1),      CELL::Rgba4Byte(255,255,255,255),
				};

				CELL::matrix4       matMVP = screenProj;

				//updateVideo(0, 0, 0);
				//if (bTest)
				//{
				//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, "");
				//} 
				//else
				//{
				//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _videoWidth, _videoHeight, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, _videoBuffer);
				//}
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _videoWidth, _videoHeight, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, _videoBuffer);

				//glBindTexture(GL_TEXTURE_2D,_textureId);

				glUniformMatrix4fv(_shader._MVP, 1, false, matMVP.data());
				glUniform1i(_shader._texture, 0);
				glVertexAttribPointer(_shader._position, 2, GL_FLOAT, false, sizeof(Vertex), vertex);
				glVertexAttribPointer(_shader._uv, 2, GL_FLOAT, false, sizeof(Vertex), &vertex[0].uv);
				glVertexAttribPointer(_shader._color, 4, GL_UNSIGNED_BYTE, true, sizeof(Vertex), &vertex[0].color);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

				if (m_bShowFullscreenTips)
				{
					Vertex  vertex1[] =
					{
						CELL::float2((_width - m_tipsBuffer.nWidth) / 2,(_height - m_tipsBuffer.nHeight) / 2),      CELL::float2(0,0),      CELL::Rgba4Byte(255,255,255,255),
						CELL::float2((_width + m_tipsBuffer.nWidth) / 2,(_height - m_tipsBuffer.nHeight) / 2),      CELL::float2(1,0),      CELL::Rgba4Byte(255,255,255,255),
						CELL::float2((_width - m_tipsBuffer.nWidth) / 2,(_height + m_tipsBuffer.nHeight) / 2),      CELL::float2(0,1),      CELL::Rgba4Byte(255,255,255,255),
						CELL::float2((_width + m_tipsBuffer.nWidth) / 2,(_height + m_tipsBuffer.nHeight) / 2),		CELL::float2(1,1),      CELL::Rgba4Byte(255,255,255,255),
					};
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_tipsBuffer.nWidth, m_tipsBuffer.nHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_tipsBuffer.pData);
					glVertexAttribPointer(_shader._position, 2, GL_FLOAT, false, sizeof(Vertex), vertex1);
					glVertexAttribPointer(_shader._uv, 2, GL_FLOAT, false, sizeof(Vertex), &vertex1[0].uv);
					glVertexAttribPointer(_shader._color, 4, GL_UNSIGNED_BYTE, true, sizeof(Vertex), &vertex1[0].color);
					glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				}

				/*if (m_nPauseCountdown >= 0)
				{
					Vertex  vertex1[] =
					{
						CELL::float2(x,y),          CELL::float2(0,0),      CELL::Rgba4Byte(255,255,255,255),
						CELL::float2(x + w,y),      CELL::float2(1,0),      CELL::Rgba4Byte(255,255,255,255),
						CELL::float2(x,y + h),      CELL::float2(0,1),      CELL::Rgba4Byte(255,255,255,255),
						CELL::float2(x + w, y + h), CELL::float2(1,1),      CELL::Rgba4Byte(255,255,255,255),
					};
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_pauseCountdownBuffer[0].nWidth, m_pauseCountdownBuffer[0].nHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pauseCountdownBuffer[0].pData);
					glVertexAttribPointer(_shader._position, 2, GL_FLOAT, false, sizeof(Vertex), vertex1);
					glVertexAttribPointer(_shader._uv, 2, GL_FLOAT, false, sizeof(Vertex), &vertex1[0].uv);
					glVertexAttribPointer(_shader._color, 4, GL_UNSIGNED_BYTE, true, sizeof(Vertex), &vertex1[0].color);
					glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

					switch (m_nPauseCountdown)
					{
					case 1:
					case 2:
					case 3:
					{
						Vertex  vertex1[] =
						{
							CELL::float2((_width - m_pauseCountdownBuffer[m_nPauseCountdown].nWidth) / 2,(_height - m_pauseCountdownBuffer[m_nPauseCountdown].nHeight) / 2),      CELL::float2(0,0),      CELL::Rgba4Byte(255,255,255,255),
							CELL::float2((_width + m_pauseCountdownBuffer[m_nPauseCountdown].nWidth) / 2,(_height - m_pauseCountdownBuffer[m_nPauseCountdown].nHeight) / 2),      CELL::float2(1,0),      CELL::Rgba4Byte(255,255,255,255),
							CELL::float2((_width - m_pauseCountdownBuffer[m_nPauseCountdown].nWidth) / 2,(_height + m_pauseCountdownBuffer[m_nPauseCountdown].nHeight) / 2),      CELL::float2(0,1),      CELL::Rgba4Byte(255,255,255,255),
							CELL::float2((_width + m_pauseCountdownBuffer[m_nPauseCountdown].nWidth) / 2,(_height + m_pauseCountdownBuffer[m_nPauseCountdown].nHeight) / 2),		CELL::float2(1,1),      CELL::Rgba4Byte(255,255,255,255),
						};
						glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_pauseCountdownBuffer[m_nPauseCountdown].nWidth, m_pauseCountdownBuffer[m_nPauseCountdown].nHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pauseCountdownBuffer[m_nPauseCountdown].pData);
						glVertexAttribPointer(_shader._position, 2, GL_FLOAT, false, sizeof(Vertex), vertex1);
						glVertexAttribPointer(_shader._uv, 2, GL_FLOAT, false, sizeof(Vertex), &vertex1[0].uv);
						glVertexAttribPointer(_shader._color, 4, GL_UNSIGNED_BYTE, true, sizeof(Vertex), &vertex1[0].color);
						glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
					}
					break;
					}
				}*/
			}
			_shader.end();
		}

	}

	void	swapBuffer()
	{
		SwapBuffers(m_hDC);
	}

	//bool SaveImage(char *imgpath)
	//{
	//	unsigned char *mpixels = new unsigned char[800 * 600 * 4];
	//	//glReadBuffer(GL_FRONT);
	//	//glReadPixels(0, 0, 800, 600, GL_RGBA, GL_UNSIGNED_BYTE, mpixels);
	//	//glReadBuffer(GL_BACK);
	//	for (int i = 0; i<(int)800 * 600 * 4; i += 4)
	//	{

	//		mpixels[i] ^= mpixels[i + 2] ^= mpixels[i] ^= mpixels[i + 2];
	//	}
	//	FIBITMAP* bitmap = FreeImage_Allocate(800, 600, 32, 8, 8, 8);

	//	for (unsigned y = 0; y<FreeImage_GetHeight(bitmap); y++)
	//	{
	//		BYTE *bits = FreeImage_GetScanLine(bitmap, y);
	//		for (unsigned x = 0; x<FreeImage_GetWidth(bitmap); x++)
	//		{
	//			bits[0] = mpixels[(y * 800 + x) * 4 + 0];
	//			bits[1] = mpixels[(y * 800 + x) * 4 + 1];
	//			bits[2] = mpixels[(y * 800 + x) * 4 + 2];
	//			bits[3] = 255;
	//			bits += 4;

	//		}

	//	}
	//	bool bSuccess = FreeImage_Save(FIF_PNG, bitmap, imgpath, PNG_DEFAULT) == TRUE;
	//	FreeImage_Unload(bitmap);

	//	return bSuccess;

	//}

	//unsigned    loadTexture(const char* fileName)
	//{
	//	unsigned    textureId = 0;
	//	//1 获取图片格式
	//	FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(fileName, 0);

	//	//2 加载图片
	//	FIBITMAP    *dib = FreeImage_Load(fifmt, fileName, 0);

	//	//3 转化为rgb 24色
	//	//dib     =   FreeImage_ConvertTo24Bits(dib);
	//	dib = FreeImage_ConvertTo16Bits565(dib);



	//	//4 获取数据指针
	//	BYTE    *pixels = (BYTE*)FreeImage_GetBits(dib);

	//	int     width = FreeImage_GetWidth(dib);
	//	int     height = FreeImage_GetHeight(dib);

	//	//for (size_t i = 0 ;i < width * height * 3 ; i+=3 )
	//	//{
	//	//	BYTE temp       =   pixels[i];
	//	//	pixels[i]       =   pixels[i + 2];
	//	//	pixels[i + 2]   =   temp;
	//	//}

	//	/**
	//	*   产生一个纹理Id,可以认为是纹理句柄，后面的操作将书用这个纹理id
	//	*/
	//	glGenTextures(1, &textureId);

	//	/**
	//	*   使用这个纹理id,或者叫绑定(关联)
	//	*/
	//	glBindTexture(GL_TEXTURE_2D, textureId);
	//	/**
	//	*   指定纹理的放大,缩小滤波，使用线性方式，即当图片放大的时候插值方式
	//	*/
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//	/**
	//	*   将图片的rgb数据上传给opengl.
	//	*/
	//	glTexImage2D(
	//		GL_TEXTURE_2D,      //! 指定是二维图片
	//		0,                  //! 指定为第一级别，纹理可以做mipmap,即lod,离近的就采用级别大的，远则使用较小的纹理
	//		GL_RGB,             //! 纹理的使用的存储格式
	//		width,              //! 宽度，老一点的显卡，不支持不规则的纹理，即宽度和高度不是2^n。
	//		height,             //! 宽度，老一点的显卡，不支持不规则的纹理，即宽度和高度不是2^n。
	//		0,                  //! 是否的边
	//		GL_RGB,             //! 数据的格式，bmp中，windows,操作系统中存储的数据是bgr格式
	//		GL_UNSIGNED_SHORT_5_6_5,   //! 数据是8bit数据
	//		pixels
	//	);
	//	/**
	//	*   释放内存
	//	*/
	//	FreeImage_Unload(dib);

	//	return  textureId;
	//}

	void    loadVideo(const char* fileName)
	{
		FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(fileName, 0);
		FIBITMAP    *dib = FreeImage_Load(fifmt, fileName, 0);
		dib = FreeImage_ConvertTo16Bits565(dib);
		BYTE    *pixels = (BYTE*)FreeImage_GetBits(dib);

		int     width = FreeImage_GetWidth(dib);
		int     height = FreeImage_GetHeight(dib);

		_videoWidth = width;
		_videoHeight = height;
		_videoBuffer = new BYTE[width*height * 2];
		memcpy(_videoBuffer, pixels, width*height * 2);

		FreeImage_Unload(dib);
	}

	//static void    loadImgBuffer(const char* fileName, void* pPixelBuffer, int& width, int& height)
	//{
	//	FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(fileName, 0);
	//	FIBITMAP    *dib = FreeImage_Load(fifmt, fileName, 0);
	//	FreeImage_FlipVertical(dib);//旋转
	//	dib = FreeImage_ConvertTo16Bits565(dib);
	//	BYTE    *pixels = (BYTE*)FreeImage_GetBits(dib);

	//	width = FreeImage_GetWidth(dib);
	//	height = FreeImage_GetHeight(dib);

	//	memcpy(pPixelBuffer, pixels, width*height * 2);

	//	FreeImage_Unload(dib);
	//}

	void setupVideoTexture(unsigned width, unsigned height, BYTE *video)
	{
		unsigned    textureId = 0;
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _videoWidth, _videoWidth, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, _videoBuffer);
		_textureId = textureId;
	}

	//void updateVideo(unsigned width, unsigned height, BYTE *video, bool bTest)
	//{
	//	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _videoWidth, _videoWidth, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, _videoBuffer);
	//	glBindTexture(GL_TEXTURE_2D, _textureId);
	//	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _videoWidth, _videoWidth, GL_RGB, GL_UNSIGNED_BYTE, _videoBuffer);//效率更高
	//}


	void initVideoTexture()
	{
		unsigned    textureId = 0;
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		_textureId = textureId;
	}

	void initTipsBuffer(const char* filePathName)
	{
		FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(filePathName, 0);
		FIBITMAP    *dib = FreeImage_Load(fifmt, filePathName, 0);
		FreeImage_FlipVertical(dib);//旋转
		BYTE    *pixels = (BYTE*)FreeImage_GetBits(dib);

		int width = FreeImage_GetWidth(dib);
		int height = FreeImage_GetHeight(dib);

		dib = FreeImage_ConvertTo32Bits(dib);
		for (int i = 0; i < width * height * 4; i += 4)
		{
			BYTE temp = pixels[i];
			pixels[i] = pixels[i + 2];
			pixels[i + 2] = temp;
		}

		memcpy(m_tipsBuffer.pData, pixels, width*height * 4);
		m_tipsBuffer.nWidth = width;
		m_tipsBuffer.nHeight = height;

		FreeImage_Unload(dib);
	}

	void initBMP(const char* filePathName)
	{
		FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(filePathName, 0);
		FIBITMAP    *dib = FreeImage_Load(fifmt, filePathName, 0);
		FreeImage_FlipVertical(dib);//旋转
		dib = FreeImage_ConvertTo16Bits565(dib);
		BYTE    *pixels = (BYTE*)FreeImage_GetBits(dib);

		int width = FreeImage_GetWidth(dib);
		int height = FreeImage_GetHeight(dib);

		memcpy(m_bmpBk.pData, pixels, width*height * 2);
		m_bmpBk.nWidth = width;
		m_bmpBk.nHeight = height;

		FreeImage_Unload(dib);
	}

	//void initCountDownBuffer(const char* filePathName, int nIndex)
	//{
	//	FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(filePathName, 0);
	//	FIBITMAP    *dib = FreeImage_Load(fifmt, filePathName, 0);
	//	FreeImage_FlipVertical(dib);//旋转
	//	BYTE    *pixels = (BYTE*)FreeImage_GetBits(dib);

	//	int width = FreeImage_GetWidth(dib);
	//	int height = FreeImage_GetHeight(dib);

	//	dib = FreeImage_ConvertTo32Bits(dib);
	//	for (int i = 0; i < width * height * 4; i += 4)
	//	{
	//		BYTE temp = pixels[i];
	//		pixels[i] = pixels[i + 2];
	//		pixels[i + 2] = temp;
	//	}

	//	LoadingBuffer* pBuffer = &(m_pauseCountdownBuffer[nIndex]);
	//	memcpy(pBuffer->pData, pixels, width*height * 4);
	//	pBuffer->nWidth = width;
	//	pBuffer->nHeight = height;

	//	FreeImage_Unload(dib);
	//}

	void loopVideo(int width, int height, BYTE *video)
	{
		_videoWidth = width;
		_videoHeight = height;
		if (_videoBuffer == NULL)
		{
			//_videoBuffer = new BYTE[width*height*2];
			_videoBuffer = new BYTE[1024 * 1024 * 2];
		}
		if (video)
			memcpy(_videoBuffer, video, width*height * 2);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, video);
	}

	void initShader()
	{
		_shader.initialize();
	}

	void setTextureId(unsigned id)
	{
		_textureId = id;
	}
};