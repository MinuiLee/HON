#ifndef BASEWINDOW_H
#define BASEWINDOW_H
/******************************************************************************/
/*!
\file   BaseWindow.h
\author Jaejun Jang
\par    email: jjj404001\@gmail.com
\date   2018/12/14
\brief
		Header file for BaseWindow class.

\copyright
All content 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <SDL2/include/SDL.h>

#include "View.h"
#include "Sprite.h"
#include "FloatRect.h"
#include "ShaderContainer.h"
#include "DepthList.h"

#ifdef _DEBUG
class C_Collidable;
class C_Attackable;
#endif

class Animation;
struct EventDetails;
class BaseWindow
{
	enum RenderBuffer
	{
		Color_buffer,
		Depth_buffer,
		Particle_color_Buffer,
		Num_of_buffers
	};

public:
	BaseWindow() : m_view(nullptr), m_shader_container("config") {}

	virtual ~BaseWindow() { Destroy(); }
	void Destroy();

	void Create(const Vector2u & size, const std::string & title);
	void ResizeWindow(EventDetails * details = nullptr);
	SDL_Window * GetSDLWindow() { return m_window; }

	// Matrix
	void CalculateMatrix(Sprite & _sprite, float const & _depth);
	void CalculateMatrixForParallax(Sprite & _sprite, float factor_ = 0.0f);
	void CalculateSemiStaticMatrx(Sprite & _sprite, float _depth);
	void CalculateStaticMatrx(Sprite & _sprite, float _depth);
	void CalculateDirectProjectionDraw(Sprite & _sprite, float _depth);

	// Drawing
	void StartDraw();
	void Draw(Sprite & _sprite);
	void ParallaxDrawing(Sprite & _sprite, float factor_);
	void SemiStaticDraw(Sprite & _sprite);
	void StaticDraw(Sprite & _sprite);
	void DirectProjectionDraw(Sprite & _sprite);
	void DrawAnim(Sprite & _sprite, const Vector2u & sprite_size, float depth_, Animation * anim = nullptr);
	void EndDraw();

	// Zoom
	void SetZoom(float _amount);


#ifdef _DEBUG
	void DebugDraw(Sprite & sprite_, const C_Collidable * collision_ = nullptr);
	void DebugDraw(Sprite & sprite_, const C_Attackable * attack = nullptr);
	void DebugDraw(
		const FloatRect _rect, glm::vec4 _color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)) const;
#endif

	// View
	void SetView(View * view) { m_view = view; }
	View * GetView() { return m_view; }
	FloatRect GetViewSpace() const;
	FloatRect GetUpdateCullingSpace() const;

	ShaderContainer & GetShaders() { return m_shader_container; }

	Shader * GetErrorShader() { return m_shader_container.GetShader("ERROR"); }

	void UpdateShakeTimer(float dt);

	float const & GetZoom();

	float m_shake_timer   = 10.f; 
	float m_fadeIn_timer  = 1.0f; 
	float m_fadeOut_timer = 0.0f; 
	bool m_is_shake		  = false;
	bool m_is_fadeIn	  = false;
	bool m_is_fadeOut	  = false; 
	const glm::mat4 & GetProjection() { return m_projection; }
	glm::mat4 * GetProjectionPtr() { return &m_projection; }

	Vector2f GetInitialWindowSize() { return m_initial_window_size; }
	void SetShakeDuration(float time) { m_shake_duration = time; }

private:
	SDL_Window * m_window	  = nullptr;
	SDL_GLContext m_gl_context = nullptr;

	View * m_view;
	Vector2f m_initial_window_size;
	Color m_clear_color{ 0.0f, 0.3f, 0.0f, 0.0f };

	glm::mat4 m_projection;
	glm::mat4 m_orthgonal_projection;

	float m_zoom_amount = 1.0f;

	bool m_updated = false;

	Shader * m_shadowShader		 = nullptr;
	Shader * m_animShadowShader  = nullptr;
	Shader * m_DebugDrawShader   = nullptr;
	Shader * m_frameBufferShader = nullptr;

	ShaderContainer m_shader_container;

	unsigned int m_framebuffer					= 0;
	unsigned int m_framebuffer_texture			= 0;
	unsigned int m_particle_framebuffer_texture = 0;

	unsigned int m_render_buffer_objects[RenderBuffer::Num_of_buffers];


	float m_shake_duration = 0.15f;

	Color m_collision_color  = { 1.0f, 0.0f, 0.0f, 1.0f };
	Color m_attackable_color = { 0.0f, 0.0f, 1.0f, 1.0f };

	glm::vec2 view_delta_stack = glm::vec2();
};

#endif