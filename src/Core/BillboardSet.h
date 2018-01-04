#pragma once


#include "RenderObject.h"
#include "Billboard.h"
namespace SRE {

	/** 对齐方式 */
	enum BillboardOrigin
	{
		BBO_TOP_LEFT,
		BBO_TOP_CENTER,
		BBO_TOP_RIGHT,
		BBO_CENTER_LEFT,
		BBO_CENTER,
		BBO_CENTER_RIGHT,
		BBO_BOTTOM_LEFT,
		BBO_BOTTOM_CENTER,
		BBO_BOTTOM_RIGHT
	};
	
	enum BillboardRotationType
	{
		/// Rotate the billboard's vertices around their facing direction
		BBR_VERTEX,
		/// Rotate the billboard's texture coordinates
		BBR_TEXCOORD
	};
	enum BillboardType
	{
		///一直正对着相机的布告板
		BBT_POINT,
		/// Y轴有统一的方向
		BBT_ORIENTED_COMMON,
		/// Y轴有自己设置的方向
		BBT_ORIENTED_SELF,
		///X和Y轴由统一的direction和up方向决定
		BBT_PERPENDICULAR_COMMON,
		/// 布告板与自己设置的direction垂直
		BBT_PERPENDICULAR_SELF
	};
	class Camera;
	class BillboardSet : public RenderObject {

	public:
		BillboardSet(unsigned int poolSize = 10);
		~BillboardSet();
		void update(size_t renderSize);
		Billboard* createBillboard(const Vector3D& position, const Vector3D& color = Vector3D(1.0,1.0,1.0));

		Billboard* getBillboard(unsigned int index) const;
		void removeBillboard(unsigned int index);
		void removeBillboard(Billboard* pBill);
		void setBillboardOrigin(BillboardOrigin origin) { _originType = origin; }
		BillboardOrigin getBillboardOrigin() const { return _originType; }
		void setBillboardRotationType(BillboardRotationType rotationType) { _rotationType = rotationType; }
		BillboardRotationType getBillboardRotationType() const { return _rotationType; }
		void setDefaultWidth(float width) { _defaultWidth = width; }
		float getDefaultWidth() const { return _defaultWidth; }
		void setDefaultHeight(float height) { _defaultHeight = height; }
		float getDefaultHeight() const { return _defaultHeight; }
		void setBillboardType(BillboardType bbt) { _billboardType = bbt; }
		BillboardType getBillboardType(void) const { return _billboardType; }
		void setCommonDirection(const Vector3D& vec) { _commonDirection = vec; }
		const Vector3D& getCommonDirection(void) const { return _commonDirection; }
		void setCommonUpVector(const Vector3D& vec) { _commonUpVector = vec; }
		const Vector3D& getCommonUpVector(void) const { return _commonUpVector; }
	protected:
		void beginUpdate(size_t numBillboards);
		void endUpdate();
		virtual void createBuffer();
		void updateBillboard(const Billboard& billboard);
		bool visibleBillboard(Camera* camera, const Billboard& billboard);
		//根据BillboardOrigin对齐方式获取偏移信息
		void getParametricOffsets(float& left, float& right, float& top, float& bottom);
		void genBillboardAxes(Vector3D* pX, Vector3D *pY, const Billboard* bb = 0);
		//主要是根据billboard的偏移信息来生成四个角的顶点偏移信息
		void genVertOffsets(float left, float right, float top, float bottom, float width, float height, const Vector3D& x, const Vector3D& y, Vector3D* outVec);
		void genVertices(const Vector3D* const offsets, const Billboard& pBillboard);
		
	
	protected:
		//根据当前billboard刷新buffer
		float * _lockData;
		HardwareVertexBuffer::ptr _vertexBuffer;
		
		BillboardType _billboardType;
		BillboardRotationType _rotationType;
		BillboardOrigin _originType;

		size_t _maxSize;
		size_t _currentNumVisible;//当前帧可绘制的数量

		float _leftOff, _rightOff, _topOff, _bottomOff;

		unsigned int _defaultWidth, _defaultHeight;//每个bb大小都一样时，用默认值
		bool _useDefaultSize;

		typedef std::vector<Billboard::ptr> BillboardList;
		BillboardList _billboards;

		typedef std::vector<Texture::ptr> TextureSet;//如果每个billboard纹理不一样
		TextureSet _textureSet;

		Camera* _currentCamera;
		
		bool _pointRender;//是否是点绘制方式
		bool _useDefaultOriention;//是否用默认的方向
		bool _accurateFacing;//精准正对相机

		Vector3D _xAxis, _yAxis;
		Vector3D _offsets[4];

		Vector3D _commonDirection, _commonUpVector;

	};
}