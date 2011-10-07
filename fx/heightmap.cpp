#include "heightmap.h"
#include "core/system.h"
#include "core/time.h"
#include "util/imagefactory.h"

using namespace redemo;
using namespace recore;
using namespace render;
using namespace remath;
using namespace reutil;
using namespace std;

void HeightMapScene::init()
{
	terrain.loadHeightMap("data/graphics/terrain-heightmap.bmp");

	m_projectionMatrix.LoadMatrix(Matrix4f(gl::m_viewFrustum.GetProjectionMatrix()).ptr());
	m_transformPipeline.SetMatrixStacks(m_modelViewMatrix, m_projectionMatrix);

	m_wireframe = false;
	m_points = false;

	InputManager::inst().acceptKeyboardEvents(this);
}

void HeightMapScene::release()
{
}

void HeightMapScene::update()
{
}

void HeightMapScene::draw()
{
	gl::clear(gl::ALL);

	// Save the current modelview matrix (the identity matrix)
	m_modelViewMatrix.PushMatrix();

	M3DMatrix44f mCamera;
	m_cameraFrame.GetCameraMatrix(mCamera);
	m_modelViewMatrix.PushMatrix(mCamera);

	m_modelViewMatrix.Translate(0.0f, 0.0f, -2.5f);

	ShaderManager::inst().UseStockShader(GLT_SHADER_SHADED,
										 StockShaderUniforms(Matrix4f(m_transformPipeline.GetModelViewProjectionMatrix()).ptr()));

//	GLfloat vRed[] = { 0.0f, 1.0f, 0.0f, 1.0f };

//	ShaderManager::inst().UseStockShader(GLT_SHADER_FLAT,
//										 StockShaderUniforms(Matrix4f(m_transformPipeline.GetModelViewProjectionMatrix()).ptr(), vRed));

	terrain.getMesh().Draw();

	// Restore the previous modleview matrix (the identity matrix)
	m_modelViewMatrix.PopMatrix();
	m_modelViewMatrix.PopMatrix();
}

void HeightMapScene::handleKeyboardEvent(Key key)
{
	float linear = 0.5f;
	float angular = deg2rad(10.0f);

	switch (key) {
	case KeyTab:
	{
		m_wireframe = !m_wireframe;
		if (m_wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	}
	case KeyCapsLock:
	{
		m_points = !m_points;
		if (m_points)
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	}
	case KeyUp:			m_cameraFrame.MoveForward(linear); break;
	case KeyDown:		m_cameraFrame.MoveForward(-linear); break;
	case KeyRight:		m_cameraFrame.MoveRight(-linear); break;
	case KeyLeft:		m_cameraFrame.MoveRight(linear); break;
	case KeyPageUp:		m_cameraFrame.MoveUp(linear); break;
	case KeyPageDown:	m_cameraFrame.MoveUp(-linear); break;
	case KeyW:			m_cameraFrame.RotateWorld( angular, 0.0f, 1.0f, 0.0f); break;
	case KeyS:			m_cameraFrame.RotateWorld(-angular, 0.0f, 1.0f, 0.0f); break;
	case KeyA:			m_cameraFrame.RotateWorld( angular, 0.0f, 0.0f, 1.0f); break;
	case KeyD:			m_cameraFrame.RotateWorld(-angular, 0.0f, 0.0f, 1.0f); break;
	case KeyQ:			m_cameraFrame.RotateWorld( angular, 1.0f, 0.0f, 0.0f); break;
	case KeyE:			m_cameraFrame.RotateWorld(-angular, 1.0f, 0.0f, 0.0f); break;
	default: return;
	}
}


/////////////////////////////////////////////////////////////////
// Terrain class methods
//////////////////////////////////////////////////////////////////

Terrain::Terrain(int maxLOD, E_TERRAIN_PATCH_SIZE patchSize,
				 const remath::Vector3f& position,
				 const remath::Vector3f& rotation,
				 const remath::Vector3f& scale)
	: TerrainData(patchSize, maxLOD, position, rotation, scale),// RenderBuffer(0),
	  VerticesToRender(0), IndicesToRender(0), DynamicSelectorUpdate(false),
	  OverrideDistanceThreshold(false), UseDefaultRotationPivot(true), ForceRecalculation(true),
	  CameraMovementDelta(10.0f), CameraRotationDelta(1.0f), CameraFOVDelta(0.1f),
	  TCoordScale1(1.0f), TCoordScale2(1.0f)
{
	//	Mesh = new SMesh();
	//	RenderBuffer = new CDynamicMeshBuffer(video::EVT_2TCOORDS, video::EIT_16BIT);
	//	RenderBuffer->setHardwareMappingHint(scene::EHM_STATIC, scene::EBT_VERTEX);
	//	RenderBuffer->setHardwareMappingHint(scene::EHM_DYNAMIC, scene::EBT_INDEX);
}

Terrain::~Terrain()
{
	delete [] TerrainData.Patches;


	//	if (Mesh)
	//		Mesh->drop();

	//	if (RenderBuffer)
	//		RenderBuffer->drop();
}

bool Terrain::loadHeightMap(const string& path, Color4 vertexColor, int smoothFactor)
{
	HeightmapFile = path;

	if (HeightmapFile.empty())
		return false;

	Image* heightMap = ImageFactory::loadBMP(HeightmapFile);

	if (!heightMap)
	{
		g_debug << "Unable to load heightmap." << endl;
		return false;
	}

	// Get the dimension of the heightmap data
	TerrainData.Size = heightMap->getWidth();

	switch (TerrainData.PatchSize)
	{
	case ETPS_9:
		if (TerrainData.MaxLOD > 3)
		{
			TerrainData.MaxLOD = 3;
		}
		break;
	case ETPS_17:
		if (TerrainData.MaxLOD > 4)
		{
			TerrainData.MaxLOD = 4;
		}
		break;
	case ETPS_33:
		if (TerrainData.MaxLOD > 5)
		{
			TerrainData.MaxLOD = 5;
		}
		break;
	case ETPS_65:
		if (TerrainData.MaxLOD > 6)
		{
			TerrainData.MaxLOD = 6;
		}
		break;
	case ETPS_129:
		if (TerrainData.MaxLOD > 7)
		{
			TerrainData.MaxLOD = 7;
		}
		break;
	}

	const unsigned int numVertices = TerrainData.Size * TerrainData.Size;

	Mesh.BeginMesh(numVertices);

	// Read the heightmap to get the vertex data
	// Apply positions changes, scaling changes
	const float tdSize = 1.0f/(float)(TerrainData.Size-1);
	int index = 0;
	float fx=0.f;
	float fx2=0.f;
	for (int x = 0; x < TerrainData.Size; ++x)
	{
		float fz=0.f;
		float fz2=0.f;
		for (int z = 0; z < TerrainData.Size; ++z)
		{
			static int i = 0;
			static Vector3f Normal[3], Vertex[3];
			static Vector2f TexCoord[3];

			//			video::S3DVertex2TCoords& vertex= static_cast<video::S3DVertex2TCoords*>(mb->getVertexBuffer().pointer())[index++];
			Normal[i] = Vector3f(0.0f, 1.0f, 0.0f);
			//			vertex.Normal.set(0.0f, 1.0f, 0.0f);
			//			vertex.Color = vertexColor;

			unsigned int pixel = heightMap->getData()[(TerrainData.Size-x-1) * TerrainData.Size + z];

			//			Vertex.x = fx;
			//			Vertex.y =
			//			Vertex.z = fz;

			Vertex[i].x = fx;
			Vertex[i].y = getLightness(pixel);
			Vertex[i].z = fz;

			//			vertex.Pos.X = fx;
			//			vertex.Pos.Y = (float) heightMap->getPixel(TerrainData.Size-x-1,z).getLightness();
			//			vertex.Pos.Z = fz;

			TexCoord[i].x = 1.f-fx2;
			TexCoord[i].y = fz2;

			//			vertex.TCoords.X = vertex.TCoords2.X = 1.f-fx2;
			//			vertex.TCoords.Y = vertex.TCoords2.Y = fz2;

			++fz;
			fz2 += tdSize;

			// Flush a triangle if counted three vertices
			i++;
			if (i == 3)
			{
				Mesh.AddTriangle(Vertex, Normal, TexCoord);
				i = 0;
			}
		}
		++fx;
		fx2 += tdSize;
	}

	smoothTerrain(smoothFactor);

	Mesh.End();

	// calculate smooth normals for the vertices
//	calculateNormals();

	// calculate all the necessary data for the patches and the terrain
	calculateDistanceThresholds();
	createPatches();
	calculatePatchData();

	return true;
}

//! smooth the terrain
void Terrain::smoothTerrain(int smoothFactor)
{
	const int Y = 1;

	for (int run = 0; run < smoothFactor; ++run)
	{
		int yd = TerrainData.Size;
		for (int y = 1; y < TerrainData.Size - 1; ++y)
		{
			for (int x = 1; x < TerrainData.Size - 1; ++x)
			{
				Mesh.getVerts()[x + yd][Y] =
					   (Mesh.getVerts()[x-1 + yd][Y] + //left
						Mesh.getVerts()[x+1 + yd][Y] + //right
						Mesh.getVerts()[x   + yd - TerrainData.Size][Y] + //above
						Mesh.getVerts()[x   + yd + TerrainData.Size][Y]) * 0.25f; //below

//				Mesh.pVerts[x + yd][1] =

//				        ->getVertexBuffer()[x + yd].Pos.Y =
//						(mb->getVertexBuffer()[x-1 + yd].Pos.Y + //left
//						 mb->getVertexBuffer()[x+1 + yd].Pos.Y + //right
//						 mb->getVertexBuffer()[x   + yd - TerrainData.Size].Pos.Y + //above
//						 mb->getVertexBuffer()[x   + yd + TerrainData.Size].Pos.Y) * 0.25f; //below
			}
			yd += TerrainData.Size;
		}
	}
}

//! create patches, stuff that needs to be done only once for patches goes here.
void Terrain::createPatches()
{
	TerrainData.PatchCount = (TerrainData.Size - 1) / (TerrainData.CalcPatchSize);

	if (TerrainData.Patches)
		delete [] TerrainData.Patches;

	TerrainData.Patches = new SPatch[TerrainData.PatchCount * TerrainData.PatchCount];
}

//! used to calculate the internal STerrainData structure both at creation and after scaling/position calls.
void Terrain::calculatePatchData()
{
#if 0
	// Reset the Terrains Bounding Box for re-calculation
	TerrainData.BoundingBox.reset(RenderBuffer->getPosition(0));

	for (int x = 0; x < TerrainData.PatchCount; ++x)
	{
		for (int z = 0; z < TerrainData.PatchCount; ++z)
		{
			const int index = x * TerrainData.PatchCount + z;
			SPatch& patch = TerrainData.Patches[index];
			patch.CurrentLOD = 0;

			const int xstart = x*TerrainData.CalcPatchSize;
			const int xend = xstart+TerrainData.CalcPatchSize;
			const int zstart = z*TerrainData.CalcPatchSize;
			const int zend = zstart+TerrainData.CalcPatchSize;
			// For each patch, calculate the bounding box (mins and maxes)
			patch.BoundingBox.reset(RenderBuffer->getPosition(xstart*TerrainData.Size + zstart));

			for (int xx = xstart; xx <= xend; ++xx)
				for (int zz = zstart; zz <= zend; ++zz)
					patch.BoundingBox.addInternalPoint(RenderBuffer->getVertexBuffer()[xx * TerrainData.Size + zz].Pos);

			// Reconfigure the bounding box of the terrain as a whole
			TerrainData.BoundingBox.addInternalBox(patch.BoundingBox);

			// get center of Patch
			patch.Center = patch.BoundingBox.getCenter();

			// Assign Neighbours
			// Top
			if (x > 0)
				patch.Top = &TerrainData.Patches[(x-1) * TerrainData.PatchCount + z];
			else
				patch.Top = 0;

			// Bottom
			if (x < TerrainData.PatchCount - 1)
				patch.Bottom = &TerrainData.Patches[(x+1) * TerrainData.PatchCount + z];
			else
				patch.Bottom = 0;

			// Left
			if (z > 0)
				patch.Left = &TerrainData.Patches[x * TerrainData.PatchCount + z - 1];
			else
				patch.Left = 0;

			// Right
			if (z < TerrainData.PatchCount - 1)
				patch.Right = &TerrainData.Patches[x * TerrainData.PatchCount + z + 1];
			else
				patch.Right = 0;
		}
	}

	// get center of Terrain
	TerrainData.Center = TerrainData.BoundingBox.getCenter();

	// if the default rotation pivot is still being used, update it.
	if (UseDefaultRotationPivot)
	{
		TerrainData.RotationPivot = TerrainData.Center;
	}
#endif
}

//! used to calculate or recalculate the distance thresholds
void Terrain::calculateDistanceThresholds(bool scalechanged)
{
	// Only update the LODDistanceThreshold if it's not manually changed
	if (!OverrideDistanceThreshold)
	{
		TerrainData.LODDistanceThreshold.clear();
		// Determine new distance threshold for determining what LOD to draw patches at
		TerrainData.LODDistanceThreshold.resize(TerrainData.MaxLOD);
		const double size = TerrainData.PatchSize * TerrainData.PatchSize *
				TerrainData.Scale.x * TerrainData.Scale.z;
		for (int i=0; i<TerrainData.MaxLOD; ++i)
		{
			TerrainData.LODDistanceThreshold.push_back(size * ((i+1+ i / 2) * (i+1+ i / 2)));
		}
	}
}

//! Get lightness of the color in the range [0,255]
float Terrain::getLightness(unsigned int color) const
{
	unsigned int r, g, b;

	r = (color>>16) & 0xff;
	g = (color>>8) & 0xff;
	b = color & 0xff;

	return 0.5f*(max_(max_(r, g), b) + min_(min_(r, g), b));
}
