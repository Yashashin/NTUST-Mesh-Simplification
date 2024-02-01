#pragma once

#include <string>
#include <vector>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include "Texture.h"

//Eigen
#include <Eigen/Dense>
#include <Eigen/Sparse>

//Include glew
#include <gl/glew.h>
#include <gl/gl.h>
#include <gl/glu.h>

//Include glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

typedef OpenMesh::TriMesh_ArrayKernelT<>  TriMesh;
//typedef OpenMesh::PolyMesh_ArrayKernelT<OMT::MyTraits> MyOpenMesh;



//Error of an edge
class Error 
{
public:
	Error() {}
	Error(double q, TriMesh::EdgeIter ei, int id) :Qe(q), edgeIter(ei), id(id) {}
	
	bool operator < (const Error& error) const
	{
		return (Qe) < (error.Qe);
	}

	double Qe;
	TriMesh::EdgeIter edgeIter;
	int id;

};


class MyMesh : public TriMesh
{
public:
	MyMesh();
	~MyMesh();

	std::multiset <Error> error;								//< Error records from small to large
	OpenMesh::EPropHandleT<Eigen::Vector4d> newVertexHandle;	//< New vertex of edge
	OpenMesh::VPropHandleT<Eigen::Matrix4d> qvHandle;			//< Qv of vertex
	OpenMesh::EPropHandleT<double> qeHandle;					//< Qe of edge
	OpenMesh::EPropHandleT<int> idHandle;
	OpenMesh::VPropHandleT<double> ai;
	OpenMesh::HPropHandleT<double> wi;
														
	int deletedEdgeCount = 0;
	double wl = 1.0;
	double sl = 35;
	double threshould = 0.00001f;
	double InitTotalA=0;

	void Init();
	int FindVertex(MyMesh::Point pointToFind);
	void ClearMesh();
	bool Simplification();
	void CalculateErrorMatrix();
	void UpdateErrorMatrix(VertexHandle vh, std::map<int, double>& checkQe);
	void UpdateEdgeError(std::map<int, double>& checkQe);
	void UpdateSelectedFaceWeight(std::vector<unsigned int> selectedFaces);
	void CalculateQVertex(VertexHandle vh);
	double CalculateQEdge(EdgeHandle eh);
	bool CheckAngle(EdgeHandle eh);
	double Angle(Point point1, Point point2, Point point0);
	void LeastSquare(bool isInit);
	void CalculateAi();
	double CalculateAverageArea();
	void CalculateWi();

};

class GLMesh
{
public:
	GLMesh();
	~GLMesh();

	bool Init(std::string fileName);
	void Render();


	MyMesh mesh;
	GLuint vao;
	std::vector<GLuint> tVbos;
	GLuint ebo;
	GLuint vboVertices, vboNormal;

	bool LoadModel(std::string fileName);
	void LoadToShader();
};

class MeshObject
{
public:
	MeshObject();
	~MeshObject();

	bool Init(std::string fileName);
	void Render();
	void RenderSelectedFace();
	void RenderTexture(int id);
	bool AddSelectedFace(unsigned int faceID);
	void DeleteSelectedFace(unsigned int faceID);
	bool FindClosestPoint(unsigned int faceID, glm::vec3 worldPos, glm::vec3& closestPos);


	//hover method
	bool AddHoverSelectedFace(unsigned int faceID);
	void DeleteHoverSelectedFace(unsigned int faceID);
	void RenderHoverSelectedFace();

	GLMesh model;
	std::vector<Texture*> textures;
	std::vector<unsigned int> selectedFace;
	
	//temp hover buffer
	std::vector<unsigned int> hoverSelectedFaces;

	//custom color
	glm::vec4 selectedFaceColor;
	glm::vec4 modelFaceColor;
	glm::vec4 hoverFaceColor;
	glm::vec4 wireFaceColor;
	glm::vec4 wireColor;
};



