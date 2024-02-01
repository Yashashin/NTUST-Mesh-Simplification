#pragma once

#include "MeshObject.h"
#include <vector>

class Paintbrush {

public:
	//get
	int GetPaintbrushSize() { return this->paintbrushSize; }

	//set
	bool SetPaintbrushSize(int size) {
		if (size <= 0) {
			std::cout << "error size of brush\n";
			return false;//rejection
		}
		this->paintbrushSize = size;

		return true;//successful
	}
	bool AddPaintbrushSize(int val) {
		int tmp = this->paintbrushSize + val;
		if (tmp <= 0) {
			return false;
		}
		this->paintbrushSize = tmp;
	}

	//method

	static bool ScaleSelectedFaces(MeshObject& meshobj, int& currentTexID, int times) {
		if (times <= 0)return false;
		//call findnearfaces
		for (int i = 0; i < times; ++i) {
			Paintbrush::FindNearFaces(meshobj, currentTexID);
		}
		return true;

	}
	static bool ShrinkSelectedFaces(MeshObject& meshobj, int& currentTexID, int times) {
		if (times <= 0)return false;
		//call findnearfaces
		for (int i = 0; i < times; ++i) {
			Paintbrush::FindNearFaces(meshobj, currentTexID,false,false);
		}
		return true;
	}



	static bool ScaleHoverSelectedFaces(MeshObject& meshobj,int times) {
		if (times <= 0)return false;
		//call findnearfaces
		for (int i = 0; i < times; ++i) {
			Paintbrush::FindNearFaces(meshobj,0,true);
		}
		return true;

	}

	Paintbrush() {
		paintbrushSize = 1;
	}
private:
	int paintbrushSize;

	static void HelperFunctionTemporaryMeshUpdateFaces(MeshObject& meshobj, MyMesh& mesh, int selectedTexID, bool hasTexture,bool isHover=false) {
		//update need call it first
		mesh.request_vertex_normals();
		mesh.request_face_normals();

		if (hasTexture) {
			//temp record vertex
			std::map<int, int> usedVertexs;
			//loop all selected faces
			for (auto faceIDIterator = meshobj.textures[selectedTexID]->faceIDs.begin(); faceIDIterator != meshobj.textures[selectedTexID]->faceIDs.end(); ++faceIDIterator)
			{
				//get the face handle from selected texture id 
				MyMesh::FaceHandle faceHandle = meshobj.model.mesh.face_handle(*faceIDIterator);

				std::vector<MyMesh::VertexHandle> vertexHandles;//needed to push into mesh data's face
				vertexHandles.reserve(3);
				for (MyMesh::FaceVertexIter faceVertexIterator = meshobj.model.mesh.fv_iter(faceHandle); faceVertexIterator.is_valid(); ++faceVertexIterator) {

					MyMesh::VertexHandle vertexHandle;

					//first check if already used means we don't need to push into openmesh data
					std::map<int, int>::iterator usedVertexsIterator = usedVertexs.find(faceVertexIterator->idx());

					if (usedVertexsIterator == usedVertexs.end()) //means not added to mesh data
					{
						MyMesh::Point p = meshobj.model.mesh.point(*faceVertexIterator);//get point
						vertexHandle = mesh.add_vertex(p);
						usedVertexs[faceVertexIterator->idx()] = vertexHandle.idx();//record it to vertex handle id next time used
					}
					else {
						vertexHandle = mesh.vertex_handle(usedVertexs[faceVertexIterator->idx()]);
					}
					//finally add into vertex handles
					vertexHandles.push_back(vertexHandle);
				}
				//when finish three vertex then add vertex handles into faces
				mesh.add_face(vertexHandles);
			}

			mesh.update_normals();
		}
		else if(!isHover)
		{
			//is not already selected texture id
			//and it is not hover mode

			//temp record vertex
			std::map<int, int> usedVertexs;
			//loop all selected faces
			for (auto faceIDIterator = meshobj.selectedFace.begin(); faceIDIterator != meshobj.selectedFace.end(); ++faceIDIterator)
			{
				//get the face handle from selected texture id 
				MyMesh::FaceHandle faceHandle = meshobj.model.mesh.face_handle(*faceIDIterator);

				std::vector<MyMesh::VertexHandle> vertexHandles;//needed to push into mesh data's face
				vertexHandles.reserve(3);
				for (MyMesh::FaceVertexIter faceVertexIterator = meshobj.model.mesh.fv_iter(faceHandle); faceVertexIterator.is_valid(); ++faceVertexIterator) {

					MyMesh::VertexHandle vertexHandle;

					//first check if already used means we don't need to push into openmesh data
					std::map<int, int>::iterator usedVertexsIterator = usedVertexs.find(faceVertexIterator->idx());

					if (usedVertexsIterator == usedVertexs.end()) //means not added to mesh data
					{
						MyMesh::Point p = meshobj.model.mesh.point(*faceVertexIterator);//get point
						vertexHandle = mesh.add_vertex(p);
						usedVertexs[faceVertexIterator->idx()] = vertexHandle.idx();//record it to vertex handle id next time used
					}
					else {
						vertexHandle = mesh.vertex_handle(usedVertexs[faceVertexIterator->idx()]);
					}
					//finally add into vertex handles
					vertexHandles.push_back(vertexHandle);
				}
				//when finish three vertex then add vertex handles into faces
				mesh.add_face(vertexHandles);
			}

			mesh.update_normals();
		}
		else if (isHover) 
		{
			//temp record vertex
			std::map<int, int> usedVertexs;
			//loop all selected faces
			for (auto faceIDIterator = meshobj.hoverSelectedFaces.begin(); faceIDIterator != meshobj.hoverSelectedFaces.end(); ++faceIDIterator)
			{
				//get the face handle from selected texture id 
				MyMesh::FaceHandle faceHandle = meshobj.model.mesh.face_handle(*faceIDIterator);

				std::vector<MyMesh::VertexHandle> vertexHandles;//needed to push into mesh data's face
				vertexHandles.reserve(3);
				for (MyMesh::FaceVertexIter faceVertexIterator = meshobj.model.mesh.fv_iter(faceHandle); faceVertexIterator.is_valid(); ++faceVertexIterator) {

					MyMesh::VertexHandle vertexHandle;

					//first check if already used means we don't need to push into openmesh data
					std::map<int, int>::iterator usedVertexsIterator = usedVertexs.find(faceVertexIterator->idx());

					if (usedVertexsIterator == usedVertexs.end()) //means not added to mesh data
					{
						MyMesh::Point p = meshobj.model.mesh.point(*faceVertexIterator);//get point
						vertexHandle = mesh.add_vertex(p);
						usedVertexs[faceVertexIterator->idx()] = vertexHandle.idx();//record it to vertex handle id next time used
					}
					else {
						vertexHandle = mesh.vertex_handle(usedVertexs[faceVertexIterator->idx()]);
					}
					//finally add into vertex handles
					vertexHandles.push_back(vertexHandle);
				}
				//when finish three vertex then add vertex handles into faces
				mesh.add_face(vertexHandles);
			}

			mesh.update_normals();
		}
	}

	static void FindNearFaces(MeshObject& meshobj, int currentTexID,bool isHover=false, bool isScale=true) {
		//1. request face info
		//need temp mesh
		MyMesh mesh;
		std::vector<unsigned int> selectedFaces;
		if (isHover) {
			HelperFunctionTemporaryMeshUpdateFaces(meshobj, mesh, currentTexID, false, isHover);//copy it to make our's boundary detector
			selectedFaces = meshobj.hoverSelectedFaces;
		}
		else if (currentTexID==-1||currentTexID >= meshobj.textures.size()) {
			HelperFunctionTemporaryMeshUpdateFaces(meshobj, mesh, currentTexID, false,isHover);//copy it to make our's boundary detector
			//2. copy just selected faces
			selectedFaces = meshobj.selectedFace;
		}
		else {
			HelperFunctionTemporaryMeshUpdateFaces(meshobj, mesh, currentTexID, true,isHover);//copy it to make our's boundary detector
			//2. copy just selected faces
			selectedFaces = (meshobj.textures[currentTexID]->faceIDs);
		}

		if (selectedFaces.size() <= 0)return;
		//first find the boundary
		std::vector<unsigned int> boundary;

		int index = 0;
		//use temporary mesh to loop all faces
		for (MyMesh::FaceIter tempMeshFaceIterator = mesh.faces_begin(); tempMeshFaceIterator != mesh.faces_end(); ++tempMeshFaceIterator)
		{

			//we need mesh obj's faces handler to find near faces by vertex!
			//second we need temporary mesh for boundary detect!
			MyMesh::FaceHandle meshobjSelectedFacesHandle = meshobj.model.mesh.face_handle(selectedFaces[index++]);//find the handle on mesh obj
			MyMesh::FaceHandle selectedFacesHandle = tempMeshFaceIterator.handle();//find the handle on temp mesh

			//then find the face
			//check if is valid keep loop
			MyMesh::FaceVertexIter meshobjSelectedFacesVertexIterator = meshobj.model.mesh.fv_iter(meshobjSelectedFacesHandle);
			MyMesh::FaceVertexIter selectedFacesVertexIterator = mesh.fv_iter(selectedFacesHandle);
			for (; selectedFacesVertexIterator.is_valid() && meshobjSelectedFacesVertexIterator.is_valid(); ++selectedFacesVertexIterator, ++meshobjSelectedFacesVertexIterator)
			{
				//now we get the selected faces' vertex find the vertex near's faces "specific is meshobj's selected face nearly faces"
				MyMesh::VertexFaceIter meshobjSelectedFacesVertexFaceIterator = meshobj.model.mesh.vf_iter(meshobjSelectedFacesVertexIterator.handle());//from selected face find vertex then find near face

				for (; meshobjSelectedFacesVertexFaceIterator.is_valid(); ++meshobjSelectedFacesVertexFaceIterator) {

					if (mesh.is_boundary(selectedFacesVertexIterator.handle()))
					{
						//use vertexhandle to check is on boundary
						MyMesh::FaceHandle nearFacesOnBoundary = (meshobjSelectedFacesVertexFaceIterator.handle());
						boundary.push_back(nearFacesOnBoundary.idx());
					}
				}

			}
		}

		//put it into selected faces.

		if (!isHover) //not hover
		{
			if (isScale) {
				for (auto i = boundary.begin(); i != boundary.end(); ++i)
				{
					meshobj.AddSelectedFace(*i);
				}
			}
			else //we care about is scale or shrink
			{
				for (auto i = boundary.begin(); i != boundary.end(); ++i)
				{
					meshobj.DeleteSelectedFace(*i);
				}
			}
			
		}
		else //is hover just add into it
		{
			for (auto i = boundary.begin(); i != boundary.end(); ++i)
			{
				meshobj.AddHoverSelectedFace(*i);
			}
		}
		
	}

};
