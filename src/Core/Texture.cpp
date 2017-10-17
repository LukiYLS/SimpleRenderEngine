#include "Texture.h"


namespace Core {



	void Texture::setFiltering(int magnification, int minification)
	{
		glBindSampler(0, _sampler);
		
		if (magnification == TEXTURE_FILTER_MAG_NEAREST)
			glSamplerParameteri(_sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		else if (magnification == TEXTURE_FILTER_MAG_BILINEAR)
			glSamplerParameteri(_sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		if (minification == TEXTURE_FILTER_MIN_NEAREST)
			glSamplerParameteri(_sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		else if (minification == TEXTURE_FILTER_MIN_BILINEAR)
			glSamplerParameteri(_sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		else if (minification == TEXTURE_FILTER_MIN_NEAREST_MIPMAP)
			glSamplerParameteri(_sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		else if (minification == TEXTURE_FILTER_MIN_BILINEAR_MIPMAP)
			glSamplerParameteri(_sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		else if (minification == TEXTURE_FILTER_MIN_TRILINEAR)
			glSamplerParameteri(_sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}

	void Texture::bindTexture(int unit)
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		switch (_type)
		{
		case Core::TEX_TYPE_1D:
			glBindTexture(GL_TEXTURE_1D, _textureID);
			break;
		case Core::TEX_TYPE_2D:
			glBindTexture(GL_TEXTURE_2D, _textureID);
			break;
		case Core::TEX_TYPE_3D:
			glBindTexture(GL_TEXTURE_3D, _textureID);
			break;
		case Core::TEX_TYPE_CUBE_MAP:
			glBindTexture(GL_TEXTURE_CUBE_MAP, _textureID);
			break;
		case Core::TEX_TYPE_2D_ARRAY:
			glBindTexture(GL_TEXTURE_2D_ARRAY, _textureID);
			break;
		case Core::TEX_TYPE_2D_RECT:
			glBindTexture(GL_TEXTURE_2D_ARRAY_EXT, _textureID);
			break;
		default:
			break;
		}
		glBindSampler(unit, _sampler);
	}

}