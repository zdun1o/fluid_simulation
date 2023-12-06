#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>
#include "stb_image/stb_image.h"
#include "Particle.h"
#include "Obstacle.h"
#include <vector>


class SandboxLayer : public GLCore::Layer
{
public:
	SandboxLayer();
	virtual ~SandboxLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(GLCore::Event& event) override;
	virtual void OnUpdate(GLCore::Timestep ts) override;
	virtual void OnImGuiRender() override;
private:
	std::vector <Particle>m_Particles;
	std::vector <Obstacle>m_Obstacle;
	std::vector <Obstacle>m_Pigulka;


	GLCore::Utils::Shader* m_Shader;
	GLCore::Utils::Shader* m_CircleShader;
	GLCore::Utils::OrthographicCameraController m_CameraController;

	GLuint m_QuadVA, m_QuadVB, m_QuadIB,
		m_LineVA, m_LineVB, m_LineIB;
	GLuint m_txt1, m_txt2,m_txt3,m_txt4,m_txt5,m_txt6;
	float m_QuadPosition[2] = {-1.5f,-0.5f};
	Particle* m_p1 = nullptr;
	friend class Particle;
};