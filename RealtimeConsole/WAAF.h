#ifndef WAAF_H
#define WAAF_H

// Wonko's Ascii Adventure File

#include <string>
#include "Vector3f.h"

class AsciiRenderer;
class btTriangleIndexVertexArray;
class btBvhTriangleMeshShape;

class WAAF
{
	static const unsigned int VERSION = 4;

public:
	WAAF();
	~WAAF();

	const bool LoadFromFile(const std::string& filename);

	void Render(AsciiRenderer& renderer) const;

	btBvhTriangleMeshShape* GetPhysMesh() { return m_OtherPhysMesh; }

	const Vector3f& GetPlayerPos() const { return m_PlayerPos; }

private:
	struct RenderMesh
	{
		unsigned int numFaces;
		struct Face
		{
			char material;
			Vector3f vertices[3];
		};
		Face* faces;
	};
	RenderMesh m_RenderMesh;

	Vector3f m_PlayerPos;

	btTriangleIndexVertexArray* m_PhysMesh;
	btBvhTriangleMeshShape* m_OtherPhysMesh;
};

#endif
