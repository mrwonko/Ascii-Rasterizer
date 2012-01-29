#include "WAAF.h"
#include <fstream>
#include <cstring> //NULL

WAAF::WAAF()
{
	m_RenderMesh.numFaces = 0;
	m_RenderMesh.faces = NULL;

	m_PhysMesh.numVertices = 0;
	m_PhysMesh.vertices = NULL;
	m_PhysMesh.numFaces = 0;
	m_PhysMesh.faces = NULL;
}

WAAF::~WAAF()
{
	delete[] m_RenderMesh.faces;
	delete[] m_PhysMesh.vertices;
	delete[] m_PhysMesh.faces;
}

const bool WAAF::LoadFromFile(const std::string& filename)
{
	std::ifstream file(filename.c_str(), std::ios::in | std::ios::binary);
	if(file.fail())
	{
		return false;
	}
	std::string ID;
	{
		char buf[4];
		file.read(buf, 4);
		ID = std::string(buf, 4);
	}

	//  Read Header
	if(ID != "WAAF")
	{
		return false;
	}
	unsigned int version;
	file >> version;
	if(version != VERSION)
	{
		return false;
	}

	//  Read RenderMesh
	file >> m_RenderMesh.numFaces;
	if(file.fail())
	{
		return false;
	}
	m_RenderMesh.faces = new RenderMesh::Face[m_RenderMesh.numFaces];
	for(unsigned int i = 0; i < m_RenderMesh.numFaces; ++i)
	{
		file >> m_RenderMesh.faces[i].material;
		for(unsigned int j = 0; j < 3; ++j)
		{
			file >> m_RenderMesh.faces[i].vertices[j].X >> m_RenderMesh.faces[i].vertices[j].Y >> m_RenderMesh.faces[i].vertices[j].Z;
		}
	}

	//TODO: Read gameplay data

	//  Read PhysMesh
	//vertices
	file >> m_PhysMesh.numVertices;
	if(file.fail())
	{
		delete[] m_RenderMesh.faces;
		m_RenderMesh.faces = NULL;
		return false;
	}
	m_PhysMesh.vertices = new PhysMesh::Vertex[m_PhysMesh.numVertices];
	for(unsigned int i = 0; i < m_PhysMesh.numVertices; ++i)
	{
		file >> m_PhysMesh.vertices[i].X >> m_PhysMesh.vertices[i].Y >> m_PhysMesh.vertices[i].Z;
	}
	//faces
	file >> m_PhysMesh.numFaces;
	if(file.fail())
	{
		delete[] m_RenderMesh.faces;
		m_RenderMesh.faces = NULL;
		delete[] m_PhysMesh.vertices;
		m_PhysMesh.vertices = NULL;
		return false;
	}
	m_PhysMesh.faces = new PhysMesh::Face[m_PhysMesh.numFaces];
	for(unsigned int i = 0; i < m_PhysMesh.numFaces; ++i)
	{
		file >> m_PhysMesh.faces[i].indices[0] >>m_PhysMesh.faces[i].indices[1] >> m_PhysMesh.faces[i].indices[2];
	}
	if(file.fail())
	{
		delete[] m_RenderMesh.faces;
		m_RenderMesh.faces = NULL;
		delete[] m_PhysMesh.vertices;
		m_PhysMesh.vertices = NULL;
		delete[] m_PhysMesh.faces;
		m_PhysMesh.faces = NULL;
		return false;
	}
	return true;
}