#include <cstdio>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cassert>
#include <vector>
#include <queue>
#include <map>

#include "edge.hpp"
#include "face.hpp"
#include "mesh.hpp"
#include "vector3.hpp"

void mesh::Mesh::checkCorrectness() const {
	// printf("\nBeg check correctness\n");
	// check number of elements
	assert(mNbFaces == int(mFaces.size()));
	assert(mNbEdges == int(mEdges.size()));
	assert(mNbVertices == int(mVertices.size()));

	// check turn right
	std::vector<mesh::Edge*> surEdges;
	// turn arround every faces
	// printf("\nBeg check faces\n");
	for(int i=0; i<mNbFaces; i++){
		surEdges = mFaces[i]->getSurroundingEdges();
		assert(!mesh::Edge::hasDoubles(surEdges));
	}
	// printf("End check faces\n");

	// turn arround every edges
	// printf("\nBeg check edges\n");
	for(int i=0; i<mNbEdges; i++){
		surEdges = mEdges[i]->mFaceLeft->getSurroundingEdges();
		assert(!mesh::Edge::hasDoubles(surEdges));
		surEdges = mEdges[i]->mFaceRight->getSurroundingEdges();
		assert(!mesh::Edge::hasDoubles(surEdges));
	}
	// printf("End check edges\n");


	// turn arround every vertices
	// printf("\nBeg check vertices\n");
	for(int i=0; i<mNbVertices; i++){
		surEdges = mVertices[i]->mEdge->mFaceLeft->getSurroundingEdges();
		assert(!mesh::Edge::hasDoubles(surEdges));
		surEdges = mVertices[i]->mEdge->mFaceRight->getSurroundingEdges();
		assert(!mesh::Edge::hasDoubles(surEdges));
	}
	// printf("End check vertices\n");

	// check reversed
	// printf("\nBeg check reversed\n");
	for(int i=0; i<mNbEdges; i++){
		mesh::Edge* curEdge = mEdges[i];
		mesh::Edge* revEdge = curEdge->mReverseEdge;
		assert(curEdge->mReverseEdge);
		assert(curEdge->mVertexOrigin      == revEdge->mVertexDestination);
		assert(curEdge->mVertexDestination == revEdge->mVertexOrigin);
		assert(curEdge->mFaceLeft  == revEdge->mFaceRight);
		assert(curEdge->mFaceRight == revEdge->mFaceLeft);
		assert(curEdge->mEdgeLeftCCW  == revEdge->mEdgeRightCCW->mReverseEdge);
		assert(curEdge->mEdgeLeftCW   == revEdge->mEdgeRightCW->mReverseEdge);
		assert(curEdge->mEdgeRightCCW == revEdge->mEdgeLeftCCW->mReverseEdge);
		assert(curEdge->mEdgeRightCW  == revEdge->mEdgeLeftCW->mReverseEdge);
	}
	// printf("End check reversed\n");


	// check edges' edges
	// printf("\nBeg check unique edges\n");
	assert(!mesh::Vertex::twoSameEdges(mEdges, mNbVertices));
	// printf("End check unique edges\n");

	// printf("\nEnd check correctness\n");
}

std::vector<std::string> mesh::Mesh::verticesToString() const {
	std::vector<std::string> strings;
	for(int i=0; i<int(mVertices.size()); i++){
		strings.push_back(mVertices[i]->toString());
	}
	return strings;
}

std::vector<std::string> mesh::Mesh::facesToString() const {
	std::vector<std::string> strings;
	for(int i=0; i<int(mFaces.size()); i++){
		strings.push_back(mFaces[i]->toString());
	}
	return strings;
}

std::vector<std::string> mesh::Mesh::edgesToString() const {
	std::vector<std::string> strings;
	for(int i=0; i<int(mEdges.size()); i++){
		strings.push_back(mEdges[i]->toString());
	}
	return strings;
}


std::vector<std::string> mesh::Mesh::toString() const{
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

void mesh::Mesh::printStats() const{
	fprintf(stdout, "\n\nMesh: nbVert=%d, nbFaces=%d, nbEdges=%d\n", mNbVertices, mNbFaces, mNbEdges);
}


void mesh::Mesh::print() const{
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
	// printf("nbVertices: %d, nbFaces: %d\n", m, int(faces.size()));

	std::vector<std::vector<int>> edgeTable(m, std::vector<int>(m, -1));

	for (int i = 0; i < int(faces.size()); i++) {
		mesh::Face* f = faceList[i];
		
		for (int v = 0; v < vnum; v++) {
			int v0 = faces[i][v];
			int v1 = faces[i][(v+1)%vnum];

			int idx = vnum*i + v;

			mesh::Vertex* vStart; 
			mesh::Vertex* vEnd;
			mesh::Edge* eCur;
			mesh::Edge* eNext = edgeList[vnum*i + (v + 1) % vnum];
			mesh::Edge* ePrev = edgeList[vnum*i + (v - 1 + vnum) % vnum];

			// printf("v0: %d, v1: %d, idx: %d, lccw: %d, lcw: %d\n", v0, v1, idx, vnum*i + (v + 1) % vnum, vnum*i + (v - 1 + vnum) % vnum);

			vStart = vertexList[v0];
			vEnd = vertexList[v1];
			eCur = edgeList[idx];

			eCur->mVertexOrigin = vStart;
			eCur->mVertexDestination = vEnd;

			eCur->mEdgeRightCW = eNext;
			eCur->mEdgeRightCCW = ePrev;
			eCur->mFaceRight = f;

			edgeTable[v0][v1] = idx;
			int revIdx = edgeTable[v1][v0];
			if(revIdx != -1){
				eCur->mReverseEdge = edgeList[revIdx];
				edgeList[revIdx]->mReverseEdge = eCur;
			}
			f->mEdge = eCur;
			vStart->mEdge = eCur;
		}
	}

	// save left according to the table
	for (int i = 0; i < m; i++){
		for (int j = 0; j < m; j++){
			int idx = edgeTable[i][j];
			if (idx != -1){
				mesh::Edge *edge = edgeList[idx];
				int flipIdx = edgeTable[j][i];
				// printf("idx: %d, flip: %d\n", idx, flipIdx);
				mesh::Edge *edgeFlip = edgeList[flipIdx];

				edge->mEdgeLeftCW = edgeFlip->mEdgeRightCW->mReverseEdge;
				edge->mEdgeLeftCCW = edgeFlip->mEdgeRightCCW->mReverseEdge; 
				edge->mFaceLeft = edgeFlip->mFaceRight;
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
	// create a list of candidates for all faces
	std::vector<mesh::Edge*> candidateEdges;
	for (int i=0; i<int(mFaces.size()); i++){
		mesh::Edge* edgeToRemove = nullptr;
		float maxSquared = -1.0f;
		float maxLength = -1.0f;

		// for all edges surrounding the face
		std::vector<mesh::Edge*> surEdges = mFaces[i]->getSurroundingEdges();
		// print();
		for (int j=0; j<int(surEdges.size()); j++){
			mesh::Edge* curEdge = surEdges[j];
			// get the sum of pairwised dot product
			std::vector<mesh::Edge*> newQuadEdges = {
				curEdge->mEdgeLeftCW,
				curEdge->mEdgeLeftCCW->mReverseEdge,
				curEdge->mEdgeRightCCW,
				curEdge->mEdgeRightCW->mReverseEdge
			};
			float sumDotProd = mesh::Edge::getSumPairwiseDotProd(newQuadEdges);
			float length = curEdge->getLength();
			// update max sum
			if(sumDotProd >= maxSquared){
				if(sumDotProd > maxSquared || length >= maxLength){
					maxSquared = sumDotProd;
					maxLength = length;
					edgeToRemove = curEdge;
				}
			}
		}

		edgeToRemove->mSumDotProd = maxSquared;
		candidateEdges.push_back(edgeToRemove);
	}

	// make a max heap of the candidates
	std::make_heap(candidateEdges.begin(), candidateEdges.end(), mesh::Edge::cmp);
	while(candidateEdges.size() > 0){
		mesh::Edge* curEdge = candidateEdges.back();
		candidateEdges.pop_back();
		// if there is a removable face, set its flag to True and update flags for its faces
		if(curEdge->isRemovable()){
			curEdge->mToDelete = true;
			curEdge->mFaceLeft->mToMerge = true;
			curEdge->mFaceRight->mToMerge = true;
			curEdge->mReverseEdge->mToDelete = true;
		}
	}
}

void mesh::Mesh::removeFaceFromList(mesh::Face* face){
	for(int i = 0; i<int(mFaces.size()); i++){
		if(mFaces[i]->mId == face->mId){
			mFaces.erase(mFaces.begin()+i);
			mNbFaces--;
			delete(face);
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
			delete(edge);
			return;
		}
	}
}

void mesh::Mesh::removeEdge(mesh::Edge* edge){
	// TODO reorient edges
	// print();
	// printf("\n");
	// edge->print();
	// fixing vertices
	if(edge->mVertexOrigin->mEdge->mId == edge->mId)
		edge->mVertexOrigin->mEdge = edge->mEdgeRightCCW;
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
			leftFace->mEdge = edge->mEdgeLeftCW->mReverseEdge;
		if(rightFace->mEdge->mId == edge->mId)
			rightFace->mEdge = edge->mEdgeRightCW;

		// fusioning faces
		mesh::Face* newFace = rightFace->mergeFace(leftFace);
		// edge->print();

		// deleting the faces from the list
		removeFaceFromList(leftFace);
		// edge->print();
		removeFaceFromList(rightFace);
		// edge->print();

		// adding the new face to the list
		mFaces.push_back(newFace);
		mNbFaces++;
	}

	// fixing edges
	edge->updateAllNeighbours();
	removeEdgeFromList(edge);
}

std::vector<mesh::Edge*> mesh::Mesh::getAllEdgesToDelete(){
	std::vector<mesh::Edge*> edgeList;
	for(int i=0; i<mNbEdges; i++){
		if(mEdges[i]->mToDelete) {
			assert(mEdges[i]->mReverseEdge->mToDelete);
			edgeList.push_back(mEdges[i]->mReverseEdge);
			edgeList.push_back(mEdges[i]);
			mEdges[i]->mToDelete = false;
			mEdges[i]->mReverseEdge->mToDelete = false;
		}
	}
	return edgeList;
}

void mesh::Mesh::removeMarkedEdges(){
	// making a list of edges to remove
	std::vector<mesh::Edge*> edgeList = getAllEdgesToDelete();
	assert(getAllEdgesToDelete().size() % 2 == 0);

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
	// print();
	triToQuadRemovalMarkingPhase();
	printStats();
	// printf("\nEdgesToDelete: %d", int(getAllEdgesToDelete().size()));
	// checkCorrectness();
	// print();
	removeMarkedEdges();
	printStats();
	// checkCorrectness();
	// print();
	assert(getAllEdgesToDelete().size() == 0);

	// subdivide all remaining triangles
	assert(howManyTriangles() % 2 == 0);

	triToPureQuad();
	printStats();
	assert(howManyTriangles() == 0);

	// print();
}

mesh::Face* mesh::Mesh::getTriangle() const {
	for(int i=0; i<mNbFaces; i++){
		if(mFaces[i]->isTriangle()) return mFaces[i];
	}
	return nullptr;
}


int mesh::Mesh::howManyTriangles() const {
	int sum = 0;
	for(int i=0; i<mNbFaces; i++){
		if(mFaces[i]->isTriangle()) sum++;
	}
	return sum;
}

void mesh::Mesh::createEdge(mesh::Face* face, mesh::Vertex* v1, mesh::Vertex* v2){
	std::vector<mesh::Edge*> surEdges = face->getSurroundingEdges();
	int nbSurEdges = surEdges.size() >> 1;
	// for(int i=0; i<nbSurEdges; i++){
	// 	// assert(surEdges[i]->mFaceLeft->mId == face->mId);
	// 	printf("surEdge[%d]\n", i);
	// 	surEdges[i]->print();
	// }

	// initiate the new edges
	mesh::Edge* edge = new mesh::Edge();
	mesh::Edge* edgeRev = new mesh::Edge();

	// initiate the new faces
	mesh::Face* halfFace1 = new mesh::Face();
	mesh::Face* halfFace2 = new mesh::Face();

	// update new faces
	halfFace1->mEdge = edge;
	halfFace2->mEdge = edgeRev;

	// update first edge
	edge->mVertexOrigin = v1;
	edge->mVertexDestination = v2;
	edge->mFaceLeft = halfFace2;
	edge->mFaceRight = halfFace1;
	edge->mReverseEdge = edgeRev;
	edgeRev->mReverseEdge = edge;

	// update first edge neighbours
	// get the edge that goes into v1
	int startIdx = 0;
	while(surEdges[startIdx]->mVertexDestination->mId != v1->mId) startIdx++;

	// the current face we're surrounding
	int curFace = 1;
	for(int i=startIdx; i<nbSurEdges+startIdx; i++){
		int idx = i%nbSurEdges;
		// printf("\nidx: %d\n", idx);
		// surEdges[idx]->print();

		// update new edge
		if(surEdges[idx]->mVertexDestination->mId == v1->mId) {
			edge->mEdgeRightCCW = surEdges[idx];
			surEdges[idx]->mEdgeRightCW = edge;
			surEdges[idx]->mReverseEdge->mEdgeLeftCW = edge->mReverseEdge;
			curFace = 1;
		}
		if(surEdges[idx]->mVertexOrigin->mId == v2->mId) {
			edge->mEdgeRightCW = surEdges[idx];
			surEdges[idx]->mEdgeRightCCW = edge;
			surEdges[idx]->mReverseEdge->mEdgeLeftCCW = edge->mReverseEdge;
			curFace = 1;
		}
		if(surEdges[idx]->mVertexDestination->mId == v2->mId) {
			edge->mEdgeLeftCCW = surEdges[idx]->mReverseEdge;
			surEdges[idx]->mEdgeRightCW = edge->mReverseEdge;
			surEdges[idx]->mReverseEdge->mEdgeLeftCW = edge;
			curFace = 2;
		}
		if(surEdges[idx]->mVertexOrigin->mId == v1->mId) {
			edge->mEdgeLeftCW = surEdges[idx]->mReverseEdge;
			surEdges[idx]->mEdgeRightCCW = edge->mReverseEdge;
			surEdges[idx]->mReverseEdge->mEdgeLeftCCW = edge;
			curFace = 2;
		}

		// update old edge
		assert(surEdges[idx]->mFaceRight->mId == face->mId);
		switch(curFace){
			case 1:
				surEdges[idx]->mFaceRight = halfFace1;
				surEdges[idx]->mReverseEdge->mFaceLeft = halfFace1;
				break;
			case 2:
				surEdges[idx]->mFaceRight = halfFace2;
				surEdges[idx]->mReverseEdge->mFaceLeft = halfFace2;
				break;
			default:
				assert(false);
		}

	}
	assert(edge->mEdgeRightCCW);
	assert(edge->mEdgeRightCW);
	assert(edge->mEdgeLeftCCW);
	assert(edge->mEdgeLeftCW);
	// update reversed edge
	edge->createReversed(edgeRev);
	assert(edgeRev->mEdgeRightCCW);
	assert(edgeRev->mEdgeRightCW);
	assert(edgeRev->mEdgeLeftCCW);
	assert(edgeRev->mEdgeLeftCW);

	// remove old face from list
	removeFaceFromList(face);

	// add new elements to list
	mFaces.push_back(halfFace1);
	mFaces.push_back(halfFace2);
	mNbFaces += 2;

	mEdges.push_back(edge);
	mEdges.push_back(edgeRev);
	mNbEdges += 2;

	// check if new faces are triangles
	surEdges = halfFace1->getSurroundingEdges();
	if(surEdges.size() != 6) halfFace1->mIsTriangle = false;
	surEdges = halfFace2->getSurroundingEdges();
	if(surEdges.size() != 6) halfFace2->mIsTriangle = false;

	// printf("\nnewEdge:\n");
	// edge->print();
	// printf("newRevEdge:\n");
	// edgeRev->print();
	// printf("newFace1:\n");
	// halfFace1->mEdge->print();
	// halfFace1->print();
	// printf("newFace2:\n");
	// halfFace2->mEdge->print();
	// halfFace2->print();

}

std::vector<mesh::Face*> mesh::Mesh::pathToClosestTriangle(mesh::Face* triangle) const {
	const int VISITED = 2;
	const int ONGOING = 1;
	const int UNVISITED = 0;

	// the path between the two triangles
	std::vector<mesh::Face*> path;

	// initiate a dictionary containing the face's id as keys and the state of each faces (unvisited, ongoing or visited) as values
	std::map<int, int> status;
	// initiate a dictionary containing the face's id as keys and the parent in the search as value
	std::map<int, mesh::Face*> parents;
	// assert(mNbFaces == int(mFaces.size()));
	for(int i=0; i<mNbFaces; i++){
		status[mFaces[i]->mId] = UNVISITED;
		parents[mFaces[i]->mId] = nullptr;
	}
	parents[triangle->mId] = nullptr;
	status[triangle->mId] = ONGOING;

	// initiate the queue for the BFS search
	std::queue<mesh::Face*> queue;
	queue.push(triangle);

	// to leave the while loop
	bool quit = false;
	int closestTriangleId = -1;
	mesh::Face* pathFace = nullptr;
	// print();
	// printf("how many triangle: %d\n", howManyTriangles());

	while(!queue.empty()){
		// remove current face from queue
		mesh::Face* curFace = queue.front();
		queue.pop();
		// printf("\n\nCur face:\n");
		// curFace->print();

		// for each neighbours
		std::vector<mesh::Face*> neighbours = curFace->getSurroundingFaces();
		// printf("\nnbNeighbours: %d\n", int(neighbours.size()));
		for(int i=0; i<int(neighbours.size()); i++){
			int curNeighbourFaceId = neighbours[i]->mId;
			// printf("\nCur Neighbour:\n");
			// neighbours[i]->print();

			// check if we've found a triangle different from the origin triangle
			if(neighbours[i]->isTriangle() && status[curNeighbourFaceId] == UNVISITED){
				quit = true;
				// save the face and update it's parent
				closestTriangleId = curNeighbourFaceId;
				pathFace = neighbours[i];
				parents[curNeighbourFaceId] = curFace;
				break;
			}

			// if current face not visited yet update its status and add it to the queue
			if(status[curNeighbourFaceId] == UNVISITED){
				status[curNeighbourFaceId] = ONGOING;
				// update parent
				parents[curNeighbourFaceId] = curFace;
				queue.push(neighbours[i]);
			}
		}

		if(quit) break;

		// done visiting the current face
		status[curFace->mId] = VISITED;
	}

	// test if we've found a triangle
	assert(closestTriangleId != -1);

	// get the path from the target triangle to the current one
	while(pathFace != nullptr){
		// printf("\nPathFace:\n");
		// pathFace->print();
		path.push_back(pathFace);
		pathFace = parents[pathFace->mId];
	}

	return path;
}


void mesh::Mesh::triToPureQuad(){
	/* 
	for each triangle
		get a list of faces between it and another triangle using BFS
		for each of the quads in the list
			find the edge to remove
			remove the edge
			find the vertices between which you'll rebuild a new edge
			build a new edge
		remove the edge between the two triangles
	*/
	mesh::Face* curTriangle = getTriangle();
	while(curTriangle != nullptr){
		// printf("\nwhile curTriangle\n");
		// checkCorrectness();
		// get a list of faces between it and another triangle using BFS
		std::vector<mesh::Face*> path = pathToClosestTriangle(curTriangle);

		mesh::Face* curTri = nullptr;
		mesh::Face* curQuad = nullptr;
		mesh::Edge* edgeToRemove = nullptr;
		mesh::Edge* revEdgeToRemove = nullptr;
		bool shouldRemoveEdge = true;

		// print();
		// printf("\n###################### NEW PATH #######################\n");
		while(path.size() != 0){
			// get current triangle and current quad neighbour
			curTri = path.back();
			assert(curTri != nullptr);
			path.pop_back();
			// printf("\nCurTri:\n");
			// curTri->print();

			curQuad = path.back();
			assert(curQuad != nullptr);
			path.pop_back();
			// printf("\nCurQuad:\n");
			// curQuad->print();

			// if two edges in common, add a new edge to form 3 triangles and find another path
			int nbSharedEdges = curTri->getNumberOfSharedEdges(curQuad);
			// printf("\nNbSharedEdges: %d\n", nbSharedEdges);
			if( nbSharedEdges == 4){
				// printf("\nAdd triangle\n");
				std::vector<mesh::Vertex*> unconnected = curTri->getUnconnectedVertices(curQuad);
				// printf("\nNbUnconnected: %d\n", int(unconnected.size()));
				mesh::Vertex* v1 = unconnected[0];
				assert(v1 != nullptr);
				// printf("\nv1:\n");
				// v1->print();
				mesh::Vertex* v2 = unconnected[1];
				assert(v2 != nullptr);
				// printf("v2:\n");
				// v2->print();
				std::vector<mesh::Vertex*> surTmp = curQuad->getSurroundingVertices();
				assert(v1->isInList(surTmp) && v2->isInList(surTmp));
				createEdge(curQuad, v1, v2);
				shouldRemoveEdge = false;
				break;
			}

			// find the edges to remove
			edgeToRemove = curTri->getEdgeBetween(curQuad);
			assert(edgeToRemove != nullptr);
			revEdgeToRemove = edgeToRemove->mReverseEdge;
			assert(revEdgeToRemove != nullptr);

			// test if found triangle
			if(curQuad->isTriangle()) break;

			mesh::Face* nextQuad = path.back();
			assert(nextQuad != nullptr);
			// printf("\nNextQuad:\n");
			// nextQuad->print();

			// find the future vertices from which we'll add new edges
			mesh::Vertex* v1 = mesh::Vertex::getCommonVertex(curTri, curQuad, nextQuad);
			if(v1 == nullptr){ // three faces not sticked together
				v1 = mesh::Vertex::getCommonVertices(curTri, curQuad)[0];
			} else if(int(mesh::Vertex::getCommonVertices(curQuad, nextQuad).size()) >= 3){
				v1 = edgeToRemove->mVertexOrigin->mId == v1->mId ? edgeToRemove->mVertexDestination : edgeToRemove->mVertexOrigin;
			}
			mesh::Vertex* v2 = mesh::Vertex::getOppositeVertex(curQuad,v1);
			// test if the two chosen vertices are correct 
			assert(v1 != nullptr);
			assert(v2 != nullptr);
			std::vector<mesh::Vertex*> surTmp = curQuad->getSurroundingVertices();
			assert(v1->isInList(surTmp) && v2->isInList(surTmp));

			// printf("\nv1:\n");
			// v1->print();
			// printf("v2:\n");
			// v2->print();

			// remove the edges
			// printf("\nEdge to remove:\n");
			// edgeToRemove->print();
			// printf("RevEdge to remove:\n");
			// revEdgeToRemove->print();
			removeEdge(edgeToRemove);
			removeEdge(revEdgeToRemove);

			// get the newly created face
			mesh::Face* newPoly = mFaces.back(); // the last added 
			assert(newPoly->mEdge->mFaceRight->mId == newPoly->mId);
			// printf("\nNewPoly:\n");
			// newPoly->print();
			createEdge(newPoly, v1, v2);

			mesh::Face* newTriangle = mFaces.back(); // the last added
			if(newTriangle->isQuad()) newTriangle = mFaces[mNbFaces-2];
			// printf("\nNew Triangle:\n");
			// newTriangle->print();
			assert(newTriangle->isTriangle());

			// putting the new triangle at the end of the path
			path.push_back(newTriangle);

		}

		if(shouldRemoveEdge){
			// remove the edges
			assert(edgeToRemove != nullptr);
			assert(revEdgeToRemove != nullptr);
			// printf("\nEdge to remove:\n");
			// edgeToRemove->print();
			// printf("RevEdge to remove:\n");
			// revEdgeToRemove->print();
			removeEdge(edgeToRemove);
			removeEdge(revEdgeToRemove);
			// printf("\ndone removing\n");
		}

		// get a new triangle
		curTriangle = getTriangle();
	}
}




// void mesh::Mesh::triToPureQuadMarkingPhase(){
// 	/**
// 	for all triangles
// 		create new vertex at center
// 		create 3 new faces arround that barycenter
// 		remove old triangular face

// 		mark the old triangular edges as "toDelete"
// 	remove these edges
// 	*/

// 	// get the remaining triangles
// 	std::vector<mesh::Face*> triangles = getTriangles();
// 	for(int i=0; i<int(triangles.size()); i++){ // change access order to avoid emptying the array
// 		mesh::Face* curTriangle = triangles[i];
// 		// create new vertex
// 		mesh::Vertex* midVertex = triToQuadNewMiddleVertex(curTriangle);
// 		// create new faces
// 		std::vector<mesh::Face*> newFaces = triToQuadNewFaces(curTriangle);
// 		// create new edges
// 		std::vector<mesh::Edge*> newEdges = triToQuadNewEdges(curTriangle, midVertex);

// 		// remove old triangle face
// 		triToQuadRemoveOldTriangle(curTriangle);
// 	}
// }

// mesh::Vertex* mesh::Mesh::triToQuadNewMiddleVertex(mesh::Face* triangle){
// 	std::vector<mesh::Vertex*> oldVertices = triangle->getSurroundingVertices();
// 	assert(oldVertices.size() == 3);
// 	// middle vertex
// 	mesh::Vertex* midVertex = new mesh::Vertex();
// 	midVertex->mCoords = new maths::Vector3( (*(oldVertices[0]->mCoords)+*(oldVertices[1]->mCoords)+*(oldVertices[2]->mCoords)) / 3.0f);
// 	return midVertex;
// }

// std::vector<mesh::Face*> mesh::Mesh::triToQuadNewFaces(mesh::Face* triangle){
// 	std::vector<mesh::Edge*> surEdges = triangle->getSurroundingEdges();
// 	std::vector<mesh::Face*> newFaces;
// 	int nbNewFaces = 3;
// 	for(int i=0; i<nbNewFaces; i++){
// 		newFaces.push_back(new mesh::Face());
// 	}

// 	return newFaces;
// }



// std::vector<mesh::Face*> mesh::Mesh::getTriangles(){
// 	// check for all the mesh's faces if it is a triangle
// 	std::vector<mesh::Face*> triangles;
// 	for(int i=0; i<mNbFaces; i++){
// 		// printf("i: %d\n", i);
// 		// mFaces[i]->print();
// 		// mFaces[i]->mEdge->print();
// 		// print();
// 		if(mFaces[i]->isTriangle())
// 			triangles.push_back(mFaces[i]);
// 	}
// 	return triangles;
// }

// int mesh::Mesh::howManyTriangles(){
// 	return getTriangles().size();
// }



// void mesh::Mesh::subdivideTriangle(mesh::Face* triangle){
// 	// create the 4 new vertices
// 	std::vector<mesh::Vertex*> newVertices;
// 	// create the 18 new edges
// 	std::vector<mesh::Edge*> newEdges;
// 	// create the 3 new faces
// 	std::vector<mesh::Face*> newFaces;

// 	createQuads(triangle, newVertices, newEdges, newFaces);

// 	// print newly created elements
// 	/**
// 	printf("\n\nnew vertices:\n");
// 	for(int i=0; i<int(newVertices.size()); i++){
// 		newVertices[i]->print();
// 	}
// 	printf("\n\nnew faces:\n");
// 	for(int i=0; i<int(newFaces.size()); i++){
// 		newFaces[i]->print();
// 	}
// 	printf("\n\nnew edges:\n");
// 	for(int i=0; i<int(newEdges.size()); i++){
// 		newEdges[i]->print();
// 	}
// 	**/

// 	// add new elements to mesh
// 	mVertices.insert(mVertices.end(), newVertices.begin(), newVertices.end());
// 	mEdges.insert(mEdges.end(), newEdges.begin(), newEdges.end());
// 	mFaces.insert(mFaces.end(), newFaces.begin(), newFaces.end());
// 	mNbVertices += int(newVertices.size());
// 	mNbEdges += int(newEdges.size());
// 	mNbFaces += int(newFaces.size());
// }

// void mesh::Mesh::createQuads(mesh::Face* triangle, 
// 	std::vector<mesh::Vertex*> &newVertices, 
// 	std::vector<mesh::Edge*> &newEdges, 
// 	std::vector<mesh::Face*> & newFaces){
	
// 	initQuads(triangle, newVertices, newEdges, newFaces);
// }

// void mesh::Mesh::initEdgesArroundTriFace(
// 	std::vector<mesh::Vertex*> &newVertices, 
// 	std::vector<mesh::Edge*> &newEdges, 
// 	std::vector<mesh::Face*> & newFaces,
// 	std::vector<mesh::Vertex*> & oldVertices,
// 	std::vector<mesh::Edge*> & oldEdges){

// 	for(int i=0; i<6; i+=2){
// 		int half_i = i>>1;
// 		// first half of the old edge
// 		newEdges[i]->mVertexOrigin = oldVertices[half_i];
// 		newEdges[i]->mVertexDestination = newVertices[half_i];
// 		newEdges[i]->mFaceLeft = newFaces[half_i];
// 		newEdges[i]->mFaceRight = oldEdges[half_i]->mFaceRight;
// 		newEdges[i]->mEdgeRightCCW = oldEdges[half_i]->mEdgeRightCCW;
// 		newEdges[i]->mEdgeRightCW = newEdges[i+1];
// 		newEdges[i]->mEdgeLeftCCW = newEdges[12+i]; 
// 		newEdges[i]->mEdgeLeftCW  = newEdges[(i-1+6)%6];
		
// 		// second half of the old edge
// 		newEdges[i+1]->mVertexOrigin = newVertices[half_i];
// 		newEdges[i+1]->mVertexDestination = oldVertices[(half_i+1) % 3];
// 		newEdges[i+1]->mFaceLeft = newFaces[(half_i+1) % 3];
// 		newEdges[i+1]->mFaceRight = oldEdges[half_i]->mFaceRight;
// 		newEdges[i+1]->mEdgeRightCCW = newEdges[i];
// 		newEdges[i+1]->mEdgeRightCW = oldEdges[half_i]->mEdgeRightCW;
// 		newEdges[i+1]->mEdgeLeftCCW = newEdges[(i+2)%6]; 
// 		newEdges[i+1]->mEdgeLeftCW  = newEdges[12+1+i];

// 		// reversed edges
// 		newEdges[i]->mReverseEdge = newEdges[i+6];
// 		newEdges[i+6]->mReverseEdge = newEdges[i];
// 		newEdges[i+1]->mReverseEdge = newEdges[i+1+6];
// 		newEdges[i+1+6]->mReverseEdge = newEdges[i+1];

// 		// update old edges
// 		newEdges[i]->mEdgeRightCCW->mEdgeRightCW = newEdges[i];
// 		newEdges[i+1]->mEdgeRightCW->mEdgeRightCCW = newEdges[i+1];
// 		oldEdges[half_i]->mReverseEdge->mEdgeLeftCCW->mEdgeLeftCW = newEdges[i]->mReverseEdge;
// 		oldEdges[half_i]->mReverseEdge->mEdgeLeftCW->mEdgeLeftCCW = newEdges[i+1]->mReverseEdge;

// 		// update old vertices
// 		oldVertices[half_i]->mEdge = newEdges[i];
// 	}
	
// }

// void mesh::Mesh::initEdgesInsideTriFace(
// 	std::vector<mesh::Vertex*> &newVertices, 
// 	std::vector<mesh::Edge*> &newEdges, 
// 	std::vector<mesh::Face*> & newFaces){
// 	for(int i=12; i<17; i+=2){
// 		int tmpIndex = (i-12);
// 		// vertex pointing to the middle
// 		newEdges[i]->mVertexOrigin = newVertices[tmpIndex>>1];
// 		newEdges[i]->mVertexDestination = newVertices[3];
// 		newEdges[i]->mFaceLeft = newFaces[tmpIndex>>1];
// 		newEdges[i]->mFaceRight = newFaces[((tmpIndex>>1)+1)%3];
// 		newEdges[i]->mEdgeRightCCW = newEdges[tmpIndex+1+6];
// 		if(i+3 <= 17)
// 			newEdges[i]->mEdgeRightCW = newEdges[(i+3)];
// 		else
// 			newEdges[i]->mEdgeRightCW = newEdges[13];
// 		if(i-1 >= 12)
// 			newEdges[i]->mEdgeLeftCCW = newEdges[i-1];
// 		else 
// 			newEdges[i]->mEdgeLeftCCW = newEdges[17]; 

// 		newEdges[i]->mEdgeLeftCW  = newEdges[tmpIndex];

// 		// init new faces
// 		newFaces[tmpIndex>>1]->mEdge = newEdges[i];

// 		// create the reversed edges
// 		newEdges[i]->mReverseEdge = newEdges[i+1];
// 		newEdges[i+1]->mReverseEdge = newEdges[i];
// 	}
// }

// void mesh::Mesh::initReversedEdgesTriFace(std::vector<mesh::Edge*> &newEdges){
// 	// edges arround triangle
// 	for(int i=0; i<6; i+=2){
// 		newEdges[i]->createReversed(newEdges[i+6]);
// 		newEdges[i+1]->createReversed(newEdges[i+1+6]);
// 	}

// 	// edges inside triangle
// 	for(int i=12; i<17; i+=2){
// 		newEdges[i]->createReversed(newEdges[i+1]);
// 	}

// }

// void mesh::Mesh::initQuads(mesh::Face* triangle,
// 	std::vector<mesh::Vertex*> &newVertices, 
// 	std::vector<mesh::Edge*> &newEdges, 
// 	std::vector<mesh::Face*> & newFaces){
// 	// initialize the new elements
// 	for(int i=0; i<4; i++){
// 		newVertices.push_back(new mesh::Vertex());
// 	}
// 	assert(newVertices.size() == 4);

// 	for(int i=0; i<(9<<1); i++){
// 		newEdges.push_back(new mesh::Edge());
// 	}
// 	assert(newEdges.size() == 18);

// 	for(int i=0; i<3; i++){
// 		newFaces.push_back(new mesh::Face());
// 	}
// 	assert(newFaces.size() == 3);

// 	// get the new vertices coordinates
// 	std::vector<mesh::Vertex*> oldVertices = triangle->getSurroundingVertices();
// 	assert(oldVertices.size() == 3);
// 	for(int i=0; i<3; i++){
// 		maths::Vector3 coord1 = *(oldVertices[i]->mCoords);
// 		maths::Vector3 coord2 = *(oldVertices[(i+1)%3]->mCoords);
// 		newVertices[i]->mCoords = new maths::Vector3((coord2+coord1)/2.0f);
// 		newVertices[i]->mEdge = newEdges[2*i + 1];
// 	}
// 	// middle vertex
// 	newVertices[3]->mCoords = new maths::Vector3(
// 									(*(oldVertices[0]->mCoords)
// 									+*(oldVertices[1]->mCoords)
// 									+*(oldVertices[2]->mCoords)
// 									) / 3.0f);
// 	newVertices[3]->mEdge = newEdges[17];

// 	// init the new edges' around the face
// 	std::vector<mesh::Edge*> oldEdges = triangle->getSurroundingEdges();
// 	initEdgesArroundTriFace(newVertices, newEdges, newFaces, oldVertices, oldEdges);

// 	// init the vertices in the middle
// 	initEdgesInsideTriFace(newVertices, newEdges, newFaces);

// 	// create the reversed edges
// 	initReversedEdgesTriFace(newEdges);

// 	// update old faces
// 	for(int i=0; i<int(oldEdges.size())>>1; i++){
// 		if (oldEdges[i]->mFaceRight->mEdge == oldEdges[i]->mReverseEdge){
// 			oldEdges[i]->mFaceRight->mEdge = newEdges[2*i + 6];
// 		}
// 	}

// 	// remove old triangle
// 	removeFaceFromList(triangle);

// 	// remove old edges
// 	for(int i = 0; i<int(oldEdges.size()); i++){
// 		removeEdgeFromList(oldEdges[i]);
// 	}

// }

