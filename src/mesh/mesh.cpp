#include <cstdio>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cassert>
#include <vector>

#include "edge.hpp"
#include "face.hpp"
#include "mesh.hpp"
#include "vector3.hpp"

mesh::Mesh mesh::Mesh::loadOBJ(std::string file){
	std::vector<maths::Vector3*> vertices;
	std::vector<std::vector<int>> faces;
	
	std::ifstream objFile(file.c_str());
	if (!objFile){
        std::fprintf(stderr, "Error, failed to open %s!\n", file.c_str());
		throw std::invalid_argument("Need a file as input!\n");
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
	int nbVertices = int(vertices.size());
	for (int i = 0; i < nbVertices; i++)
		vertexList.push_back(new mesh::Vertex(vertices[i]));

	// create mesh::Edge and save to edge
    std::vector<mesh::Edge*> edgeList;
	int nbEdges = int(faces.size()*3);
	for (int i = 0; i < nbEdges; i++)
		edgeList.push_back(new mesh::Edge());

	// create mesh::Face and save to face
    std::vector<mesh::Face*> faceList;
	int nbFaces = int(faces.size());
	for (int i = 0; i < nbFaces; i++)
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
			mesh::Vertex *v_start = vertexList[v0];

			int v1 = faces[i][(v+1)%vnum];
			mesh::Vertex *v_end = vertexList[v1];

			mesh::Edge *edge = edgeList[vnum*i + v];
			edge->mVertexOrigin = v_start;
			edge->mVertexDestination = v_end;

			mesh::Edge *edge_nex = edgeList[vnum*i + (v +1) % vnum];
			edge->mEdgeLeftCCW = edge_nex;

			mesh::Edge *edge_pre = edgeList[vnum*i + (v - 1 + vnum) % vnum];
			edge->mEdgeLeftCW = edge_pre;

			edgeTable[v0][v1] = vnum * i + v;
			edge->mFaceLeft = f;

			f->mEdge = edge;
			v_start->mEdge = edge;
		   // v_end->mEdge = edge; 
		}
	}

	// save left according to the table
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

				edge->mEdgeRightCW = edgeFlip->mEdgeLeftCW;
				edge->mEdgeRightCCW = edgeFlip->mEdgeLeftCCW; 

				edge->mFaceRight = edgeFlip->mFaceLeft;
				edge->mReverseEdge = edgeFlip;
			}
		}
	}

	assert(nbVertices == int(vertexList.size()));
	assert(nbFaces == int(faceList.size()));
	assert(nbEdges == int(edgeList.size()));

    return * new mesh::Mesh(nbVertices, nbFaces, nbEdges, vertexList, faceList, edgeList);

}


std::vector<std::vector<mesh::Vertex *>> mesh::Mesh::verticesOfFaces(){
	// the vertices index
	std::vector<std::vector<mesh::Vertex *>> vertices(mFaces.size());

	for (int i = 0; i < int(mFaces.size()); i++)
		vertices[i] = mFaces[i]->getSurroundingVertices();

	return vertices;
}


void mesh::Mesh::toObj(std::string file){
	// check whether we could create the file
	std::ofstream objFile(file);
	if (!objFile){
        std::fprintf(stderr, "Error, failed to open %s!\n", file.c_str());
		throw std::invalid_argument("Need a file as input!\n");
    }

	// extract the face list from winged-edge mesh
	std::vector<std::vector<mesh::Vertex *>> faces = verticesOfFaces();

	// put number of vertices and faces as commtent
	objFile << "# " << mNbVertices << " " << mNbFaces << std::endl;

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

void mesh::Mesh::triToQuadRemovalMarkingPhase(){
	// for all faces
	for (int i=0; i<int(mFaces.size()); i++){
		mesh::Edge* edgeToRemove = nullptr;
		int maxSquared = -1;

		// for all edges surrounding the face
		std::vector<mesh::Edge*> surEdges = mFaces[i]->getSurroundingEdges();
		// print();
		for (int j=0; j<int(surEdges.size()); j++){
			// if the edge is removable
			if(surEdges[j]->isRemovable()){
				// get the sum of pairwised dot product
				int sumDotProd = mesh::Edge::getSumPairwiseDotProd(surEdges);
				// update max sum
				if(sumDotProd > maxSquared){
					maxSquared = sumDotProd;
					edgeToRemove = surEdges[j];
				}
			}
		}

		// if there is a removable face, set its flag to True and update flags for its faces
		if(edgeToRemove){
			edgeToRemove->mToDelete = true;
			edgeToRemove->mFaceLeft->mToMerge = true;
			edgeToRemove->mFaceRight->mToMerge = true;
			edgeToRemove->mReverseEdge->mToDelete = true;
			edgeToRemove = nullptr;
		}
	}	
}

void mesh::Mesh::removeFaceFromList(mesh::Face* face){
	for(int i = 0; i<int(mFaces.size()); i++){
		if(mFaces[i]->mId == face->mId){
			mFaces.erase(mFaces.begin()+i);
		}
	}
	// delete face;
}

void mesh::Mesh::removeEdgeFromList(mesh::Edge* edge){
	for(int i = 0; i<int(mEdges.size()); i++){
		if(mEdges[i]->mId == edge->mId){
			mEdges.erase(mEdges.begin()+i);
		}
	}
	// delete edge;
}

void mesh::Mesh::removeEdge(mesh::Edge* edge){
	// print();
	// printf("\n");
	// edge->print();
	// fixing vertices
	if(edge->mVertexOrigin->mEdge->mId == edge->mId)
		edge->mVertexOrigin->mEdge = edge->mEdgeLeftCW;
	if(edge->mVertexDestination->mEdge->mId == edge->mId)
		edge->mVertexDestination->mEdge = edge->mEdgeRightCW;

	// check if reversed edge has already been taken care of
	if(edge->mFaceLeft != edge->mFaceRight){
		// fixing faces
		mesh::Face* leftFace = edge->mFaceLeft;
		mesh::Face* rightFace = edge->mFaceRight;
		// leftFace->print();
		// rightFace->print();
		// update faces' edges to update the new face
		if(leftFace->mEdge->mId == edge->mId)
			leftFace->mEdge = edge->mEdgeLeftCW;
		if(rightFace->mEdge->mId == edge->mId)
			rightFace->mEdge = edge->mEdgeRightCW;

		// fusioning faces
		mesh::Face* newFace = leftFace->mergeFace(rightFace);
		// edge->print();

		// deleting the faces from the list
		removeFaceFromList(leftFace);
		// edge->print();
		removeFaceFromList(rightFace);
		// edge->print();
		mNbFaces--;

		// adding the new face to the list
		mFaces.push_back(newFace);
	}

	// fixing edges
	edge->updateAllNeighbours();
	removeEdgeFromList(edge);

	mNbEdges--;
}

void mesh::Mesh::removeMarkedEdges(){
	// making a list of edges to remove
	std::vector<mesh::Edge*> edgeList;
	for(int i=0; i<int(mEdges.size()); i++){
		if(mEdges[i]->mToDelete) edgeList.push_back(mEdges[i]);
	}
	// print();
	// removing each edge of the list
	while(edgeList.size() != 0){
		mesh::Edge* edgeTmp = edgeList.back();
		edgeList.pop_back();
		removeEdge(edgeTmp);
	}
	// print();
	// cleaning the list
	for(int i=0; i<int(mEdges.size()); i++){
		if (!mEdges[i]){
			mEdges.erase(mEdges.begin()+i);
		}
	}
}


void mesh::Mesh::triToQuad(){
	triToQuadRemovalMarkingPhase();
	// print();
	removeMarkedEdges();
}


std::vector<std::string> mesh::Mesh::verticesToString(){
	std::vector<std::string> strings;
	for(int i=0; i<int(mVertices.size()); i++){
		strings.push_back(mVertices[i]->toString());
	}
	return strings;
}

std::vector<std::string> mesh::Mesh::facesToString(){
	std::vector<std::string> strings;
	for(int i=0; i<int(mFaces.size()); i++){
		strings.push_back(mFaces[i]->toString());
	}
	return strings;
}

std::vector<std::string> mesh::Mesh::edgesToString(){
	std::vector<std::string> strings;
	for(int i=0; i<int(mEdges.size()); i++){
		strings.push_back(mEdges[i]->toString());
	}
	return strings;
}


std::vector<std::string> mesh::Mesh::toString(){
    std::vector<std::string> vertices = verticesToString();
    std::vector<std::string> faces = facesToString();
    std::vector<std::string> edges = edgesToString();

	std::vector<std::string> result;
	result.reserve(vertices.size() + faces.size() + edges.size());
	result.insert(result.end(), vertices.begin(), vertices.end());
	result.insert(result.end(), faces.begin(), faces.end());
	result.insert(result.end(), edges.begin(), edges.end());

	return result;
}


void mesh::Mesh::print(){
	std::vector<std::string> strings = toString();

	fprintf(stdout, "\n\nMesh: nbVert=%d, nbFaces=%d, nbEdges=%d\n", mNbVertices, mNbFaces, mNbEdges);

	fprintf(stdout, "\n\nVertices:\n");
	for (int i=0; i<mNbVertices; i++){
		fprintf(stdout, "%s\n", strings[i].c_str());
	}

	fprintf(stdout, "\n\nFaces:\n");
	for (int i=0; i<mNbFaces; i++){
		fprintf(stdout, "%s\n", strings[i+mNbVertices].c_str());
	}

	fprintf(stdout, "\n\nEdges:\n");
	for (int i=0; i<mNbEdges; i++){
		fprintf(stdout, "%s\n", strings[i+mNbVertices+mNbFaces].c_str());
	}
}