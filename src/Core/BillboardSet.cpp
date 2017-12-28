#include "BillboardSet.h"
#include "Camera.h"

namespace SRE {

	BillboardSet::BillboardSet(unsigned int poolSize)
	:_maxSize(poolSize),		
	_originType(BBO_CENTER),
	_rotationType(BBR_TEXCOORD),
	_useDefaultSize(true),	
	_accurateFacing(false),		
	_billboardType(BBT_POINT),
	_commonDirection(Vector3D(0.0,0.0,1.0)),
	_commonUpVector(Vector3D(0.0,1.0,0.0)),
	_pointRender(false),
	_bufferCreated(false)
	{

	}
	BillboardSet::~BillboardSet()
	{
		
	}

	Billboard* BillboardSet::createBillboard(const Vector3D& position, const Vector3D& color = Vector3D(1.0, 1.0, 1.0))
	{

	}

	void BillboardSet::update(size_t renderSize)
	{
		beginUpdate(renderSize);

		for (BillboardList::iterator it = _billboards.begin(); it != _billboards.end(); it++)
		{
			updateBillboard(*it);
		}
		
		endUpdate();

	}
	
	void BillboardSet::beginUpdate(size_t numBillboards)
	{
		//第一次先创建Vertex Buffer
		if (!_bufferCreated)
			createBuffer();

		if (!_pointRender)
		{
			//先获取初始化偏移信息
			getParametricOffsets(_leftOff, _rightOff, _topOff, _bottomOff);


			if (_billboardType != BBT_ORIENTED_SELF &&
				_billboardType != BBT_PERPENDICULAR_SELF &&
				!(_accurateFacing && _billboardType != BBT_PERPENDICULAR_COMMON))
			{
				genBillboardAxes(&_xAxis, &_yAxis);

				genVertOffsets(_leftOff, _rightOff, _topOff, _bottomOff, _defaultWidth, _defaultHeight, _xAxis, _yAxis, _offsets);

			}
		}
		//初始化
		_currentNumVisible = 0;

		if (numBillboards)
		{
			numBillboards = std::min(numBillboards, _maxSize);

			size_t billboardSize;
			if (_pointRender)
			{			
				billboardSize = _vertexBuffer->getVertexSize();
			}
			else
			{				
				billboardSize = _vertexBuffer->getVertexSize() * 4;
			}

			_lockData = static_cast<float*>(_vertexBuffer->lock(0, numBillboards * billboardSize,
				_vertexBuffer->getUsage() & HardwareBuffer::HBU_DYNAMIC ?
					HardwareBuffer::HBL_DISCARD : HardwareBuffer::HBL_NORMAL));
		}
		else
		{
			_lockData = static_cast<float*>(_vertexBuffer->lock(_vertexBuffer->getUsage() & HardwareBuffer::HBU_DYNAMIC ?
				HardwareBuffer::HBL_DISCARD : HardwareBuffer::HBL_NORMAL));
		}
	}

	void BillboardSet::updateBillboard(const Billboard& billboard)
	{
		if (_currentNumVisible >= _maxSize)return;

		if (!visibleBillboard(_currentCamera, billboard))return;

		if (!_pointRender &&
			(_billboardType == BBT_ORIENTED_SELF ||
				_billboardType == BBT_PERPENDICULAR_SELF ||
				(_accurateFacing && _billboardType != BBT_PERPENDICULAR_COMMON)))
		{

			genBillboardAxes(&_xAxis, &_yAxis, &billboard);
		}

		if (_useDefaultSize || _pointRender)
		{

			if (!_pointRender &&
				(_billboardType == BBT_ORIENTED_SELF ||
					_billboardType == BBT_PERPENDICULAR_SELF ||
					(_accurateFacing && _billboardType != BBT_PERPENDICULAR_COMMON)))
			{
				genVertOffsets(_leftOff, _rightOff, _topOff, _bottomOff,
					_defaultWidth, _defaultHeight, _xAxis, _yAxis, _offsets);
			}
			genVertices(_offsets, billboard);
		}
		else // not all default size and not point rendering
		{
			Vector3D vOwnOffset[4];
			// If it has own dimensions, or self-oriented, gen offsets
			if (_billboardType == BBT_ORIENTED_SELF ||
				_billboardType == BBT_PERPENDICULAR_SELF ||
				billboard._ownDimensions ||
				(_accurateFacing && _billboardType != BBT_PERPENDICULAR_COMMON))
			{

				genVertOffsets(_leftOff, _rightOff, _topOff, _bottomOff,
					billboard._width, billboard._height, _xAxis, _yAxis, _offsets);

				genVertices(vOwnOffset, billboard);
			}
			else // Use default dimension, already computed before the loop, for faster creation
			{
				genVertices(vOwnOffset, billboard);
			}
		}
		_currentNumVisible++;

	}

	void BillboardSet::endUpdate()
	{
		_vertexBuffer->unlock();
	}

	void BillboardSet::getParametricOffsets(float& left, float& right, float& top, float& bottom)
	{
		switch (_originType)
		{
		case BBO_TOP_LEFT:
			left = 0.0f;
			right = 1.0f;
			top = 0.0f;
			bottom = -1.0f;
			break;

		case BBO_TOP_CENTER:
			left = -0.5f;
			right = 0.5f;
			top = 0.0f;
			bottom = -1.0f;
			break;

		case BBO_TOP_RIGHT:
			left = -1.0f;
			right = 0.0f;
			top = 0.0f;
			bottom = -1.0f;
			break;

		case BBO_CENTER_LEFT:
			left = 0.0f;
			right = 1.0f;
			top = 0.5f;
			bottom = -0.5f;
			break;

		case BBO_CENTER:
			left = -0.5f;
			right = 0.5f;
			top = 0.5f;
			bottom = -0.5f;
			break;

		case BBO_CENTER_RIGHT:
			left = -1.0f;
			right = 0.0f;
			top = 0.5f;
			bottom = -0.5f;
			break;

		case BBO_BOTTOM_LEFT:
			left = 0.0f;
			right = 1.0f;
			top = 1.0f;
			bottom = 0.0f;
			break;

		case BBO_BOTTOM_CENTER:
			left = -0.5f;
			right = 0.5f;
			top = 1.0f;
			bottom = 0.0f;
			break;

		case BBO_BOTTOM_RIGHT:
			left = -1.0f;
			right = 0.0f;
			top = 1.0f;
			bottom = 0.0f;
			break;
		}
	}

	void BillboardSet::genBillboardAxes(Vector3D* pX, Vector3D *pY, const Billboard* bb)
	{
		Vector3D toCameraDir;
		Quaternion quat = _currentCamera->getOrientation();
		if (_accurateFaceing &&
			(_billboardType == BBT_POINT ||
				_billboardType == BBT_ORIENTED_COMMON ||
				_billboardType == BBT_ORIENTED_SELF))
		{			
			toCameraDir = bb->getPosition() - _currentCamera->getPosition();
			toCameraDir.normalize();
		}

		switch (_billboardType)
		{
		case BBT_POINT:
			if (_accurateFaceing)
			{			
				*pY = _currentCamera->getUp();
				*pX = toCameraDir.cross(*pY);
				pX->normalize();
				*pY = pX->cross(toCameraDir); 
			}
			else
			{			
				*pX = _currentCamera->getRight();
				*pY = _currentCamera->getUp();
			}
			break;

		case BBT_ORIENTED_COMMON:
			// Y朝着统一的方向		
			*pY = _commonDirection;
			*pX = toCameraDir.cross(*pY);
			pX->normalize();
			break;

		case BBT_ORIENTED_SELF:
			//Y有自己的方向
			*pY = bb->getDirection();
			*pX = toCameraDir.cross(*pY);
			pX->normalize();
			break;

		case BBT_PERPENDICULAR_COMMON:
			//X和Y都有自己的方向
			*pX = _commonUpVector.cross(_commonDirection);
			*pY = _commonDirection.cross(*pX);
			break;

		case BBT_PERPENDICULAR_SELF:
			//
			*pX = _commonUpVector.cross(bb->getDirection());
			pX->normalize();
			*pY = bb->getDirection().cross(*pX); // both should be normalised
			break;
		}

	}

	void BillboardSet::genVertices(const Vector3D* const offsets, const Billboard& billboard)
	{
		//纹理坐标
		Vector4D texRect = billboard.getTexCoordRect();
		if (_pointRender)
		{
			Vector3D position = billboard.getPosition();
			*_lockData++ = position.x;
			*_lockData++ = position.y;
			*_lockData++ = position.z;

			Vector3D color = billboard.getColor();
			*_lockData++ = color.x;
			*_lockData++ = color.y;
			*_lockData++ = color.z;
		}
		else if (_useDefaultOriention || billboard.getRotateAngle() == 0.0)//没有旋转的情况下
		{
			Vector3D position = billboard.getPosition();
			*_lockData++ = offsets[0].x + position.x;
			*_lockData++ = offsets[0].y + position.y;
			*_lockData++ = offsets[0].z + position.z;
			
			Vector3D color = billboard.getColor();
			*_lockData++ = color.x;
			*_lockData++ = color.y;
			*_lockData++ = color.z;

			*_lockData++ = texRect.x;
			*_lockData++ = texRect.z;

			// Right-top
			// Positions
			*_lockData++ = offsets[1].x + position.x;
			*_lockData++ = offsets[1].y + position.y;
			*_lockData++ = offsets[1].z + position.z;
			
			*_lockData++ = color.x;
			*_lockData++ = color.y;
			*_lockData++ = color.z;

			*_lockData++ = texRect.y;
			*_lockData++ = texRect.z;

			// Left-bottom
			// Positions
			*_lockData++ = offsets[2].x + position.x;
			*_lockData++ = offsets[2].y + position.y;
			*_lockData++ = offsets[2].z + position.z;
			
			*_lockData++ = color.x;
			*_lockData++ = color.y;
			*_lockData++ = color.z;

			*_lockData++ = texRect.x;
			*_lockData++ = texRect.w;

			// Right-bottom
			// Positions
			*_lockData++ = offsets[3].x + position.x;
			*_lockData++ = offsets[3].y + position.y;
			*_lockData++ = offsets[3].z + position.z;

			*_lockData++ = color.x;
			*_lockData++ = color.y;
			*_lockData++ = color.z;

			*_lockData++ = texRect.y;
			*_lockData++ = texRect.w;
		}
		else if (_rotationType == BBR_VERTEX)
		{

		}
	}

	void BillboardSet::genVertOffsets(float left, float right, float top, float bottom, float width, float height, const Vector3D& x, const Vector3D& y, Vector3D* outVec)
	{
		Vector3D vLeftOff, vRightOff, vTopOff, vBottomOff;
		
		vLeftOff = x * (left   * width);
		vRightOff = x * (right  * width);
		vTopOff = y * (top   * height);
		vBottomOff = y * (bottom * height);

		
		outVec[0] = vLeftOff + vTopOff;
		outVec[1] = vRightOff + vTopOff;
		outVec[2] = vLeftOff + vBottomOff;
		outVec[3] = vRightOff + vBottomOff;
	}
	
	void BillboardSet::createBuffer()
	{

		VertexData* vertex_data = new VertexData;
		vertex_data->setVertexStart(0);
		vertex_data->setVertexCount(_maxSize * 4);
		VertexDeclaration::ptr vd = vertex_data->getVertexDeclaration();
		VertexBufferBinding::ptr bind = vertex_data->getVertexBufferBinding();
		
		size_t offset = 0;
		VertexElement::ptr tmp_ve = vd->addElement(0, offset, VET_FLOAT3, VES_POSITION);
		offset += tmp_ve->getTypeSize(VET_FLOAT3);

		tmp_ve = vd->addElement(0, offset, VET_FLOAT3, VES_DIFFUSE);
		offset += tmp_ve->getTypeSize(VET_FLOAT3);

		tmp_ve = vd->addElement(0, offset, VET_FLOAT2, VES_TEXTURE_COORDINATES);
		offset += tmp_ve->getTypeSize(VET_FLOAT2);

		HardwareVertexBuffer* buffer = new HardwareVertexBuffer(offset, _maxSize * 4, HardwareBuffer::HBU_STATIC_WRITE_ONLY);
		_vertexBuffer = (HardwareVertexBuffer::ptr)buffer;
		bind->setBinding(0, _vertexBuffer);

		IndexData* index_data = new IndexData;
		index_data->setIndexStart(0);
		index_data->setIndexCount(_maxSize * 6);
		HardwareIndexBuffer * index_buffer = new HardwareIndexBuffer(HardwareIndexBuffer::IT_16BIT, _maxSize * 6, HardwareBuffer::HBU_STATIC_WRITE_ONLY);
		index_data->setHardwareIndexBuffer((HardwareIndexBuffer::ptr)index_buffer);

		unsigned short* pIdx = static_cast<unsigned short*>(index_buffer->lock(HardwareBuffer::HBL_DISCARD));		

		for (size_t idx, idxOff, bboard = 0; bboard < _maxSize;	++bboard)
		{			
			idx = bboard * 6;
			idxOff = bboard * 4;

			pIdx[idx] = static_cast<unsigned short>(idxOff); 
			pIdx[idx + 1] = static_cast<unsigned short>(idxOff + 2);
			pIdx[idx + 2] = static_cast<unsigned short>(idxOff + 1);
			pIdx[idx + 3] = static_cast<unsigned short>(idxOff + 1);
			pIdx[idx + 4] = static_cast<unsigned short>(idxOff + 2);
			pIdx[idx + 5] = static_cast<unsigned short>(idxOff + 3);

		}

		index_buffer->unlock();

		_vertex_data = (VertexData::ptr)vertex_data;
		_index_data = (IndexData::ptr)index_data;
		
		_bufferCreated = true;
	}
}