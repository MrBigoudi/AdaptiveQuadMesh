#include <string>
#include <fstream>
#include <sstream>

#include "mesh.hpp"
#include "vector3.hpp"

mesh::Mesh mesh::Mesh::loadOBJ(std::string file){
	std::vector<maths::Vector3*> vertices;
	std::vector<std::vector<int>> faces;
	
	std::ifstream objFile(file.c_str());
	if (!objFile){
        std::fprintf(stderr, "Error, failed to open %s!", file.c_str());
		throw std::invalid_argument("Need a file as input!");
    }

	// load vertices and faces
	std::string line;
	while (std::getline(objFile, line)) 
	{
		if (line.length() < 2)
			continue;

		else if (line[0] == 'v' && line[1] == ' ') 
		{
			// load a vertex
			line.erase(line.begin());
			std::istringstream iss(line);
            float x,y,z;
			iss >> x >> y >> z;
			maths::Vector3* v = new maths::Vector3(x,y,z);
			vertices.push_back(v);
		}
		else if (line[0] == 'f') 
		{
			// load a face
			line.erase(line.begin());
			faces.resize(faces.size() + 1);
			std::istringstream iss(line);
			int idx;
			char binBSlash;
			int binIdx;
			/*
			check if format:
			f v1 v2 v3
			or
			f v1/./. v2/./. v3/./.
			*/
			std::size_t found = line.find("/");
  			if (found == std::string::npos){
				while (iss >> idx)
					faces.back().push_back(idx - 1);
			} else {
				while (iss >> idx >> binBSlash >> binIdx >> binBSlash >> binIdx){
					faces.back().push_back(idx - 1);
				}
			}
		}
		else 
			continue;
	}

	// convert the mesh to winged-edge form
	return mesh::Mesh::objToMesh(vertices, faces);
}


mesh::Mesh mesh::Mesh::objToMesh(std::vector<maths::Vector3*> &vertices, std::vector<std::vector<int>> &faces){
	// create mesh::Vertex and save to vertexlist
	std::vector<mesh::Vertex*> vertexList;
	for (int i = 0; i < int(vertices.size()); i++)
		vertexList.push_back(new mesh::Vertex(i, vertices[i]));

	// create mesh::Edge and save to edge
    std::vector<mesh::Edge*> edgeList;
	for (int i = 0; i < int(faces.size()*3); i++)
		edgeList.push_back(new mesh::Edge());

	// create mesh::Face and save to face
    std::vector<mesh::Face*> faceList;
	for (int i = 0; i < int(faces.size()); i++)
		faceList.push_back(new mesh::Face());

	int vnum = faces[0].size();

	// use edgetable to record indices of start and end vertex, edge index
	int m = vertices.size();
	std::vector<std::vector<int>> edgeTable(m, std::vector<int>(m, -1));

	for (int i = 0; i < int(faces.size()); i++) 
	{
		mesh::Face *f = faceList[i];
		
		for (int v = 0; v < vnum; v++) 
		{
			int v0 = faces[i][v];
			mesh::Vertex *v_end = vertexList[v0];

			int v1 = faces[i][(v+1)%vnum];
			mesh::Vertex *v_start = vertexList[v1];

			mesh::Edge *edge = edgeList[vnum*i + v];
			edge->mVertexOrigin = v_start;
			edge->mVertexDestination = v_end;

			mesh::Edge *edge_nex = edgeList[vnum*i + (v - 1 + vnum) % vnum];
			edge->mEdgeRightCW = edge_nex;

			mesh::Edge *edge_pre = edgeList[vnum*i + (v + 1) % vnum];
			edge->mEdgeRightCCW = edge_pre;

			edgeTable[v1][v0] = vnum * i + v;
			edge->mFaceRight = f;

			f->mEdge = edge;
			v_start->mEdge = edge;
		   // v_end->mEdge = edge; 
		}
	}

	// save left according the table
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < m; j++)
		{
			int idx = edgeTable[i][j];
			if (idx != -1)
			{
				mesh::Edge *edge = edgeList[idx];
				int flipIdx = edgeTable[j][i];
				mesh::Edge *edgeFlip = edgeList[flipIdx];

				edge->mEdgeLeftCCW = edgeFlip->mEdgeRightCCW;
				edge->mEdgeLeftCW = edgeFlip->mEdgeRightCW; 

				edge->mFaceLeft = edgeFlip->mFaceRight;
			}
		}
	}

    return * new mesh::Mesh(vertices.size(), faces.size(), faces.size(), vertexList, faceList, edgeList);

}


std::vector<std::vector<mesh::Vertex *>> mesh::Mesh::verticesOfFaces(){
	// the vertices index
	std::vector<std::vector<mesh::Vertex *>> vertices(mFaces.size());

	for (int i = 0; i < int(mFaces.size()); i++)
	{
		mesh::Face *f = mFaces[i];
		mesh::Edge *e0 = f->mEdge;
		mesh::Edge *edge = e0;

		// find all the edges related to the face
		do
		{
			if (edge->mFaceRight == f)
				edge = edge->mEdgeRightCCW;
			vertices[i].push_back(edge->mVertexOrigin);
		} 
		while (edge != e0);
	}

	return vertices;
}


void mesh::Mesh::toObj(std::string file){
	// check whether we could create the file
	std::ofstream objFile(file);
	if (!objFile){
        std::fprintf(stderr, "Error, failed to open %s!", file.c_str());
		throw std::invalid_argument("Need a file as input!");
    }

	// extract the face list from winged-edge mesh
	std::vector<std::vector<mesh::Vertex *>> faces = verticesOfFaces();

	// export vertices
	for (int i = 0; i < int(mVertices.size()); i++) 
	{
		maths::Vector3* v = mVertices[i]->mCoords;
		objFile << "v " << v->x()
			<< " " << v->y()
			<< " " << v->z() << std::endl;
	}

	int idx;

	// export faces
	for (int i = 0; i < int(faces.size()); i++) 
	{
		objFile << "f";

		for (int j = 0; j < int(faces[i].size()); j++)
		{
			idx = faces[i][j]->mId;
			objFile << " " << idx + 1;
		}
		objFile << std::endl;
	}
}