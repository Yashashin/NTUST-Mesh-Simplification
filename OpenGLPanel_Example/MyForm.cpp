#include "MyForm.h"

#include "Paintbrush.h"

using namespace System;
using namespace System::Windows::Forms;
[STAThread]

void main(array<String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	OpenGLPanel_Example::MyForm form;
	Application::Run(% form);
}

void OpenGLPanel_Example::MyForm::OnMouseWheel(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
{
	if (e->Delta > 0)
	{
		main_camera.wheelEvent(-5);
	}
	else
	{
		main_camera.wheelEvent(5);
	}
}

void OpenGLPanel_Example::MyForm::hkoglPanelControl1_Load(System::Object^ sender, System::EventArgs^ e)
{
	// Initialize GLEW
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "GLEW is not initialized!\n";
		return;
	}

	//Init 
	paramGL = new ParamGL();
	model = new MeshObject();
	Texture* tex = new Texture();
	model->textures.push_back(tex);
	pickingTexture = new PickingTexture();
	currentTextureID = 0;
	renderMode = RenderMode::WIREFRAME;
	main_camera.Reset();

	// Load shader from files
	ShaderInfo wireframeShaders[] =
	{
		{ GL_VERTEX_SHADER,  "../Shader/wireframe.vs" },
		{ GL_GEOMETRY_SHADER, "../Shader/wireframe.gs"},
		{ GL_FRAGMENT_SHADER, "../Shader/wireframe.fs" },
		{ GL_NONE, NULL }
	};
	paramGL->wireframeShader = LoadShaders(wireframeShaders);
	ShaderInfo pickingShaders[] = {
		{GL_VERTEX_SHADER, "../Shader/picking.vs"},
		{GL_FRAGMENT_SHADER, "../Shader/picking.fs"},
		{GL_NONE,NULL}
	};
	paramGL->pickingShader = LoadShaders(pickingShaders);
	ShaderInfo modelShaders[] = {
	{GL_VERTEX_SHADER, "../Shader/model.vs"},
	{GL_FRAGMENT_SHADER, "../Shader/model.fs"},
	{GL_NONE,NULL}
	};
	paramGL->modelShader = LoadShaders(modelShaders);


	//Load model
	this->Cursor = Cursors::AppStarting;
	if (model->Init("../Model/bear10.obj"))
	{
		std::cout << "Load Model" << std::endl;
	}
	else
	{
		std::cout << "Load Model Failed" << std::endl;
	}
	this->Cursor = Cursors::Arrow;

	//Init Mesh
	model->model.mesh.Init();
	model->model.mesh.CalculateErrorMatrix();


	//Picking Texture (frame buffer)
	pickingTexture->Init(this->hkoglPanelControl1->Size.Width, this->hkoglPanelControl1->Size.Height);

	//Set matrix
	paramGL->projection = glm::perspective(80.0f, ((float)this->hkoglPanelControl1->Size.Width / (float)this->hkoglPanelControl1->Size.Height), 0.1f, 100.0f);
	paramGL->view = glm::lookAt(glm::vec3(0, 0, 50), glm::vec3(0, 0, 0), glm::vec3(0, -1, 0));
	glm::mat4 model = glm::mat4(1);
	model = glm::scale(model, glm::vec3(100, 100, 100));
	paramGL->model = model;


	// Get and print used OpenGL version
	int ver[2] = { 0, 0 };
	glGetIntegerv(GL_MAJOR_VERSION, &ver[0]);
	glGetIntegerv(GL_MINOR_VERSION, &ver[1]);
	std::cout << "OpenGL Version: " << ver[0] << "." << ver[1] << std::endl;

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	addBrush = new Paintbrush();
	delBrush = new Paintbrush();
	currentBrush = addBrush;
}

void OpenGLPanel_Example::MyForm::hkoglPanelControl1_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(255, 255, 255, 255);
	paramGL->view = glm::lookAt(glm::vec3(50 * sin(eye_x), 3, 50 * cos(eye_x)), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	main_camera.SetWindowSize(this->hkoglPanelControl1->Size.Width, (float)this->hkoglPanelControl1->Size.Height);

	glm::mat4 model_matrix = glm::mat4(1);
	model_matrix = glm::scale(model_matrix, glm::vec3(100, 100, 100));
	paramGL->model = main_camera.GetModelMatrix() /* * model_matrix*/;
	paramGL->projection = main_camera.GetProjectionMatrix();
	paramGL->view = main_camera.GetViewMatrix();

	if (renderMode == WIREFRAME)
	{
		RenderToPickingTexture();
		RenderWireframe();
		//RenderWireframeTexture();
		RenderSelectedFace();
		if (mouseState == MouseState::RELEASE) {
			glUniform1i(glGetUniformLocation(paramGL->modelShader, "isTexture"), false);
			glUniform4f(glGetUniformLocation(paramGL->wireframeShader, "faceColor"), model->hoverFaceColor.r, model->hoverFaceColor.g, model->hoverFaceColor.b, model->hoverFaceColor.a);
			model->RenderHoverSelectedFace();
		}
	}
	else
	{
		RenderModel();
	//	RenderModelTexture();
	}
}

void OpenGLPanel_Example::MyForm::hkoglPanelControl1_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
{
	mouseState = MouseState::PRESS;
	if (e->Button == System::Windows::Forms::MouseButtons::Left) //select face
	{
		int posX = e->X;
		int posY = this->hkoglPanelControl1->Size.Height - (e->Y);
		GLuint faceID = pickingTexture->ReadTexture(posX, posY) - 1;
		if (faceID == -1) {
			return;
		}
		if (selectMode == SelectMode::PICKFACE) {
			//model->AddSelectedFace(faceID);
			//unsigned int id = faceID;
			//Paintbrush::ScaleSelectedFaces(*model, id, addBrush->GetPaintbrushSize());

			//check is inside
			for (int i = 0; i < model->hoverSelectedFaces.size(); ++i) {
				model->AddSelectedFace(model->hoverSelectedFaces[i]);
			}
			//model->selectedFace.insert(model->selectedFace.end(),model->hoverSelectedFaces.begin(),model->hoverSelectedFaces.end());
		}
		else if (selectMode == SelectMode::DELFACE) {
			std::cout << "Not yet to delete hover faces\n";
			for (int i = 0; i < model->hoverSelectedFaces.size(); ++i) {
				model->DeleteSelectedFace(model->hoverSelectedFaces[i]);
			}


			//unsigned int id = faceID;
			//Paintbrush::ScaleSelectedFaces(*model, id, addBrush->GetPaintbrushSize());
		}

	}
	else if (e->Button == System::Windows::Forms::MouseButtons::Middle)
	{
		//drag view
		main_camera.mouseEvents(GLUT_MIDDLE_BUTTON, GLUT_DOWN, e->X, e->Y);
	}
	else if (e->Button == System::Windows::Forms::MouseButtons::Right)
	{
		main_camera.mouseEvents(GLUT_LEFT_BUTTON, GLUT_DOWN, e->X, e->Y);
	}
}

void OpenGLPanel_Example::MyForm::hkoglPanelControl1_MouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
{
	if (e->Button == System::Windows::Forms::MouseButtons::Left) {
		model->hoverSelectedFaces.clear();

		int posX = e->X;
		int posY = this->hkoglPanelControl1->Size.Height - (e->Y);
		GLuint faceID = pickingTexture->ReadTexture(posX, posY) - 1;
		if (faceID == -1) {
			return;
		}
		if (selectMode == SelectMode::PICKFACE) {
			model->AddHoverSelectedFace(faceID);
			Paintbrush::ScaleHoverSelectedFaces(*model, addBrush->GetPaintbrushSize() - 1);
		}
		else if (selectMode == SelectMode::DELFACE) {
			//model->DeleteHoverSelectedFace(faceID);
			model->AddHoverSelectedFace(faceID);
			Paintbrush::ScaleHoverSelectedFaces(*model, delBrush->GetPaintbrushSize() - 1);
		}


		if (selectMode == SelectMode::PICKFACE) {
			for (int i = 0; i < model->hoverSelectedFaces.size(); ++i) {
				model->AddSelectedFace(model->hoverSelectedFaces[i]);
			}
		}
		else if (selectMode == SelectMode::DELFACE) {
			for (int i = 0; i < model->hoverSelectedFaces.size(); ++i) {
				model->DeleteSelectedFace(model->hoverSelectedFaces[i]);
			}
			//model->DeleteSelectedFace(faceID);
		}
	}
	else if (e->Button == System::Windows::Forms::MouseButtons::Right) {
		main_camera.mouseMoveEvent(e->X, e->Y);
	}
	else if (e->Button == System::Windows::Forms::MouseButtons::Middle) {
		main_camera.mouseMoveEvent(e->X, e->Y);
	}
	else //just hover on it
	{
		model->hoverSelectedFaces.clear();

		int posX = e->X;
		int posY = this->hkoglPanelControl1->Size.Height - (e->Y);
		GLuint faceID = pickingTexture->ReadTexture(posX, posY) - 1;
		if (faceID == -1) {
			return;
		}
		if (selectMode == SelectMode::PICKFACE) {
			model->AddHoverSelectedFace(faceID);
			//Paintbrush::ScaleHoverSelectedFaces(*model, addBrush->GetPaintbrushSize() - 1);
		}
		else if (selectMode == SelectMode::DELFACE) {
			//model->DeleteHoverSelectedFace(faceID);
			model->AddHoverSelectedFace(faceID);
			//Paintbrush::ScaleHoverSelectedFaces(*model, delBrush->GetPaintbrushSize() - 1);
		}
	}
}

void OpenGLPanel_Example::MyForm::hkoglPanelControl1_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
{
	mouseState = MouseState::RELEASE;
	if (e->Button == System::Windows::Forms::MouseButtons::Left) {
		//main_camera.mouseReleaseEvent(GLUT_LEFT_BUTTON, e->X, e->Y);
	}
	else if (e->Button == System::Windows::Forms::MouseButtons::Middle) {
		main_camera.mouseReleaseEvent(GLUT_MIDDLE_BUTTON, e->X, e->Y);
	}
	else if (e->Button == System::Windows::Forms::MouseButtons::Right) {
		//set our project's right click to left click which is in viewManager
		//main_camera.mouseReleaseEvent(GLUT_RIGHT_BUTTON, e->X, e->Y);
		main_camera.mouseReleaseEvent(GLUT_LEFT_BUTTON, e->X, e->Y);
	}
}

void OpenGLPanel_Example::MyForm::Load_Texture_Btn_Click(System::Object^ sender, System::EventArgs^ e)
{
	/*String^ FileName;
	if (!Directory::Exists(LastTextureDirectory))
	{
		LastTextureDirectory = Environment::GetFolderPath(Environment::SpecialFolder::MyDocuments);
	}
	OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog;

	openFileDialog1->Filter = "Image Files (*.bmp;*.jpg;*.png)|*.bmp;*.jpg;*.png|All Files(*.*)|*.*";
	openFileDialog1->FilterIndex = 1;
	try
	{
		openFileDialog1->InitialDirectory = LastTextureDirectory;
	}
	catch (Exception^)
	{
		openFileDialog1->InitialDirectory = Environment::GetFolderPath(Environment::SpecialFolder::MyDocuments);
	}

	if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
	{
		FileName = openFileDialog1->FileName;
		if (FileName->LastIndexOf("\\") >= 0)
		{
			LastTextureDirectory = FileName->Substring(0, (FileName->LastIndexOf("\\") + 1));
		}
	}*/
}


void OpenGLPanel_Example::MyForm::PaintbrushStripButton_Click(System::Object^ sender, System::EventArgs^ e)
{
	System::Windows::Forms::ToolStripButton^ ctrl = (ToolStripButton^)sender;
	int tagInteger = Int32::Parse(ctrl->Tag->ToString());
	std::cout << "Button type:" << tagInteger << std::endl;
	switch (tagInteger) {
	case 0: //add
		this->selectMode = SelectMode::PICKFACE; //set mode
		this->currentBrush = addBrush;
		this->PaintADDStripButton1->Checked = true;
		this->PaintDELStripButton2->Checked = false;
		this->PaintSizeScaleStripButton3->Checked = false;
		this->PaintSizeShrinkStripButton4->Checked = false;
		this->PaintFaceScaleStripButton2->Checked = false;
		this->PaintFaceShrinkStripButton1->Checked = false;
		break;
	case 1: //del
		this->selectMode = SelectMode::DELFACE; //set mode
		this->currentBrush = delBrush;
		this->PaintADDStripButton1->Checked = false;
		this->PaintDELStripButton2->Checked = true;
		this->PaintSizeScaleStripButton3->Checked = false;
		this->PaintSizeShrinkStripButton4->Checked = false;
		this->PaintFaceScaleStripButton2->Checked = false;
		this->PaintFaceShrinkStripButton1->Checked = false;
		break;
	case 2: //plus
		this->PaintADDStripButton1->Checked = false;
		this->PaintDELStripButton2->Checked = false;
		this->PaintSizeScaleStripButton3->Checked = false;
		this->PaintSizeShrinkStripButton4->Checked = false;
		this->PaintFaceScaleStripButton2->Checked = false;
		this->PaintFaceShrinkStripButton1->Checked = false;
		this->currentBrush->AddPaintbrushSize(1);
		break;
	case 3: //shrink
		this->PaintADDStripButton1->Checked = false;
		this->PaintDELStripButton2->Checked = false;
		this->PaintSizeScaleStripButton3->Checked = false;
		this->PaintSizeShrinkStripButton4->Checked = false;
		this->PaintFaceScaleStripButton2->Checked = false;
		this->PaintFaceShrinkStripButton1->Checked = false;
		this->currentBrush->AddPaintbrushSize(-1);
		break;
	case 4: //scale up by button
		this->PaintADDStripButton1->Checked = false;
		this->PaintDELStripButton2->Checked = false;
		this->PaintSizeScaleStripButton3->Checked = false;
		this->PaintSizeShrinkStripButton4->Checked = false;
		this->PaintFaceScaleStripButton2->Checked = false;
		this->PaintFaceShrinkStripButton1->Checked = false;
		this->ScaleSelectedFaces();
		break;
	case 5: //shrink down by button
		this->PaintADDStripButton1->Checked = false;
		this->PaintDELStripButton2->Checked = false;
		this->PaintSizeScaleStripButton3->Checked = false;
		this->PaintSizeShrinkStripButton4->Checked = false;
		this->PaintFaceScaleStripButton2->Checked = false;
		this->PaintFaceShrinkStripButton1->Checked = false;
		this->ShrinkSelectedFaces();
		break;
	default:
		std::cout << "Error button tag\n";
		break;
	}
}

void OpenGLPanel_Example::MyForm::RenderModeToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	System::Windows::Forms::ToolStripMenuItem^ ctrl = (ToolStripMenuItem^)sender;
	int tagInteger = Int32::Parse(ctrl->Tag->ToString());
	std::cout << "Item type:" << tagInteger << std::endl;
	switch (tagInteger) {
	case 0: //wireframe
		this->renderMode = RenderMode::WIREFRAME;
		break;
	case 1: //normal
		this->renderMode = RenderMode::NORMAL;
		break;
	default:
		std::cout << "Error Item tag\n";
		break;
	}
}

void OpenGLPanel_Example::MyForm::FaceColorToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (this->colorDialog1 == nullptr) {
		this->colorDialog1 = gcnew ColorDialog;
	}


	colorDialog1->AllowFullOpen = true;//allow custom color
	colorDialog1->ShowHelp = true; //idk

	if (colorDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
		Color color = colorDialog1->Color;
		System::Windows::Forms::ToolStripMenuItem^ ctrl = (ToolStripMenuItem^)sender;
		int tagInteger = Int32::Parse(ctrl->Tag->ToString());
		std::cout << "Item type:" << tagInteger << std::endl;
		float MAXIMUMCOLOR = 255.0f;
		glm::vec4 colorFloatValue = glm::vec4(color.R / MAXIMUMCOLOR, color.G / MAXIMUMCOLOR, color.B / MAXIMUMCOLOR, color.A / MAXIMUMCOLOR);
		switch (tagInteger) {
		case 0: //selected faces color
			model->selectedFaceColor = colorFloatValue;
			break;
		case 1: //hover faces color
			model->hoverFaceColor = colorFloatValue;
			break;
		case 2: //model faces color
			model->modelFaceColor = colorFloatValue;
			break;
		case 3:
			model->wireFaceColor = colorFloatValue;
			break;
		case 4:
			model->wireColor = colorFloatValue;
			break;

		default:
			std::cout << "Error Item tag\n";
			break;
		}
	}
}


void OpenGLPanel_Example::MyForm::Simplify_Button_Click(System::Object^ sender, System::EventArgs^ e)
{
		int edgeSize = model->model.mesh.n_edges();
		std::cout << "Original edge number: " << edgeSize << std::endl;
		
		for (int i = 0; i < 250; i++)
		{
			//model->model.mesh.UpdateSelectedFaceWeight(model->selectedFace);
			if (!model->model.mesh.Simplification())
			{
				break;
			}
		}
		model->selectedFace.clear();
		//model->model.mesh.garbage_collection();
		edgeSize = model->model.mesh.n_edges();
		std::cout << "New edge number: "<<edgeSize - model->model.mesh.deletedEdgeCount << std::endl;
		model->model.LoadToShader();
		hkoglPanelControl1->Invalidate();
	/*	if (edgeSize < 4)
		{*/
			model->model.mesh.garbage_collection();
		//}
	
}

void OpenGLPanel_Example::MyForm::ModelImportToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	String^ FileName;
	if (!Directory::Exists(LastModelDirectory))
	{
		LastModelDirectory = Environment::GetFolderPath(Environment::SpecialFolder::MyDocuments);
	}
	OpenFileDialog^ openFileDialog2 = gcnew OpenFileDialog;
	openFileDialog2->Filter = "Object Files (*.obj)|*.obj|*.*| All Files(*.*)";
	try
	{
		openFileDialog2->InitialDirectory = LastModelDirectory;
	}
	catch (Exception^)
	{
		openFileDialog2->InitialDirectory = Environment::GetFolderPath(Environment::SpecialFolder::MyDocuments);
	}

	if (openFileDialog2->ShowDialog() == System::Windows::Forms::DialogResult::OK)
	{
		FileName = openFileDialog2->FileName;
		if (FileName->LastIndexOf("\\") >= 0)
		{
			LastModelDirectory = FileName->Substring(0, (FileName->LastIndexOf("\\") + 1));
		}
		msclr::interop::marshal_context converter;
		std::string fileName = converter.marshal_as<std::string>(FileName);
		std::cout << "Load model: " << fileName << std::endl;

		//New model
		currentTextureID = 0;
		delete model;

		//Model 1
		model = new MeshObject();
		Texture* tex = new Texture();
		model->textures.push_back(tex);
		this->Cursor = Cursors::AppStarting;
		if (model->Init(fileName))
		{
			std::cout << "Load model successfully!" << std::endl;
		}
		else
		{
			std::cout << "Load model failed." << std::endl;
		}
		this->Cursor = Cursors::Arrow;
		modelPath = fileName;
		hkoglPanelControl1->Refresh();
	}
}

void OpenGLPanel_Example::MyForm::ModelLoadToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	String^ FileName;
	if (!Directory::Exists(LastSaveDirectory))
	{
		LastSaveDirectory = Environment::GetFolderPath(Environment::SpecialFolder::MyDocuments);
	}
	OpenFileDialog^ openFileDialog3 = gcnew OpenFileDialog;

	openFileDialog3->Filter = "Load Files (*.txt)|*.txt|All Files(*.*)|*.*";
	openFileDialog3->FilterIndex = 1;
	try
	{
		openFileDialog3->InitialDirectory = LastSaveDirectory;
	}
	catch (Exception^)
	{
		openFileDialog3->InitialDirectory = Environment::GetFolderPath(Environment::SpecialFolder::MyDocuments);
	}

	if (openFileDialog3->ShowDialog() == System::Windows::Forms::DialogResult::OK)
	{
		FileName = openFileDialog3->FileName;
		if (FileName->LastIndexOf("\\") >= 0)
		{
			LastSaveDirectory = FileName->Substring(0, (FileName->LastIndexOf("\\") + 1));
		}
		msclr::interop::marshal_context converter;
		std::string path = converter.marshal_as<std::string>(FileName);
		//this->LoadFile(path);
	}
}

void OpenGLPanel_Example::MyForm::ModelSaveToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	String^ FileName;
	if (!Directory::Exists(LastSaveDirectory))
	{
		LastSaveDirectory = Environment::GetFolderPath(Environment::SpecialFolder::MyDocuments);
	}
	SaveFileDialog^ saveFileDialog1 = gcnew SaveFileDialog;

	saveFileDialog1->Filter = "Save Files (*.txt)|*.txt|All Files(*.*)|*.*";
	saveFileDialog1->FilterIndex = 1;
	try
	{
		saveFileDialog1->InitialDirectory = LastSaveDirectory;
	}
	catch (Exception^)
	{
		saveFileDialog1->InitialDirectory = Environment::GetFolderPath(Environment::SpecialFolder::MyDocuments);
	}

	if (saveFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
	{
		FileName = saveFileDialog1->FileName;
		if (FileName->LastIndexOf("\\") >= 0)
		{
			LastSaveDirectory = FileName->Substring(0, (FileName->LastIndexOf("\\") + 1));
		}
		msclr::interop::marshal_context converter;
		std::string path = converter.marshal_as<std::string>(FileName);
		//this->SaveFile(path);
	}
}

void OpenGLPanel_Example::MyForm::LeastSquare_Button_Click(System::Object^ sender, System::EventArgs^ e)
{
	//model->model.mesh.sl += 1.0f;

	if (countLeastSquare >= 8)
	{
		//model->model.mesh.threshould = 100;
		//model->model.mesh.sl = 10;
	}
	model->model.mesh.LeastSquare(isInit);
	countLeastSquare++;
	if (isInit)
	{
		isInit = false;
	}
	model->model.LoadToShader();
	hkoglPanelControl1->Invalidate();
//	throw gcnew System::NotImplementedException();
}


void OpenGLPanel_Example::MyForm::timer1_Tick(System::Object^ sender, System::EventArgs^ e)
{
	hkoglPanelControl1->Invalidate();
}



//void OpenGLPanel_Example::MyForm::SolveUV()
//{
//	if (model->selectedFace.size() <= 0 || textureImages.size() == 0)
//	{
//		return;
//	}
//	std::sort(model->selectedFace.begin(), model->selectedFace.end());
//
//	MyMesh mesh;
//
//	//custom properties
//	OpenMesh::HPropHandleT<double> weight;
//	OpenMesh::VPropHandleT<int> row;
//	mesh.add_property(weight, "weight");
//	mesh.add_property(row, "row");
//
//	//standard properties
//	mesh.request_vertex_texcoords2D();
//	mesh.request_vertex_normals();
//	mesh.request_face_normals();
//
//	//Copy selected faces to mesh
//	std::vector<MyMesh::VertexHandle> vertexHandles;
//	vertexHandles.reserve(3); //allocate space 
//
//	std::map<int, int> usedVertex; //record used vertex
//	for (std::vector<unsigned int>::iterator it = model->selectedFace.begin(); it != model->selectedFace.end(); ++it)
//	{
//		MyMesh::FaceHandle faceHandle = model->model.mesh.face_handle(*it); //get face handle
//		for (MyMesh::FaceVertexIter faceVertexIt = model->model.mesh.fv_iter(faceHandle); faceVertexIt.is_valid(); ++faceVertexIt)
//		{
//			MyMesh::VertexHandle vertexHandle;
//			MyMesh::Point point = model->model.mesh.point(*faceVertexIt);
//
//			std::map<int, int>::iterator usedVertexIt = usedVertex.find(faceVertexIt->idx());
//
//			if (usedVertexIt == usedVertex.end()) //vertex not used
//			{
//				vertexHandle = mesh.add_vertex(point);
//				usedVertex[faceVertexIt->idx()] = vertexHandle.idx();
//			}
//			else //duplicated vertex
//			{
//				vertexHandle = mesh.vertex_handle(usedVertexIt->second);
//			}
//			vertexHandles.push_back(vertexHandle);
//		}
//		mesh.add_face(vertexHandles);
//		vertexHandles.clear();
//	}
//	mesh.update_normals();
//
//
//	//Weight Calculation
//	MyMesh::HalfedgeHandle halfEdgeHandle;
//	for (MyMesh::EdgeIter edgeIt = mesh.edges_begin(); edgeIt != mesh.edges_end(); ++edgeIt)
//	{
//		if (!mesh.is_boundary(*edgeIt)) //if this edge not boundary
//		{
//
//			MyMesh::HalfedgeHandle _halfEdgeHandle = mesh.halfedge_handle(*edgeIt, 0);
//			MyMesh::Point pointFrom = mesh.point(mesh.from_vertex_handle(_halfEdgeHandle));
//			MyMesh::Point pointTo = mesh.point(mesh.to_vertex_handle(_halfEdgeHandle));
//			MyMesh::Point point1 = mesh.point(mesh.opposite_vh(_halfEdgeHandle));
//			MyMesh::Point point2 = mesh.point(mesh.opposite_he_opposite_vh(_halfEdgeHandle));
//
//			double edgeLength = (pointFrom - pointTo).length();
//
//			//Weight from -> to
//			OpenMesh::Vec3d vector1 = OpenMesh::Vec3d(pointTo - pointFrom);
//			vector1.normalized();
//
//			OpenMesh::Vec3d vector2 = OpenMesh::Vec3d(point1 - pointFrom);
//			vector2.normalized();
//
//			GLdouble angle1 = std::acos(OpenMesh::dot(vector1, vector2));
//
//			OpenMesh::Vec3d vector3 = OpenMesh::Vec3d(point2 - pointFrom);
//			vector3.normalized();
//
//			GLdouble angle2 = std::acos(OpenMesh::dot(vector1, vector3));
//
//			GLdouble _weight = (tan(angle1 / 2.0f) + tan(angle2 / 2.0f)) / edgeLength;
//
//			mesh.property(weight, _halfEdgeHandle) = _weight;
//
//			//Weight to -> from
//			vector1 = -vector1;
//			vector2 = OpenMesh::Vec3d(point1 - pointTo);
//			vector2.normalized();
//
//			angle1 = acos(OpenMesh::dot(vector1, vector2));
//
//			vector3 = OpenMesh::Vec3d(point2 - pointTo);
//			vector3.normalized();
//
//			angle2 = acos(OpenMesh::dot(vector1, vector3));
//
//			_weight = (tan(angle1 / 2.0f) + tan(angle2 / 2.0f)) / edgeLength;
//			mesh.property(weight, mesh.opposite_halfedge_handle(_halfEdgeHandle)) = _weight;
//		}
//		else
//		{
//			if (!halfEdgeHandle.is_valid())
//			{
//				halfEdgeHandle = mesh.halfedge_handle(*edgeIt, 1);
//			}
//		}
//	}
//
//	//Calculate matrix size
//	int count = 0;
//	for (MyMesh::VertexIter vertexIt = mesh.vertices_begin(); vertexIt != mesh.vertices_end(); ++vertexIt)
//	{
//		if (mesh.is_boundary(*vertexIt))
//		{
//			mesh.property(row, *vertexIt) = -1;
//		}
//		else
//		{
//			mesh.property(row, *vertexIt) = count++;
//		}
//	}
//
//	double perimeter = 0;
//	std::vector<double> segmentLength;
//	std::vector<MyMesh::VertexHandle> _vertexHandles;
//	MyMesh::HalfedgeHandle next = halfEdgeHandle;
//	do
//	{
//		MyMesh::Point from = mesh.point(mesh.from_vertex_handle(next));
//		MyMesh::Point to = mesh.point(mesh.to_vertex_handle(next));
//		perimeter += (from - to).length();
//
//		segmentLength.push_back(perimeter);
//		_vertexHandles.push_back(mesh.from_vertex_handle(next));
//
//		next = mesh.next_halfedge_handle(next);
//	} while (halfEdgeHandle != next);
//
//
//	MyMesh::TexCoord2D coord(1, 0.5);
//	mesh.set_texcoord2D(_vertexHandles[0], coord);
//
//	for (int i = 1; i < _vertexHandles.size(); i++)
//	{
//		double angle = 2 * M_PI * segmentLength[i - 1] / perimeter;
//
//		coord[0] = (std::cos(angle) + 1) / 2;
//		coord[1] = (std::sin(angle) + 1) / 2;
//
//		mesh.set_texcoord2D(_vertexHandles[i], coord);
//	}
//
//
//	Eigen::SparseMatrix<double> A(count, count);
//	Eigen::VectorXd BX(count);
//	Eigen::VectorXd BY(count);
//	Eigen::SimplicialLDLT<Eigen::SparseMatrix<double> > linear;
//	BX.setZero();
//	BY.setZero();
//
//	for (MyMesh::VertexIter vertexIt = mesh.vertices_begin(); vertexIt != mesh.vertices_end(); ++vertexIt)
//	{
//		if (!mesh.is_boundary(*vertexIt))
//		{
//			int i = mesh.property(row, *vertexIt);
//			double totalWeight = 0;
//
//			for (MyMesh::VertexVertexIter vertexVertexIt = mesh.vv_iter(*vertexIt); vertexVertexIt.is_valid(); ++vertexVertexIt)
//			{
//				MyMesh::HalfedgeHandle _halfEdgeHandle = mesh.find_halfedge(*vertexIt, *vertexVertexIt);
//				double _weight = mesh.property(weight, _halfEdgeHandle);
//
//				if (mesh.is_boundary(*vertexVertexIt))
//				{
//					MyMesh::TexCoord2D texCoord = mesh.texcoord2D(*vertexVertexIt);
//					BX[i] += _weight * texCoord[0];
//					BY[i] += _weight * texCoord[1];
//				}
//				else
//				{
//					int j = mesh.property(row, *vertexVertexIt);
//					A.insert(i, j) = -_weight;
//				}
//				totalWeight += _weight;
//			}
//			A.insert(i, i) = totalWeight;
//		}
//	}
//	A.makeCompressed();
//
//	//Solve linear
//	Eigen::SparseMatrix<double> At = A.transpose();
//	linear.compute(At * A);
//	Eigen::VectorXd TX = linear.solve(At * BX);
//	Eigen::VectorXd TY = linear.solve(At * BY);
//
//	for (MyMesh::VertexIter vertexIt = mesh.vertices_begin(); vertexIt != mesh.vertices_end(); ++vertexIt)
//	{
//		if (!mesh.is_boundary(*vertexIt))
//		{
//			int i = mesh.property(row, *vertexIt);
//			mesh.set_texcoord2D(*vertexIt, MyMesh::TexCoord2D(TX[i], TY[i]));
//		}
//	}
//
//	if (!model->model.mesh.has_vertex_texcoords2D())
//	{
//		model->model.mesh.request_vertex_texcoords2D();
//		for (MyMesh::VertexIter vertexIt = model->model.mesh.vertices_begin(); vertexIt != model->model.mesh.vertices_end(); ++vertexIt)
//		{
//			model->model.mesh.set_texcoord2D(*vertexIt, MyMesh::TexCoord2D(-1, -1));
//		}
//	}
//
//	int index = 0;
//	for (MyMesh::FaceIter faceIt = mesh.faces_begin(); faceIt != mesh.faces_end(); ++faceIt)
//	{
//		MyMesh::FaceHandle faceHandle = *faceIt;
//		MyMesh::FaceHandle selectedFace_h = model->model.mesh.face_handle(model->selectedFace[index++]);
//
//		MyMesh::FaceVertexIter faceVertexIt = mesh.fv_iter(faceHandle);
//		MyMesh::FaceVertexIter selectedFaceVertexIt = model->model.mesh.fv_iter(selectedFace_h);
//		for (; faceVertexIt.is_valid() && selectedFaceVertexIt.is_valid(); ++faceVertexIt, ++selectedFaceVertexIt)
//		{
//			MyMesh::TexCoord2D texCoord = mesh.texcoord2D(*faceVertexIt);
//			model->model.mesh.set_texcoord2D(*selectedFaceVertexIt, texCoord);
//		}
//	}
//	if (model->model.mesh.has_vertex_texcoords2D())
//	{
//		std::vector<MyMesh::TexCoord2D> texCoords;
//		texCoords.reserve(model->model.mesh.n_vertices());
//		for (MyMesh::VertexIter vertexIt = model->model.mesh.vertices_begin(); vertexIt != model->model.mesh.vertices_end(); ++vertexIt)
//		{
//			MyMesh::TexCoord2D texCoord = model->model.mesh.texcoord2D(*vertexIt);
//			texCoords.push_back(texCoord);
//		}
//		std::vector<MyMesh::TexCoord2D> texCoords2 = texCoords;
//		//Model 1
//		glBindVertexArray(model->model.vao);
//		glGenBuffers(1, &model->textures[currentTextureID]->textureVbo);
//
//		glBindBuffer(GL_ARRAY_BUFFER, model->textures[currentTextureID]->textureVbo);
//		glBufferData(GL_ARRAY_BUFFER, sizeof(MyMesh::TexCoord2D) * texCoords.size(), &texCoords[0], GL_STATIC_DRAW);
//		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
//		glEnableVertexAttribArray(2);
//
//		glBindBuffer(GL_ARRAY_BUFFER, 0);
//		glBindVertexArray(0);
//
//	}
//
//	hkoglPanelControl1->Refresh();
//	//Model 1
//	model->textures[currentTextureID]->fvIDsPtr.swap(std::vector<unsigned int*>(model->selectedFace.size()));
//	for (int i = 0; i < model->textures[currentTextureID]->fvIDsPtr.size(); i++)
//	{
//		model->textures[currentTextureID]->fvIDsPtr[i] = (GLuint*)(model->selectedFace[i] * 3 * sizeof(GLuint));
//	}
//	model->textures[currentTextureID]->elementCount.swap(std::vector<int>(model->selectedFace.size(), 3));
//
//	model->textures[currentTextureID]->faceIDs = model->selectedFace;
//	model->textures[currentTextureID]->textureID = textureImages[selectTextureImage]->textureID;
//	selectedTextureRecord.push_back((int)selectTextureImage);
//	Texture* tmp = new Texture();
//	model->textures.push_back(tmp);
//
//	hkoglPanelControl1->Refresh();
//	model->selectedFace.clear();
//	currentTextureID++;
//}



void OpenGLPanel_Example::MyForm::RenderToPickingTexture()
{
	pickingTexture->Enable();
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(paramGL->pickingShader);
	glUniformMatrix4fv(
		glGetUniformLocation(paramGL->pickingShader, "Model"), 1, GL_FALSE, &paramGL->model[0][0]);
	glUniformMatrix4fv(
		glGetUniformLocation(paramGL->pickingShader, "Projection"), 1, GL_FALSE, &paramGL->projection[0][0]);
	glUniformMatrix4fv(
		glGetUniformLocation(paramGL->pickingShader, "View"), 1, GL_FALSE, &paramGL->view[0][0]);
	model->Render();
	pickingTexture->Disable();
}

void OpenGLPanel_Example::MyForm::RenderWireframeTexture()
{
	glUseProgram(paramGL->wireframeShader);
	glUniformMatrix4fv(
		glGetUniformLocation(paramGL->wireframeShader, "Model"), 1, GL_FALSE, &paramGL->model[0][0]);
	glUniformMatrix4fv(
		glGetUniformLocation(paramGL->wireframeShader, "Projection"), 1, GL_FALSE, &paramGL->projection[0][0]);
	glUniformMatrix4fv(
		glGetUniformLocation(paramGL->wireframeShader, "View"), 1, GL_FALSE, &paramGL->view[0][0]);
	glUniform1i(glGetUniformLocation(paramGL->wireframeShader, "isTexture"), true);

	for (int i = 0; i < currentTextureID; i++)
	{
		glUniformMatrix4fv(
			glGetUniformLocation(paramGL->wireframeShader, "uvScale"), 1, GL_FALSE, glm::value_ptr(glm::scale(glm::vec3(model->textures[i]->uvScale, model->textures[i]->uvScale, model->textures[i]->uvScale)))
		);
		glUniformMatrix4fv(
			glGetUniformLocation(paramGL->wireframeShader, "uvRotate"), 1, GL_FALSE, glm::value_ptr(glm::rotate(model->textures[i]->uvRotate, glm::vec3(0, 0, 1)))
		);
		glUniformMatrix4fv(
			glGetUniformLocation(paramGL->wireframeShader, "uvOffset"), 1, GL_FALSE, glm::value_ptr(glm::translate(glm::mat4(1.0), glm::vec3(model->textures[i]->uvOffsetX, model->textures[i]->uvOffsetY, 0)))
		);

		glBindTexture(GL_TEXTURE_2D, model->textures[i]->textureID);
		model->RenderTexture(i);
	}
}

void OpenGLPanel_Example::MyForm::RenderSelectedFace()
{
	glUniform1i(glGetUniformLocation(paramGL->wireframeShader, "isTexture"), false);
	glUniform4f(glGetUniformLocation(paramGL->wireframeShader, "faceColor"), model->selectedFaceColor.r, model->selectedFaceColor.g, model->selectedFaceColor.b, model->selectedFaceColor.a);
	model->RenderSelectedFace();
}

void OpenGLPanel_Example::MyForm::RenderModelTexture()
{
	glUseProgram(paramGL->modelShader);
	glUniformMatrix4fv(
		glGetUniformLocation(paramGL->modelShader, "Model"), 1, GL_FALSE, &paramGL->model[0][0]);
	glUniformMatrix4fv(
		glGetUniformLocation(paramGL->modelShader, "Projection"), 1, GL_FALSE, &paramGL->projection[0][0]);
	glUniformMatrix4fv(
		glGetUniformLocation(paramGL->modelShader, "View"), 1, GL_FALSE, &paramGL->view[0][0]);
	glUniform1i(glGetUniformLocation(paramGL->modelShader, "isTexture"), true);

	for (int i = 0; i < currentTextureID; i++)
	{
		glUniformMatrix4fv(
			glGetUniformLocation(paramGL->modelShader, "uvScale"), 1, GL_FALSE, glm::value_ptr(glm::scale(glm::vec3(model->textures[i]->uvScale, model->textures[i]->uvScale, model->textures[i]->uvScale)))
		);
		glUniformMatrix4fv(
			glGetUniformLocation(paramGL->modelShader, "uvRotate"), 1, GL_FALSE, glm::value_ptr(glm::rotate(model->textures[i]->uvRotate, glm::vec3(0, 0, 1)))
		);
		glUniformMatrix4fv(
			glGetUniformLocation(paramGL->wireframeShader, "uvOffset"), 1, GL_FALSE, glm::value_ptr(glm::translate(glm::mat4(1.0), glm::vec3(model->textures[i]->uvOffsetX, model->textures[i]->uvOffsetY, 0)))
		);
		glBindTexture(GL_TEXTURE_2D, model->textures[i]->textureID);
		model->RenderTexture(i);
	}
}

void OpenGLPanel_Example::MyForm::RenderWireframe()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(255, 255, 255, 0);
	glUseProgram(paramGL->wireframeShader);
	glUniformMatrix4fv(
		glGetUniformLocation(paramGL->wireframeShader, "Model"), 1, GL_FALSE, &paramGL->model[0][0]);
	glUniformMatrix4fv(
		glGetUniformLocation(paramGL->wireframeShader, "Projection"), 1, GL_FALSE, &paramGL->projection[0][0]);
	glUniformMatrix4fv(
		glGetUniformLocation(paramGL->wireframeShader, "View"), 1, GL_FALSE, &paramGL->view[0][0]);

	glUniform1i(glGetUniformLocation(paramGL->wireframeShader, "isTexture"), false);
	glUniform4f(glGetUniformLocation(paramGL->wireframeShader, "faceColor"), model->wireFaceColor.r,model->wireFaceColor.g,model->wireFaceColor.b,model->wireFaceColor.a);
	glUniform4f(glGetUniformLocation(paramGL->wireframeShader, "wireColor"), model->wireColor.r,model->wireColor.g,model->wireColor.b,model->wireColor.a);
	model->Render();
}

void OpenGLPanel_Example::MyForm::RenderModel()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(255, 255, 255, 0);
	glUseProgram(paramGL->modelShader);
	glUniformMatrix4fv(
		glGetUniformLocation(paramGL->modelShader, "Model"), 1, GL_FALSE, &paramGL->model[0][0]);
	glUniformMatrix4fv(
		glGetUniformLocation(paramGL->modelShader, "Projection"), 1, GL_FALSE, &paramGL->projection[0][0]);
	glUniformMatrix4fv(
		glGetUniformLocation(paramGL->modelShader, "View"), 1, GL_FALSE, &paramGL->view[0][0]);

	glUniform1i(glGetUniformLocation(paramGL->modelShader, "isTexture"), false);
	glUniform4f(glGetUniformLocation(paramGL->modelShader, "faceColor"), model->modelFaceColor.r,model->modelFaceColor.g,model->modelFaceColor.b,model->modelFaceColor.a);
	model->Render();
}

void OpenGLPanel_Example::MyForm::DeleteFace()
{
	for (std::vector<unsigned int>::iterator it = model->selectedFace.begin(); it != model->selectedFace.end(); ++it)
	{
		MyMesh::FaceHandle fh = model->model.mesh.face_handle(*it); //get face handle
		model->model.mesh.delete_face(fh, true);
		if (model->model.mesh.status(fh).deleted())
		{
			std::cout << "haha";
		}
	}
	model->model.mesh.garbage_collection();
	model->model.LoadToShader();
}

void OpenGLPanel_Example::MyForm::ScaleSelectedFaces()
{
	if (model->selectedFace.size() > 0) {
		int id = -1;
		Paintbrush::ScaleSelectedFaces(*model, id, 1);
	}

}

void OpenGLPanel_Example::MyForm::ShrinkSelectedFaces()
{
	if (model->selectedFace.size() > 0) {
		int id = -1;
		Paintbrush::ShrinkSelectedFaces(*model, id, 1);
	}

}

