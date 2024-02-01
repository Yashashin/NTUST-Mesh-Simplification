#include "MeshObject.h"
#include <Eigen/Sparse>
#include <map>
#include <algorithm>

#define Quad
//#define Harmonic

struct OpenMesh::VertexHandle const OpenMesh::PolyConnectivity::InvalidVertexHandle;



#pragma region MyMesh

MyMesh::MyMesh()
{
	request_vertex_normals();
	request_vertex_status();
	request_face_status();
	request_edge_status();
}

MyMesh::~MyMesh()
{

}

int MyMesh::FindVertex(MyMesh::Point pointToFind)
{
	int idx = -1;
	for (MyMesh::VertexIter v_it = vertices_begin(); v_it != vertices_end(); ++v_it)
	{
		MyMesh::Point p = point(*v_it);
		if (pointToFind == p)
		{
			idx = v_it->idx();
			break;
		}
	}

	return idx;
}

void MyMesh::ClearMesh()
{
	if (!faces_empty())
	{
		for (MyMesh::FaceIter f_it = faces_begin(); f_it != faces_end(); ++f_it)
		{
			delete_face(*f_it, true);
		}

		garbage_collection();
	}
}

bool MyMesh::Simplification()
{
	EdgeHandle eh;
	Eigen::Vector4d eTmpV;
	Point tmpV;

	std::multiset<Error>::iterator eEIter = error.begin();
	//std::multiset<Error>::iterator eEIter = error.end();
	//Loop every edge error
	while (eEIter != error.end())
	{
		if (status(eEIter->edgeIter.handle()).deleted()) //If edge is deleted, remove it from mutiset 
		{
			eEIter = error.erase(eEIter);
			continue;
		}
		eh = eEIter->edgeIter.handle();
		eTmpV = property(newVertexHandle, eh);
		tmpV[0] = eTmpV[0];
		tmpV[1] = eTmpV[1];
		tmpV[2] = eTmpV[2];

		std::map<int, double> checkQe;
		if (CheckAngle(eh)) //Check edge not overlap
		{
			HalfedgeHandle heh = halfedge_handle(eh, 1);
			VHandle to = to_vertex_handle(heh);
			VHandle from = from_vertex_handle(heh);
			Point tmpTo = point(to);
			set_point(to, tmpV); //Set to new vertex
			if (is_collapse_ok(heh))
			{
				collapse(heh);
				UpdateErrorMatrix(to, checkQe);
				UpdateEdgeError(checkQe);
				return true;
			}
			else
			{
				set_point(to, tmpTo); //Set back
				eEIter++;
			}
		}
		else
		{
			eEIter++;
		}
	}
	return false;
}

void MyMesh::CalculateErrorMatrix()
{
	//Calculate Qv
	for (VIter vi = vertices_begin(); vi != vertices_end(); vi++)
	{
		CalculateQVertex(vi.handle());
	}

	error.clear();
	//Calculate Qe
	for (EIter ei = edges_begin(); ei != edges_end(); ei++)
	{
		this->property(idHandle, ei) = ei.handle().idx();
		double Qe = CalculateQEdge(ei.handle());
		error.insert(Error(Qe,ei, property(idHandle, ei)));
	}
}

void MyMesh::UpdateErrorMatrix(VertexHandle vh,std::map<int,double>&checkQe)
{
	CalculateQVertex(vh); //Update vertex error

	//Update neighbor vertex error
	for (VVIter vvi = vv_begin(vh); vvi; vvi++)
	{
		if (status(vvi).deleted())
		{
			continue;
		}
		CalculateQVertex(vvi.handle());
	}

	//Update vertex edge error
	for (VEIter vei = ve_begin(vh); vei; vei++)
	{
		if (status(vei).deleted())
		{
			continue;
		}
		double Qe=CalculateQEdge(vei.handle());
		checkQe[property(idHandle, vei)] = property(qeHandle, vei);
	}

	//Update neighbor vertex edge error
	for (VVIter vvi = vv_begin(vh); vvi; vvi++)
	{
		for (VEIter vei = ve_begin(vvi); vei; vei++)
		{
			if (status(vei).deleted() || checkQe.find(vei.handle().idx()) != checkQe.end())
			{
				continue;
			}
			CalculateQEdge(vei.handle());
			checkQe[property(idHandle, vei)] = property(qeHandle, vei);
		}
	}
}

void MyMesh::UpdateEdgeError(std::map<int, double>& checkQe)
{
	std::vector<Error> newData;
	std::multiset<Error>::iterator it = error.begin();
	while (it != error.end())
	{
		int id = it->edgeIter.handle().idx();
		if (is_valid_handle(it->edgeIter) && !status(it->edgeIter).deleted())
		{
			if (checkQe.find(it->id) != checkQe.end())
			{
				newData.push_back(Error(property(qeHandle, it->edgeIter), it->edgeIter, property(idHandle, it->edgeIter)));
				it = error.erase(it);
			}
			else
			{
				if (property(qeHandle, it->edgeIter) != it->Qe)
				{
					newData.push_back(Error(property(qeHandle, it->edgeIter), it->edgeIter, property(idHandle, it->edgeIter)));
					it = error.erase(it);
				}
				else
				{
					it++;
				}
			}
		}
		else
		{
			it = error.erase(it);
			deletedEdgeCount++;
		}
	}
	for (int i = 0; i < newData.size(); i++)
	{
		error.insert(newData[i]);
	}

}

void MyMesh::UpdateSelectedFaceWeight(std::vector<unsigned int> selectedFaces)
{
	std::map <int,double> recordIndex;
	
	for (int i = 0; i < selectedFaces.size(); i++)
	{
		FHandle fh = face_handle(selectedFaces[i]);
		for (FaceEdgeIter fe = fe_begin(fh); fe; fe++)
		{
			property(qeHandle, fe.handle()) = 100;
			recordIndex[fe.handle().idx()]=100;
			
		}
	}
	std::multiset<Error>::iterator it = error.begin();
	std::vector<Error> newData;
	while (it != error.end())
	{
		int index = it->edgeIter.handle().idx();
		if (recordIndex.find(index) != recordIndex.end()) //If this edge error appear in recordIndex
		{
			//std::cout << property(qeHandle, it->edgeIter)<<std::endl;
			newData.push_back(Error(property(qeHandle, it->edgeIter), it->edgeIter, property(idHandle, it->edgeIter)));
			it = error.erase(it);
		}
		else
		{
			it++;
		}
		
	}
	for (int i = 0; i < newData.size(); i++)
	{
		error.insert(newData[i]);
	}
}

void MyMesh::CalculateQVertex(VertexHandle vh)
{
	Eigen::Matrix4d Qv = Eigen::Matrix4d::Zero();

	for(VFIter vfh = vf_begin(vh);vfh;vfh++)
	{
		if (status(vfh).deleted()) //If face is deleted
		{
			continue;
		}

		Normal fn = calc_face_normal(vfh);
		Point p = point(vh);
		//  q = {a,b,c,d}
		//  ax+by+cz+d=0
		//	Qv = q*q = { 
		//				aa ab ac ad
		//				ab bb bc bd
		//				ac bc cc cd
		//				ad bd cd dd
		//		       }
		Eigen::Vector4d q(fn[0], fn[1], fn[2], 0);
		q[3] = -(fn[0] *p[0] + fn[1] *p[1] + fn[2] * p[2]);
		Qv += q * q.transpose();
	}
	property(qvHandle, vh) = Qv;
}


double MyMesh::CalculateQEdge(EdgeHandle eh)
{
	HalfedgeHandle heh = halfedge_handle(eh, 0);
	VHandle toH = to_vertex_handle(heh);
	VHandle fromH = from_vertex_handle(heh);
	Eigen::Matrix4d newQ = property(qvHandle, toH) + property(qvHandle, fromH);
	Eigen::Matrix4d tmpQ = newQ;
	Eigen::Vector4d T(0, 0, 0, 1);
	tmpQ.row(3) = T;
	Eigen::Vector4d newVertex = tmpQ.partialPivLu().solve(T);
	double Qe = newVertex.transpose() * (newQ * newVertex);
	property(newVertexHandle, eh) = newVertex;
	property(qeHandle, eh) = Qe;
	return Qe;
}

bool MyMesh::CheckAngle(EdgeHandle eh)
{
	HalfedgeHandle heh = halfedge_handle(eh, 1);
	VHandle to = to_vertex_handle(heh);
	VHandle from = from_vertex_handle(heh);
	std::vector<Point> points;

	//One ring of to_vertex
	for(VVIter vvi = vv_begin(to);vvi ;vvi++)
	{
		if (status(vvi).deleted())
		{
			continue;
		}
		VHandle neighborV = vvi.handle();
		if (!neighborV.idx() == from.idx()) //if neighbor is not from_vertex
		{
			points.push_back(point(neighborV));
		}

	}

	//One ring of from_vertex
	for (VVIter vvi = vv_begin(to); vvi; vvi++)
	{
		if (status(vvi).deleted())
		{
			continue;
		}
		VHandle neighborV = vvi.handle();
		if (!neighborV.idx() == to.idx()) //if neighbor is not to_vertex
		{
			bool flag = false;
			for (int i = 0; i < points.size(); i++)
			{
				if (points[i] == point(neighborV)) //Duplicated vertex
				{
					flag = true;
					break;
				}
			}
			if (!flag)
			{
				points.push_back(point(neighborV));
			}
		}
	}

	double angle;
	for (int i = 0; i < points.size(); i++)
	{
		if (points.size() < 2)
		{
			return false;
		}
		if (i == 0)
		{
			angle = Angle(points[points.size() - 1], points[i + 1], points[i]);
		}
		else if (i == points.size() - 1)
		{
			angle = Angle(points[i - 1], points[0], points[i]);
		}
		else
		{
			angle = Angle(points[i - 1], points[i + 1], points[i]);
		}
		if (angle > 180)
		{
			return false;
		}
	}
	return true;
}

double MyMesh::Angle(Point point1, Point point2, Point point0)
{
	Eigen::Vector3d v1((point1 - point0)[0], (point1 - point0)[1], (point1 - point0)[2]);
	Eigen::Vector3d v2((point2 - point0)[0], (point2 - point0)[1], (point2 - point0)[2]);
	double v1v2 = v1.dot(v2);
	double v1Length = v1.norm();
	double v2Length = v2.norm();
	return acos(v1v2/(v1Length*v2Length));
}

void MyMesh::LeastSquare(bool isInit)
{
	Eigen::SparseMatrix<double> L(n_vertices() * 2, n_vertices());
	Eigen::VectorXd Bx(n_vertices() * 2);
	Eigen::VectorXd By(n_vertices() * 2);
	Eigen::VectorXd Bz(n_vertices() * 2);
	L.setZero();
	Bx.setZero();
	By.setZero();
	Bz.setZero();
	CalculateWi();
	if (isInit)
	{
		CalculateAi();
		double a = CalculateAverageArea();
		wl = 0.001f* std::sqrtf(a); //Init wl
	}
	else
	{
		wl = wl * sl;
	}

	//Large matrix
	for (VIter vi = vertices_begin(); vi != vertices_end(); vi++)
	{
		double totalWi = 0;
		double ai = property(this->ai, vi);
		double at = 0;
		std::vector<Point> points;
		for (VVIter vvi = vv_begin(vi); vvi; vvi++)
		{
			HalfedgeHandle heh = find_halfedge(vi, vvi);
			//Fill matrix
			L.insert(vi->idx(), vvi->idx())= property(this->wi, heh) * wl;
			
			points.push_back(point(vvi));
			totalWi += property(wi, heh);
		}

		//Loop one-ring vertice
		Point centerPoint = point(vi);
		for (int i = 0; i < points.size(); i++)
		{
			double a = (centerPoint - points[i % points.size()]).length();
			double b = (centerPoint - points[(i+1) % points.size()]).length();
			double c = (points[i % points.size()] - points[(i + 1) % points.size()]).length();
			double s = (a + b + c) / 2.0f;
			at += std::sqrtf(s * (s - a) * (s - b) * (s - c));
		}

		double wh=1.0;
		if (isInit)
		{
			wh = 1.0; //Init wh
			InitTotalA = at;
		}
		else
		{
			//Prevent mesh dissapear
			if (at < threshould)
			{
				wh = wl;
			}
			else
			{
				wh = wh* std::sqrtf(ai / at);
			}
			//std::cout << wh << std::endl;
		}
		//Fill diagonal
		L.insert(vi->idx(), vi->idx()) = -1 * totalWi * wl;

		//Fill down part
		L.insert(vi->idx() + n_vertices(), vi->idx()) = wh;

		//Fill B
		Bx[vi->idx() + n_vertices()] = point(vi)[0] * wh;
		By[vi->idx() + n_vertices()] = point(vi)[1] * wh;
		Bz[vi->idx() + n_vertices()] = point(vi)[2] * wh;
	}


	Eigen::SimplicialLDLT<Eigen::SparseMatrix<double>> solver;
	Bx = L.transpose() * Bx;
	By = L.transpose() * By;
	Bz = L.transpose() * Bz;
	L = L.transpose() * L;
	L.makeCompressed();
	//Solver linear
	solver.compute(L);
	Eigen::VectorXd Vx = solver.solve(Bx);
	Eigen::VectorXd Vy = solver.solve(By);
	Eigen::VectorXd Vz = solver.solve(Bz);
	
	//Modify vertex
	for (VIter vi = this->vertices_begin(); vi != this->vertices_end(); vi++)
	{
		this->point(vi)[0] = Vx[vi->idx()];
		this->point(vi)[1] = Vy[vi->idx()];
		this->point(vi)[2] = Vz[vi->idx()];
	}

	request_face_normals();
	update_normals();
	release_face_normals();
	//sl -= 0.1;
	std::cout << "LS!" << std::endl;
	std::cout << sl << std::endl;
}

void MyMesh::CalculateAi()
{
	
	// Loop all vertice
	for (VIter vi = vertices_begin(); vi != vertices_end(); vi++)
	{
		std::vector<Point> neighborPoints;
		Point centerPoint;
		//Get one-ring vertice
		for (VVIter vvi = vv_begin(vi.handle()); vvi; vvi++)
		{
			neighborPoints.push_back(point(vvi));
		}
		centerPoint = point(vi);
		double ai = 0;
		for (int i = 0; i < neighborPoints.size(); i++)
		{
			double a = (centerPoint - neighborPoints[i % neighborPoints.size()]).length();
			double b = (centerPoint - neighborPoints[(i+1) % neighborPoints.size()]).length();
			double c = (neighborPoints[i % neighborPoints.size()] - neighborPoints[(i+1) % neighborPoints.size()]).length();
			double s = (a + b + c) / 2.0f;
			ai += std::sqrtf(s * (s - a) * (s - b) * (s - c));
		}
		property(this->ai, vi.handle()) = ai;
	}
}

double MyMesh::CalculateAverageArea()
{
	double totalArea = 0;
	//Loop all faces
	for (FIter fi = faces_begin(); fi != faces_end(); fi++)
	{
		std::vector<Point> points;
		//Get vertice on face
		for (FVIter fvi = fv_begin(fi);fvi ; fvi++)
		{
			points.push_back(point(fvi));
		}
		double a = (points[0] - points[1]).length();
		double b = (points[0] - points[2]).length();
		double c = (points[1] - points[2]).length();
		double s = (a + b + c) / 2.0f;
		totalArea += std::sqrtf(s * (s - a) * (s - b) * (s - c));

	}
	double averageArea = totalArea / n_faces();
	return averageArea;
}

void MyMesh::CalculateWi()
{
	//Loop all edge
	for (EIter ei = edges_begin(); ei != edges_end(); ei++)
	{
		double wi;
		EHandle eh = ei.handle();
		HHandle heh = halfedge_handle(eh, 0);
		HHandle heh2 = halfedge_handle(eh, 1);
		VHandle fromV = from_vertex_handle(heh);
		VHandle toV = to_vertex_handle(heh);
		VHandle oppositeV = opposite_vh(heh);
		VHandle oppositeOppositeV = opposite_he_opposite_vh(heh);
		Point fromP = point(fromV);
		Point toP = point(toV);
		Point oppoP = point(oppositeV);
		Point oppoOppoP = point(oppositeOppositeV);
		double angle1, angle2;
		angle1 = Angle(fromP, toP, oppoP);
		angle2 = Angle(fromP, toP, oppoOppoP);
		double tmpWi = (1.0f / tan(angle1))+(1.0f/tan(angle2));
		property(this->wi, heh)=tmpWi;
		property(this->wi, heh2)=tmpWi;
	}
}

void MyMesh::Init()
{
	add_property(qvHandle, "Qv");
	property(qvHandle, vertices_begin()) = Eigen::Matrix4d::Zero();
	add_property(qeHandle, "Qe");
	add_property(newVertexHandle,"New Vertex");
	add_property(idHandle, "ID");
	add_property(ai, "Ai");
	add_property(wi, "Wi");
	deletedEdgeCount = 0;
	error.clear();
}

#pragma endregion

#pragma region GLMesh

GLMesh::GLMesh()
{

}

GLMesh::~GLMesh()
{

}

bool GLMesh::Init(std::string fileName)
{
	if (LoadModel(fileName))
	{
		LoadToShader();
		return true;
	}
	return false;
}

void GLMesh::Render()
{
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, mesh.n_faces() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


bool GLMesh::LoadModel(std::string fileName)
{
	OpenMesh::IO::Options ropt;
	if (OpenMesh::IO::read_mesh(mesh, fileName, ropt))
	{
		if (!ropt.check(OpenMesh::IO::Options::VertexNormal) && mesh.has_vertex_normals())
		{
			mesh.request_face_normals();
			mesh.update_normals();
			mesh.release_face_normals();
		}

		return true;
	}

	return false;
}

void GLMesh::LoadToShader()
{
	std::vector<MyMesh::Point> vertices;
	vertices.reserve(mesh.n_vertices());
	for (MyMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
	{
		vertices.push_back(mesh.point(*v_it));

		MyMesh::Point p = mesh.point(*v_it);
	}

	std::vector<MyMesh::Normal> normals;
	normals.reserve(mesh.n_vertices());
	for (MyMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
	{
		normals.push_back(mesh.normal(*v_it));
	}

	std::vector<unsigned int> indices;
	indices.reserve(mesh.n_faces() * 3);
	for (MyMesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it)
	{
		for (MyMesh::FaceVertexIter fv_it = mesh.fv_iter(*f_it); fv_it.is_valid(); ++fv_it)
		{
			indices.push_back(fv_it->idx());
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vboVertices);
	glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MyMesh::Point) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &vboNormal);
	glBindBuffer(GL_ARRAY_BUFFER, vboNormal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MyMesh::Normal) * normals.size(), &normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

#pragma endregion

MeshObject::MeshObject()
{

}

MeshObject::~MeshObject()
{
}

bool MeshObject::Init(std::string fileName)
{
	selectedFace.clear();
	this->modelFaceColor = glm::vec4(1, 1, 1, 1);
	this->hoverFaceColor = glm::vec4(0, 1, 0, 1);
	this->selectedFaceColor = glm::vec4(1, 0, 0, 1);
	this->wireFaceColor = glm::vec4(1, 1, 0, 1);
	this->wireColor = glm::vec4(0, 105.0 / 255.0, 148.0 / 255.0, 1);
	return model.Init(fileName);
}

void MeshObject::Render()
{
	glBindVertexArray(model.vao);
	glDrawElements(GL_TRIANGLES, model.mesh.n_faces() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void MeshObject::RenderSelectedFace()
{
	if (selectedFace.size() > 0)
	{
		std::vector<unsigned int*> offsets(selectedFace.size());
		for (int i = 0; i < offsets.size(); ++i)
		{
			offsets[i] = (GLuint*)(selectedFace[i] * 3 * sizeof(GLuint));
		}

		std::vector<int> count(selectedFace.size(), 3);

		glBindVertexArray(model.vao);
		glMultiDrawElements(GL_TRIANGLES, &count[0], GL_UNSIGNED_INT, (const GLvoid**)&offsets[0], selectedFace.size());
		glBindVertexArray(0);
	}
}

void MeshObject::RenderTexture(int id)
{
		glBindVertexArray(model.vao);
		glBindBuffer(GL_ARRAY_BUFFER,textures[id]->textureVbo);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);

		glMultiDrawElements(GL_TRIANGLES, &(textures[id]->elementCount[0]), GL_UNSIGNED_INT, (const GLvoid**)&(textures[id]->fvIDsPtr[0]), textures[id]->elementCount.size());
		glBindVertexArray(0);
}

bool MeshObject::AddSelectedFace(unsigned int faceID)
{
	if (std::find(selectedFace.begin(), selectedFace.end(), faceID) == selectedFace.end() &&
		faceID >= 0 && faceID < model.mesh.n_faces())
	{
		selectedFace.push_back(faceID);
		return true;
	}
	return false;
}

void MeshObject::DeleteSelectedFace(unsigned int faceID)
{
	selectedFace.erase(std::remove(selectedFace.begin(), selectedFace.end(), faceID), selectedFace.end());
}

bool MeshObject::FindClosestPoint(unsigned int faceID, glm::vec3 worldPos, glm::vec3& closestPos)
{
	OpenMesh::FaceHandle fh = model.mesh.face_handle(faceID);
	if (!fh.is_valid())
	{
		return false;
	}

	double minDistance = 0.0;
	MyMesh::Point p(worldPos.x, worldPos.y, worldPos.z);
	MyMesh::FVIter fv_it = model.mesh.fv_iter(fh);
	MyMesh::VertexHandle closestVH = *fv_it;
	MyMesh::Point v1 = model.mesh.point(*fv_it);
	++fv_it;

	minDistance = (p - v1).norm();
	for (; fv_it.is_valid(); ++fv_it)
	{
		MyMesh::Point v = model.mesh.point(*fv_it);
		double distance = (p - v).norm();
		if (minDistance > distance)
		{
			minDistance = distance;
			closestVH = *fv_it;
		}
	}
	MyMesh::Point closestPoint = model.mesh.point(closestVH);
	closestPos.x = closestPoint[0];
	closestPos.y = closestPoint[1];
	closestPos.z = closestPoint[2];
	return true;
}

bool MeshObject::AddHoverSelectedFace(unsigned int faceID)
{
	if (std::find(hoverSelectedFaces.begin(), hoverSelectedFaces.end(), faceID) == hoverSelectedFaces.end() &&
		faceID >= 0 && faceID < model.mesh.n_faces())
	{
		hoverSelectedFaces.push_back(faceID);
		return true;
	}
	return false;


}

void MeshObject::DeleteHoverSelectedFace(unsigned int faceID)
{
	hoverSelectedFaces.erase(std::remove(hoverSelectedFaces.begin(), hoverSelectedFaces.end(), faceID), hoverSelectedFaces.end());
}

void MeshObject::RenderHoverSelectedFace()
{
	if (hoverSelectedFaces.size() > 0)
	{
		std::vector<unsigned int*> offsets(hoverSelectedFaces.size());
		for (int i = 0; i < offsets.size(); ++i)
		{
			offsets[i] = (GLuint*)(hoverSelectedFaces[i] * 3 * sizeof(GLuint));
		}

		std::vector<int> count(hoverSelectedFaces.size(), 3);

		glBindVertexArray(model.vao);
		glMultiDrawElements(GL_TRIANGLES, &count[0], GL_UNSIGNED_INT, (const GLvoid**)&offsets[0], hoverSelectedFaces.size());
		glBindVertexArray(0);
	}
}
