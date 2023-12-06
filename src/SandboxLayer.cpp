#include "SandboxLayer.h"
#include <algorithm>
#include "GLCore/Core/Input.h"
#include "GLCore/Core/KeyCodes.h"
using namespace GLCore;
using namespace GLCore::Utils;
const size_t NumsofParticles = 100;
const size_t MaxParticleVertexCount = NumsofParticles * 4;
const size_t MaxParticleIndexCount = NumsofParticles * 6;
const float BaseSize = 1.5f;
bool	LottoToggler = false;
bool	ElevatorToggler = false;
bool	ResetToggler = false;
float ClearColor[4];
glm::vec2 newObstaclePos;
bool addObstacle = false;


SandboxLayer::SandboxLayer()
	: m_CameraController(16.0f / 9.0f, true)

{

}



SandboxLayer::~SandboxLayer()
{
}
static GLuint LoadTexture(const std::string& path)
{
	int w, h, bits;
	stbi_set_flip_vertically_on_load(1);
	auto* pixels = stbi_load(path.c_str(), &w, &h, &bits, STBI_rgb_alpha);
	GLuint textureID;
	glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	stbi_image_free(pixels);
	return textureID;
}


void SandboxLayer::OnAttach()
{

	EnableGLDebugging();

	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_Shader = Shader::FromGLSLTextFiles(
		"assets/shaders/test.vert.glsl",
		"assets/shaders/test.frag.glsl"
	);
	glUseProgram(m_Shader->GetRendererID());
	auto loc = glGetUniformLocation(m_Shader->GetRendererID(), "u_Textures");
	int samplers[5] = { 0,1,2,3,4 };
	glUniform1iv(loc, 5, samplers);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	m_Obstacle.push_back(Obstacle({ 10.0f,0.0f }, { 50.0f,0.0f }, { 50.0f,1.0f }, { 10.0f,1.0f }, { 1.0f,1.0f,1.0f,1.0f }, false));
	m_Obstacle.push_back(Obstacle({ 49.0f,0.0f }, { 50.0f,0.0f }, { 50.0f,35.0f }, { 49.0f,35.0f }, { 1.0f,1.0f,1.0f,1.0f }, false));
	m_Obstacle.push_back(Obstacle({ 10.0f,0.0f }, { 11.0f,0.0f }, { 11.0f,35.0f }, { 10.0f,35.0f }, { 0.0f,1.0f,1.0f,1.0f }, false));
	m_Obstacle.push_back(Obstacle({ 18.0f,0.0f }, { 19.0f,0.0f }, { 19.0f,16.0f }, { 18.0f,16.0f }, { 0.0f,1.0f,1.0f,1.0f }, false));
	m_Obstacle.push_back(Obstacle({ 10.0f,34.0f }, { 50.0f,34.0f }, { 50.0f,35.0f }, { 10.0f,35.0f }, { 0.0f,1.0f,1.0f,1.0f }, false));


	m_Obstacle.push_back(Obstacle({ 18.0f,15.0f }, { 25.0f,15.0f }, { 25.0f,16.0f }, { 18.0f,16.0f }, { 0.0f,1.0f,1.0f,1.0f }, false));




















	//~~~~~~~~~~~~~~~~~~~~CIRCLES~~~~~~~~~~~~~~~~~~~~~~
	glCreateVertexArrays(1, &m_QuadVA);
	glBindVertexArray(m_QuadVA);
	glCreateBuffers(1, &m_QuadVB);
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * (MaxParticleVertexCount + 4), nullptr, GL_DYNAMIC_DRAW);

	uint32_t indices[(MaxParticleIndexCount + 6)];
	uint32_t offset = 0;
	for (size_t i = 0; i < (MaxParticleIndexCount + 6); i += 6)
	{
		indices[i + 0] = 0 + offset;
		indices[i + 1] = 1 + offset;
		indices[i + 2] = 2 + offset;


		indices[i + 3] = 2 + offset;
		indices[i + 4] = 3 + offset;
		indices[i + 5] = 0 + offset;
		offset += 4;
	}


	glCreateBuffers(1, &m_QuadIB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadIB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);





	float _x = 0.45f;
	float _y = 14.0f;
	bool direction = false;
	m_Particles.reserve(NumsofParticles);
	float bufSize = BaseSize;
	int count = 0;

	while (true)
	{
		if (count > NumsofParticles)
		{
			_y = 14.0f;
			break;

		}
		else bufSize /= 1.5f;
		count = 0;
		_y = 14.0f;
		while (true)
		{

			if (_y < 1.5f)break;


			_x += bufSize * 3;
			if (_x >= 15.0f - BaseSize)
			{
				_y -= bufSize * 1.2;

				if (direction)
				{
					direction = !direction;
					_x = bufSize * 9 / 10;
				}
				else
				{
					direction = !direction;
					_x = 0.0f;
				}
			}
			++count;


		}


	}

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> dist(2.1 * bufSize, 3 * bufSize);


	for (size_t i = 0; i < NumsofParticles; ++i)
	{




		m_Particles.push_back(Particle({ _x + 12.0f ,_y + 12.0f }, bufSize));
		_x += dist(mt);
		if (_x >= 15.0f - bufSize)
		{
			_y -= bufSize * 1.2;

			if (direction)
			{
				direction = !direction;
				_x = dist(mt);
			}
			else
			{
				direction = !direction;
				_x = dist(mt);
			}
		}

	}









	//koordynaty
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position));

	//kolor
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color));


	//mapowanie textury
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexCoords));


	//index textury
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexID));
	//glDisableVertexAttribArray(2);
	//glDisableVertexAttribArray(3);
	;



	// Init here

	m_txt1 = LoadTexture("assets/textures/wb.png");
	m_txt2 = LoadTexture("assets/textures/bb.png");
	m_txt3 = LoadTexture("assets/textures/gb.png");
	m_txt4 = LoadTexture("assets/textures/rb.png");
	m_txt5 = LoadTexture("assets/textures/yb.png");
	m_CameraController.SetZoomLevel(0.0f);











}



static void SetUniformMat4(uint32_t shader, const char* name, const glm::mat4& matrix)
{
	int loc = glGetUniformLocation(shader, name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}
void SandboxLayer::OnDetach()
{
	// Shutdown here
}

void SandboxLayer::OnEvent(Event& event)
{
	m_CameraController.OnEvent(event);


	// Events here
}


static oVertex* CreateLine(oVertex* target)
{


	//target->Position = { botL.x,botL.y,0.0f };//15 15
	target->Color = { 1.0f,0.0f,0.0f,1.0f };
	target->TexCoords = { 0.0f,0.0f };
	target->TexID = { 5.0f };
	float startx = target->Position.x;
	float starty = target->Position.y;
	++target;


	//target->Position = { botR.x,botR.y,0.0f };//15 15
	target->Color = { 1.0f,0.0f,0.0f,1.0f };
	target->TexCoords = { 0.0f,0.0f };
	target->TexID = { 5.0f };
	++target;

	//target->Position = { topR.x,topR.y,0.0f };//15 15
	target->Color = { 1.0f,0.0f,0.0f,1.0f };
	target->TexCoords = { 0.0f,0.0f };
	target->TexID = { 5.0f };
	++target;

	//target->Position = { topL.x,topL.y,0.0f};//15 15
	target->Color = { 1.0f,0.0f,0.0f,1.0f };
	target->TexCoords = { 0.0f,0.0f };
	target->TexID = { 5.0f };
	++target;


	target->Position = { startx,starty,0.0f };//15 15
	target->Color = { 1.0f,0.0f,0.0f,1.0f };
	target->TexCoords = { 0.0f,0.0f };
	target->TexID = { 5.0f };
	++target;
	return target;
}



void SandboxLayer::OnUpdate(Timestep ts)
{
	m_CameraController.OnUpdate(ts);






	//`````````````````````QUADDDDDDDDS``````````````

	glBindVertexArray(m_QuadVA);

	std::vector<Vertex>Vertices;
	Vertices.reserve(MaxParticleVertexCount + 4);
	std::pair<double, double>CurrentMousePosition = GLCore::Input::GetMousePosition();
	bool LMB = GLCore::Input::IsMouseButtonPressed(0);
	bool RMB = GLCore::Input::IsMouseButtonPressed(1);
	glm::vec2 MouseVec = { (CurrentMousePosition.first - 550.0f) / 20.0f,-(CurrentMousePosition.second - 550.0f) / 20.0f };
	float angle = m_CameraController.GetRotation();
	float bufAngle = angle * M_PI / 180.0f;
	float Vx = MouseVec.x * cos(bufAngle) - MouseVec.y * sin(bufAngle);
	float Vy = MouseVec.x * sin(bufAngle) + MouseVec.y * cos(bufAngle);
	//std::cout << "X:" << Vx << " Y:" << Vy << std::endl;
	m_Particles.push_back(Particle({ Vx ,Vy }, 1.0f));
	//CreateQuad(&MouseParticle, (CurrentMousePosition.first -645.0f)/100.0f, (CurrentMousePosition.second - 470.0f)/100.0f, 2.0f, 1.0f);

	//for (size_t i = 0; i < NumsofParticles+1; ++i)
	//{

	//	std::copy(m_Particles[i].GetBegVertex(), m_Particles[i].GetBegVertex()+4 , std::back_inserter(Vertices));

	//}
	for (size_t i = 0; i < NumsofParticles + 1; ++i)
	{

		std::copy(m_Particles[i].GetBegVertex(), m_Particles[i].GetBegVertex() + 4, std::back_inserter(Vertices));

	}
	//Set dynamic vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
	glBufferSubData(GL_ARRAY_BUFFER, 0, (MaxParticleVertexCount + 4) * sizeof(Vertex), Vertices.data());

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_Shader->GetRendererID());
	glBindTextureUnit(0, m_txt1);
	glBindTextureUnit(1, m_txt2);
	glBindTextureUnit(2, m_txt3);
	glBindTextureUnit(3, m_txt4);
	glBindTextureUnit(4, m_txt5);

	auto vp = m_CameraController.GetCamera().GetViewProjectionMatrix();
	SetUniformMat4(m_Shader->GetRendererID(), "u_ViewProjection", vp);
	SetUniformMat4(m_Shader->GetRendererID(), "u_Transform", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));



	glBindVertexArray(m_QuadVA);

	SweepAndPrune(m_Particles,m_Pigulka);
	for (auto& i : m_Particles)
	{

		i.Update(0.002f, m_Obstacle, m_CameraController.GetRotation());

	}
	glDrawElements(GL_TRIANGLES, (MaxParticleIndexCount + 6), GL_UNSIGNED_INT, nullptr);
	m_Particles.pop_back();




	glCreateVertexArrays(1, &m_LineVA);
	glBindVertexArray(m_LineVA);
	glCreateBuffers(1, &m_LineVB);
	glBindBuffer(GL_ARRAY_BUFFER, m_LineVB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(oVertex) * 5, nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(oVertex), (const void*)offsetof(oVertex, Position));

	////kolor
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(oVertex), (const void*)offsetof(oVertex, Color));


	////mapowanie textury
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(oVertex), (const void*)offsetof(oVertex, TexCoords));


	////index textury
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(oVertex), (const void*)offsetof(oVertex, TexID));


	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ÓBSTACLES~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	for (size_t i = 0; i < m_Obstacle.size(); ++i)
	{
		std::vector<oVertex>my_vertices;
		my_vertices.resize(5);
		 if (m_Obstacle[i].m_Drag) {
			float w = m_Obstacle[i].GetWidth() ;
			float h = m_Obstacle[i].GetHeight();
		
			if (LMB)m_Obstacle[i].m_Drag = false;
			if (RMB)
			{
				m_Obstacle[i].SetHeight(w);
				m_Obstacle[i].SetWidth(h);
			}
			m_Obstacle[i].SetPosition(std::make_pair((CurrentMousePosition.first - 550.0f) / 20.0f, -(CurrentMousePosition.second - 550.0f) / 20.0f));
		}

		my_vertices[0] = m_Obstacle[i].GetBegVertex()[0];
		my_vertices[1] = m_Obstacle[i].GetBegVertex()[1];
		my_vertices[2] = m_Obstacle[i].GetBegVertex()[2];
		my_vertices[3] = m_Obstacle[i].GetBegVertex()[3];
		my_vertices[4] = m_Obstacle[i].GetBegVertex()[0];
		//	oVertex* my_buffer = my_vertces.data();
			//my_buffer = CreateLine(my_buffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, my_vertices.size() * sizeof(oVertex), my_vertices.data());
		//float my_indices[] =
		//{
		//0.0f
		//};
		//glCreateBuffers(1, &m_LineIB);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_LineIB);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float)*4, my_indices, GL_STATIC_DRAW);

		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}
	for (size_t i = 0; i < m_Pigulka.size(); ++i)
	{
		std::vector<oVertex>my_vertices;
		my_vertices.resize(5);
		if (m_Pigulka[i].m_Drag) {
			float w = m_Pigulka[i].GetWidth();
			float h = m_Pigulka[i].GetHeight();

			if (LMB)m_Pigulka[i].m_Drag = false;
			if (RMB)
			{
				m_Pigulka[i].SetHeight(w);
				m_Pigulka[i].SetWidth(h);
			}
			m_Pigulka [i] .SetPosition(std::make_pair((CurrentMousePosition.first - 550.0f) / 20.0f, -(CurrentMousePosition.second - 550.0f) / 20.0f));
		}

		my_vertices[0] = m_Pigulka[i].GetBegVertex()[0];
		my_vertices[1] = m_Pigulka[i].GetBegVertex()[1];
		my_vertices[2] = m_Pigulka[i].GetBegVertex()[2];
		my_vertices[3] = m_Pigulka[i].GetBegVertex()[3];
		my_vertices[4] = m_Pigulka[i].GetBegVertex()[0];
		//	oVertex* my_buffer = my_vertces.data();
			//my_buffer = CreateLine(my_buffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, my_vertices.size() * sizeof(oVertex), my_vertices.data());
		//float my_indices[] =
		//{
		//0.0f
		//};
		//glCreateBuffers(1, &m_LineIB);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_LineIB);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float)*4, my_indices, GL_STATIC_DRAW);

		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}
	m_Pigulka.clear();
}

void SandboxLayer::OnImGuiRender()
{
	
	ImGui::Begin("Options");

	if (ImGui::Button("Zwolnienie blokady")) {

		LottoToggler = !LottoToggler;
	}
	if (ImGui::Button("Winda")) {

		ElevatorToggler = !ElevatorToggler;
	}
	if (ImGui::Button("Reset")) {
		LottoToggler = false;
		ElevatorToggler = false;

		m_CameraController.SetRotation(0.0f);

		for (auto& i : m_Particles) 
		{
			i.Reset();
		}
			m_Obstacle.erase(m_Obstacle.begin() + 6, m_Obstacle.end());
			m_Obstacle.shrink_to_fit();
	}

	if (ImGui::Button("Dodaj sciane")) {
		addObstacle = !addObstacle;
	}

	if (addObstacle) {
		ImGui::SliderFloat2("Size: ", &newObstaclePos.x, 0.0f, 15.0f);
		
		ImGui::ColorEdit4("Color:", ClearColor,ImGuiColorEditFlags_AlphaBar);
		if (ImGui::Button("Stworz")) {
			addObstacle = !addObstacle;
			glm::vec4 ColorIn2;
				ColorIn2.x= ClearColor[0];
				ColorIn2.y= ClearColor[1];
				ColorIn2.z= ClearColor[2];
				ColorIn2.w = ClearColor[3];
			m_Obstacle.push_back(Obstacle({ 0.0f,0.0f }, { newObstaclePos.x,0.0f }, { newObstaclePos.x,newObstaclePos.y }, { 0.0f,newObstaclePos.y }, ColorIn2, true));

			newObstaclePos = { 0.0f, 0.0f };
		}
	}

	ImGui::End();

	/*ImGui::Begin("Controls");
	ImGui::DragFloat2("Quad Position", m_QuadPosition, 0.1f);
	ImGui::End();*/

	// ImGui here
}