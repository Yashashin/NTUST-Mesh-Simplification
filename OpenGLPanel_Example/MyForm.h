#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

#include "Param.h"
#include "LoadShaders.h"
#include "PickingTexture.h"
#include "TextureImage.h"
#include "Texture.h"
#include "MeshObject.h"
#include <Eigen/Sparse>
#include "./camera/ViewManager.h"


#include "Paintbrush.h"

#include <windows.h>
#include <msclr\marshal_cppstd.h>

//Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "SimpleBuffer.h"


//std::vector<TextureImage*> textureImages;
std::map<std::string, int> textureRecord;
//std::vector<TextureImage*> uvMapTextures;
//std::vector<std::string> texturePaths;
std::string modelPath;
//std::vector<int> selectedTextureRecord;




ViewManager main_camera, tex_camera;
enum RenderMode {
	WIREFRAME,
	NORMAL
};

enum SelectMode {
	PICKFACE,
	DELFACE,
	NONE
};

enum MouseState {
	PRESS,
	RELEASE,
};


namespace OpenGLPanel_Example
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Diagnostics;
	using namespace System::IO;

	/// <summary>
	/// MyForm 的摘要
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();

		}

	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::Timer^ timer1;

	private: HKOGLPanel::HKOGLPanelControl^ hkoglPanelControl1;
	private: System::Windows::Forms::OpenFileDialog^ openFileDialog1;







	private: System::Windows::Forms::MenuStrip^ menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^ modelToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ modelImportToolStripMenuItem;



	private: System::Windows::Forms::ToolStripMenuItem^ textureToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ TextureImportToolStripMenuItem1;
	private: System::Windows::Forms::ToolStripButton^ PaintADDStripButton1;

	private: System::Windows::Forms::ToolStrip^ toolStrip1;
	private: System::Windows::Forms::ToolStripButton^ PaintDELStripButton2;




	private: System::Windows::Forms::ToolStripButton^ PaintSizeScaleStripButton3;
	private: System::Windows::Forms::ToolStripButton^ PaintSizeShrinkStripButton4;


	private: System::Windows::Forms::ToolStripDropDownButton^ toolStripDropDownButton1;
	private: System::Windows::Forms::ToolStripMenuItem^ wireframeToolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^ normalToolStripMenuItem1;

	private: System::Windows::Forms::ToolStripButton^ PaintFaceShrinkStripButton1;

	private: System::Windows::Forms::ToolStripMenuItem^ FaceColorToolStripMenuItem;

	private: System::Windows::Forms::ColorDialog^ colorDialog1;
	private: System::Windows::Forms::ToolStripMenuItem^ selectedFaceToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ hoverFaceToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ modelFaceToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ wireFaceToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ wireToolStripMenuItem;






	private: System::Windows::Forms::Button^ TextureRESETButton;

	private: System::Windows::Forms::OpenFileDialog^ openFileDialog2;
	private: System::Windows::Forms::ToolStripMenuItem^ modelSaveToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ modelLoadStripMenuItem1;
	private: System::Windows::Forms::OpenFileDialog^ openFileDialog3;
	private: System::Windows::Forms::SaveFileDialog^ saveFileDialog1;

	private: System::Windows::Forms::ToolStripButton^ PaintFaceScaleStripButton2;
	private: System::Windows::Forms::Button^ button1;







	protected:

		/// <summary>
		/// 設計工具所需的變數。
		/// </summary>
	private: System::ComponentModel::IContainer^ components;

#pragma region Windows Form Designer generated code

	private:
		/// <summary>
		/// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改
		/// 這個方法的內容。
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			HKOGLPanel::HKCOGLPanelCameraSetting^ hkcoglPanelCameraSetting3 = (gcnew HKOGLPanel::HKCOGLPanelCameraSetting());
			HKOGLPanel::HKCOGLPanelPixelFormat^ hkcoglPanelPixelFormat3 = (gcnew HKOGLPanel::HKCOGLPanelPixelFormat());
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->hkoglPanelControl1 = (gcnew HKOGLPanel::HKOGLPanelControl());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->modelToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->modelImportToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->modelLoadStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->modelSaveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->textureToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->TextureImportToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->PaintADDStripButton1 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStrip1 = (gcnew System::Windows::Forms::ToolStrip());
			this->PaintDELStripButton2 = (gcnew System::Windows::Forms::ToolStripButton());
			this->PaintSizeScaleStripButton3 = (gcnew System::Windows::Forms::ToolStripButton());
			this->PaintSizeShrinkStripButton4 = (gcnew System::Windows::Forms::ToolStripButton());
			this->PaintFaceScaleStripButton2 = (gcnew System::Windows::Forms::ToolStripButton());
			this->PaintFaceShrinkStripButton1 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripDropDownButton1 = (gcnew System::Windows::Forms::ToolStripDropDownButton());
			this->wireframeToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->normalToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->FaceColorToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->selectedFaceToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->hoverFaceToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->modelFaceToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->wireFaceToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->wireToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->colorDialog1 = (gcnew System::Windows::Forms::ColorDialog());
			this->TextureRESETButton = (gcnew System::Windows::Forms::Button());
			this->openFileDialog2 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->openFileDialog3 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->menuStrip1->SuspendLayout();
			this->toolStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 16;
			this->timer1->Tick += gcnew System::EventHandler(this, &MyForm::timer1_Tick);
			// 
			// hkoglPanelControl1
			// 
			hkcoglPanelCameraSetting3->Far = 1000;
			hkcoglPanelCameraSetting3->Fov = 45;
			hkcoglPanelCameraSetting3->Near = -1000;
			hkcoglPanelCameraSetting3->Type = HKOGLPanel::HKCOGLPanelCameraSetting::CAMERATYPE::ORTHOGRAPHIC;
			this->hkoglPanelControl1->Camera_Setting = hkcoglPanelCameraSetting3;
			this->hkoglPanelControl1->Cursor = System::Windows::Forms::Cursors::Cross;
			this->hkoglPanelControl1->Location = System::Drawing::Point(10, 24);
			this->hkoglPanelControl1->Name = L"hkoglPanelControl1";
			hkcoglPanelPixelFormat3->Accumu_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			hkcoglPanelPixelFormat3->Alpha_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			hkcoglPanelPixelFormat3->Stencil_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			this->hkoglPanelControl1->Pixel_Format = hkcoglPanelPixelFormat3;
			this->hkoglPanelControl1->Size = System::Drawing::Size(916, 684);
			this->hkoglPanelControl1->TabIndex = 0;
			this->hkoglPanelControl1->Load += gcnew System::EventHandler(this, &MyForm::hkoglPanelControl1_Load);
			this->hkoglPanelControl1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::hkoglPanelControl1_Paint);
			this->hkoglPanelControl1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl1_MouseDown);
			this->hkoglPanelControl1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl1_MouseMove);
			this->hkoglPanelControl1->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl1_MouseUp);
			this->hkoglPanelControl1->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::OnMouseWheel);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// menuStrip1
			// 
			this->menuStrip1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(70)), static_cast<System::Int32>(static_cast<System::Byte>(70)),
				static_cast<System::Int32>(static_cast<System::Byte>(70)));
			this->menuStrip1->ImageScalingSize = System::Drawing::Size(20, 20);
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->modelToolStripMenuItem,
					this->textureToolStripMenuItem
			});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Padding = System::Windows::Forms::Padding(4, 2, 0, 2);
			this->menuStrip1->Size = System::Drawing::Size(1213, 24);
			this->menuStrip1->TabIndex = 12;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// modelToolStripMenuItem
			// 
			this->modelToolStripMenuItem->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(70)),
				static_cast<System::Int32>(static_cast<System::Byte>(70)), static_cast<System::Int32>(static_cast<System::Byte>(70)));
			this->modelToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->modelImportToolStripMenuItem,
					this->modelLoadStripMenuItem1, this->modelSaveToolStripMenuItem
			});
			this->modelToolStripMenuItem->ForeColor = System::Drawing::Color::White;
			this->modelToolStripMenuItem->Name = L"modelToolStripMenuItem";
			this->modelToolStripMenuItem->Size = System::Drawing::Size(57, 20);
			this->modelToolStripMenuItem->Text = L"Model";
			this->modelToolStripMenuItem->ToolTipText = L"Model tool";
			// 
			// modelImportToolStripMenuItem
			// 
			this->modelImportToolStripMenuItem->BackColor = System::Drawing::SystemColors::Control;
			this->modelImportToolStripMenuItem->ForeColor = System::Drawing::Color::Black;
			this->modelImportToolStripMenuItem->Name = L"modelImportToolStripMenuItem";
			this->modelImportToolStripMenuItem->Size = System::Drawing::Size(136, 22);
			this->modelImportToolStripMenuItem->Text = L"Import Obj";
			this->modelImportToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::ModelImportToolStripMenuItem_Click);
			// 
			// modelLoadStripMenuItem1
			// 
			this->modelLoadStripMenuItem1->BackColor = System::Drawing::SystemColors::Control;
			this->modelLoadStripMenuItem1->ForeColor = System::Drawing::Color::Black;
			this->modelLoadStripMenuItem1->Name = L"modelLoadStripMenuItem1";
			this->modelLoadStripMenuItem1->Size = System::Drawing::Size(136, 22);
			this->modelLoadStripMenuItem1->Text = L"Load File";
			this->modelLoadStripMenuItem1->Click += gcnew System::EventHandler(this, &MyForm::ModelLoadToolStripMenuItem_Click);
			// 
			// modelSaveToolStripMenuItem
			// 
			this->modelSaveToolStripMenuItem->BackColor = System::Drawing::SystemColors::Control;
			this->modelSaveToolStripMenuItem->ForeColor = System::Drawing::Color::Black;
			this->modelSaveToolStripMenuItem->Name = L"modelSaveToolStripMenuItem";
			this->modelSaveToolStripMenuItem->Size = System::Drawing::Size(136, 22);
			this->modelSaveToolStripMenuItem->Text = L"Save File";
			this->modelSaveToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::ModelSaveToolStripMenuItem_Click);
			// 
			// textureToolStripMenuItem
			// 
			this->textureToolStripMenuItem->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(70)),
				static_cast<System::Int32>(static_cast<System::Byte>(70)), static_cast<System::Int32>(static_cast<System::Byte>(70)));
			this->textureToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->TextureImportToolStripMenuItem1 });
			this->textureToolStripMenuItem->ForeColor = System::Drawing::Color::White;
			this->textureToolStripMenuItem->Name = L"textureToolStripMenuItem";
			this->textureToolStripMenuItem->Size = System::Drawing::Size(61, 20);
			this->textureToolStripMenuItem->Text = L"Texture";
			this->textureToolStripMenuItem->ToolTipText = L"Texture tool";
			// 
			// TextureImportToolStripMenuItem1
			// 
			this->TextureImportToolStripMenuItem1->BackColor = System::Drawing::SystemColors::Control;
			this->TextureImportToolStripMenuItem1->ForeColor = System::Drawing::Color::Black;
			this->TextureImportToolStripMenuItem1->Name = L"TextureImportToolStripMenuItem1";
			this->TextureImportToolStripMenuItem1->Size = System::Drawing::Size(112, 22);
			this->TextureImportToolStripMenuItem1->Text = L"Import";
			this->TextureImportToolStripMenuItem1->Click += gcnew System::EventHandler(this, &MyForm::Load_Texture_Btn_Click);
			// 
			// PaintADDStripButton1
			// 
			this->PaintADDStripButton1->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->PaintADDStripButton1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"PaintADDStripButton1.Image")));
			this->PaintADDStripButton1->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->PaintADDStripButton1->Name = L"PaintADDStripButton1";
			this->PaintADDStripButton1->Size = System::Drawing::Size(24, 24);
			this->PaintADDStripButton1->Tag = L"0";
			this->PaintADDStripButton1->Text = L"toolStripButton1";
			this->PaintADDStripButton1->ToolTipText = L"pick faces brush";
			this->PaintADDStripButton1->Click += gcnew System::EventHandler(this, &MyForm::PaintbrushStripButton_Click);
			// 
			// toolStrip1
			// 
			this->toolStrip1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(70)), static_cast<System::Int32>(static_cast<System::Byte>(70)),
				static_cast<System::Int32>(static_cast<System::Byte>(70)));
			this->toolStrip1->Dock = System::Windows::Forms::DockStyle::None;
			this->toolStrip1->Font = (gcnew System::Drawing::Font(L"Microsoft JhengHei UI", 9));
			this->toolStrip1->ImageScalingSize = System::Drawing::Size(20, 20);
			this->toolStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(7) {
				this->PaintADDStripButton1,
					this->PaintDELStripButton2, this->PaintSizeScaleStripButton3, this->PaintSizeShrinkStripButton4, this->PaintFaceScaleStripButton2,
					this->PaintFaceShrinkStripButton1, this->toolStripDropDownButton1
			});
			this->toolStrip1->LayoutStyle = System::Windows::Forms::ToolStripLayoutStyle::Flow;
			this->toolStrip1->Location = System::Drawing::Point(106, 0);
			this->toolStrip1->Name = L"toolStrip1";
			this->toolStrip1->RenderMode = System::Windows::Forms::ToolStripRenderMode::System;
			this->toolStrip1->Size = System::Drawing::Size(178, 27);
			this->toolStrip1->TabIndex = 13;
			this->toolStrip1->Text = L"toolStrip1";
			// 
			// PaintDELStripButton2
			// 
			this->PaintDELStripButton2->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->PaintDELStripButton2->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"PaintDELStripButton2.Image")));
			this->PaintDELStripButton2->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->PaintDELStripButton2->Name = L"PaintDELStripButton2";
			this->PaintDELStripButton2->Size = System::Drawing::Size(24, 24);
			this->PaintDELStripButton2->Tag = L"1";
			this->PaintDELStripButton2->Text = L"toolStripButton2";
			this->PaintDELStripButton2->ToolTipText = L"erase faces brush";
			this->PaintDELStripButton2->Click += gcnew System::EventHandler(this, &MyForm::PaintbrushStripButton_Click);
			// 
			// PaintSizeScaleStripButton3
			// 
			this->PaintSizeScaleStripButton3->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->PaintSizeScaleStripButton3->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"PaintSizeScaleStripButton3.Image")));
			this->PaintSizeScaleStripButton3->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->PaintSizeScaleStripButton3->Name = L"PaintSizeScaleStripButton3";
			this->PaintSizeScaleStripButton3->Size = System::Drawing::Size(24, 24);
			this->PaintSizeScaleStripButton3->Tag = L"2";
			this->PaintSizeScaleStripButton3->Text = L"toolStripButton3";
			this->PaintSizeScaleStripButton3->ToolTipText = L"scale current brush size";
			this->PaintSizeScaleStripButton3->Click += gcnew System::EventHandler(this, &MyForm::PaintbrushStripButton_Click);
			// 
			// PaintSizeShrinkStripButton4
			// 
			this->PaintSizeShrinkStripButton4->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->PaintSizeShrinkStripButton4->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"PaintSizeShrinkStripButton4.Image")));
			this->PaintSizeShrinkStripButton4->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->PaintSizeShrinkStripButton4->Name = L"PaintSizeShrinkStripButton4";
			this->PaintSizeShrinkStripButton4->Size = System::Drawing::Size(24, 24);
			this->PaintSizeShrinkStripButton4->Tag = L"3";
			this->PaintSizeShrinkStripButton4->Text = L"toolStripButton4";
			this->PaintSizeShrinkStripButton4->ToolTipText = L"shrink brush size";
			this->PaintSizeShrinkStripButton4->Click += gcnew System::EventHandler(this, &MyForm::PaintbrushStripButton_Click);
			// 
			// PaintFaceScaleStripButton2
			// 
			this->PaintFaceScaleStripButton2->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->PaintFaceScaleStripButton2->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"PaintFaceScaleStripButton2.Image")));
			this->PaintFaceScaleStripButton2->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->PaintFaceScaleStripButton2->Name = L"PaintFaceScaleStripButton2";
			this->PaintFaceScaleStripButton2->Size = System::Drawing::Size(24, 24);
			this->PaintFaceScaleStripButton2->Tag = L"4";
			this->PaintFaceScaleStripButton2->Text = L"toolStripButton2";
			this->PaintFaceScaleStripButton2->ToolTipText = L"scale selected faces";
			this->PaintFaceScaleStripButton2->Click += gcnew System::EventHandler(this, &MyForm::PaintbrushStripButton_Click);
			// 
			// PaintFaceShrinkStripButton1
			// 
			this->PaintFaceShrinkStripButton1->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->PaintFaceShrinkStripButton1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"PaintFaceShrinkStripButton1.Image")));
			this->PaintFaceShrinkStripButton1->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->PaintFaceShrinkStripButton1->Name = L"PaintFaceShrinkStripButton1";
			this->PaintFaceShrinkStripButton1->Size = System::Drawing::Size(24, 24);
			this->PaintFaceShrinkStripButton1->Tag = L"5";
			this->PaintFaceShrinkStripButton1->Text = L"toolStripButton1";
			this->PaintFaceShrinkStripButton1->Click += gcnew System::EventHandler(this, &MyForm::PaintbrushStripButton_Click);
			// 
			// toolStripDropDownButton1
			// 
			this->toolStripDropDownButton1->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripDropDownButton1->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->wireframeToolStripMenuItem1,
					this->normalToolStripMenuItem1, this->FaceColorToolStripMenuItem
			});
			this->toolStripDropDownButton1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripDropDownButton1.Image")));
			this->toolStripDropDownButton1->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripDropDownButton1->Name = L"toolStripDropDownButton1";
			this->toolStripDropDownButton1->Size = System::Drawing::Size(33, 24);
			this->toolStripDropDownButton1->Text = L"toolStripDropDownButton1";
			this->toolStripDropDownButton1->ToolTipText = L"ViewTool";
			// 
			// wireframeToolStripMenuItem1
			// 
			this->wireframeToolStripMenuItem1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(70)),
				static_cast<System::Int32>(static_cast<System::Byte>(70)), static_cast<System::Int32>(static_cast<System::Byte>(70)));
			this->wireframeToolStripMenuItem1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"wireframeToolStripMenuItem1.Image")));
			this->wireframeToolStripMenuItem1->Name = L"wireframeToolStripMenuItem1";
			this->wireframeToolStripMenuItem1->Size = System::Drawing::Size(152, 26);
			this->wireframeToolStripMenuItem1->Tag = L"0";
			this->wireframeToolStripMenuItem1->Text = L"Wireframe";
			this->wireframeToolStripMenuItem1->Click += gcnew System::EventHandler(this, &MyForm::RenderModeToolStripMenuItem_Click);
			// 
			// normalToolStripMenuItem1
			// 
			this->normalToolStripMenuItem1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(70)),
				static_cast<System::Int32>(static_cast<System::Byte>(70)), static_cast<System::Int32>(static_cast<System::Byte>(70)));
			this->normalToolStripMenuItem1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"normalToolStripMenuItem1.Image")));
			this->normalToolStripMenuItem1->Name = L"normalToolStripMenuItem1";
			this->normalToolStripMenuItem1->Size = System::Drawing::Size(152, 26);
			this->normalToolStripMenuItem1->Tag = L"1";
			this->normalToolStripMenuItem1->Text = L"Normal";
			this->normalToolStripMenuItem1->Click += gcnew System::EventHandler(this, &MyForm::RenderModeToolStripMenuItem_Click);
			// 
			// FaceColorToolStripMenuItem
			// 
			this->FaceColorToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {
				this->selectedFaceToolStripMenuItem,
					this->hoverFaceToolStripMenuItem, this->modelFaceToolStripMenuItem, this->wireFaceToolStripMenuItem, this->wireToolStripMenuItem
			});
			this->FaceColorToolStripMenuItem->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"FaceColorToolStripMenuItem.Image")));
			this->FaceColorToolStripMenuItem->Name = L"FaceColorToolStripMenuItem";
			this->FaceColorToolStripMenuItem->Size = System::Drawing::Size(152, 26);
			this->FaceColorToolStripMenuItem->Text = L"Color Setting";
			// 
			// selectedFaceToolStripMenuItem
			// 
			this->selectedFaceToolStripMenuItem->Name = L"selectedFaceToolStripMenuItem";
			this->selectedFaceToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->selectedFaceToolStripMenuItem->Tag = L"0";
			this->selectedFaceToolStripMenuItem->Text = L"Selected Face";
			this->selectedFaceToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::FaceColorToolStripMenuItem_Click);
			// 
			// hoverFaceToolStripMenuItem
			// 
			this->hoverFaceToolStripMenuItem->Name = L"hoverFaceToolStripMenuItem";
			this->hoverFaceToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->hoverFaceToolStripMenuItem->Tag = L"1";
			this->hoverFaceToolStripMenuItem->Text = L"Hover Face";
			this->hoverFaceToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::FaceColorToolStripMenuItem_Click);
			// 
			// modelFaceToolStripMenuItem
			// 
			this->modelFaceToolStripMenuItem->Name = L"modelFaceToolStripMenuItem";
			this->modelFaceToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->modelFaceToolStripMenuItem->Tag = L"2";
			this->modelFaceToolStripMenuItem->Text = L"Model Face";
			this->modelFaceToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::FaceColorToolStripMenuItem_Click);
			// 
			// wireFaceToolStripMenuItem
			// 
			this->wireFaceToolStripMenuItem->Name = L"wireFaceToolStripMenuItem";
			this->wireFaceToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->wireFaceToolStripMenuItem->Tag = L"3";
			this->wireFaceToolStripMenuItem->Text = L"Wire Face";
			this->wireFaceToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::FaceColorToolStripMenuItem_Click);
			// 
			// wireToolStripMenuItem
			// 
			this->wireToolStripMenuItem->Name = L"wireToolStripMenuItem";
			this->wireToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->wireToolStripMenuItem->Tag = L"4";
			this->wireToolStripMenuItem->Text = L"Wire Line";
			this->wireToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::FaceColorToolStripMenuItem_Click);
			// 
			// colorDialog1
			// 
			this->colorDialog1->FullOpen = true;
			// 
			// TextureRESETButton
			// 
			this->TextureRESETButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(70)),
				static_cast<System::Int32>(static_cast<System::Byte>(70)), static_cast<System::Int32>(static_cast<System::Byte>(70)));
			this->TextureRESETButton->FlatAppearance->BorderSize = 0;
			this->TextureRESETButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->TextureRESETButton->Font = (gcnew System::Drawing::Font(L"Microsoft JhengHei UI", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->TextureRESETButton->ForeColor = System::Drawing::Color::White;
			this->TextureRESETButton->Location = System::Drawing::Point(944, 35);
			this->TextureRESETButton->Margin = System::Windows::Forms::Padding(2);
			this->TextureRESETButton->Name = L"TextureRESETButton";
			this->TextureRESETButton->Size = System::Drawing::Size(258, 22);
			this->TextureRESETButton->TabIndex = 12;
			this->TextureRESETButton->Text = L"Simplify";
			this->TextureRESETButton->UseVisualStyleBackColor = false;
			this->TextureRESETButton->Click += gcnew System::EventHandler(this, &MyForm::Simplify_Button_Click);
			// 
			// openFileDialog2
			// 
			this->openFileDialog2->FileName = L"openFileDialog2";
			// 
			// openFileDialog3
			// 
			this->openFileDialog3->FileName = L"openFileDialog3";
			// 
			// button1
			// 
			this->button1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(70)), static_cast<System::Int32>(static_cast<System::Byte>(70)),
				static_cast<System::Int32>(static_cast<System::Byte>(70)));
			this->button1->FlatAppearance->BorderSize = 0;
			this->button1->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button1->Font = (gcnew System::Drawing::Font(L"Microsoft JhengHei UI", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->button1->ForeColor = System::Drawing::Color::White;
			this->button1->Location = System::Drawing::Point(944, 85);
			this->button1->Margin = System::Windows::Forms::Padding(2);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(258, 22);
			this->button1->TabIndex = 14;
			this->button1->Text = L"Least Square";
			this->button1->UseVisualStyleBackColor = false;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::LeastSquare_Button_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(51)), static_cast<System::Int32>(static_cast<System::Byte>(51)),
				static_cast<System::Int32>(static_cast<System::Byte>(51)));
			this->ClientSize = System::Drawing::Size(1213, 720);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->TextureRESETButton);
			this->Controls->Add(this->toolStrip1);
			this->Controls->Add(this->hkoglPanelControl1);
			this->Controls->Add(this->menuStrip1);
			this->Cursor = System::Windows::Forms::Cursors::Arrow;
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"MyForm";
			this->Text = L"Mesh Simplification";
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->toolStrip1->ResumeLayout(false);
			this->toolStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}

#pragma endregion
		//--------------EVENT--------------------
		//Form
		void OnMouseWheel(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
		void hkoglPanelControl1_Load(System::Object^ sender, System::EventArgs^ e);
		void hkoglPanelControl1_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e);
		void hkoglPanelControl1_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
		void hkoglPanelControl1_MouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
		void hkoglPanelControl1_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);

		//Button
		void Load_Texture_Btn_Click(System::Object^ sender, System::EventArgs^ e);//load texture
		void PaintbrushStripButton_Click(System::Object^ sender, System::EventArgs^ e);//brush mode
		void RenderModeToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);//render mode
		void FaceColorToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);//custom color
		void Simplify_Button_Click(System::Object^ sender, System::EventArgs^ e);
		
		void ModelImportToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
		void ModelLoadToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
		void ModelSaveToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
		void LeastSquare_Button_Click(System::Object^ sender, System::EventArgs^ e);

		//Timer
		void timer1_Tick(System::Object^ sender, System::EventArgs^ e);
		//---------------END--------------------

		//---------------CUSTOM-----------------
		//File Management
	/*	void LoadFile(std::string path);
		void SaveFile(std::string path);*/

		//Render method
		void RenderToPickingTexture();
		void RenderWireframeTexture();
		void RenderSelectedFace();
		void RenderModelTexture();
		void RenderWireframe();
		void RenderModel();

		//Mesh operation
		void DeleteFace();

		//Paintbrush
		//1. paint brush size -> recurrence times
		//2. first iterate current selected face
		//3. put it into already selected face -> will cause only one texture on it.
		void ScaleSelectedFaces();
		void ShrinkSelectedFaces();

	//Variables
	private:
		//Picturebox pointer
		System::Windows::Forms::PictureBox^ pictureBoxPtr = nullptr;

		//Record last path
		String^ LastTextureDirectory;
		String^ LastModelDirectory;
		String^ LastSaveDirectory;

		ParamGL* paramGL = nullptr;
		MeshObject* model = nullptr;
		PickingTexture* pickingTexture = nullptr;
		int currentTextureID;
		int selectTextureImage;
		RenderMode renderMode; //to indicate render mode initize it by load function
		SelectMode selectMode = SelectMode::PICKFACE;//to indicate picking faces or delete faces mode
		MouseState mouseState = MouseState::RELEASE; //to indicate mouse state
		float eye_x = 0.0;

		//paint brush
		Paintbrush* addBrush = NULL; //pick faces brush
		Paintbrush* delBrush = NULL; //delete faces brush
		Paintbrush* currentBrush = NULL; //brush control to trigger event 

		bool isInit = true;
		int countLeastSquare = 0;


};

}
