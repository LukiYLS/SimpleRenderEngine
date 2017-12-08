#include "../Core/RenderSystem.h"
#include "../Core/TextureManager.h"
#include "../Core/ShaderManager.h"
#include "../Core/PerspectiveCamera.h"
#include "../Core/Mesh.h"
#include "../Light/Light.h"
#include "../Core/Scene.h"
#include "../Utils/CamerControl.h"
#include "../Utils/Event.h"
#include "../Math/MathHelper.h"
#include "../Core/GeometryFactory.h"
#include "../Core/Win.h"
#include "../Core/BillboardCollection.h"
#include "../Core/TerrianTile.h"
#include "../Core/ParticleSystem.h"
#include <vector>
#include <iostream>
#include <string>
using namespace SRE;
using namespace std;
using namespace Utils;
using namespace Math;
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

