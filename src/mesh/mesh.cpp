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
	// printf("vnum: %d, m: %d\n", vnum, m);

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

			mesh::Edge *edge_nex = edgeList[vnum*i + (v + 1) % vnum];
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
				// printf("idx: %d, flip: %d\n", idx, flipIdx);
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
			mNbFaces--;
			return;
		}
	}
}

void mesh::Mesh::removeEdgeFromList(mesh::Edge* edge){
	for(int i = 0; i<int(mEdges.size()); i++){
		if(mEdges[i]->mId == edge->mId){
			// delete edge;
			mEdges.erase(mEdges.begin()+i);
			mNbEdges--;
			return;
		}
	}
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
		mNbFaces++;

		// deleting the faces from the list
		removeFaceFromList(leftFace);
		// edge->print();
		removeFaceFromList(rightFace);
		// edge->print();

		// adding the new face to the list
		mFaces.push_back(newFace);
	}

	// fixing edges
	edge->updateAllNeighbours();
	removeEdgeFromList(edge);
}

std::vector<mesh::Edge*> mesh::Mesh::getAllEdgesToDelete(){
	std::vector<mesh::Edge*> edgeList;
	for(int i=0; i<int(mEdges.size()); i++){
		if(mEdges[i]->mToDelete) edgeList.push_back(mEdges[i]);
	}
	return edgeList;
}

void mesh::Mesh::removeMarkedEdges(){
	// making a list of edges to remove
	std::vector<mesh::Edge*> edgeList = getAllEdgesToDelete();

	// print();
	// removing each edge of the list
	while(edgeList.size() != 0){
		mesh::Edge* edgeTmp = edgeList.back();
		edgeList.pop_back();
		removeEdge(edgeTmp);
	}
	// print();
}


void mesh::Mesh::triToQuad(){
	triToQuadRemovalMarkingPhase();
	// print();
	removeMarkedEdges();
	assert(getAllEdgesToDelete().size() == 0);

	// subdivide all remaining triangles
	assert(howManyTriangles() % 2 == 0);

	triToPureQuad();
	assert(howManyTriangles() == 0);
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

void mesh::Mesh::triToPureQuad(){
	// get one of the remaining triangles
	mesh::Face* triangle = getTriangle();

	int nbEdges = mNbEdges;
	int nbFaces = mNbFaces;
	int nbVertices = mNbVertices;

	while(triangle != nullptr){
		// printf("beg while\n");
		// printf("nbFaces: %d, nbEdges: %d, nbVertices: %d\n", mNbFaces, mNbEdges, mNbVertices);
		subdivideTriangle(triangle);
		// printf("end sub\n");
		assert(mNbEdges == nbEdges + 18 - 6);
		assert(mNbFaces == nbFaces + 3 - 1);
		assert(mNbVertices == nbVertices + 4);
		nbEdges = mNbEdges;
		nbFaces = mNbFaces;
		nbVertices = mNbVertices;
		triangle = getTriangle();
		// printf("end while\n");
	};
	// last check
	assert(triangle == nullptr);
}

mesh::Face* mesh::Mesh::getTriangle(){
	// check for all the mesh's faces if it is a triangle
	for(int i=0; i<mNbFaces; i++){
		if(mFaces[i]->isTriangle())
			return mFaces[i];
	}
	return nullptr;
}

void mesh::Mesh::subdivideTriangle(mesh::Face* triangle){
	// create the 4 new vertices
	std::vector<mesh::Vertex*> newVertices;
	// create the 18 new edges
	std::vector<mesh::Edge*> newEdges;
	// create the 3 new faces
	std::vector<mesh::Face*> newFaces;

	createQuads(triangle, newVertices, newEdges, newFaces);

	// print newly created elements
	/**
	printf("\n\nnew vertices:\n");
	for(int i=0; i<int(newVertices.size()); i++){
		newVertices[i]->print();
	}
	printf("\n\nnew faces:\n");
	for(int i=0; i<int(newFaces.size()); i++){
		newFaces[i]->print();
	}
	printf("\n\nnew edges:\n");
	for(int i=0; i<int(newEdges.size()); i++){
		newEdges[i]->print();
	}
	**/

	// add new elements to mesh
	mVertices.insert(mVertices.end(), newVertices.begin(), newVertices.end());
	mEdges.insert(mEdges.end(), newEdges.begin(), newEdges.end());
	mFaces.insert(mFaces.end(), newFaces.begin(), newFaces.end());
	mNbVertices += int(newVertices.size());
	mNbEdges += int(newEdges.size());
	mNbFaces += int(newFaces.size());
}

void mesh::Mesh::createQuads(mesh::Face* triangle, 
	std::vector<mesh::Vertex*> &newVertices, 
	std::vector<mesh::Edge*> &newEdges, 
	std::vector<mesh::Face*> & newFaces){
	
	initQuads(triangle, newVertices, newEdges, newFaces);
}

void mesh::Mesh::initEdgesArroundTriFace(
	std::vector<mesh::Vertex*> &newVertices, 
	std::vector<mesh::Edge*> &newEdges, 
	std::vector<mesh::Face*> & newFaces,
	std::vector<mesh::Vertex*> & oldVertices,
	std::vector<mesh::Edge*> & oldEdges){

	for(int i=0; i<6; i+=2){
		int half_i = i>>1;
		// first half of the old edge
		newEdges[i]->mVertexOrigin = oldVertices[half_i];
		newEdges[i]->mVertexDestination = newVertices[half_i];
		newEdges[i]->mFaceLeft = newFaces[half_i];
		newEdges[i]->mFaceRight = oldEdges[half_i]->mFaceRight;
		newEdges[i]->mEdgeRightCCW = oldEdges[half_i]->mEdgeRightCCW;
		newEdges[i]->mEdgeRightCW = newEdges[i+1];
		newEdges[i]->mEdgeLeftCCW = newEdges[12+i]; 
		newEdges[i]->mEdgeLeftCW  = newEdges[(i-1+6)%6];
		
		// second half of the old edge
		newEdges[i+1]->mVertexOrigin = newVertices[half_i];
		newEdges[i+1]->mVertexDestination = oldVertices[(half_i+1) % 3];
		newEdges[i+1]->mFaceLeft = newFaces[(half_i+1) % 3];
		newEdges[i+1]->mFaceRight = oldEdges[half_i]->mFaceRight;
		newEdges[i+1]->mEdgeRightCCW = newEdges[i];
		newEdges[i+1]->mEdgeRightCW = oldEdges[half_i]->mEdgeRightCW;
		newEdges[i+1]->mEdgeLeftCCW = newEdges[(i+2)%6]; 
		newEdges[i+1]->mEdgeLeftCW  = newEdges[12+1+i];

		// update old edges
		oldEdges[half_i]->mEdgeRightCCW->mEdgeRightCW = newEdges[i];
		oldEdges[half_i]->mEdgeRightCW->mEdgeRightCCW = newEdges[i+1];
		oldEdges[half_i]->mReverseEdge->mEdgeLeftCCW->mEdgeLeftCW = newEdges[i+6];
		oldEdges[half_i]->mReverseEdge->mEdgeLeftCW->mEdgeLeftCCW = newEdges[i+1+6];

		// update old vertices
		oldVertices[half_i]->mEdge = newEdges[i];

		// reversed edges
		newEdges[i]->mReverseEdge = newEdges[i+6];
		newEdges[i+6]->mReverseEdge = newEdges[i];
		newEdges[i+1]->mReverseEdge = newEdges[i+1+6];
		newEdges[i+1+6]->mReverseEdge = newEdges[i+1];
	}
	
}

void mesh::Mesh::initEdgesInsideTriFace(
	std::vector<mesh::Vertex*> &newVertices, 
	std::vector<mesh::Edge*> &newEdges, 
	std::vector<mesh::Face*> & newFaces){
	for(int i=12; i<17; i+=2){
		int tmpIndex = (i-12);
		// vertex pointing to the middle
		newEdges[i]->mVertexOrigin = newVertices[tmpIndex>>1];
		newEdges[i]->mVertexDestination = newVertices[3];
		newEdges[i]->mFaceLeft = newFaces[tmpIndex>>1];
		newEdges[i]->mFaceRight = newFaces[((tmpIndex>>1)+1)%3];
		newEdges[i]->mEdgeRightCCW = newEdges[tmpIndex+1+6];
		if(i+3 <= 17)
			newEdges[i]->mEdgeRightCW = newEdges[(i+3)];
		else
			newEdges[i]->mEdgeRightCW = newEdges[13];
		if(i-1 >= 12)
			newEdges[i]->mEdgeLeftCCW = newEdges[i-1];
		else 
			newEdges[i]->mEdgeLeftCCW = newEdges[17]; 

		newEdges[i]->mEdgeLeftCW  = newEdges[tmpIndex];

		// init new faces
		newFaces[tmpIndex>>1]->mEdge = newEdges[i];

		// create the reversed edges
		newEdges[i]->mReverseEdge = newEdges[i+1];
		newEdges[i+1]->mReverseEdge = newEdges[i];
	}
}

void mesh::Mesh::initReversedEdgesTriFace(std::vector<mesh::Edge*> &newEdges){
	// edges arround triangle
	for(int i=0; i<6; i+=2){
		newEdges[i]->createReversed(newEdges[i+6]);
		newEdges[i+1]->createReversed(newEdges[i+1+6]);
	}

	// edges inside triangle
	for(int i=12; i<17; i+=2){
		newEdges[i]->createReversed(newEdges[i+1]);
	}

}

void mesh::Mesh::initQuads(mesh::Face* triangle,
	std::vector<mesh::Vertex*> &newVertices, 
	std::vector<mesh::Edge*> &newEdges, 
	std::vector<mesh::Face*> & newFaces){
	// initialize the new elements
	for(int i=0; i<4; i++){
		newVertices.push_back(new mesh::Vertex());
	}
	assert(newVertices.size() == 4);

	for(int i=0; i<(9<<1); i++){
		newEdges.push_back(new mesh::Edge());
	}
	assert(newEdges.size() == 18);

	for(int i=0; i<3; i++){
		newFaces.push_back(new mesh::Face());
	}
	assert(newFaces.size() == 3);

	// get the new vertices coordinates
	std::vector<mesh::Vertex*> oldVertices = triangle->getSurroundingVertices();
	assert(oldVertices.size() == 3);
	for(int i=0; i<3; i++){
		maths::Vector3 coord1 = *(oldVertices[i]->mCoords);
		maths::Vector3 coord2 = *(oldVertices[(i+1)%3]->mCoords);
		newVertices[i]->mCoords = new maths::Vector3((coord2+coord1)/2.0f);
		newVertices[i]->mEdge = newEdges[2*i + 1];
	}
	// middle vertex
	newVertices[3]->mCoords = new maths::Vector3(
									(*(oldVertices[0]->mCoords)
									+*(oldVertices[1]->mCoords)
									+*(oldVertices[2]->mCoords)
									) / 3.0f);
	newVertices[3]->mEdge = newEdges[17];

	// init the new edges' around the face
	std::vector<mesh::Edge*> oldEdges = triangle->getSurroundingEdges();
	initEdgesArroundTriFace(newVertices, newEdges, newFaces, oldVertices, oldEdges);

	// init the vertices in the middle
	initEdgesInsideTriFace(newVertices, newEdges, newFaces);

	// create the reversed edges
	initReversedEdgesTriFace(newEdges);

	// update old faces
	for(int i=0; i<int(oldEdges.size()); i++){
		if (oldEdges[i]->mFaceRight->mEdge == oldEdges[i]){
			oldEdges[i]->mFaceRight->mEdge = newEdges[2*i];
		}
		if (oldEdges[i]->mFaceLeft->mEdge == oldEdges[i]){
			oldEdges[i]->mFaceLeft->mEdge = newEdges[2*i];
		}
	}

	// remove old triangle
	removeFaceFromList(triangle);

	// remove old edges
	for(int i = 0; i<int(oldEdges.size()); i++){
		removeEdgeFromList(oldEdges[i]);
	}

}


int mesh::Mesh::howManyTriangles(){
	int res = 0;
	for(int i=0; i<mNbFaces; i++){
		if(mFaces[i]->isTriangle()) res++;
	}
	return res;
}