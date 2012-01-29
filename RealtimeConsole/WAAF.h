#ifndef WAAF_H
#define WAAF_H

// Wonko's Ascii Adventure File

#include <string>
#include "Vector3f.h"

class AsciiRenderer;

class WAAF
{
	static const unsigned int VERSION = 1;

public:
	WAAF();
	~WAAF();

	const bool LoadFromFile(const std::string& filename);

	void Render(AsciiRenderer& renderer) const;

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

	struct PhysMesh
	{
		unsigned int numVertices;
		typedef Vector3f Vertex;
		Vertex* vertices;

		unsigned int numFaces;
		struct Face
		{
			unsigned int indices[3];
		};
		Face* faces;
	};
	PhysMesh m_PhysMesh;
};

#endif
