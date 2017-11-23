#include "GeometryFactory.h"
#include "../Math/MathHelper.h"

using namespace Math;
namespace Core {

	RenderObject::ptr GeometryFactory::MakeBox(int width, int height, int depth)
	{
		size_t vertex_count = 6 * 2 * 3;
		size_t index_count = 36;
		VertexData* vertexdata = new VertexData;
		vertexdata->setVertexStart(0);
		vertexdata->setVertexCount(vertex_count);
		VertexDeclaration::ptr vd = vertexdata->getVertexDeclaration();
		VertexBufferBinding::ptr bind = vertexdata->getVertexBufferBinding();
		size_t offset = 0;
		VertexElement::ptr tmp_ve = vd->addElement(0, offset, VET_FLOAT3, VES_POSITION);
		offset += tmp_ve->getTypeSize(VET_FLOAT3);

		tmp_ve = vd->addElement(0, offset, VET_FLOAT3, VES_NORMAL);
		offset += tmp_ve->getTypeSize(VET_FLOAT3);

		tmp_ve = vd->addElement(0, offset, VET_FLOAT2, VES_TEXTURE_COORDINATES);
		offset += tmp_ve->getTypeSize(VET_FLOAT2);

		char* data = (char*)malloc(sizeof(char)*vd->getVertexSize(0)*vertex_count);

		float halfW = width * 0.5f;
		float halfH = height * 0.5f;
		float halfD = depth * 0.5f;
		float vertices[48 * 8] = {
			-halfW, -halfH, -halfD, 0.f, 0.f, -1.f, 0.f, 1.f,
			-halfW, halfH, -halfD, 0.f, 0.f, -1.f, 0.f, 0.f,
			halfW, halfH, -halfD, 0.f, 0.f, -1.f, 1.f, 0.f,
			-halfW, -halfH, halfD, -1.f, 0.f, 0.f, 0.f, 1.f,

			halfW, -halfH, -halfD, 0.f, 0.f, -1.f, 1.f, 1.f,
			-halfW, halfH, halfD, -1.f, 0.f, 0.f, 0.f, 0.f,
			-halfW, halfH, -halfD, -1.f, 0.f, 0.f, 1.f, 0.f,
			-halfW, -halfH, -halfD, -1.f, 0.f, 0.f, 1.f, 1.f,

			halfW, -halfH, halfD, 0.f, 0.f, 1.f, 0.f, 1.f,
			halfW, halfH, halfD, 0.f, 0.f, 1.f, 0.f, 0.f,
			-halfW, halfH, halfD, 0.f, 0.f, 1.f, 1.f, 0.f,
			-halfW, -halfH, halfD, 0.f, 0.f, 1.f, 1.f, 1.f,

			halfW, -halfH, -halfD, 1.f, 0.f, 0.f, 0.f, 1.f,
			halfW, halfH, -halfD, 1.f, 0.f, 0.f, 0.f, 0.f,
			halfW, halfH, halfD, 1.f, 0.f, 0.f, 1.f, 0.f,
			halfW, -halfH, halfD, 1.f, 0.f, 0.f, 1.f, 1.f,

			-halfW, halfH, -halfD, 0.f, 1.f, 0.f, 0.f, 1.f,
			-halfW, halfH, halfD, 0.f, 1.f, 0.f, 0.f, 0.f,
			halfW, halfH, halfD, 0.f, 1.f, 0.f, 1.f, 0.f,
			halfW, halfH, -halfD, 0.f, 1.f, 0.f, 1.f, 1.f,

			-halfW, -halfH, halfD, 0.f, -1.f, 0.f, 0.f, 1.f,
			-halfW, -halfH, -halfD, 0.f, -1.f, 0.f, 0.f, 0.f,
			halfW, -halfH, -halfD, 0.f, -1.f, 0.f, 1.f, 0.f,
			halfW, -halfH, halfD, 0.f, -1.f, 0.f, 1.f, 1.f
		};
		HardwareVertexBuffer* buffer = new HardwareVertexBuffer(offset, vertex_count, HardwareBuffer::HBU_STATIC_WRITE_ONLY);
		bind->setBinding(0, (HardwareVertexBuffer::ptr)buffer);
		buffer->writeData(0, buffer->getSizeInBytes(), vertices);

		IndexData* indexdata = new IndexData;
		indexdata->setIndexStart(0);
		indexdata->setIndexCount(36);
		HardwareIndexBuffer * index_buffer = new HardwareIndexBuffer(HardwareIndexBuffer::IT_16BIT, index_count, HardwareBuffer::HBU_STATIC_WRITE_ONLY);
		indexdata->setHardwareIndexBuffer((HardwareIndexBuffer::ptr)index_buffer);

		unsigned short faces[36] = {
			// front
			0,1,2,
			0,2,3,

			// back
			4,5,6,
			4,6,7,

			// left
			8,9,10,
			8,10,11,

			// right
			12,13,14,
			12,14,15,

			// up
			16,17,18,
			16,18,19,

			// down
			20,21,22,
			20,22,23
		};
		index_buffer->writeData(0, index_buffer->getSizeInBytes(), faces);
		RenderObject::ptr ro = std::make_shared<RenderObject>();
		ro->setVertexData((VertexData::ptr)vertexdata);
		ro->setIndexData((IndexData::ptr)indexdata);
		
		return ro;
		
	}

	/*Mesh* GeometryFactory::MakeSphere(double radius, int widthSegments, int heightSegments)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		Mesh* sphere = new Mesh();

		double phiStart = 0.0;
		double phiLength = math_pi * 2;

		double thetaStart = 0.0;
		double thetaLength = math_pi;

		for (int i = 0; i <= heightSegments; i++) {

			float v = i / (float)heightSegments;

			for (int j = 0; j <= widthSegments; j++) {

				float u = j / (float)widthSegments;
				Vertex vertex;
				vertex.position_x = -radius * cos(phiStart + u * phiLength) * sin(thetaStart + v * thetaLength);
				vertex.position_y = radius * cos(thetaStart + v * thetaLength);
				vertex.position_z = radius * sin(phiStart + u * phiLength) * sin(thetaStart + v * thetaLength);
				vertex.texcoord_x = u;
				vertex.texcoord_y = v;
				vertices.push_back(vertex);
			}
		}
		for (int i = 0; i < heightSegments; i++) {

			for (int j = 0; j < widthSegments; j++) {

				indices.push_back(i*widthSegments + j);
				indices.push_back(i*widthSegments + j + 1);
				indices.push_back((i + 1)*widthSegments + j);
				indices.push_back((i + 1)*widthSegments + j);
				indices.push_back(i*widthSegments + j + 1);
				indices.push_back((i + 1)*widthSegments + j + 1);

			}

		}
		sphere->setVertices(vertices);
		sphere->setIndex(indices);
		sphere->computeNormals();

		return sphere;*/
	
	/*Mesh* GeometryFactory::MakeSphere(const Vector3D& radii, int widthSegments, int heightSegments)
	{

		if ((radii.x <= 0) || (radii.y <= 0) || (radii.z <= 0)) {
			return NULL;
		}
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		Mesh* sphere = new Mesh();
		int slicePartitions = widthSegments + 1;
		int stackPartitions = heightSegments + 1;

		int vertexCount = stackPartitions * slicePartitions;
		//var positions = new Float32Array(vertexCount * 3);
		//var numIndices = 6 * (slicePartitions - 1) * (stackPartitions - 2);
		//var indices = new Uint32Array(numIndices);

		float *cosTheta = new float[slicePartitions];
		float *sinTheta = new float[slicePartitions];
		int index = 0;

		for (int i = 0; i < slicePartitions; i++) {
			float theta = math_pi * 2 * i / (slicePartitions - 1);
			cosTheta[i] = cos(theta);
			sinTheta[i] = sin(theta);

			// duplicate first point for correct
			// texture coordinates at the north pole.
			vertices.push_back(Vertex(0.0, 0.0, radii[2], (float)(i + 0.5f) / (float)slicePartitions, 0.0f));
			
		}

		for (int i = 1; i < stackPartitions - 1; i++) {
			float phi = math_pi * i / (stackPartitions - 1);
			float sinPhi = sin(phi);

			float xSinPhi = radii.x * sinPhi;
			float ySinPhi = radii.y * sinPhi;
			float zCosPhi = radii.z * cos(phi);

			for (int j = 0; j < slicePartitions; j++) {
				vertices.push_back(Vertex(cosTheta[j] * xSinPhi, sinTheta[j] * ySinPhi, zCosPhi, (float)(j + 0) / (float)slicePartitions, (float)(1 + i + 0) / (float)stackPartitions));
				
			}
		}

		for (int i = 0; i < slicePartitions; i++) {
			vertices.push_back(Vertex(0.0, 0.0, -radii[2], (float)(i + 0) / (float)slicePartitions, (float)(stackPartitions - 1) / (float)stackPartitions));
			
		}
		int i;
		int j;
		for (j = 0; j < slicePartitions - 1; j++) {
			indices.push_back(slicePartitions + j);
			indices.push_back(slicePartitions + j + 1);
			indices.push_back(j + 1);			
		}

		int topOffset;
		int bottomOffset;
		for (i = 1; i < stackPartitions - 2; i++) {
			topOffset = i * slicePartitions;
			bottomOffset = (i + 1) * slicePartitions;
			for (j = 0; j < slicePartitions - 1; j++) {
				indices.push_back(bottomOffset + j);
				indices.push_back(bottomOffset + j + 1);
				indices.push_back(topOffset + j + 1);
				indices.push_back(bottomOffset + j);
				indices.push_back(topOffset + j + 1);
				indices.push_back(topOffset + j);				
			}
		}

		i = stackPartitions - 2;
		topOffset = i * slicePartitions;
		bottomOffset = (i + 1) * slicePartitions;

		for (j = 0; j < slicePartitions - 1; j++) {
			indices.push_back(bottomOffset + j);
			indices.push_back(topOffset + j + 1);
			indices.push_back(topOffset + j);			
		}

		sphere->setVertices(vertices);
		sphere->setIndex(indices);
		sphere->computeNormals();

		return sphere;
	}*/
	Mesh* GeometryFactory::MakeSphere(double radius, int widthSegments, int heightSegments)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		Mesh* sphere = new Mesh();

		int X = widthSegments, Y = heightSegments;

		float a, stepa = (float)math_pi * 2.0f / (float)X, stepb = (float)math_pi / (float)Y, b = -(float)math_half_pi + stepb;

		Vector3D *positions = new Vector3D[X * (Y - 1)];

		for (int y = 0; y < (Y - 1); y++)
		{
			a = -(float)math_pi;

			for (int x = 0; x < X; x++)
			{
				positions[y * X + x] = Vector3D(radius * sin(a) * cos(b), radius *sin(b), radius *cos(a) * cos(b));
				a += stepa;
			}

			b += stepb;
		}

		//VerticesCount = (X * (Y - 2) * 2 + X * 2) * 3;

		///Vector3D *Vertices = new Vector3D[VerticesCount];
		//vec2 *TexCoords = new vec2[VerticesCount];

		for (int x = 0; x < X; x++)
		{
			Vertex v1(0.0f, -radius, 0.0f, (float)(x + 0.5f) / (float)X, 0.0f);
			vertices.push_back(v1);

			Vertex v2;
			v2.setPosition(positions[(0 + 0) * X + ((x + 1) % X)]);
			v2.setTex((float)(x + 1) / (float)X, (float)(0 + 1) / (float)Y);			
			vertices.push_back(v2);
			Vertex v3;
			v3.setPosition(positions[(0 + 0) * X + ((x + 0) % X)]);
			v3.setTex((float)(x + 0) / (float)X, (float)(0 + 1) / (float)Y);
			vertices.push_back(v3);
			//Vertices[vpos++] = Vector3D(0.0f, -1.0f, 0.0f);
			//Vertices[vpos++] = vertices[(0 + 0) * X + ((x + 1) % X)];
			//Vertices[vpos++] = vertices[(0 + 0) * X + ((x + 0) % X)];

			//TexCoords[tpos++] = vec2((float)(x + 0.5f) / (float)X, 0.0f);
			//TexCoords[tpos++] = vec2((float)(x + 1) / (float)X, (float)(0 + 1) / (float)Y);
			//TexCoords[tpos++] = vec2((float)(x + 0) / (float)X, (float)(0 + 1) / (float)Y);
		}

		for (int y = 0; y < Y - 2; y++)
		{
			for (int x = 0; x < X; x++)
			{
				Vertex v1;
				v1.setPosition(positions[(y + 0) * X + ((x + 0) % X)]);
				v1.setTex((float)(x + 0) / (float)X, (float)(1 + y + 0) / (float)Y);
				vertices.push_back(v1);

				Vertex v2;
				v2.setPosition(positions[(y + 0) * X + ((x + 1) % X)]);
				v2.setTex((float)(x + 1) / (float)X, (float)(1 + y + 0) / (float)Y);
				vertices.push_back(v2);


				Vertex v3;
				v3.setPosition(positions[(y + 1) * X + ((x + 1) % X)]);
				v3.setTex((float)(x + 1) / (float)X, (float)(1 + y + 1) / (float)Y);
				vertices.push_back(v3);

				//Vertices[vpos++] = vertices[(y + 0) * X + ((x + 0) % X)];
				//Vertices[vpos++] = vertices[(y + 0) * X + ((x + 1) % X)];
				//Vertices[vpos++] = vertices[(y + 1) * X + ((x + 1) % X)];
//
				//TexCoords[tpos++] = vec2((float)(x + 0) / (float)X, (float)(1 + y + 0) / (float)Y);
				//TexCoords[tpos++] = vec2((float)(x + 1) / (float)X, (float)(1 + y + 0) / (float)Y);
				//TexCoords[tpos++] = vec2((float)(x + 1) / (float)X, (float)(1 + y + 1) / (float)Y);
				Vertex v4;
				v4.setPosition(positions[(y + 1) * X + ((x + 1) % X)]);
				v4.setTex((float)(x + 1) / (float)X, float(1 + y + 1) / (float)Y);
				vertices.push_back(v4);

				Vertex v5;
				v5.setPosition(positions[(y + 1) * X + ((x + 0) % X)]);
				v5.setTex((float)(x + 0) / (float)X, float(1 + y + 1) / (float)Y);
				vertices.push_back(v5);


				Vertex v6;
				v6.setPosition(positions[(y + 0) * X + ((x + 0) % X)]);
				v6.setTex((float)(x + 0) / (float)X, float(1 + y + 0) / (float)Y);
				vertices.push_back(v6);

				//Vertices[vpos++] = vertices[(y + 1) * X + ((x + 1) % X)];
				//Vertices[vpos++] = vertices[(y + 1) * X + ((x + 0) % X)];
				//Vertices[vpos++] = vertices[(y + 0) * X + ((x + 0) % X)];

				//TexCoords[tpos++] = vec2((float)(x + 1) / (float)X, float(1 + y + 1) / (float)Y);
				//TexCoords[tpos++] = vec2((float)(x + 0) / (float)X, float(1 + y + 1) / (float)Y);
				//TexCoords[tpos++] = vec2((float)(x + 0) / (float)X, float(1 + y + 0) / (float)Y);
			}
		}

		for (int x = 0; x < X; x++)
		{
			Vertex v1;
			v1.setPosition(positions[(Y - 2) * X + ((x + 0) % X)]);
			v1.setTex((float)(x + 0) / (float)X, (float)(Y - 1) / (float)Y);
			vertices.push_back(v1);

			Vertex v2;
			v2.setPosition(positions[(Y - 2) * X + ((x + 1) % X)]);
			v2.setTex((float)(x + 1) / (float)X, (float)(Y - 1) / (float)Y);
			vertices.push_back(v2);


			Vertex v3;
			v3.setPosition(0.0f, radius, 0.0f);
			v3.setTex((float)(x + 0.5f) / (float)X, 1.0f);
			vertices.push_back(v3);

			//Vertices[vpos++] = vertices[(Y - 2) * X + ((x + 0) % X)];
			//Vertices[vpos++] = vertices[(Y - 2) * X + ((x + 1) % X)];
			//Vertices[vpos++] = Vector3D(0.0f, 1.0f, 0.0f);

			//TexCoords[tpos++] = vec2((float)(x + 0) / (float)X, (float)(Y - 1) / (float)Y);
			//TexCoords[tpos++] = vec2((float)(x + 1) / (float)X, (float)(Y - 1) / (float)Y);
			//TexCoords[tpos++] = vec2((float)(x + 0.5f) / (float)X, 1.0f);
		}
		sphere->setVertices(vertices);
		//sphere->setIndex(indices);
		sphere->computeNormals();

		return sphere;
	}


}