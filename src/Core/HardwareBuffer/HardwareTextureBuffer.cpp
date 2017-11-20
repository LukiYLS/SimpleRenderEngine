#include "HardwareTextureBuffer.h"
#include "PixelUtil.h"
namespace SRE {

	HardwareTextureBuffer::HardwareTextureBuffer(GLenum target, GLuint id, GLuint face, GLuint level, HardwareBuffer::Usage usage, bool software_mipmap)
		: _target(target), _faceTarget(0), _textureID(id), _face(face), _level(level),
		_softwareMipmap(software_mipmap)
	{
		GLint value;
		glBindTexture(_target, _textureID);

		_faceTarget = _target;
		if (target == GL_TEXTURE_CUBE_MAP)
			_faceTarget = GL_TEXTURE_CUBE_MAP_POSITIVE_X + face;


		glGetTexLevelParameteriv(_faceTarget, level, GL_TEXTURE_WIDTH, &value);
		_width = value;

		// Get height
		if (target == GL_TEXTURE_1D)
			value = 1;  // Height always 1 for 1D textures
		else
			glGetTexLevelParameteriv(_faceTarget, level, GL_TEXTURE_HEIGHT, &value);
		_height = value;

		// Get depth
		if (target != GL_TEXTURE_3D && target != GL_TEXTURE_2D_ARRAY_EXT)
			value = 1; // Depth always 1 for non-3D textures
		else
			glGetTexLevelParameteriv(_faceTarget, level, GL_TEXTURE_DEPTH, &value);
		_depth = value;

		glGetTexLevelParameteriv(_faceTarget, level, GL_TEXTURE_INTERNAL_FORMAT, &value);
		_internalFormat = value;
		//_format = GLPixelUtil::getClosestOGREFormat(value);

		// Default
		_rowPitch = _width;
		_slicePitch = _height*_width;
		//_sizeInBytes = PixelUtil::getMemorySize(mWidth, mHeight, mDepth, mFormat);

		_buffer = std::make_shared<PixelBox>(_width, _height, _depth, _format, 0);

	}

	void HardwareTextureBuffer::blitFromMemory(PixelBox::ptr src)
	{
		PixelBox *box = new PixelBox(_width, _height, _depth, _format, 0); //只是构造一个框
		blitFromMemory(src, (PixelBox::ptr)box);
	}

	void HardwareTextureBuffer::blitFromMemory(PixelBox::ptr src, PixelBox::ptr dst)
	{
		//PixelBox scaled;

		if (src->getWidth() != dst->getWidth() ||
			src->getHeight() != dst->getHeight() ||
			src->getDepth() != dst->getDepth())
		{
			//需要缩放
			// Scale to destination size.
			// This also does pixel format conversion if needed
			//allocateBuffer();
			//scaled = mBuffer.getSubVolume(dstBox);
			//Image::scale(src, scaled, Image::FILTER_BILINEAR);
		}
		else if (PixelUtil::getGLOriginFormat(src->getPixelFormat()) == 0)
		{
			// Extents match, but format is not accepted as valid source format for GL
			// do conversion in temporary buffer
			//allocateBuffer();
			//scaled = mBuffer.getSubVolume(dstBox);
			//PixelUtil::bulkPixelConversion(src, scaled);
		}
		else
		{
			//allocateBuffer();
			// No scaling or conversion needed
			//scaled = src;
		}

		upload(src, dst);
		//freeBuffer();
	}
	void HardwareTextureBuffer::download(PixelBox::ptr data)
	{
		if (PixelUtil::isCompressed(data->getPixelFormat()))
		{
			//if (data.format != mFormat || !data.isConsecutive())
			//	OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
			//		"Compressed images must be consecutive, in the source format",
			//		"GLTextureBuffer::download");
			// Data must be consecutive and at beginning of buffer as PixelStorei not allowed
			// for compressed formate
			glGetCompressedTexImageARB(_faceTarget, _level, data->getData());
		}
		else
		{
			if (data->getRowPitch() != data->getWidth())
				glPixelStorei(GL_PACK_ROW_LENGTH, data->getRowPitch());
			if (data->getTop() != 0 || data->getLeft() != 0)
				glPixelStorei(GL_PACK_SKIP_PIXELS, data->getLeft() + data->getRowPitch() * data->getTop());
			if (data->getWidth() * PixelUtil::getNumElemBytes(data->getPixelFormat()) & 3);
				glPixelStorei(GL_PACK_ALIGNMENT, 1);

			glGetTexImage(_faceTarget, _level, PixelUtil::getGLOriginFormat(data->getPixelFormat()), PixelUtil::getGLOriginDataType(data->getPixelFormat()), (GLvoid*)data->getData());
		}
	}
	void HardwareTextureBuffer::upload(PixelBox::ptr data, PixelBox::ptr dst)
	{
		if (PixelUtil::isCompressed(data->getPixelFormat()))
		{
			GLenum format = PixelUtil::getClosestGLInternalFormat(_format);
			switch (_target) {
			case GL_TEXTURE_1D:
				// some systems (e.g. old Apple) don't like compressed subimage calls
				// so prefer non-sub versions
				if (dst->getLeft() == 0)
				{
					glCompressedTexImage1DARB(GL_TEXTURE_1D, _level,
						format,
						dst->getWidth(),
						0,
						data->getConsecutive(),
						data->getData());
				}
				else
				{
					glCompressedTexSubImage1DARB(GL_TEXTURE_1D, _level,
						dst->getLeft(),
						dst->getWidth(),
						format, data->getConsecutive(),
						data->getData());
				}
				break;
			case GL_TEXTURE_2D:
			case GL_TEXTURE_CUBE_MAP:
				// some systems (e.g. old Apple) don't like compressed subimage calls
				// so prefer non-sub versions
				if (dst->getLeft() == 0 && dst->getTop() == 0)
				{
					glCompressedTexImage2DARB(_faceTarget, _level,
						format,
						dst->getWidth(),
						dst->getHeight(),
						0,
						data->getConsecutive(),
						data->getData());
				}
				else
				{
					glCompressedTexSubImage2DARB(_faceTarget, _level,
						dst->getLeft(), dst->getTop(),
						dst->getWidth(), dst->getHeight(),
						format, data->getConsecutive(),
						data->getData());
				}
				break;
			case GL_TEXTURE_3D:
			case GL_TEXTURE_2D_ARRAY_EXT:
				// some systems (e.g. old Apple) don't like compressed subimage calls
				// so prefer non-sub versions
				if (dst->getLeft() == 0 && dst->getTop() == 0 && dst->getFront() == 0)
				{
					glCompressedTexImage3DARB(_target, _level,
						format,
						dst->getWidth(), dst->getHeight(),
						dst->getDepth(),
						0,
						data->getConsecutive(),
						data->getData());
				}
				else
				{
					glCompressedTexSubImage3DARB(_target, _level,
						dst->getLeft(), dst->getTop(), dst->getFront(),
						dst->getWidth(), dst->getHeight(),dst->getDepth(),
						format, data->getConsecutive(),
						data->getData());
				}
				break;
			}
		}
		else if (_softwareMipmap)
		{
			/*GLenum format = PixelUtil::getClosestGLInternalFormat(_format);
			if (data->getWidth() != data->getRowPitch())
				glPixelStorei(GL_UNPACK_ROW_LENGTH, data->getRowPitch());
			if (data->getHeight()*data->getWidth() != data->getSlicePitch())
				glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, (data->getSlicePitch() / data->getWidth()));
			if (data->getLeft() > 0 || data->getTop() > 0 || data->getFront() > 0)
				glPixelStorei(GL_UNPACK_SKIP_PIXELS, data->getLeft() + data->getRowPitch() * data->getTop() + data->getRowPitch() * data->getFront());
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			switch (_target)
			{

			}
			glTexSubImage2D(_target, 0,
				dst->getLeft(), dst->getTop(),
				dst->getWidth(), dst->getHeight(),
				PixelUtil::getGLOriginFormat(data->getPixelFormat), PixelUtil::getGLOriginDataType(data->getPixelFormat),
				data->getData());

			glGenerateMipmap(_target);*/

			/*switch (_target)
			{
			case GL_TEXTURE_1D:
				gluBuild1DMipmaps(
					GL_TEXTURE_1D, format,
					dst->getWidth(),
					PixelUtil::getGLOriginFormat(data->getPixelFormat), PixelUtil::getGLOriginDataType(data->getPixelFormat),
					data->getData());
				break;
			case GL_TEXTURE_2D:
			case GL_TEXTURE_CUBE_MAP:
				gluBuild2DMipmaps(
					_faceTarget,
					format, dst->getWidth(), dst->getHeight(),
					PixelUtil::getGLOriginFormat(data->getPixelFormat()), PixelUtil::getGLOriginDataType(data->getPixelFormat),
					data->getData());
				break;
			case GL_TEXTURE_3D:
			case GL_TEXTURE_2D_ARRAY_EXT:
				/* Requires GLU 1.3 which is harder to come by than cards doing hardware mipmapping
				Most 3D textures don't need mipmaps?
				gluBuild3DMipmaps(
				GL_TEXTURE_3D, internalFormat,
				data.getWidth(), data.getHeight(), data.getDepth(),
				GLPixelUtil::getGLOriginFormat(data.format), GLPixelUtil::getGLOriginDataType(data.format),
				data.data);
				
				glTexImage3D(
					_target, 0, format,
					dst->getWidth(), dst->getHeight(), dst->getDepth(), 0,
					PixelUtil::getGLOriginFormat(data->getPixelFormat()), PixelUtil::getGLOriginDataType(data->getPixelFormat),
					data->getData());
				break;
			}*/


		}
		else
		{
			if (data->getWidth() != data->getRowPitch())
				glPixelStorei(GL_UNPACK_ROW_LENGTH, data->getRowPitch());
			if (data->getHeight()*data->getWidth() != data->getSlicePitch())
				glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, (data->getSlicePitch() / data->getWidth()));
			if (data->getLeft() > 0 || data->getTop() > 0 || data->getFront() > 0)
				glPixelStorei(GL_UNPACK_SKIP_PIXELS, data->getLeft() + data->getRowPitch() * data->getTop() + data->getRowPitch() * data->getFront());
			//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			//if ((data.getWidth()*PixelUtil::getNumElemBytes(data.format)) & 3) {
				// Standard alignment of 4 is not right
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			GLuint level = _level;
			if (_softwareMipmap)
				level = 0;
			switch (_target) {
			case GL_TEXTURE_1D:
				glTexSubImage1D(GL_TEXTURE_1D, _level,
					dst->getLeft(),
					dst->getWidth(),
					PixelUtil::getGLOriginFormat(data->getPixelFormat), PixelUtil::getGLOriginDataType(data->getPixelFormat),
					data->getData());
				break;
			case GL_TEXTURE_2D:
			case GL_TEXTURE_CUBE_MAP:
				glTexSubImage2D(_faceTarget, _level,
					dst->getLeft(), dst->getTop(),
					dst->getWidth(), dst->getHeight(),
					PixelUtil::getGLOriginFormat(data->getPixelFormat), PixelUtil::getGLOriginDataType(data->getPixelFormat),
					data->getData());
				break;
			case GL_TEXTURE_3D:
			case GL_TEXTURE_2D_ARRAY_EXT:
				glTexSubImage3D(
					_target, _level,
					dst->getLeft(), dst->getTop(), dst->getFront(),
					dst->getWidth(), dst->getHeight(), dst->getDepth(),
					PixelUtil::getGLOriginFormat(data->getPixelFormat), PixelUtil::getGLOriginDataType(data->getPixelFormat),
					data->getData());
				break;
			}
			if(_softwareMipmap)
				glGenerateMipmap(_target);
		}

		glPixelStorei(GL_UNPACK_ALIGNMENT, 4); //恢复默认值 
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
		glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	}

	void* HardwareTextureBuffer::lock(LockOptions options)
	{
		return lock(0, _sizeInBytes, options);
	}

	void* HardwareTextureBuffer::lock(size_t offset, size_t length, LockOptions options)
	{
		if (isLocked())
		{
			//assert or log
		}
		if (offset != 0 || length != _sizeInBytes)
		{
			//assert or log
		}
		PixelBox *box = new PixelBox(_width, _height, 1, _format, 0);
		PixelBox::ptr retV = lock((PixelBox::ptr)box, options);
		return retV->getData();
	}

	PixelBox::ptr HardwareTextureBuffer::lock(PixelBox::ptr box, HardwareBuffer::LockOptions options)
	{
		PixelBox::ptr  pixel_box = _buffer;
		pixel_box->allocateBuffer(_sizeInBytes);
		if (options != HBL_DISCARD)
		{
			//不能从显卡直接得到部分吗？(不能）
			download(_buffer);
		}
		_current_lock_options = options;
		_isLocked = true;
		_current_locked_buffer = _buffer->getSubVolume(box);
		_current_lock_box = box;

		return _current_locked_buffer;
	}

	void HardwareTextureBuffer::unlock(void)
	{
		if (_current_lock_options != HBL_READ_ONLY)
			upload(_current_locked_buffer, _current_lock_box);

		PixelBox::ptr  pixel_box = _buffer;
		pixel_box->deallocBuffer();
		_isLocked = false;
	}
	
}