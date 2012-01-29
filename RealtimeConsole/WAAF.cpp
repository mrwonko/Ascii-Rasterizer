#include "WAAF.h"
#include <fstream>
#include <cstring> //NULL
#include <iostream>
#include "AsciiRenderer.h"
#include "btBulletDynamicsCommon.h"

WAAF::WAAF() :
	m_PhysMesh(NULL),
	m_OtherPhysMesh(NULL)
{
	m_RenderMesh.numFaces = 0;
	m_RenderMesh.faces = NULL;
}

WAAF::~WAAF()
{
	delete[] m_RenderMesh.faces;
	delete m_PhysMesh;
	delete m_OtherPhysMesh;
}

const bool WAAF::LoadFromFile(const std::string& filename)
{
	std::ifstream file(filename.c_str(), std::ios::in | std::ios::binary);
	if(file.fail())
	{
		std::cerr<<"Could not open file!"<<std::endl;
		return false;
	}
	std::string ID;
	{
		char buf[4];
		file.read(buf, 4);
		ID = std::string(buf, 4);
	}
	if(file.fail())
	{
		std::cerr<<"Could not read ID!"<<std::endl;
		return false;
	}

	//  Read Header
	if(ID != "WAAF")
	{
		std::cerr<<"No WAAF file!"<<std::endl;
		return false;
	}
	unsigned int version;
	file.read(reinterpret_cast<char*>(&version), 4);
	if(file.fail())
	{
		std::cerr<<"Could not read Version!"<<std::endl;
		return false;
	}
	if(version != VERSION)
	{
		std::cerr<<"Wrong file version! (" << version << " should be " << VERSION << ")"<<std::endl;
		return false;
	}

	//  Read RenderMesh
	file.read(reinterpret_cast<char*>(&m_RenderMesh.numFaces), 4);
	if(file.fail())
	{
		std::cerr<<"Invalid file!"<<std::endl;
		return false;
	}
	m_RenderMesh.faces = new RenderMesh::Face[m_RenderMesh.numFaces];
	for(unsigned int i = 0; i < m_RenderMesh.numFaces; ++i)
	{
		file.read(&m_RenderMesh.faces[i].material, 1);
		for(unsigned int j = 0; j < 3; ++j)
		{
			file.read(reinterpret_cast<char*>(&m_RenderMesh.faces[i].vertices[j].X), 4);
			file.read(reinterpret_cast<char*>(&m_RenderMesh.faces[i].vertices[j].Y), 4);
			file.read(reinterpret_cast<char*>(&m_RenderMesh.faces[i].vertices[j].Z), 4);
		}
	}

	file.read(reinterpret_cast<char*>(&m_PlayerPos.X), sizeof(float));
	file.read(reinterpret_cast<char*>(&m_PlayerPos.Y), sizeof(float));
	file.read(reinterpret_cast<char*>(&m_PlayerPos.Z), sizeof(float));
	m_PlayerPos.Y += 0.75f; //half player height, fixme: un-hardcode this (= softcode?)

	//  Read PhysMesh
	{
		char buf[4];
		file.read(buf, 4);
		ID = std::string(buf, 4);
	}
	if(ID != "PHYS")
	{
		delete[] m_RenderMesh.faces;
		m_RenderMesh.faces = NULL;
		std::cerr<<"Invalid file!"<<std::endl;
		return false;
	}

	int numPhysVertices;

	//vertices
	file.read(reinterpret_cast<char*>(&numPhysVertices), 4);
	if(file.fail())
	{
		delete[] m_RenderMesh.faces;
		m_RenderMesh.faces = NULL;
		std::cerr<<"Invalid file!"<<std::endl;
		return false;
	}
	if(numPhysVertices == 0)
	{
		delete[] m_RenderMesh.faces;
		m_RenderMesh.faces = NULL;
		std::cerr<<"Error: 0 vertices!"<<std::endl;
		return false;
	}
	float* physVertices = new float[numPhysVertices*3];
	file.read(reinterpret_cast<char*>(physVertices), 4*3*numPhysVertices);

	//faces
	int numPhysIndices;
	file.read(reinterpret_cast<char*>(&numPhysIndices), 4);
	if(file.fail())
	{
		delete[] m_RenderMesh.faces;
		m_RenderMesh.faces = NULL;
		delete[] physVertices;
		std::cerr<<"Invalid file!"<<std::endl;
		return false;
	}
	if(numPhysIndices == 0)
	{
		delete[] m_RenderMesh.faces;
		m_RenderMesh.faces = NULL;
		delete[] physVertices;
		std::cerr<<"Error: 0 tris!"<<std::endl;
		return false;
	}
	int* physIndices = new int[3*numPhysIndices];
	file.read(reinterpret_cast<char*>(physIndices), 4*3*numPhysIndices);
	if(file.fail())
	{
		delete[] m_RenderMesh.faces;
		m_RenderMesh.faces = NULL;
		delete[] physVertices;
		delete[] physIndices;
		std::cerr<<"Invalid file!"<<std::endl;
		return false;
	}
	m_PhysMesh = new btTriangleIndexVertexArray(numPhysIndices, physIndices, 3*sizeof(int), numPhysVertices, physVertices, 3*sizeof(float));
	m_OtherPhysMesh = new btBvhTriangleMeshShape(m_PhysMesh, true);
	return true;
}

void WAAF::Render(AsciiRenderer& renderer) const
{
	for(unsigned int i = 0; i < m_RenderMesh.numFaces; ++i)
	{
		renderer.SetMaterial(m_RenderMesh.faces[i].material);
		renderer.DrawTriangle(m_RenderMesh.faces[i].vertices[0], m_RenderMesh.faces[i].vertices[1], m_RenderMesh.faces[i].vertices[2]);
	}
}
