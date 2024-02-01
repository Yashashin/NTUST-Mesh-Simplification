#pragma once
#include <iostream>
#include <vector>
#include <gl/glew.h>


namespace MyBufferPlugin {

	class Texture {
	public:
		int textureNum;
		GLuint* id;
		int width;
		int height;
	private:
		void GenerateTextures(int num=1) {
			if (this->id != nullptr) {
				glDeleteTextures(this->textureNum, this->id);
				delete this->id;
			}

			glGenTextures(num, this->id);
			this->textureNum = num;
		}
	public:
		Texture() {
			std::cout << "Not implment by texture default constructor\n";
		}
		Texture(int w, int h) {
			this->width = w;
			this->height = h;
			this->id = new GLuint;
			this->GenerateTextures();
		}
		Texture(const char* filePath) {
			std::cout << "Not yet done in Texture constructor with file path\n";
		}

		GLuint GetID(){return *this->id; }


		void UseTexture() {
			if(this->id)
				glBindTexture(GL_TEXTURE_2D, *this->id);
		}
		void UnBindTexture() {
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		virtual void SetTextureParameter() = 0;

		~Texture() {
			if (this->id != nullptr) {
				glDeleteTextures(this->textureNum, this->id);
				delete this->id;
			}
			
		}

	};

	class RGBTexture :public MyBufferPlugin::Texture {
	private:
	
		
	public:
		
		RGBTexture(int w, int h):MyBufferPlugin::Texture(w,h) {
			//MyBufferPlugin::Texture(w, h);
		
		}
		void SetTextureParameter() {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		void BindToFramebuffer() {
			//use texture buffer for fbo
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *this->id, 0);
		}

		~RGBTexture() {
			MyBufferPlugin::Texture::~Texture();
		}
		
	};

	class RBO {
	private:
		GLuint* rboID;
		int rboNum;
		void GenerateRBO(int num = 1) {
			this->rboID = new GLuint;
			glGenRenderbuffers(num, this->rboID);
		}
	public:
		RBO() {
			this->GenerateRBO();
		}
		RBO(int num) {
			GenerateRBO(num);
		}

		void SetRBOStorage(int w,int h) {
			//can be changed?
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);

			//changed for depth component32
			//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, this->windows_width, this->windwos_height);
		}
		void BindToFramebuffer() {
			//can be changed?
			// attach it to framebuffer
			//bind render buffer with depth and stencil attachment
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, *(this->rboID));

			//changed for depth component32
			//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, *(this->m_RBO));
		}

		void BindRBO() {
			glBindRenderbuffer(GL_RENDERBUFFER, *(this->rboID));
		}
		void UnBindRBO() {
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
		}
		
		~RBO() {
			if (this->rboID) {
				glDeleteRenderbuffers(rboNum, this->rboID);
				delete this->rboID;
			}
		}
	};

	class VAO {
	public:
		GLuint* m_VAO;
		int vao_size;
		VAO()
		{
			this->m_VAO = new GLuint;
			this->vao_size = 1;
			glGenVertexArrays(1, this->m_VAO);
		}

		VAO(int vaoSize) {
			this->m_VAO = new GLuint[vaoSize];
			this->vao_size = vaoSize;
			glGenVertexArrays(1, this->m_VAO);
		}

	public:
		void UseVAOByIndex(int index) {
			if (index < this->vao_size) {
				glBindBuffer(GL_ARRAY_BUFFER, this->m_VAO[index]);
			}
			else {
				printf("Index out of range in VAO\n");
			}
		}


	public:
		~VAO()
		{
			if (this->m_VAO)
			{
				glDeleteVertexArrays(1, this->m_VAO);
				delete this->m_VAO;
			}
		}

	public:
		void Use()//default is index 0
		{
			glBindVertexArray(*this->m_VAO);
		}
		void UnBind() {
			glBindVertexArray(0);
		}
	};

	class VBO
	{
	public:
		GLuint* m_VBO; // any size you want
		int vbo_size;//how many buffer we want init is 3 vertice, normal, texture
		VBO(int size = 3) : vbo_size(size)
		{
			this->m_VBO = new GLuint[vbo_size];
			glGenBuffers(1, this->m_VBO);

		}
		~VBO()
		{
			glDeleteBuffers(this->vbo_size, m_VBO);
			delete this->m_VBO;
			this->vbo_size = 0;
		}

	public:
		void UseBufferByIndex(int index) //set this by your own
		{
			if (index < this->vbo_size) {
				glBindBuffer(GL_ARRAY_BUFFER, this->m_VBO[index]);
			}
			else {
				printf("Index out of range in MyEffect.h VBO\n");
			}
		}

		void SetBufferData(std::vector<float>& vertices, int index, int nodeSize, int isStaticDraw = 1) //set this by your own accept vector
		{
			if (isStaticDraw) {
				glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
			}
			else {
				glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_DYNAMIC_DRAW);
			}

			//void glVertexAttribPointer( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride,const GLvoid * pointer);
			//given index
			//given every node size such as vertex with 3 data nodesize will be 3
			//init type is GL_FLOAT
			//is going to be normalized?
			//stride means next positin by this offset ex (0,1,2,3,4,5) -> I want 1 3 5 etc. so the offset is 2
			//pointer means start position so we need the address!
			glVertexAttribPointer(index, nodeSize, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(index);
		}
		void SetBufferData(float* vertices, int sizeofArray/*This only accept sizeof function by c*/, int index, int nodeSize, int isStaticDraw = 1) //The second attribute is only accepeted by sizeof(array)
		{
			if (isStaticDraw) {
				glBufferData(GL_ARRAY_BUFFER, sizeofArray, vertices, GL_STATIC_DRAW);
			}
			else {
				glBufferData(GL_ARRAY_BUFFER, sizeofArray, vertices, GL_DYNAMIC_DRAW);
			}
			//void glVertexAttribPointer( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride,const GLvoid * pointer);
			//given index
			//given every node size such as vertex with 3 data nodesize will be 3
			//init type is GL_FLOAT
			//is going to be normalized?
			//stride means next positin by this offset ex (0,1,2,3,4,5) -> I want 1 3 5 etc. so the offset is 2
			//pointer means start position so we need the address!
			glVertexAttribPointer(index, nodeSize, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(index);
		}

		void UnBind() {
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	};



	class QuadModel {
	public:
		MyBufferPlugin::VAO* vao;
		MyBufferPlugin::VBO* vbo;
	public:

		QuadModel() {
			this->vao = new MyBufferPlugin::VAO();
			this->vbo = new MyBufferPlugin::VBO(2);//only point and texture coordinate

			float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
			// positions   
			-1.0f,  1.0f,
			-1.0f, -1.0f,
			 1.0f, -1.0f,
			-1.0f,  1.0f,
			 1.0f, -1.0f,
			 1.0f,  1.0f
			};
			// texCoords
			float texturePoint[] = {
				0.0f, 1.0f,
				0.0f, 0.0f,
				1.0f, 0.0f,
				0.0f, 1.0f,
				1.0f, 0.0f,
				1.0f, 1.0f
			};

			this->vao->Use();

			this->vbo->UseBufferByIndex(0);
			this->vbo->SetBufferData(quadVertices, sizeof(quadVertices), 0, 2);

			this->vbo->UseBufferByIndex(1);
			this->vbo->SetBufferData(texturePoint, sizeof(texturePoint), 1, 2);
		}
		~QuadModel() {
			if (this->vao) {
				delete this->vao;
			}
			if (this->vbo) {
				delete this->vbo;
			}
		}
	};

	class FBO {
	public:
		int windowWidth;
		int windowHeight;
		GLuint* id;
		int fboNum;
		void GenerateFBO(int num = 1) {
			this->fboNum = num;
			this->id = new GLuint;
			glCreateFramebuffers(this->fboNum, this->id);
		}
	public:
		FBO() {

		}
		FBO(int w, int h) :windowHeight(h), windowWidth(w) {
			this->GenerateFBO();
		}
		virtual void SetFBOParameter(int& w,int& h)=0;

		void UseFBO() {
			glBindFramebuffer(GL_FRAMEBUFFER, *this->id);
		}
		void UnBindFBO() {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		void CheckFramebufferStatus() {
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		}
		~FBO() {
			if (this->id) {
				glDeleteFramebuffers(this->fboNum, this->id);
				delete this->id;
			}
		}
	};

	class myPostEffectFBO:public MyBufferPlugin::FBO {

	private:
		//rbo
		MyBufferPlugin::RBO* rbo;
		//texture
		MyBufferPlugin::RGBTexture* rgbTexture;
		//quad
		MyBufferPlugin::QuadModel* quad;
	public:
		myPostEffectFBO(int w, int h): MyBufferPlugin::FBO(w, h) {
			this->rgbTexture = new MyBufferPlugin::RGBTexture(w, h);
			this->rbo = new MyBufferPlugin::RBO();
			this->SetFBOParameter(w,h);

			this->quad = new MyBufferPlugin::QuadModel();
		}


		void SetFBOParameter(int& w,int& h) {
			this->UseFBO();

			this->rgbTexture->UseTexture();
			this->rgbTexture->SetTextureParameter();
			this->rgbTexture->BindToFramebuffer();
			
			this->rbo->BindRBO();
			this->rbo->SetRBOStorage(w, h);
			this->rbo->BindToFramebuffer();

			this->CheckFramebufferStatus();

			this->rbo->UnBindRBO();
			this->rgbTexture->UnBindTexture();

			this->UnBindFBO();
		}

		GLuint GetFBOTexture() { return this->rgbTexture->GetID(); }

		void UseQuadVAO() {
			this->quad->vao->Use();
		}
		void UnBindQuadVAO() {
			this->quad->vao->UnBind();
		}

		void UseFBOTexture() {
			this->rgbTexture->UseTexture();
		}
		void UnBindFBOTexture() {
			this->rgbTexture->UnBindTexture();
		}


		~myPostEffectFBO() {
			MyBufferPlugin::FBO::~FBO();
			delete this->rgbTexture;
			delete this->quad;
			delete this->rbo;
		}

	};

}
