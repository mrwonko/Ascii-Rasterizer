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
	static const unsigned int VERSION = 3;

public:
	WAAF();
	~WAAF();

	const bool LoadFromFile(const std::string& filename);

	void Render(AsciiRenderer& renderer) const;

	btBvhTriangleMeshShape* GetPhysMesh() { return m_OtherPhysMesh; }

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

	//todo: gameplay info goes here

	btTriangleIndexVertexArray* m_PhysMesh;
	btBvhTriangleMeshShape* m_OtherPhysMesh;
};

#endif
