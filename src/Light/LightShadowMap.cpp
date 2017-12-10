#include "LightShadowMap.h"

#include "DirectionLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "../Core/FrameBuffer.h"
#include "../Core/RenderState.h"
namespace SRE {

	void LightShadowMap::render(std::vector<Light::ptr> lights, std::vector<Mesh::ptr> meshs)
	{
		if (lights.empty())
			return;

		Vector3D cubeDirections[6] = { Vector3D(1, 0, 0),Vector3D(-1, 0, 0),Vector3D(0, 0, 1), 
					Vector3D(0, 0, -1),Vector3D(0, 1, 0),Vector3D(0, -1, 0) };

		Vector3D cubeUps[6] = {
			 Vector3D(0, 1, 0), Vector3D(0, 1, 0), Vector3D(0, 1, 0),
			 Vector3D(0, 1, 0), Vector3D(0, 0, 1), Vector3D(0, 0, -1)
		};

		Vector4D cube2DViewPorts[6] = {
			 Vector4D(), Vector4D(), Vector4D(),
			 Vector4D(), Vector4D(), Vector4D()
		};

		Matrix4D shadowMatrix;
		Shader::ptr shader = std::make_shared<Shader>("../../../src/Data/shader/shadowmapdepth.vs", "../../../src/Data/shader/shadowmapdepth.fs");
		for (auto light : lights)
		{
			if (!light->getCastShadow())
			{
				continue;
			}

			unsigned int faceCount;
			
			Light::LightType type = light->getType();
			Camera* shadowCamera = light->getShadowCamera();
			shadowCamera->updateMatrixWorld();
			Vector2D mapSize = light->getShadowMapSize();
			if (type == Light::PointLightType)
			{
				//cube map render target

				float vpWidth = mapSize.x;
				float vpHeight = mapSize.y;
				// positive X
				cube2DViewPorts[0] = Vector4D(vpWidth * 2, vpHeight, vpWidth, vpHeight);
				// negative X
				cube2DViewPorts[1] = Vector4D(0, vpHeight, vpWidth, vpHeight);
				// positive Z
				cube2DViewPorts[2] = Vector4D(vpWidth * 3, vpHeight, vpWidth, vpHeight);
				// negative Z
				cube2DViewPorts[3] = Vector4D(vpWidth, vpHeight, vpWidth, vpHeight);
				// positive Y
				cube2DViewPorts[4] = Vector4D(vpWidth * 3, 0, vpWidth, vpHeight);
				// negative Y
				cube2DViewPorts[5] = Vector4D(vpWidth, 0, vpWidth, vpHeight);

				mapSize.x *= 4.0;
				mapSize.y *= 2.0;

				faceCount = 6;

				Vector3D position = light->getPosition();

				shadowMatrix = Matrix4D::makeTranslation(-position.x, -position.y, -position.z);

			}
			else {
				/*tips: how to avoid shadow acne(because of resolution,one depth pixel correspoinding a region,so it show up 
				some fragmen greater than depth, and some fragment less than depth,so we can see stagger(交错) shadow),the method
				is rise depth (add a bias value 0.0005)
				
				*/
				faceCount = 1;
				
				Matrix4D view = shadowCamera->getViewMatrix();

				Matrix4D projection = shadowCamera->getProjectionMatrix();

				shadowMatrix = projection * view;

				
			}
			FrameBuffer::ptr fbo = std::make_shared<FrameBuffer>();		
			fbo->createFrameBufferWithTexture(mapSize.x, mapSize.y);

			Light::ShadowInfo shadowInfo;
			shadowInfo.shadowMatrix = shadowMatrix;
			shadowInfo.depthFBO = fbo;
			light->setShadowInfo(shadowInfo);
			type == Light::PointLightType ? fbo->bingForWriting(false): fbo->bingForWriting();
			for (int i = 0; i < faceCount; i++)
			{
				if (type == Light::PointLightType)
				{
					shadowCamera->lookAt(shadowCamera->getPosition(), shadowCamera->getPosition + cubeDirections[i], cubeUps[i]);
					//shadowMatrix = shadowCamera->getProjectionMatrix() * shadowCamera->getViewMatrix();
					RenderState::setViewPort(cube2DViewPorts[i].x, cube2DViewPorts[i].y, cube2DViewPorts[i].z, cube2DViewPorts[i].w);
				}

				
				for (auto mesh : meshs)
				{
					shader->use();
					shader->setMat4("viewMatrix", shadowCamera->getViewMatrix());
					shader->setMat4("projectionMatrix", shadowCamera->getProjectionMatrix());
					mesh->drawPrimitive();
				}

				//这里要存储shadowmatrix以及depth map

			}
			
			
		}
	}
}