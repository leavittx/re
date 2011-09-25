#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <math3d.h>
#include <GLBatchBase.h>
#include <GLShaderManager.h>

#define VERTEX_DATA     0
#define NORMAL_DATA     1
#define TEXTURE_DATA    2
#define INDEX_DATA      3

class GLTriangleBatch : public GLBatchBase
{
public:
	GLTriangleBatch(void);
	virtual ~GLTriangleBatch(void);

	// Use these three functions to add triangles
	void BeginMesh(GLuint nMaxVerts);
	void AddTriangle(M3DVector3f verts[3], M3DVector3f vNorms[3], M3DVector2f vTexCoords[3]);
	void End(void);

	// Useful for statistics
	inline GLuint GetIndexCount(void) { return nNumIndexes; }
	inline GLuint GetVertexCount(void) { return nNumVerts; }


	// Draw - make sure you call glEnableClientState for these arrays
	virtual void Draw(void);

protected:
	GLushort  *pIndexes;        // Array of indexes
	M3DVector3f *pVerts;        // Array of vertices
	M3DVector3f *pNorms;        // Array of normals
	M3DVector2f *pTexCoords;    // Array of texture coordinates

	GLuint nMaxIndexes;         // Maximum workspace
	GLuint nNumIndexes;         // Number of indexes currently used
	GLuint nNumVerts;           // Number of vertices actually used

	GLuint bufferObjects[4];
	GLuint vertexArrayBufferObject;
};
