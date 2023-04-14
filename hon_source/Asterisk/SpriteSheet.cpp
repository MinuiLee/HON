/******************************************************************************/
/*!
\file   SpriteSheet.cpp
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <fstream>
#include <sstream>
#include <iostream>

#include "SpriteSheet.h"
#include "TextureManager.h"
#include "Animation.h"
#include "Window.h"

SpriteSheet::SpriteSheet(const std::string & file, TextureManager * texMgr)
	: m_name(file), m_dir(Direction::Right), m_texMgr(texMgr)
{
	m_texMgr->RequireResource(m_name);
	LoadSheet(m_name);

    //Animation initialize
	SetCurrentAnimation("Body", "Idle", true, true);
	SetCurrentAnimation("AttackEffect", "Dash", false, false);
	SetCurrentAnimation("HitEffect", "Hit", false, false);
    SetCurrentAnimation("SteamRight", "Boom", false, false);
    SetCurrentAnimation("SteamLeft", "Boom", false, false);
    SetCurrentAnimation("SteamUp", "Boom", false, false);
    SetCurrentAnimation("SteamDown", "Boom", false, false);
    SetCurrentAnimation("ShockWaveRight", "Boom", false, false);
    SetCurrentAnimation("ShockWaveLeft", "Boom", false, false);
    SetCurrentAnimation("ShockWaveUp", "Boom", false, false);
    SetCurrentAnimation("ShockWaveDown", "Boom", false, false);
    SetCurrentAnimation("Star", "Star", false, false);
	SetCurrentAnimation("Flower", "Red", false, false);
    SetCurrentAnimation("Steam", "Steam", true, true);
}

SpriteSheet::~SpriteSheet()
{
	m_texMgr->ReleaseResource(m_name);
	Purge();
}

void SpriteSheet::SetCurrentAnimation(
	const std::string & part, const std::string & name, bool play, bool loop)
{
	auto itr = m_animations.find(part);
	if (itr == m_animations.end()) return;
	// Animation part exist

	auto itr2 = itr->second.find(name);
	if (itr2 == itr->second.end()) return;
	// Animation exist


	auto currentAnim = m_currentAnims.find(part);

	currentAnim->second.second = itr2->second;
	currentAnim->second.second->SetPlay(play);
	currentAnim->second.second->SetLoop(loop);
	currentAnim->second.second->Reset();

	m_currentAnimi = name;
}

void SpriteSheet::SetSpritePosition(const Vector3f & pos)
{
	auto BodyAnim = m_currentAnims.find("Body");
	if (BodyAnim != m_currentAnims.end())
	    BodyAnim->second.first.SetPosition(pos.x, pos.y, pos.z);

    int offset = 0;
    if (m_dir == Direction::Right)
        offset = m_size.x;
    else if (m_dir == Direction::Left)
        offset = -static_cast<int>(m_size.x);

	auto AttackAnim = m_currentAnims.find("AttackEffect");
    if (AttackAnim != m_currentAnims.end())
        AttackAnim->second.first.SetPosition(pos.x + static_cast<float>(offset / 2), pos.y, pos.z);
	
	auto HitEffect = m_currentAnims.find("HitEffect");
	if (HitEffect != m_currentAnims.end())
	    HitEffect->second.first.SetPosition(pos.x, pos.y, pos.z);

    auto SteamRight = m_currentAnims.find("SteamRight");
    if (SteamRight != m_currentAnims.end() && //Part exist
        (SteamRight->second.second->GetCurrentFrame().x == 0 || //Is animation starting
            !SteamRight->second.second->IsPlaying())) 
            SteamRight->second.first.SetPosition(pos.x - m_size.x, pos.y, pos.z);

    auto SteamLeft = m_currentAnims.find("SteamLeft");
    if (SteamLeft != m_currentAnims.end() && //Part exist
        (SteamLeft->second.second->GetCurrentFrame().x == 0 || //Is animation starting
            !SteamLeft->second.second->IsPlaying())) 
            SteamLeft->second.first.SetPosition(pos.x + m_size.x, pos.y, pos.z);

    auto SteamUp = m_currentAnims.find("SteamUp");
    if (SteamUp != m_currentAnims.end() && //Part exist
        (SteamUp->second.second->GetCurrentFrame().x == 0 || //Is animation starting
            !SteamUp->second.second->IsPlaying())) 
            SteamUp->second.first.SetPosition(pos.x, pos.y - m_size.y, pos.z);

    auto SteamDown = m_currentAnims.find("SteamDown");
    if (SteamDown != m_currentAnims.end() && //Part exist
        (SteamDown->second.second->GetCurrentFrame().x == 0 //Is animation starting
            || !SteamDown->second.second->IsPlaying())) 
            SteamDown->second.first.SetPosition(pos.x, pos.y + m_size.y, pos.z);

    auto ShockWaveRight = m_currentAnims.find("ShockWaveRight");
    if (ShockWaveRight != m_currentAnims.end())
        ShockWaveRight->second.first.SetPosition(pos);

    auto ShockWaveLeft = m_currentAnims.find("ShockWaveLeft");
    if (ShockWaveLeft != m_currentAnims.end())
        ShockWaveLeft->second.first.SetPosition(pos);

    auto ShockWaveUp = m_currentAnims.find("ShockWaveUp");
    if (ShockWaveUp != m_currentAnims.end())
        ShockWaveUp->second.first.SetPosition(pos);

    auto ShockWaveDown = m_currentAnims.find("ShockWaveDown");
    if (ShockWaveDown != m_currentAnims.end())
        ShockWaveDown->second.first.SetPosition(pos);

    auto Star = m_currentAnims.find("Star");
    if (Star != m_currentAnims.end())
        Star->second.first.SetPosition(pos.x, pos.y + m_size.y / 2, pos.z);

	auto Flower = m_currentAnims.find("Flower");
	if (Flower != m_currentAnims.end())
		Flower->second.first.SetPosition(pos.x, pos.y, pos.z);

    auto Steam = m_currentAnims.find("Steam");
    if (Steam != m_currentAnims.end())
        Steam->second.first.SetPosition(pos.x + static_cast<float>(offset / 2) * 3, pos.y, pos.z);
}

void SpriteSheet::SetSpriteScale(const std::string & part, float scale)
{
    auto animation = m_currentAnims.find(part);
    if (animation == m_currentAnims.end())
        return;

    animation->second.first.SetScale(Vector2f(scale, scale));
}

bool SpriteSheet::IsPlaying(const std::string & part)
{ 
	auto animation = m_currentAnims.find(part);
	if (animation == m_currentAnims.end()) return false;

	return animation->second.second->IsPlaying();
	;
}

void SpriteSheet::SetDepth(float depth_)
{
	depth = depth_;
	for (auto & itr : m_currentAnims)
		itr.second.first.SetDepth(depth_);
}

bool SpriteSheet::HasState(const std::string & state_name)
{
    auto itr = m_animations.find("Body");
    if (itr == m_animations.end()) return false;
    // Animation part exist

    auto itr2 = itr->second.find(state_name);
    if (itr2 == itr->second.end()) return false;

    return true;
}

void SpriteSheet::Update(float dt)
{
	for (auto & itr : m_currentAnims)
		itr.second.second->Update(dt);
}

void SpriteSheet::Draw(Window * wind, bool flicker)
{
	for (auto & itr : m_currentAnims)
	{
        if (itr.first == "Body" && flicker) continue;
		if (itr.second.second->IsPlaying())
			wind->GetBaseWindow()->DrawAnim(itr.second.first, GetMaxRowAndCol(), depth, itr.second.second);
	}
}

#ifdef _DEBUG
void SpriteSheet::DebugDraw(Window * wind, const C_Collidable * collision)
{
	for (auto & itr : m_currentAnims)
		if(itr.first == "Body") // HARDCODE
			wind->GetBaseWindow()->DebugDraw(itr.second.first, collision);
}
void SpriteSheet::DebugDraw(Window * wind, const C_Attackable * attack)
{
	for (auto & itr : m_currentAnims)
		if(itr.first == "Body") // HARDCODE
			wind->GetBaseWindow()->DebugDraw(itr.second.first, attack);
}
#endif

void SpriteSheet::ReinitMeshes()
{
	for (auto sprite : m_currentAnims)
	{
		sprite.second.first.GetMesh()->Update_Buffers();
	}
}

void SpriteSheet::SetShader(ShaderContainer & shader_container_, std::string shader_name_) 
{
	m_shader = shader_container_.GetShader(shader_name_);
	for (auto& itr : m_currentAnims)
	{
		Sprite* targetSprite = &(itr.second.first);
		targetSprite->SetShader(shader_container_.GetShader(shader_name_));
	}
}

bool SpriteSheet::IsDeadAnimationEnd(unsigned int /*entity*/)
{
    auto dead_animation = m_currentAnims.find("Body");
    if (dead_animation == m_currentAnims.end()) return false;

	bool playing = dead_animation->second.second->IsPlaying();
	return !playing;
}



void SpriteSheet::LoadSheet(const std::string & file)
{
	std::ifstream inFile("Media/SpriteSheet/" + file + ".sheet");
	if (!inFile.is_open())
	{
		std::cout << "Cannot open the file : Media/SpriteSheet/" << file << ".sheet" << std::endl;
		return;
	}

	std::string line;
	std::string part;
	while (std::getline(inFile, line))
	{
		if (line[0] == '|') continue;

		std::stringstream keystream(line);
		std::string type;
		keystream >> type;

		if (type == "Size")
			keystream >> m_size.x >> m_size.y;
		else if (type == "Scale")
			keystream >> m_scale.x >> m_scale.y;
		else if (type == "Part")
		{
			keystream >> part;
			Texture * texture = m_texMgr->GetResource(file);
			texture->SetSize(m_size);
			Sprite tempSprite;
			tempSprite.SetScale(m_scale.x, m_scale.y);
            if(part == "ShockWaveRight" || part == "ShockWaveLeft" || 
                part == "ShockWaveUp" || part == "ShockWaveDown")
                tempSprite.SetScale(2.5f, 2.5f);
            else if(part == "Star")
                tempSprite.SetScale(.8f, .8f);
            else if(part != "Body")
                tempSprite.SetScale(1.5f, 1.5f);

			tempSprite.SetTexture(*m_texMgr->GetResource(file));
			tempSprite.BuildMesh(texture);

			m_currentAnims.emplace(part, std::make_pair(tempSprite, nullptr));
		}
		else if (type == "Animation")
		{
			std::string name;
			keystream >> name;

			Animation * tempAnim = new Animation(this, part);
			keystream >> *tempAnim;

			if (!m_animations[part].emplace(name, tempAnim).second)
			{
				delete tempAnim;
				return;
			}
		}
	}
	inFile.close();
}

void SpriteSheet::Purge()
{
	for (auto & itr : m_animations)
	{
		for (auto & itr2 : itr.second)
			delete itr2.second;
	}

	for (auto & sprite : m_currentAnims)
		delete sprite.second.first.GetMesh();

	m_animations.clear();
	m_currentAnims.clear();
}

const Vector2u SpriteSheet::GetMaxRowAndCol()
{
	unsigned int x = 0;
	unsigned int y = 0;

	for (auto & part : m_animations)
	{
		for (auto & anim : part.second)
		{
			unsigned int end_frame = anim.second->GetEndFrame();
			if (x < end_frame)
				x = end_frame;
			++y;
		}
	}
	return Vector2u(x + 1, y * 2);
}