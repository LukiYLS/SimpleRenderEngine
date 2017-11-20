#include "ColorValue.h"

namespace SRE {

	float ColorValue::r() const
	{
		return _r;
	}

	float ColorValue::g() const
	{
		return _g;
	}

	float ColorValue::b() const
	{
		return _b;
	}

	float ColorValue::a() const
	{
		return _a;
	}
	unsigned int ColorValue::asRGBA(void) const
	{
		unsigned char val8;
		unsigned int val32 = 0;

		// Convert to 32bit pattern
		// (RGBA = 8888)

		// Red
		val8 = static_cast<unsigned char>(_r * 255);
		val32 = val8 << 24;

		// Green
		val8 = static_cast<unsigned char>(_g * 255);
		val32 += val8 << 16;

		// Blue
		val8 = static_cast<unsigned char>(_b * 255);
		val32 += val8 << 8;

		// Alpha
		val8 = static_cast<unsigned char>(_a * 255);
		val32 += val8;

		return val32;
	}

	unsigned int ColorValue::asARGB(void) const
	{
		unsigned char val8;
		unsigned int val32 = 0;

		// Convert to 32bit pattern
		// (ARGB = 8888)

		// Alpha
		val8 = static_cast<unsigned char>(_a * 255);
		val32 = val8 << 24;

		// Red
		val8 = static_cast<unsigned char>(_r * 255);
		val32 += val8 << 16;

		// Green
		val8 = static_cast<unsigned char>(_g * 255);
		val32 += val8 << 8;

		// Blue
		val8 = static_cast<unsigned char>(_b * 255);
		val32 += val8;


		return val32;
	}

	unsigned int ColorValue::asBGRA(void) const
	{
		unsigned char val8;
		unsigned int val32 = 0;

		// Convert to 32bit pattern
		// (ARGB = 8888)

		// Blue
		val8 = static_cast<unsigned char>(_b * 255);
		val32 = val8 << 24;

		// Green
		val8 = static_cast<unsigned char>(_g * 255);
		val32 += val8 << 16;

		// Red
		val8 = static_cast<unsigned char>(_r * 255);
		val32 += val8 << 8;

		// Alpha
		val8 = static_cast<unsigned char>(_a * 255);
		val32 += val8;
		return val32;
	}

	unsigned int ColorValue::asABGR(void) const
	{
		unsigned char val8;
		unsigned int val32 = 0;

		// Convert to 32bit pattern
		// (ABRG = 8888)

		// Alpha
		val8 = static_cast<unsigned char>(_a * 255);
		val32 = val8 << 24;

		// Blue
		val8 = static_cast<unsigned char>(_b * 255);
		val32 += val8 << 16;

		// Green
		val8 = static_cast<unsigned char>(_g * 255);
		val32 += val8 << 8;

		// Red
		val8 = static_cast<unsigned char>(_r * 255);
		val32 += val8;


		return val32;
	}

	void ColorValue::setRGBA(const unsigned int val)
	{

		unsigned int val32 = val;

		// Convert from 32bit pattern
		// (RGBA = 8888)

		// Red
		_r = ((val32 >> 24) & 0xFF) / 255.0f;

		// Green
		_g = ((val32 >> 16) & 0xFF) / 255.0f;

		// Blue
		_b = ((val32 >> 8) & 0xFF) / 255.0f;

		// Alpha
		_a = (val32 & 0xFF) / 255.0f;
	}

	void ColorValue::setARGB(const unsigned int val)
	{
		unsigned int val32 = val;

		// Convert from 32bit pattern
		// (ARGB = 8888)

		// Alpha
		_a = ((val32 >> 24) & 0xFF) / 255.0f;

		// Red
		_r = ((val32 >> 16) & 0xFF) / 255.0f;

		// Green
		_g = ((val32 >> 8) & 0xFF) / 255.0f;

		// Blue
		_b = (val32 & 0xFF) / 255.0f;
	}

	void ColorValue::setBGRA(const unsigned int val)
	{
		unsigned int val32 = val;

		// Convert from 32bit pattern
		// (ARGB = 8888)

		// Blue
		_b = ((val32 >> 24) & 0xFF) / 255.0f;

		// Green
		_g = ((val32 >> 16) & 0xFF) / 255.0f;

		// Red
		_r = ((val32 >> 8) & 0xFF) / 255.0f;

		// Alpha
		_a = (val32 & 0xFF) / 255.0f;
	}

	void ColorValue::setABGR(const unsigned int val)
	{
		unsigned int val32 = val;

		// Convert from 32bit pattern
		// (ABGR = 8888)

		// Alpha
		_a = ((val32 >> 24) & 0xFF) / 255.0f;

		// Blue
		_b = ((val32 >> 16) & 0xFF) / 255.0f;

		// Green
		_g = ((val32 >> 8) & 0xFF) / 255.0f;

		// Red
		_r = (val32 & 0xFF) / 255.0f;
	}

	void ColorValue::saturate(void)
	{
		if (_r < 0)
			_r = 0;
		else if (_r > 1)
			_r = 1;

		if (_g < 0)
			_g = 0;
		else if (_g > 1)
			_g = 1;

		if (_b < 0)
			_b = 0;
		else if (_b > 1)
			_b = 1;

		if (_a < 0)
			_a = 0;
		else if (_a > 1)
			_a = 1;
	}

	ColorValue::ptr ColorValue::saturateCopy(void) const
	{
		ColorValue *ret = new ColorValue();
		ret = const_cast<ColorValue*>(this);
		ret->saturate();
		return ColorValue::ptr(ret);
	}

	void ColorValue::HSB(double hue, double saturation, double brightness)
	{

	}

	void ColorValue::HSB(double* hue, double* saturation, double* brightness) const
	{

	}
}