#include "camera.h"
#include "simple_logger.h"

extern sf::Clock g_delta_clock;

Camera::Camera()
{
	m_pos = Vec3(0.0, 0.0f, 3.0f);
	m_target = Vec3(0.0f, 0.0f, 0.0f);
	m_forward = Vec3(0.0f, 0.0f, -1.0f);//get forward direction
	m_up = Vec3(0.0f, 1.0f, 0.0f); //set default up vector to positive y
	m_right = Normalize(Cross(m_forward, m_up));
	
	m_view_mat = LookAt(m_pos, m_target, m_up);//set view matrix

	sf::Vector2u win_size = g_window->getSize();
	m_proj_mat = Perspective(glm::radians(45.0f), (float)win_size.x / win_size.y, 0.1f, 100.0f);

	yaw = pitch = roll = 0.0f;
}

Mat4 Camera::GetProj()
{
	return m_proj_mat;
}

Mat4 Camera::GetCamView()
{
	return m_view_mat;
}

void Camera::UpdateView()
{
	m_view_mat = LookAt(m_pos, m_pos + m_forward, m_up);
}

void Camera::HandleInput(sf::Event event)
{
	sf::Time time = g_delta_clock.getElapsedTime();
	GLfloat cam_speed = 5.0f * time.asSeconds();

	if (event.type == sf::Event::KeyPressed)
		slog("Key Press");
	{
		switch (event.key.code)
		{
		case sf::Keyboard::W:
			m_pos += cam_speed * m_forward;
			break;
		case sf::Keyboard::S:
			m_pos -= cam_speed * m_forward;
			break;
		case sf::Keyboard::A:
			m_pos -= m_right * cam_speed;
			break;
		case sf::Keyboard::D:
			m_pos += m_right * cam_speed;
			break;
		default:
			break;
		}
	}
	
	if (event.type == sf::Event::MouseMoved)
	{
		
		GLfloat sensitivity = 0.05f; // mouse sensitivity

		sf::Vector2u win_size = g_window->getSize();
		sf::Vector2i mouse_pos = sf::Mouse::getPosition();
		float xcenter = win_size.x / 2.0f;
		float ycenter = win_size.y / 2.0f;

		float xoffset = (float)mouse_pos.x - xcenter;
		float yoffset = (float)mouse_pos.y - ycenter;

		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		else if (pitch < -89.0f)
			pitch = -89.0f;

		m_forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		m_forward.y = sin(glm::radians(pitch));
		m_forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

		m_forward = Normalize(m_forward);
		sf::Mouse::setPosition(sf::Vector2f(xcenter, ycenter));
	}


	UpdateView();
}