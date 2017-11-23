#include "../SRE/RenderSystem.h"
#include "../SRE/TextureManager.h"
#include "../SRE/ShaderManager.h"
#include "../SRE/PerspectiveCamera.h"
#include "../SRE/Mesh.h"
#include "../SRE/Light.h"
#include "../SRE/Scene.h"
#include "../Utils/CamerControl.h"
#include "../Utils/Event.h"
#include "../Math/MathHelper.h"
#include "../SRE/GeometryFactory.h"
#include "../SRE/Win.h"
#include "../SRE/BillboardCollection.h"
#include "../SRE/TerrianTile.h"
#include "../SRE/ParticleSystem.h"
#include <vector>
#include <iostream>
#include <string>
using namespace SRE;
using namespace std;
using namespace Utils;
using namespace Math;
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

