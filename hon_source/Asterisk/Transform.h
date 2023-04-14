#ifndef TRANSFORM_H
#define TRANSFORM_H
/******************************************************************************/
/*!
\headerfile   Transform.h
\author Jaejun Jang
\par    email: jjj404001\@gmail.com
\date   2018/12/14
\brief
		Header file for transform struct.

\copyright
All content 2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#define GLM_FORCE_CXX03
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

struct Transform
{
	Transform() : m_translation(0.f, 0.f, 0.f), m_scale(1.f, 1.f, 1.f), m_rotation(0.f) {}
	Transform(glm::vec3 translation_, glm::vec3 scale_, float rotation_) : m_translation(translation_), m_scale(scale_), m_rotation(rotation_) {}

	glm::vec3 m_translation;
	glm::vec3 m_scale;
	float m_rotation;

	glm::mat3 GetModel() const
	{
		glm::mat4 temp(1.f);
		temp = glm::translate(temp, m_translation);
		temp = glm::rotate(temp, m_rotation, glm::vec3(0.0f, 0.0f, 1.0f));
		temp = glm::scale(temp, m_scale);

		temp = glm::transpose(temp);
		glm::mat3 model;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				int k = i == 2 ? 3 : i, l = j == 2 ? 3 : j;
				model[i][j] = temp[k][l];
			}
		}

		return model;
	}

	void Translate(float input_x, float input_y, float input_z)
	{
		m_translation.x += input_x;
		m_translation.y += input_y;
		m_translation.z += input_z;
	}

	void SetTranslation(float input_x, float input_y, float input_z)
	{
		m_translation.x = input_x;
		m_translation.y = input_y;
		m_translation.z = input_z;
	}

	void SetScale(glm::vec3 input) { m_scale = input; }


	float GetRotation() const { return m_rotation; }
	glm::vec3 GetScale() const { return m_scale; }
};

#endif