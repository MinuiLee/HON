/******************************************************************************/
/*!
\file   BaseWindow.cpp
\author Jaejun Jang
\par    email: jjj404001\@gmail.com
\date   2018/12/14

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#define GLM_FORCE_CXX03
#include <GL/glew.h>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "BaseWindow.h"
#include "Texture.h"
#include "Animation.h"

#include "C_Collidable.h"
#include "C_Attackable.h"

#include <iostream>

#define MAX_RANGE 1000.f

void BaseWindow::Destroy()
{
	SDL_GL_DeleteContext(m_gl_context);
	SDL_Quit();
	m_view = nullptr;
}

void BaseWindow::Create(const Vector2u & size, const std::string & title)
{
	m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		size.x, size.y, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_FULLSCREEN);

	m_gl_context = SDL_GL_CreateContext(m_window);
    SDL_ShowCursor(SDL_DISABLE);

	int size_x = 0, size_y = 0;
	SDL_GetWindowSize(m_window, &size_x, &size_y);


	m_initial_window_size.x = static_cast<float>(size_x);
	m_initial_window_size.y = static_cast<float>(size_y);

	glewInit();

	m_shader_container.ConfigContainer();
	// OpenGL blending, depth testing functions
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glEnable(GL_DEPTH_TEST);

	// Depth value 0.0 ~ 1.0 maps to
	// -1 ~ 1 in model space.
	glClearDepth(-1.0f);
	glDepthFunc(GL_GEQUAL);

	glEnable(GL_CULL_FACE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	ResizeWindow();


	// Frame buffer texture
	glGenTextures(1, &m_framebuffer_texture);
	glBindTexture(GL_TEXTURE_2D, m_framebuffer_texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size_x, size_y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);

	// Particle frame buffer texture
	glGenTextures(1, &m_particle_framebuffer_texture);
	glBindTexture(GL_TEXTURE_2D, m_particle_framebuffer_texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size_x, size_y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);


	// Render buffer objects
	glGenRenderbuffers(RenderBuffer::Num_of_buffers, m_render_buffer_objects);
	// Color render buffer objects.
	glBindRenderbuffer(GL_RENDERBUFFER, m_render_buffer_objects[RenderBuffer::Color_buffer]);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, size_x, size_y);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	// Depth render buffer.
	glBindRenderbuffer(GL_RENDERBUFFER, m_render_buffer_objects[RenderBuffer::Depth_buffer]);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size_x, size_y);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	// Particle color render buffer.
	glBindRenderbuffer(GL_RENDERBUFFER, m_render_buffer_objects[RenderBuffer::Particle_color_Buffer]);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, size_x, size_y);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	
	// Frame buffer
	glGenFramebuffers(1, &m_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_framebuffer_texture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_particle_framebuffer_texture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_render_buffer_objects[RenderBuffer::Depth_buffer]);


	if (auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER); status != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR : Frame buffer state is not GL_FRAMEBUFFER_COMPLETE with " << std::hex
				  << status << "." << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	m_shadowShader		= m_shader_container.GetShader("Shadow");
	m_animShadowShader  = m_shader_container.GetShader("AnimShadow");
	m_DebugDrawShader   = m_shader_container.GetShader("Debug");
	m_frameBufferShader = m_shader_container.GetShader("FrameBuffer");


	m_orthgonal_projection =
		glm::ortho((-size_x / (2.0f)), (size_x / (2.0f)), (-size_y / (2.0f)), (size_y / (2.0f)));

	m_projection = glm::ortho((-size_x / (2.0f)) * m_zoom_amount, (size_x / (2.0f)) * m_zoom_amount,
		(-size_y / (2.0f)) * m_zoom_amount, (size_y / (2.0f)) * m_zoom_amount);

	glViewport(0, 0, static_cast<int>(size_x), static_cast<int>(size_y));

	glClearColor(0, 0, 0, 1);
}

void BaseWindow::ResizeWindow(EventDetails * /*details*/)
{
	int size_x = 0, size_y = 0;
	SDL_GetWindowSize(m_window, &size_x, &size_y);
	const glm::vec2 window_size = glm::vec2(size_x, size_y);

	glViewport(0, 0, static_cast<int>(window_size.x), static_cast<int>(window_size.y));

	m_projection = glm::ortho((-window_size.x / (2.0f)) * m_zoom_amount,
		(window_size.x / (2.0f)) * m_zoom_amount, (-window_size.y / (2.0f)) * m_zoom_amount,
		(window_size.y / (2.0f)) * m_zoom_amount);


	m_projection = glm::translate(m_projection,
		glm::vec3(((m_initial_window_size.x - window_size.x) / 2.0f),
			((m_initial_window_size.y - window_size.y) / 2.0f), 0.0f));


	m_orthgonal_projection = glm::ortho((-window_size.x / (2.0f)), (window_size.x / (2.0f)),
		(-window_size.y / (2.0f)), (window_size.y / (2.0f)));

	m_orthgonal_projection = glm::translate(m_orthgonal_projection,
		glm::vec3(((m_initial_window_size.x - window_size.x) / 2.0f),
			((m_initial_window_size.y - window_size.y) / 2.0f), 0.0f));
}

void BaseWindow::CalculateMatrix(Sprite & _sprite, float const & _depth)
{
	auto translation = _sprite.GetPosition();
	translation.z	= _depth;

	// Model
	glm::mat4 model(1.f);
	model = glm::translate(model, translation);
	model = glm::rotate(model, _sprite.GetRotation(), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, _sprite.GetScale());

	_sprite.SetModel(model);

	// View
	glm::mat4 view(1.f);
	view = glm::translate(view, -m_view->GetPosition()); // Camera location
	view = glm::scale(view, glm::vec3(1.f, 1.f, 1.f));

	_sprite.SetView(view);

	// Combine
	_sprite.GetMatrix() = glm::mat4(1.f);
	_sprite.GetMatrix() *= m_projection;
	_sprite.GetMatrix() *= view;
	_sprite.GetMatrix() *= model;
}

void BaseWindow::CalculateMatrixForParallax(Sprite & _sprite, float factor_)
{
	auto translation = _sprite.GetPosition();
	translation.z	= _sprite.GetDepth();

	if (factor_)
		translation.x += (translation.x - m_view->GetPosition().x) * (translation.z / factor_);

	// Model
	glm::mat4 model(1.f);
	model = glm::translate(model, translation);
	model = glm::rotate(model, _sprite.GetRotation(), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, _sprite.GetScale());

	_sprite.SetModel(model);

	// View
	glm::mat4 view(1.f);
	view = glm::translate(view, -m_view->GetPosition()); // Camera location
	view = glm::scale(view, glm::vec3(1.f, 1.f, 1.f));

	_sprite.SetView(view);

	// Combine
	_sprite.GetMatrix() = glm::mat4(1.f);
	_sprite.GetMatrix() *= m_projection;
	_sprite.GetMatrix() *= view;
	_sprite.GetMatrix() *= model;
}

void BaseWindow::CalculateSemiStaticMatrx(Sprite & _sprite, float _depth)
{
	auto translation = _sprite.GetPosition();
	translation.z	= _depth;

	// Model
	glm::mat4 model(1.f);
	model = glm::translate(model, translation);
	model = glm::rotate(model, _sprite.GetRotation(), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, _sprite.GetScale());

	_sprite.SetModel(model);


	// View
	glm::mat4 view(1.f);
	view = glm::translate(view, -m_view->GetPosition()); // Camera location
	view = glm::scale(view, glm::vec3(1.f, 1.f, 1.f));

	_sprite.SetView(view);

	// Combine
	_sprite.GetMatrix() = glm::mat4(1.f);
	_sprite.GetMatrix() *= m_projection;
	_sprite.GetMatrix() *= view;
	_sprite.GetMatrix() *= model;
}

void BaseWindow::CalculateStaticMatrx(Sprite & _sprite, float _depth)
{
	auto translation = _sprite.GetPosition();
	translation.z	= _depth;

	// Model
	glm::mat4 model(1.f);
	model = glm::translate(model, translation);
	model = glm::rotate(model, _sprite.GetRotation(), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, _sprite.GetScale());

	_sprite.SetModel(model);

	// View
	glm::mat4 view(1.f);
	view = glm::translate(view, -m_view->GetPosition()); // Camera location
	view = glm::scale(view, glm::vec3(1.f, 1.f, 1.f));

	_sprite.SetView(view);

	// Combine
	_sprite.GetMatrix() = glm::mat4(1.f);
	_sprite.GetMatrix() *= m_projection;
	_sprite.GetMatrix() *= _sprite.GetModel();
}

void BaseWindow::CalculateDirectProjectionDraw(Sprite & _sprite, float _depth)
{
	auto translation = _sprite.GetPosition();
	translation.z	= _depth;

	// Model
	glm::mat4 model(1.f);


	model = glm::translate(model, translation);
	model = glm::rotate(model, _sprite.GetRotation(), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, _sprite.GetScale());

	_sprite.SetModel(model);


	// Combine
	_sprite.GetMatrix() = glm::mat4(1.f);
	_sprite.GetMatrix() *= m_orthgonal_projection;
	_sprite.GetMatrix() *= _sprite.GetModel();
}

void BaseWindow::StartDraw()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

	GLenum draw_buffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, draw_buffers);



	glActiveTexture(GL_TEXTURE0);


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void BaseWindow::Draw(Sprite & _sprite)
{
	CalculateMatrix(_sprite, _sprite.GetDepth());
	if (!_sprite.GetShader()) _sprite.SetShader(m_shader_container.GetShader("Basic"));

	glUseProgram(_sprite.GetShader()->GetProgram());


	unsigned int combinedLoc = glGetUniformLocation(_sprite.GetShader()->GetProgram(), "combined");
	glUniformMatrix4fv(combinedLoc, 1, GL_FALSE, glm::value_ptr(_sprite.GetMatrix()));

	glBindTexture(GL_TEXTURE_2D, _sprite.GetTexture()->GetTextureID());
	glBindVertexArray(_sprite.GetMesh()->GetVAO());

	glDrawElements(GL_TRIANGLES, _sprite.GetMesh()->Get_NumOfVertices(), GL_UNSIGNED_INT, nullptr);
}

void BaseWindow::ParallaxDrawing(Sprite & _sprite, float factor_)
{
	if (_sprite.GetShader())
		glUseProgram(_sprite.GetShader()->GetProgram());
	else
		glUseProgram(GetErrorShader()->GetProgram());

	CalculateMatrixForParallax(_sprite, factor_);


	unsigned int combinedLoc = glGetUniformLocation(_sprite.GetShader()->GetProgram(), "combined");
	glUniformMatrix4fv(combinedLoc, 1, GL_FALSE, glm::value_ptr(_sprite.GetMatrix()));

	glBindTexture(GL_TEXTURE_2D, _sprite.GetTexture()->GetTextureID());
	glBindVertexArray(_sprite.GetMesh()->GetVAO());

	glDrawElements(GL_TRIANGLES, _sprite.GetMesh()->Get_NumOfVertices(), GL_UNSIGNED_INT, nullptr);
	_sprite.SetShadowVisibility(false);
}

// Update Camera only
void BaseWindow::SemiStaticDraw(Sprite & _sprite)
{
	if (_sprite.GetShader())
		glUseProgram(_sprite.GetShader()->GetProgram());
	else
		glUseProgram(GetErrorShader()->GetProgram());


	if (m_updated) CalculateSemiStaticMatrx(_sprite, _sprite.GetDepth());

	// Update camera's position only.
	glm::mat4 view(1.f);
	view = glm::translate(view, -m_view->GetPosition()); // Camera location
	view = glm::scale(view, glm::vec3(1.f, 1.f, 1.f));

	_sprite.GetMatrix() = glm::mat4(1.f);
	_sprite.GetMatrix() *= m_projection;
	_sprite.GetMatrix() *= view;
	_sprite.GetMatrix() *= _sprite.GetModel();


	unsigned int combinedLoc = glGetUniformLocation(_sprite.GetShader()->GetProgram(), "combined");
	glUniformMatrix4fv(combinedLoc, 1, GL_FALSE, glm::value_ptr(_sprite.GetMatrix()));

	glBindTexture(GL_TEXTURE_2D, _sprite.GetTexture()->GetTextureID());
	glBindVertexArray(_sprite.GetMesh()->GetVAO());

	glDrawElements(GL_TRIANGLES, _sprite.GetMesh()->Get_NumOfVertices(), GL_UNSIGNED_INT, nullptr);
}

void BaseWindow::StaticDraw(Sprite & _sprite)
{
	if (!_sprite.GetShader()) _sprite.SetShader(m_shader_container.GetShader("Basic"));

	glUseProgram(_sprite.GetShader()->GetProgram());

	if (m_updated) CalculateStaticMatrx(_sprite, _sprite.GetDepth());


	unsigned int combinedLoc = glGetUniformLocation(_sprite.GetShader()->GetProgram(), "combined");
	glUniformMatrix4fv(combinedLoc, 1, GL_FALSE, glm::value_ptr(_sprite.GetMatrix()));

	glBindTexture(GL_TEXTURE_2D, _sprite.GetTexture()->GetTextureID());
	glBindVertexArray(_sprite.GetMesh()->GetVAO());

	glDrawElements(GL_TRIANGLES, _sprite.GetMesh()->Get_NumOfVertices(), GL_UNSIGNED_INT, nullptr);
}

void BaseWindow::DirectProjectionDraw(Sprite & _sprite)
{
	if (!_sprite.GetShader()) _sprite.SetShader(m_shader_container.GetShader("Basic"));

	glUseProgram(_sprite.GetShader()->GetProgram());


	unsigned int combinedLoc = glGetUniformLocation(_sprite.GetShader()->GetProgram(), "combined");
	glUniformMatrix4fv(combinedLoc, 1, GL_FALSE, glm::value_ptr(_sprite.GetMatrix()));

	glBindTexture(GL_TEXTURE_2D, _sprite.GetTexture()->GetTextureID());
	glBindVertexArray(_sprite.GetMesh()->GetVAO());

	glDrawElements(GL_TRIANGLES, _sprite.GetMesh()->Get_NumOfVertices(), GL_UNSIGNED_INT, nullptr);
}

void BaseWindow::DrawAnim(Sprite & _sprite, const Vector2u & sprite_size, float depth_, Animation * anim)
{
	if (!_sprite.GetShader()) _sprite.SetShader(m_shader_container.GetShader("Basic"));
	glUseProgram(_sprite.GetShader()->GetProgram());


	CalculateMatrix(_sprite, depth_);

	const unsigned int combinedLoc =
		glGetUniformLocation(_sprite.GetShader()->GetProgram(), "combined");
	glUniformMatrix4fv(combinedLoc, 1, GL_FALSE, _sprite.GetMatrixValue());


	// Image width, height, number of row, column
	glm::vec2 uv			 = glm::vec2(sprite_size.y, sprite_size.x);
	const unsigned int uvLoc = glGetUniformLocation(_sprite.GetShader()->GetProgram(), "uv");
	glUniform2fv(uvLoc, 1, glm::value_ptr(uv));


	const unsigned int coordinateLoc =
		glGetUniformLocation(_sprite.GetShader()->GetProgram(), "coordinate");
	glUniform2fv(coordinateLoc, 1, glm::value_ptr(anim->GetCurrentFrame()));


	glBindTexture(GL_TEXTURE_2D, _sprite.GetTexture()->GetTextureID());
	glBindVertexArray(_sprite.GetMesh()->GetVAO());

	glDrawElements(GL_TRIANGLES, _sprite.GetMesh()->Get_NumOfVertices(), GL_UNSIGNED_INT, nullptr);

	_sprite.SetShadowVisibility(false);
}

void BaseWindow::EndDraw()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default


	Mesh quad(Vector2f(2.0f, 2.0f)); // 2.0f, 2.0f = full screen.
	glUseProgram(m_frameBufferShader->GetProgram());

	unsigned int is_shakeLoc = glGetUniformLocation(m_frameBufferShader->GetProgram(), "is_shake");
	unsigned int is_fadeInLoc =
		glGetUniformLocation(m_frameBufferShader->GetProgram(), "is_fadeIn");
	unsigned int is_fadeOutLoc =
		glGetUniformLocation(m_frameBufferShader->GetProgram(), "is_fadeOut");

	if (m_is_shake)
	{
		unsigned int shake_timerLoc =
			glGetUniformLocation(m_frameBufferShader->GetProgram(), "shake_timer");
		glUniform1i(is_shakeLoc, true);
		glUniform1f(shake_timerLoc, m_shake_timer);
	}
	else
		glUniform1i(is_shakeLoc, false);

	if (m_is_fadeIn)
	{
		unsigned int fadeIn_timerLoc =
			glGetUniformLocation(m_frameBufferShader->GetProgram(), "fadeIn_timer");
		glUniform1i(is_fadeInLoc, true);
		glUniform1f(fadeIn_timerLoc, m_fadeIn_timer);
	}
	else
		glUniform1i(is_fadeInLoc, false);

	if (m_is_fadeOut)
	{
		unsigned int fadeOut_timerLoc =
			glGetUniformLocation(m_frameBufferShader->GetProgram(), "fadeOut_timer");
		glUniform1i(is_fadeOutLoc, true);
		glUniform1f(fadeOut_timerLoc, m_fadeOut_timer);
	}
	else
		glUniform1i(is_fadeOutLoc, false);

	view_delta_stack += m_view->GetDelta() / m_view->GetSize();


	glUniform2fv(glGetUniformLocation(m_frameBufferShader->GetProgram(), "view_position"), 1, glm::value_ptr(view_delta_stack));
	glUniform1i(glGetUniformLocation(m_frameBufferShader->GetProgram(), "texture0"), 0);
	glUniform1i(glGetUniformLocation(m_frameBufferShader->GetProgram(), "texture1"), 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_framebuffer_texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_particle_framebuffer_texture);
	

	glBindVertexArray(quad.GetVAO());
	glDrawElements(GL_TRIANGLES, quad.Get_NumOfVertices(), GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);

	SDL_GL_SwapWindow(m_window);
	m_updated = false;
}

void BaseWindow::SetZoom(float _amount)
{
	m_zoom_amount = _amount;
	ResizeWindow();
}


#ifdef _DEBUG

void BaseWindow::DebugDraw(Sprite & sprite_, const C_Collidable * collision_)
{
	glUseProgram(m_DebugDrawShader->GetProgram());

	glm::vec4 translation = glm::vec4(sprite_.GetPosition().x - m_view->GetPosition().x,
		sprite_.GetPosition().y - m_view->GetPosition().y, sprite_.GetPosition().z, 1.0f);
	translation			  = m_projection * translation;

	glm::vec2 collisionRect =
		glm::vec2(collision_->GetAABB().m_width, collision_->GetAABB().m_height);

	glm::vec4 temp = glm::vec4(collisionRect, 0.0f, 1.0f);

	temp = m_projection * temp;

	collisionRect = glm::vec2(temp.x, temp.y);


	const auto left_x  = translation.x - collisionRect.x / 2;
	const auto right_x = collisionRect.x / 2 + translation.x;

	const auto bottom_y = translation.y - 2 * collisionRect.y;
	const auto top_y	= translation.y - collisionRect.y;


	translation = { left_x, right_x, bottom_y, top_y };

	const auto translationLoc =
		glGetUniformLocation(m_DebugDrawShader->GetProgram(), "translation");
	glUniform4fv(translationLoc, 1, glm::value_ptr(translation));
	const auto colorLoc = glGetUniformLocation(m_DebugDrawShader->GetProgram(), "color");
	glUniform4fv(colorLoc, 1, m_collision_color.GetColorPtr());


	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBindVertexArray(sprite_.GetMesh()->GetPolyVAO());
	glDepthFunc(GL_ALWAYS);

	glDrawArrays(GL_QUADS, 0, sprite_.GetMesh()->Get_NumOf_PolyVertices());

	glDepthFunc(GL_GEQUAL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void BaseWindow::DebugDraw(Sprite & sprite_, const C_Attackable * attack)
{
	glUseProgram(m_DebugDrawShader->GetProgram());

	float factor = (attack->GetAttackRange().m_width / 2); // left = center - half width.
														   // revert to center.

	glm::vec4 translation = glm::vec4(
		attack->GetAttackRange().m_left - m_view->GetPosition().x + factor,
		attack->GetAttackRange().m_top - m_view->GetPosition().y, sprite_.GetPosition().z, 1.0f);
	translation = m_projection * translation;

	glm::vec2 collisionRect =
		glm::vec2(attack->GetAttackRange().m_width, attack->GetAttackRange().m_height);

	glm::vec4 temp = glm::vec4(collisionRect, 0.0f, 1.0f);

	temp = m_projection * temp;

	collisionRect = glm::vec2(temp.x, temp.y);


	const auto left_x  = translation.x - collisionRect.x / 2;
	const auto right_x = collisionRect.x / 2 + translation.x;

	const auto bottom_y = translation.y - 2 * collisionRect.y;
	const auto top_y	= translation.y - collisionRect.y;


	translation = { left_x, right_x, bottom_y, top_y };
	const auto translationLoc =
		glGetUniformLocation(m_DebugDrawShader->GetProgram(), "translation");
	glUniform4fv(translationLoc, 1, glm::value_ptr(translation));
	const auto colorLoc = glGetUniformLocation(m_DebugDrawShader->GetProgram(), "color");
	glUniform4fv(colorLoc, 1, m_attackable_color.GetColorPtr());

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBindVertexArray(sprite_.GetMesh()->GetPolyVAO());
	glDepthFunc(GL_ALWAYS);

	glDrawArrays(GL_QUADS, 0, sprite_.GetMesh()->Get_NumOf_PolyVertices());

	glDepthFunc(GL_GEQUAL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void BaseWindow::DebugDraw(const FloatRect _rect, glm::vec4 _color) const
{
	glUseProgram(m_DebugDrawShader->GetProgram());


	auto translation = glm::vec4(_rect.m_left, _rect.m_top, _rect.m_width, _rect.m_height);

	// View
	glm::mat4 view(1.f);
	view = glm::translate(view, -m_view->GetPosition()); // Camera location


	const auto translationLoc =
		glGetUniformLocation(m_DebugDrawShader->GetProgram(), "translation");
	glUniform4fv(translationLoc, 1, glm::value_ptr(translation));

	const auto viewLoc = glGetUniformLocation(m_DebugDrawShader->GetProgram(), "view");
	glUniformMatrix4fv(viewLoc, 1, false, glm::value_ptr(view));

	const auto projectionLoc = glGetUniformLocation(m_DebugDrawShader->GetProgram(), "projection");
	glUniformMatrix4fv(projectionLoc, 1, false, glm::value_ptr(m_projection));

	const auto colorLoc = glGetUniformLocation(m_DebugDrawShader->GetProgram(), "color");
	glUniform4fv(colorLoc, 1, glm::value_ptr(_color));


	glm::vec4 vert[4] = { glm::vec4(
							  translation.x, translation.y - translation.w, 0, 1), // left bottom
		glm::vec4(
			translation.x + translation.z, translation.y - translation.w, 0, 1), // right bottom
		glm::vec4(translation.x + translation.z, translation.y, 0, 1),			 // right top
		glm::vec4(translation.x, translation.y, 0, 1) };						 // left top


	glDepthFunc(GL_ALWAYS);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_QUADS, 0, 4);
	glDepthFunc(GL_GEQUAL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


#endif

FloatRect BaseWindow::GetViewSpace() const
{
	FloatRect rect;
	rect.m_width  = m_view->GetSize().x * m_zoom_amount;
	rect.m_height = m_view->GetSize().y * m_zoom_amount;
	rect.m_left   = m_view->GetCenter().x - rect.m_width / 2;
	rect.m_top	= m_view->GetCenter().y + rect.m_height / 2;


	return rect;
}

FloatRect BaseWindow::GetUpdateCullingSpace() const
{
	FloatRect rect;
	rect.m_width  = m_view->GetSize().x * 2;
	rect.m_height = m_view->GetSize().y;
	rect.m_left   = m_view->GetCenter().x - rect.m_width / 2;
	rect.m_top	= m_view->GetCenter().y + rect.m_height / 2;
	return rect;
}

void BaseWindow::UpdateShakeTimer(float dt)
{
	if (m_is_shake) m_shake_duration -= dt;
	if (m_shake_duration < 0.f)
	{
		m_shake_duration = 0.15f;
		m_is_shake		 = false;
	}
}

float const & BaseWindow::GetZoom() { return m_zoom_amount; }
