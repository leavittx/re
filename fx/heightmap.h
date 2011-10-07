#pragma once

#include "../globals.h"


/////////////////////////////////////////////////////////////////
// Terrain class
//////////////////////////////////////////////////////////////////

//! enumeration for patch sizes specifying the size of patches in the TerrainSceneNode
enum E_TERRAIN_PATCH_SIZE {
	//! patch size of 9, at most, use 4 levels of detail with this patch size.
	ETPS_9 = 9,
	//! patch size of 17, at most, use 5 levels of detail with this patch size.
	ETPS_17 = 17,
	//! patch size of 33, at most, use 6 levels of detail with this patch size.
	ETPS_33 = 33,
	//! patch size of 65, at most, use 7 levels of detail with this patch size.
	ETPS_65 = 65,
	//! patch size of 129, at most, use 8 levels of detail with this patch size.
	ETPS_129 = 129
};

class Terrain
{
public:
	Terrain(int maxLOD = 4, E_TERRAIN_PATCH_SIZE patchSize = ETPS_17,
		const remath::Vector3f& position = remath::Vector3f(0.0f, 0.0f, 0.0f),
		const remath::Vector3f& rotation = remath::Vector3f(0.0f, 0.0f, 0.0f),
		const remath::Vector3f& scale = remath::Vector3f(1.0f, 1.0f, 1.0f));

	~Terrain();

	bool loadHeightMap(const std::string& path, remath::Color4 vertexColor = remath::Color4(1.0f, 1.0f, 1.0f, 1.0f), int smoothFactor = 0);
	render::GLBatchBase& getMesh() { return Mesh; }

private:
	struct SPatch
	{
		SPatch()
			: Top(0), Bottom(0), Right(0), Left(0), CurrentLOD(-1)
		{
		}

		SPatch* Top;
		SPatch* Bottom;
		SPatch* Right;
		SPatch* Left;
		int CurrentLOD;
		remath::aabbox3df BoundingBox;
		remath::Vector3f Center;
	};

	struct STerrainData
	{
		STerrainData(int patchSize, int maxLOD, const remath::Vector3f& position, const remath::Vector3f& rotation, const remath::Vector3f& scale)
			: Patches(0), Size(0), Position(position), Rotation(rotation),
			  Scale(scale), PatchSize(patchSize), CalcPatchSize(patchSize-1),
			  PatchCount(0), MaxLOD(maxLOD)
		{
		}

		SPatch*		Patches;
		int		Size;
		remath::Vector3f	Position;
		remath::Vector3f	Rotation;
		remath::Vector3f RotationPivot;
		remath::Vector3f	Scale;
		remath::Vector3f Center;
		int		PatchSize;
		int		CalcPatchSize;
		int		PatchCount;
		int		MaxLOD;
		remath::aabbox3df	BoundingBox;
		std::vector<double> LODDistanceThreshold;
	};

	//! smooth the terrain
	void smoothTerrain(int smoothFactor);
	//! create patches, stuff that needs to only be done once for patches goes here.
	void createPatches();
	//! calculate the internal STerrainData structure
	void calculatePatchData();
	//! calculate or recalculate the distance thresholds
	void calculateDistanceThresholds(bool scalechanged = false);

	STerrainData TerrainData;
//	SMesh* Mesh;
	render::GLTriangleBatch Mesh;

//	IDynamicMeshBuffer *RenderBuffer;

	unsigned int VerticesToRender;
	unsigned int IndicesToRender;

	bool DynamicSelectorUpdate;
	bool OverrideDistanceThreshold;
	bool UseDefaultRotationPivot;
	bool ForceRecalculation;

	remath::Vector3f	OldCameraPosition;
	remath::Vector3f	OldCameraRotation;
	remath::Vector3f	OldCameraUp;
	float				OldCameraFOV;
	float CameraMovementDelta;
	float CameraRotationDelta;
	float CameraFOVDelta;

	// needed for (de)serialization
	float TCoordScale1;
	float TCoordScale2;
	std::string HeightmapFile;
//	io::IFileSystem* FileSystem;

	//TODO: make separate color handling class
	float getLightness(unsigned int color) const;
};


/////////////////////////////////////////////////////////////////
// Main scene class
//////////////////////////////////////////////////////////////////

class HeightMapScene : public redemo::Scene, recore::KeyboardEventsListener
{
public:
	HeightMapScene() {}
	~HeightMapScene() {}

	void init();
	void update();
	void draw();
	void release();

	virtual void handleKeyboardEvent(recore::Key key);

private:
	Terrain terrain;

	bool m_wireframe;
	bool m_points;

	render::GLFrame m_cameraFrame;
	render::GLMatrixStack m_modelViewMatrix;
	render::GLMatrixStack m_projectionMatrix;
	render::GLGeometryTransform m_transformPipeline;
};
