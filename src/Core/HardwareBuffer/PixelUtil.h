#pragma once
#include <glew\glew.h>
#include "PixelBox.h"
#include "ColorValue.h"
namespace SRE {

	class PixelUtil {
	public:
		static bool isCompressed(PixelFormat pixel_format);
		static unsigned int getFlags(PixelFormat format);
		static unsigned int getMemorySize(unsigned int width, unsigned int height, unsigned int depth, PixelFormat format);
		static size_t PixelUtil::getNumElemBytes(PixelFormat format);

		static void packColor(ColorValue::ptr colour, const PixelFormat pf, void* dest);
		static void packColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a_, PixelFormat pf, void *dest);
		static void packColor(float r, float g, float b, float a, PixelFormat pf, void *dest);
		static ColorValue::ptr unpackColor(PixelFormat pf, const void* src);
		static void unpackColor(unsigned char *r, unsigned char *g, unsigned char *b, unsigned char *a, PixelFormat pf, const void* src);
		static void unpackColor(float *r, float *g, float *b, float *a, PixelFormat pf, const void* src);
									
		static GLenum getGLOriginFormat(PixelFormat format);
		static GLenum getGLOriginDataType(PixelFormat format);
		static GLenum getGLInternalFormat(PixelFormat format, bool hwGamma = false);
		static GLenum getClosestGLInternalFormat(PixelFormat format, bool hwGamma = false);
		static PixelFormat getClosestOGREFormat(GLenum fmt);
		static size_t getMaxMipmaps(unsigned int width, unsigned int height, unsigned int depth, PixelFormat format);
		static unsigned int optionalPO2(unsigned int value);
	};
}