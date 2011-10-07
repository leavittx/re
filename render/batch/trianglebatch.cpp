/*
 *  This class allows you to simply add triangles as if this class were a
 *  container. The AddTriangle() function searches the current list of triangles
 *  and determines if the vertex/normal/texcoord is a duplicate. If so, it addes
 *  an entry to the index array instead of the list of vertices.
 *  When finished, call EndMesh() to free up extra unneeded memory that is reserved
 *  as workspace when you call BeginMesh().
 *
 *  This class can easily be extended to contain other vertex attributes, and to
 *  save itself and load itself from disk (thus forming the beginnings of a custom
 *  model file format).
 *
 */

#include "trianglebatch.h"
#include "../shadermanager.h"

#ifdef __GLTOOLS_MY__

using namespace render;

///////////////////////////////////////////////////////////
// Constructor, does what constructors do... set everything to zero or NULL
GLTriangleBatch::GLTriangleBatch(void)
{
	pIndexes = NULL;
	pVerts = NULL;
	pNorms = NULL;
	pTexCoords = NULL;

	nMaxIndexes = 0;
	nNumIndexes = 0;
	nNumVerts = 0;
}

////////////////////////////////////////////////////////////
// Free any dynamically allocated memory. For those C programmers
// coming to C++, it is perfectly valid to delete a NULL pointer.
GLTriangleBatch::~GLTriangleBatch(void)
{
	// Just in case these still are allocated when the object is destroyed
	delete [] pIndexes;
	delete [] pVerts;
	delete [] pNorms;
	delete [] pTexCoords;

	// Delete buffer objects
	glDeleteBuffers(4, bufferObjects);

	glDeleteVertexArrays(1, &vertexArrayBufferObject);
}

////////////////////////////////////////////////////////////
// Start assembling a mesh. You need to specify a maximum amount
// of indexes that you expect. The EndMesh will clean up any uneeded
// memory. This is far better than shreading your heap with STL containers...
// At least that's my humble opinion.
void GLTriangleBatch::BeginMesh(GLuint nMaxVerts)
{
	// Just in case this gets called more than once...
	delete [] pIndexes;
	delete [] pVerts;
	delete [] pNorms;
	delete [] pTexCoords;

	nMaxIndexes = nMaxVerts;
	nNumIndexes = 0;
	nNumVerts = 0;

	// Allocate new blocks. In reality, the other arrays will be
	// much shorter than the index array
	pIndexes = new GLushort[nMaxIndexes];
	pVerts = new M3DVector3f[nMaxIndexes];
	pNorms = new M3DVector3f[nMaxIndexes];
	pTexCoords = new M3DVector2f[nMaxIndexes];
}

void GLTriangleBatch::AddTriangle(remath::Vector3f verts[3], remath::Vector3f vNorms[3], remath::Vector2f vTexCoords[3])
{
	M3DVector3f _verts[3], _vNorms[3];
	M3DVector2f _vTexCoords[3];

	for (int i = 0; i < 3; i++) {
		memcpy(_verts[i], &verts[i].x, sizeof(float) * 3);
		memcpy(_vNorms[0], &vNorms[0].x, sizeof(float) * 3);
		memcpy(_vTexCoords[0], &vTexCoords[0].x, sizeof(float) * 2);
	}

	AddTriangle(_verts, _vNorms, _vTexCoords);
}

/////////////////////////////////////////////////////////////////
// Add a triangle to the mesh. This searches the current list for identical
// (well, almost identical - these are floats you know...) verts. If one is found, it
// is added to the index array. If not, it is added to both the index array and the vertex
// array grows by one as well.
void GLTriangleBatch::AddTriangle(M3DVector3f verts[3], M3DVector3f vNorms[3], M3DVector2f vTexCoords[3])
{
	const  float e = 0.00001f; // How small a difference to equate

	// First thing we do is make sure the normals are unit length!
	// It's almost always a good idea to work with pre-normalized normals
	m3dNormalizeVector3(vNorms[0]);
	m3dNormalizeVector3(vNorms[1]);
	m3dNormalizeVector3(vNorms[2]);


	// Search for match - triangle consists of three verts
	for (GLuint iVertex = 0; iVertex < 3; iVertex++)
	{
		GLuint iMatch = 0;
		for (iMatch = 0; iMatch < nNumVerts; iMatch++)
		{
			// If the vertex positions are the same
			if (m3dCloseEnough(pVerts[iMatch][0], verts[iVertex][0], e) &&
			m3dCloseEnough(pVerts[iMatch][1], verts[iVertex][1], e) &&
			m3dCloseEnough(pVerts[iMatch][2], verts[iVertex][2], e) &&

			// AND the Normal is the same...
			m3dCloseEnough(pNorms[iMatch][0], vNorms[iVertex][0], e) &&
			m3dCloseEnough(pNorms[iMatch][1], vNorms[iVertex][1], e) &&
			m3dCloseEnough(pNorms[iMatch][2], vNorms[iVertex][2], e) &&

			// And Texture is the same...
			m3dCloseEnough(pTexCoords[iMatch][0], vTexCoords[iVertex][0], e) &&
			m3dCloseEnough(pTexCoords[iMatch][1], vTexCoords[iVertex][1], e))
			{
				// Then add the index only
				pIndexes[nNumIndexes] = iMatch;
				nNumIndexes++;
				break;
			}
		}

		// No match for this vertex, add to end of list
		if (iMatch == nNumVerts && nNumVerts < nMaxIndexes && nNumIndexes < nMaxIndexes)
		{
			memcpy(pVerts[nNumVerts], verts[iVertex], sizeof(M3DVector3f));
			memcpy(pNorms[nNumVerts], vNorms[iVertex], sizeof(M3DVector3f));
			memcpy(pTexCoords[nNumVerts], vTexCoords[iVertex], sizeof(M3DVector2f));
			pIndexes[nNumIndexes] = nNumVerts;
			nNumIndexes++;
			nNumVerts++;
		}
	}
}



//////////////////////////////////////////////////////////////////
// Compact the data. This is a nice utility, but you should really
// save the results of the indexing for future use if the model data
// is static (doesn't change).
void GLTriangleBatch::End(void)
{
	// Create the master vertex array object
	glGenVertexArrays(1, &vertexArrayBufferObject);
	glBindVertexArray(vertexArrayBufferObject);

	// Create the buffer objects
	glGenBuffers(4, bufferObjects);

	// Copy data to video memory
	// Vertex data
	glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[VERTEX_DATA]);
	glEnableVertexAttribArray(GLT_ATTRIBUTE_VERTEX);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*nNumVerts*3, pVerts, GL_STATIC_DRAW);
	glVertexAttribPointer(GLT_ATTRIBUTE_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, 0);


	// Normal data
	glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[NORMAL_DATA]);
	glEnableVertexAttribArray(GLT_ATTRIBUTE_NORMAL);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*nNumVerts*3, pNorms, GL_STATIC_DRAW);
	glVertexAttribPointer(GLT_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Texture coordinates
	glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[TEXTURE_DATA]);
	glEnableVertexAttribArray(GLT_ATTRIBUTE_TEXTURE0);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*nNumVerts*2, pTexCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(GLT_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Indexes
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObjects[INDEX_DATA]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*nNumIndexes, pIndexes, GL_STATIC_DRAW);


	// Done
	glBindVertexArray(0);

	// Free older, larger arrays
	delete [] pIndexes;
	delete [] pVerts;
	delete [] pNorms;
	delete [] pTexCoords;

	// Reasign pointers so they are marked as unused
	pIndexes = NULL;
	pVerts = NULL;
	pNorms = NULL;
	pTexCoords = NULL;

	// Unbind to anybody
	glBindVertexArray(0);
}

//////////////////////////////////////////////////////////////////////////
// Draw - make sure you call glEnableClientState for these arrays
void GLTriangleBatch::Draw(void)
{
	glBindVertexArray(vertexArrayBufferObject);

	glDrawElements(GL_TRIANGLES, nNumIndexes, GL_UNSIGNED_SHORT, 0);

	// Unbind to anybody
	glBindVertexArray(0);
}

#endif // __GLTOOLS_MY__
