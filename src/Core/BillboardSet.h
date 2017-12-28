#pragma once


#include "RenderObject.h"
#include "Billboard.h"
namespace SRE {
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
	/** The rotation type of billboard. */
	enum BillboardRotationType
	{
		/// Rotate the billboard's vertices around their facing direction
		BBR_VERTEX,
		/// Rotate the billboard's texture coordinates
		BBR_TEXCOORD
	};
	/** The type of billboard to use. */
	enum BillboardType
	{
		/// Standard point billboard (default), always faces the camera completely and is always upright
		BBT_POINT,
		/// Billboards are oriented around a shared direction vector (used as Y axis) and only rotate around this to face the camera
		BBT_ORIENTED_COMMON,
		/// Billboards are oriented around their own direction vector (their own Y axis) and only rotate around this to face the camera
		BBT_ORIENTED_SELF,
		/// Billboards are perpendicular to a shared direction vector (used as Z axis, the facing direction) and X, Y axis are determined by a shared up-vertor
		BBT_PERPENDICULAR_COMMON,
		/// Billboards are perpendicular to their own direction vector (their own Z axis, the facing direction) and X, Y axis are determined by a shared up-vertor
		BBT_PERPENDICULAR_SELF
	};
	class Camera;
	class BillboardSet : public RenderObject {

	public:
		BillboardSet(unsigned int poolSize = 10);
		~BillboardSet();
		void update(size_t renderSize);
		Billboard* createBillboard(const Vector3D& position, const Vector3D& color = Vector3D(1.0,1.0,1.0));
	protected:
		void beginUpdate(size_t numBillboards);
		void endUpdate();
		void createBuffer();
		void updateBillboard(const Billboard& billboard);
		bool visibleBillboard(Camera* camera, const Billboard& billboard);
		//根据BillboardOrigin对齐方式获取偏移信息
		void getParametricOffsets(float& left, float& right, float& top, float& bottom);
		void genBillboardAxes(Vector3D* pX, Vector3D *pY, const Billboard* bb = 0);
		//主要是根据billboard的偏移信息来生成四个角的顶点偏移信息
		void genVertOffsets(float left, float right, float top, float bottom, float width, float height, const Vector3D& x, const Vector3D& y, Vector3D* outVec);
		void genVertices(const Vector3D* const offsets, const Billboard& pBillboard);
		
	
	protected:
		float * _lockData;
		HardwareVertexBuffer::ptr _vertexBuffer;
		
		BillboardType _billboardType;
		BillboardRotationType _rotationType;
		BillboardOrigin _originType;

		size_t _maxSize;
		size_t _currentNumVisible;

		float _leftOff, _rightOff, _topOff, _bottomOff;

		unsigned int _defaultWidth, _defaultHeight;//每个bb大小都一样时，用默认值
		bool _useDefaultSize;

		typedef std::list<Billboard> BillboardList;
		BillboardList _billboards;

		typedef std::vector<Texture::ptr> TextureSet;
		TextureSet _textureSet;

		Camera* _currentCamera;

		bool _bufferCreated;

		bool _pointRender;

		bool _useDefaultOriention;

		bool _accurateFacing;

		Vector3D _xAxis, _yAxis;
		Vector3D _offsets[4];

		Vector3D _commonDirection, _commonUpVector;

	};
}