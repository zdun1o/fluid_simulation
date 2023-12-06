#include "Particle.h"
#include "SandboxLayer.h"
float FloorLevel = 0.0f;

//const float Particle::getCurrentVelocity()
//{
//	return sqrt(abs((2.0f*m_k*(m_PreviousHeight - m_CurrentHeight)) + static_cast<float>(pow(m_PreviousVelocity,2))));
//}

Particle::Particle(glm::vec2 StartPos, float I_size)
	:m_StartPos(StartPos), m_Size(I_size), m_DirY(false)
	, m_k(1.0f), m_AnimationSpeed(0.1f), m_CurrentVelocity({ 0.0f,0.0f }), m_Gravity{ 0.0f,1000.0f },
	m_angle(0.0f)

{
	m_CurrentPosition.x = m_StartPos.x;
	m_CurrentPosition.y = m_StartPos.y;
	m_PreviousPosition = m_CurrentPosition;
	m_Acceleration = { 0.0f,0.0f };



	CreateQuad(this, m_StartPos.x, m_StartPos.y, 0.0f, m_Size);
}

Particle::~Particle()
{
	//czy tu nie powinno czegows byc?
}
float clamp(float val, float min, float max) {
	if (val < min) return min;
	else if (val > max) return max;
	else return val;
}

void Particle::Update(float dt, std::vector<Obstacle>& Obstacles, float angle)
{

	const int sub_steps = 12;
	const float sub_dt = dt / static_cast<float>(sub_steps);
	if (LottoToggler)
	{


		for (size_t i = 0; i < sub_steps; i++)
		{
			//circular shape equations
		/*	const glm::vec2 position{ 7.5f,7.5f };
			const float radius = 10.0f;
			glm::vec2 to_obj = m_CurrentPosition - position;
			const float dist = glm::length(to_obj);
			if (dist > radius - m_Size / 2)
			{
				const glm::vec2 n = to_obj / dist;
				m_CurrentPosition = position + n * (radius - m_Size / 2);
			}*/
			//rectangular shape equations

			//winda jedzie w gore
			for (size_t Obs = 0; Obs < Obstacles.size(); ++Obs)
			{
				//0 i 1 

				// get center point circle first 
				glm::vec2 center(m_CurrentPosition.x + m_Size / 2, m_CurrentPosition.y + m_Size / 2);
				// calculate AABB info (center, half-extents)
				glm::vec2 aabb_half_extents(Obstacles[Obs].GetWidth() / 2, Obstacles[Obs].GetHeight() / 2);
				glm::vec2 aabb_center(
					Obstacles[Obs].GetVertices()[0].Position.x + aabb_half_extents.x,
					Obstacles[Obs].GetVertices()[0].Position.y + aabb_half_extents.y
				);
				// get difference vector between both centers
				glm::vec2 difference = center - aabb_center;
				glm::vec2 clamped = clamp(difference, -aabb_half_extents, aabb_half_extents);
				// add clamped value to AABB_center and we get the value of box closest to circle
				glm::vec2 closest = aabb_center + clamped;
				// retrieve vector between center circle and closest point AABB and check if length <= radius
				difference = closest - center;
				float len = glm::length(difference);
				if (glm::length(difference) < m_Size / 2)
				{


					m_CurrentPosition = m_CurrentPosition - (difference / glm::length(difference)) * 0.009f;

					//if (center.x != closest.x)m_CurrentPosition.x = closest.x;
					//else if (center.y != closest.y)m_CurrentPosition.y = closest.y;


				}


			}
			if (ElevatorToggler)
			{

				if (FloorLevel > 18.0f) {
					FloorLevel = 0.0f;
					ElevatorToggler = false;
				}
				FloorLevel += 0.000002f;

			}

			//jak jest rura zamknieta

		/*	if (m_CurrentPosition.y>15.0f&&m_CurrentPosition.x>15.0f )

			{
				glm::vec2 left = { 50.0f, 0.0f };
					m_Acceleration += left;
			}*/

			if (m_CurrentPosition.y < FloorLevel && m_CurrentPosition.x < 18.0f)m_CurrentPosition.y = FloorLevel;


			float bufAngle = angle * M_PI / 180.0f;
			float Vx = m_Gravity.x * cos(bufAngle) - m_Gravity.y * sin(bufAngle);
			float Vy = m_Gravity.x * sin(bufAngle) + m_Gravity.y * cos(bufAngle);
			m_angle = angle;
			//float Vy2 = Checked->m_CurrentVelocity.x *  sin(Dir) + Checked->m_CurrentVelocity.y *cos(Dir);
			//float Vx2 = Checked->m_CurrentVelocity.x *  cos(Dir) - Checked->m_CurrentVelocity.y *sin(Dir);
			//float buf = Vx;
			//Vx = Vx2;
			//Vx2 = buf;


			//Current->m_CurrentVelocity.x = Vx * cos(-Dir) - Vy * sin(-Dir);
			//Current->m_CurrentVelocity.y = Vx * sin(-Dir) + Vy * cos(-Dir);
			//Checked->m_CurrentVelocity.x = Vx2 * cos(-Dir) - Vy2 * sin(-Dir);
			//Checked->m_CurrentVelocity.y = Vx2 * sin(-Dir) + Vy2 * cos(-Dir);



			glm::vec2 NewGravity = { Vx,Vy };
			//std::cout << "x: " << m_Gravity.x << " y: " << m_Gravity.y << std::endl;
			m_Acceleration += NewGravity;
			m_CurrentVelocity = m_CurrentPosition - m_PreviousPosition;
			m_PreviousPosition = m_CurrentPosition;
			m_CurrentPosition = m_CurrentPosition + m_CurrentVelocity - m_Acceleration * sub_dt * sub_dt;
			//odbitka od rury
			//jak rura zamkmnieta, to ma sie odjbjac od niej.
			//if (!LottoToggler&&m_CurrentPosition.x < 15.0f)m_CurrentPosition.x = 15.0f;


			//
			//else
			//{


			//		if (m_CurrentPosition.x < 15.0f)
			//		{

			//		//if ((m_CurrentPosition.x >= 15.0f - m_Size)&&(m_CurrentPosition.y<=15.0f))m_CurrentPosition.x = 15.0f - m_Size;
			//		}
			//	
			//		if (m_CurrentPosition.y < FloorLevel)m_CurrentPosition.y = FloorLevel;
			//}
			//if (m_CurrentPosition.x > 30.0f-m_Size )m_CurrentPosition.x = 30.0f-m_Size ;


			//if (m_CurrentPosition.x > 15.0f-m_Size)
			//{

			//	if (m_CurrentPosition.y < 15.0f - m_Size * 0.25f) m_CurrentPosition.x = 15.0f - m_Size;
			//	else
			//	{
			//		if (m_CurrentPosition.y < (m_CurrentPosition.x - m_Size) / 2.0f + 7.5f + m_Size * 0.75f)
			//			m_CurrentPosition.y = (m_CurrentPosition.x - m_Size) / 2.0f + 7.5f + m_Size * 0.75f;
			//		else if (m_CurrentPosition.y > (m_CurrentPosition.x - m_Size) / 2.0f + 9.8f)
			//			m_CurrentPosition.y = (m_CurrentPosition.x - m_Size) / 2.0f + 9.8f;
			//	}
			//}
			m_Acceleration = {};
		}

		CreateQuad(this, m_CurrentPosition.x, m_CurrentPosition.y, m_Vertex[0].TexID, m_Size);
	}
}

void Particle::Reset()
{
	m_CurrentPosition.x = m_StartPos.x;
	m_CurrentPosition.y = m_StartPos.y;
	m_PreviousPosition = m_CurrentPosition;
	m_Acceleration = { 0.0f,0.0f };
	CreateQuad(this, m_StartPos.x, m_StartPos.y, 0.0f, m_Size);


}









void SweepAndPrune(std::vector<Particle>& Particles,  std::vector<Obstacle>& Pigulki)
{

	//for (size_t i = 0; i < Particles.size()-1; ++i)
	//{
	//	for (size_t j = i+1; j < Particles.size(); j++)
	//	{
	//		const glm::vec2 Collision_axis = Particles[i].m_CurrentPosition - Particles[j].m_CurrentPosition ;
	//		const float dist = glm::length(Collision_axis);
	//		float bufSize = Particles[i].m_Size+ Particles[j].m_Size;
	//		//float bufSize = Particles[i].m_Size;
	//		//if (Particles[j].m_Size>bufSize)bufSize=Particles[j].m_Size;
	//		if (dist < bufSize )
	//		{

	//									const glm::vec2 n = Collision_axis / dist;
	//									const float delta = bufSize  - dist;
	//									Particles[j].m_CurrentPosition -= 0.05f * delta * n;
	//									Particles[i].m_CurrentPosition += 0.05f * delta * n;
	//		}
	//	}

	//}


	std::vector<std::vector <std::vector<Particle*>>> result;



	result.resize(30);
	for (int i = 0; i < 30; ++i)result[i].resize(30);
	for (auto& i : Particles)
	{

		int bufRow = i.m_CurrentPosition.x;
		int bufCol = i.m_CurrentPosition.y;
		if (bufRow >= 29)bufRow = 29;
		else if (bufRow < 0)bufRow = 0;
		if (bufCol >= 29)bufCol = 29;
		else if (bufCol < 0)bufCol = 0;
		result[bufRow][bufCol].push_back(&i);
	}

	for (int i = 0; i <= 29; ++i)
	{
		for (int j = 0; j <= 29; ++j)
		{
			for (int dx = -1; dx <= 1; ++dx)
			{
				for (int dy = -1; dy <= 1; ++dy)
				{


					for (auto& Current : result[i][j])
					{


						int where_at_x = i + dx;
						int where_at_y = j + dy;

						if (where_at_x > 29 || where_at_y > 29 || where_at_x < 0 || where_at_y < 0)continue;
						for (auto& Checked : result[where_at_x][where_at_y])
						{
							//if (dx == 0 && dy == 0)continue;
							/*double Dir = Direction(*Current, *Checked);
							float Vx =  Current->m_CurrentVelocity.x * Current->m_k * cos(Dir) - Current->m_CurrentVelocity.y * Current->m_k * sin(Dir);
							float Vy = Current->m_CurrentVelocity.x * Current->m_k * sin(Dir) + Current->m_CurrentVelocity.y * Current->m_k * cos(Dir);
							float Vy2 = Checked->m_CurrentVelocity.x * Current->m_k * sin(Dir) + Checked->m_CurrentVelocity.y * Current->m_k * cos(Dir);
							float Vx2 = Checked->m_CurrentVelocity.x * Current->m_k * cos(Dir) - Checked->m_CurrentVelocity.y *Current->m_k*  sin(Dir);
							float buf = Vx;
							Vx = Vx2;
							Vx2 = buf;


							Current->m_CurrentVelocity.x = Vx * cos(-Dir) - Vy * sin(-Dir);
							Current->m_CurrentVelocity.y = Vx * sin(-Dir) + Vy * cos(-Dir);
							Checked->m_CurrentVelocity.x = Vx2 * cos(-Dir) - Vy2 * sin(-Dir);
							Checked->m_CurrentVelocity.y = Vx2 * sin(-Dir) + Vy2 * cos(-Dir);
							*/



							//float bufSize = Particles[i].m_Size;
							//if (Particles[j].m_Size>bufSize)bufSize=Particles[j].m_Size;







							float bufSize = Current->m_Size / 2 + Checked->m_Size / 2;
							const glm::vec2 Collision_axis = Current->m_CurrentPosition - Checked->m_CurrentPosition;
							const float dist = glm::length(Collision_axis);
							if (dist < bufSize && dist>0.0f)
							{
								const glm::vec2 n = Collision_axis / dist;
								const float delta = bufSize - dist;
								Current->m_CurrentPosition += 0.005f * delta * n;
								Checked->m_CurrentPosition -= 0.005f * delta * n;
								std::random_device dev;
								std::mt19937 rng(dev());
								std::uniform_int_distribution<std::mt19937::result_type> dist4(0, 4);

								//Current->m_Vertex[0].TexID = static_cast<float>(dist4(rng));
								//Checked->m_Vertex[0].TexID = static_cast<float>(dist4(rng));

							}
							else if (dist < 2.0f*bufSize)
							{
								glm::vec2 p1 = Current->m_CurrentPosition+Current->m_Size/2.0f;
								glm::vec2 p2 = Checked->m_CurrentPosition + Current->m_Size / 2.0f;
								
								float angle = atan2f(p2.y - p1.y, p2.x - p1.x) *180.0f/M_PI;
								float x1, y1;
								x1 = Current->m_Size / 2 * cos(angle);
								y1 = Current->m_Size / 2 * sin(angle);
								glm::vec2 BL{ x1 + p1.x,y1 + p1.y};
								glm::vec2 BR{ -x1 + p1.x,-y1 + p1.y};
								glm::vec2 TR{ -x1 + p2.x,-y1 + p2.y};
								glm::vec2 TL{ x1 + p2.x,y1 + p2.y };
								Pigulki.push_back(Obstacle(BL, BR, TR, TL, {1.0f,1.0f,1.0f,1.0f}, 0));
								Current->m_Vertex[0].TexID = 0.0f;
								Checked->m_Vertex[0].TexID = 0.0f;
							}



						}


					}


				}
			}
		}
	}

}

double Direction(glm::vec2 p1, glm::vec2 p2)
{

	double angle = atan2f(p2.y - p1.y, p2.x - p1.x);
	return angle;

}

void CreateQuad(Particle* target, float x, float y, float textureID, float size)
{
	{


		target->m_Vertex[0].Position = { x ,y, 0.0f };
		target->m_Vertex[0].Color = { 0.1f,0.1f,0.1f, 1.0f };
		target->m_Vertex[0].TexCoords = { 0.0f, 0.0f };
		target->m_Vertex[0].TexID = textureID;



		target->m_Vertex[1].Position = { x + size,y, 0.0f };
		target->m_Vertex[1].Color = { 0.1f,0.1f,0.1f, 1.0f };
		target->m_Vertex[1].TexCoords = { 1.0f, 0.0f };
		target->m_Vertex[1].TexID = textureID;


		target->m_Vertex[2].Position = { x + size,y + size, 0.0f };
		target->m_Vertex[2].Color = { 0.1f,0.1f,0.1f, 1.0f };
		target->m_Vertex[2].TexCoords = { 1.0f, 1.0f };
		target->m_Vertex[2].TexID = textureID;


		target->m_Vertex[3].Position = { x ,y + size, 0.0f };
		target->m_Vertex[3].Color = { 0.1f,0.1f,0.1f, 1.0f };
		target->m_Vertex[3].TexCoords = { 0.0f, 1.0f };
		target->m_Vertex[3].TexID = textureID;





	}
}