#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
/******************************************************************************/
/*!
\headerfile   ResourceManager.h
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14
\brief
		Header file for resource manager class.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <algorithm>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

using PathContainer = std::unordered_map<std::string, std::string>;

template <class T, class Derived>
class ResourceManager
{
public:
	ResourceManager(const std::string & path) { LoadPath(path); }

	~ResourceManager()
	{
		PurgePath();
		PurgeResource();
	}

	T * GetResource(const std::string & name)
	{
		auto itr = m_resource.find(name);
		return (itr != m_resource.end() ? itr->second.second : nullptr);
	}

	const std::string GetPath(const std::string & name) const
	{
		auto itr = m_path.find(name);
		return (itr != m_path.end() ? itr->second : "");
	}

	bool RequireResource(const std::string & name)
	{
		auto itr = m_resource.find(name);
		if (itr != m_resource.end())
		{
			++itr->second.first;
			return true;
		}

		T * resource = Load(GetPath(name));
		if (resource)
		{
			m_resource.emplace(name, std::make_pair(1, resource));
			return true;
		}
		return false;
	}

	bool ReleaseResource(const std::string & name)
	{
		auto itr = m_resource.find(name);
		if (itr == m_resource.end()) return false;

		if (itr->second.first > 1)
		{
			--itr->second.first;
			return true;
		}
		else
		{
			--itr->second.first;
			delete itr->second.second;
			itr->second.second = nullptr;
			m_resource.erase(itr);
			return true;
		}
	}

	T * Load(const std::string & name) { return static_cast<Derived *>(this)->Load(name); }

	const std::unordered_map<std::string, std::string> & GetAllPath() const { return m_path; }

protected:
	void LoadPath(const std::string & file)
	{
		fs::path dir = file;
		for (const auto & texture_file : fs::directory_iterator(dir))
			m_path.emplace(texture_file.path().stem().string(), texture_file.path().string());
	}

	void PurgePath() { m_path.clear(); }

	void PurgeResource()
	{
		for (auto & itr : m_resource)
			delete itr.second.second;
		m_resource.clear();
	}

	PathContainer m_path;
	std::unordered_map<std::string, std::pair<unsigned int, T *>> m_resource;
};

#endif