/******************************************************************************/
/*!
\file   Mouse.cpp
\author Minui Lee
\par    email: alsdml0727@gmail.com
\date   2018/12/14

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "Mouse.h"

void Mouse::Initialize(Texture * txt, int w, int h)
{
	width = w;
	height = h;
	mouse.BuildMesh(txt);
	mouse.SetTexture(*txt);
}

bool Mouse::IsCollidingWith(Sprite sp, float zoom, float dx, float dy)
{
	float inzoom = 1.f / zoom;
	return ((zoom *mouse.GetPosition().x + dx <= sp.GetPosition().x + inzoom * sp.GetSize().x / 2 * sp.GetScale().x) &&
		(zoom *mouse.GetPosition().x + dx >= sp.GetPosition().x - inzoom * sp.GetSize().x / 2 * sp.GetScale().x) &&
		(zoom *mouse.GetPosition().y + dy <= sp.GetPosition().y + inzoom * sp.GetSize().y / 2 * sp.GetScale().y) &&
		(zoom *mouse.GetPosition().y + dy >= sp.GetPosition().y - inzoom * sp.GetSize().y / 2 * sp.GetScale().y));
}

bool Mouse::IsCollidingWith(Sprite sp, Vector2f scale)
{
	scale = scale / sp.GetSize();

	return ((mouse.GetPosition().x <= sp.GetPosition().x + sp.GetSize().x / 2 * scale.x) &&
		(mouse.GetPosition().x >= sp.GetPosition().x - sp.GetSize().x / 2 * scale.x) &&
		(mouse.GetPosition().y <= sp.GetPosition().y + sp.GetSize().y / 2 * scale.y) &&
		(mouse.GetPosition().y >= sp.GetPosition().y - sp.GetSize().y / 2 * scale.y));
}

bool Mouse::IsCollidingWith(std::vector<std::pair<std::string, Sprite>> objects, std::string name)
{
	if (name == " ")
		return true;

	auto sp = std::find_if(
		objects.begin(), objects.end(), [&name](const std::pair <std::string, Sprite> & p) { return p.first == name; })->second;

	return ((mouse.GetPosition().x <= sp.GetPosition().x + sp.GetSize().x / 2 * sp.GetScale().x) &&
		(mouse.GetPosition().x >= sp.GetPosition().x - sp.GetSize().x / 2 * sp.GetScale().x) &&
		(mouse.GetPosition().y <= sp.GetPosition().y + sp.GetSize().y / 2 * sp.GetScale().y) &&
		(mouse.GetPosition().y >= sp.GetPosition().y - sp.GetSize().y / 2 * sp.GetScale().y));
}

void Mouse::SetPosition(Vector2i pos)
{
	mouse.SetPosition(pos);
}

