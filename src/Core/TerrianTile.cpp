#include "TerrainTile.h"
#include "TextureManager.h"
namespace SRE {

	TerrainTile::TerrainTile()
	:_blockScale(1.0),
	_heightScale(1.0/10.0){
		

	}
	TerrainTile::TerrainTile(Terrain* terrain, TerrainTile* parent, unsigned int xStart, unsigned int yStart, size_t size, Code code)
	{
		if (terrain->getMaxLevel() < code.level)
		{
			//还可以继续创建子节点
			unsigned int childSize = (unsigned int)(((size - 1) * 0.5f) + 1);
			unsigned int childOff = childSize - 1;
			unsigned int childLevel = code.level + 1; 
			unsigned int xOffset = xStart, yOffset = yStart;
			for (unsigned int i = 0; i < 4; ++i)
			{
				Code childCode;
				childCode.x = code.x * 2 + i % 2;
				childCode.y = code.y * 2 + i / 2;
				childCode.level = code.level + 1;

				if (i == 1)
					xOffset += childOff;
				if (i == 2)
					yOffset += childOff;
				_children[i] = new TerrainTile(terrain, this, xOffset, yOffset, childSize, childCode);

			}
		}
		else
		{
			//该节点叶子节点
		}
	}
	TerrainTile::~TerrainTile()
	{

	}
	Mesh* TerrainTile::createFromRandomHeght(int width, int height)
	{
		_width = width;
		_height = height;
		generateVertex();
		for (int i = 0; i < _width*_height; i++)
		{
			_vertices[i].position_y = rand() % 5;
		}
		Mesh* mesh = new Mesh;
		mesh->setVertexData(_vertices);
		mesh->setIndexData(_indices);
		mesh->computeNormals();
		return mesh;
	}
	Mesh* TerrainTile::createMeshFromHeightmap(const char* fileName)
	{		
		Image* image = new Image;
		image->load(fileName);
		unsigned char* data = image->data();
		unsigned char bpp = image->BPP();
		_width = image->width();
		_height = image->height();
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		/*unsigned int vertexCount = _width*_height;

		VertexData* vertexdata = new VertexData;
		vertexdata->setVertexStart(0);
		vertexdata->setVertexCount(vertexCount);
		VertexDeclaration::ptr vd = vertexdata->getVertexDeclaration();
		VertexBufferBinding::ptr bind = vertexdata->getVertexBufferBinding();
		size_t offset = 0;
		VertexElement::ptr tmp_ve = vd->addElement(0, offset, VET_FLOAT3, VES_POSITION);
		offset += tmp_ve->getTypeSize(VET_FLOAT3);

		tmp_ve = vd->addElement(0, offset, VET_FLOAT3, VES_NORMAL);
		offset += tmp_ve->getTypeSize(VET_FLOAT3);

		tmp_ve = vd->addElement(0, offset, VET_FLOAT2, VES_TEXTURE_COORDINATES);
		offset += tmp_ve->getTypeSize(VET_FLOAT2);

		HardwareVertexBuffer* vertex_buffer = new HardwareVertexBuffer(offset, vertexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY);
		bind->setBinding(0, (HardwareVertexBuffer::ptr)vertex_buffer);
		float* pVertex = static_cast<float*>(vertex_buffer->lock(HardwareBuffer::HBL_DISCARD));*/

		float terrainWidth = (_width - 1) * _blockScale;
		float terrainHeight = (_height - 1) * _blockScale;
		float halfTerrainWidth = terrainWidth * 0.5f;
		float halfTerrainHeight = terrainHeight * 0.5f;
		const unsigned int bytesPerPixel = bpp / 8;
		for (unsigned int j = 0; j < _height; ++j)
		{
			for (unsigned i = 0; i < _width; ++i)
			{
				unsigned int index = (j * _width) + i;				
				float heightValue = getHeightValue(&data[index * bytesPerPixel], bytesPerPixel);

				float s = (i / (float)(_width - 1));
				float t = (j / (float)(_height - 1));

				float x = (s * terrainWidth) - halfTerrainWidth;
				float y = heightValue * _heightScale;
				float z = (t * terrainHeight) - halfTerrainHeight;

				Vertex vertex;
				vertex.position_x = x;
				vertex.position_y = y;
				vertex.position_z = z;
				vertex.texcoord_x = s;
				vertex.texcoord_y = t;
				vertices.push_back(vertex);

			}
		}
		/*const unsigned int numTriangles = (_width - 1) * (_height - 1) * 2;
		unsigned int index_count = numTriangles * 3;
		IndexData* indexdata = new IndexData;
		indexdata->setIndexStart(0);
		indexdata->setIndexCount(index_count);
		HardwareIndexBuffer * index_buffer = new HardwareIndexBuffer(HardwareIndexBuffer::IT_16BIT, index_count, HardwareBuffer::HBU_STATIC_WRITE_ONLY);
		indexdata->setHardwareIndexBuffer((HardwareIndexBuffer::ptr)index_buffer);
		unsigned short* pIndices = static_cast<unsigned short*>(index_buffer->lock(HardwareBuffer::HBL_DISCARD));*/

		for (unsigned int j = 0; j < (_height - 1); ++j)
		{
			for (unsigned int i = 0; i < (_width - 1); ++i)
			{
				int vertexIndex = (j * _width) + i;
				indices.push_back(vertexIndex);
				indices.push_back(vertexIndex + terrainWidth + 1);
				indices.push_back(vertexIndex + 1);
				indices.push_back(vertexIndex);
				indices.push_back(vertexIndex + terrainWidth);
				indices.push_back(vertexIndex + terrainWidth + 1);			
			}
		}

		/*vertex_buffer->unlock();
		index_buffer->unlock();*/
		Mesh* mesh = new Mesh;
		mesh->setVertexData(vertices);
		mesh->setIndexData(indices);
		mesh->computeNormals();
		//mesh->setVertexData((VertexData::ptr)vertexdata);
	//	mesh->setIndexData((IndexData::ptr)indexdata);

		delete image;
		return mesh;

//		Texture::ptr texture = TextureManager::Inst()->loadTexture("heightmap", fileName);
//
//		_width = texture->getWidth();
//		_height = texture->getHeight();
//
//		HardwareTextureBuffer::ptr buffer = texture->getBuffer(0, 0);		
////		float* data = static_cast<float*>(buffer->lock(HardwareBuffer::HBL_READ_ONLY));
//
//		_data = new float[_width * _height];
//		for (int i = 0; i < _height; i++)
//		{
//			for (int j = 0; j < _width; j++)
//			{
//			//	*_data++ = *data++;
//			//	data += 2;
//			}
//		}
		//FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		//FIBITMAP *dib(0);

		//fif = FreeImage_GetFileType(fileName, 0);

		//if (fif == FIF_UNKNOWN)
		//	fif = FreeImage_GetFIFFromFilename(fileName);

		//if (fif == FIF_UNKNOWN)
		//	return;


		//if (FreeImage_FIFSupportsReading(fif))
		//	dib = FreeImage_Load(fif, fileName);

		//if (!dib)
		//	return;

		//dib = FreeImage_ConvertTo24Bits(dib);

		////BYTE *pixels = (BYTE*)FreeImage_GetBits(dib);

		//_width = FreeImage_GetWidth(dib);
		//_height = FreeImage_GetHeight(dib);

		//if ((_width == 0) || (_height == 0))
		//	return;
		//generateVertex();
		//int byteIndex = 0;
		//for (unsigned int i = 0; i < _height; i++)
		//{
		//	for (unsigned int j = 0; j < _width; j++)
		//	{
		//		RGBQUAD color;
		//		FreeImage_GetPixelColor(dib, j, i, &color);
		//		_vertices[i*_width + j].position_y = color.rgbGreen * 256.0f;
		//	}
		//	
		//}


	}
	float TerrainTile::getHeightValue(const unsigned char* data, unsigned char numBytes)
	{
		switch (numBytes)
		{
		case 1:
		{
			return (unsigned char)(data[0]) /*/ (float)0xff*/;
		}
		break;
		case 2:
		{
			return (unsigned short)(data[1] << 8 | data[0]) /*/ (float)0xffff*/;
		}
		break;
		case 3:
		{
			return (unsigned short)(data[2] << 16/* | data[1] << 8*/ | data[0]) /*/ (float)0xffffff*/;
		}
		case 4:
		{
			return (unsigned int)(data[3] << 24 | data[2] << 16 | data[1] << 8 | data[0]) / (float)0xffffffff;
		}
		break;
		default:
		{
			assert(false);  // Height field with non standard pixle sizes
		}
		break;
		}

		return 0.0f;
	}
	void TerrainTile::generateVertex()
	{
		float halfHeight = (_height - 1) / 2;
		float halfWidth = (_width - 2) / 2;
		for (int i = 0; i < _height; i++)
		{
			for (int j = 0; j < _width; j++)
			{
				//间隔计算点坐标
				Vertex v(j - halfWidth, 0.0, halfHeight - i, 0, 1, 0, j / (_width - 1), 1 - i / (_height - 1));
				_vertices.push_back(v);
			}
		}
		for (int i = 0; i < _height - 1; i++)
		{
			for (int j = 0; j < _width - 1; j++)
			{
				//两个三角形索引
				_indices.push_back(i*_width + j);
				_indices.push_back(i*_width + j + 1);
				_indices.push_back((i + 1)*_width + j);

				_indices.push_back((i + 1)*_width + j);
				_indices.push_back(i*_width + j + 1);
				_indices.push_back((i + 1)*_width + j + 1);
			}
		}
	}
}