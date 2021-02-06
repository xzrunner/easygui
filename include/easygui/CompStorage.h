#pragma once

#include <unirender/noncopyable.h>

#include <unordered_map>

namespace egui
{

class CompStorage : ur::noncopyable
{
public:
	void Add(ID_TYPE id, void* data) {
		m_entities.insert({ id, data });
	}
	const void* Get(ID_TYPE id) const {
		auto itr = m_entities.find(id);
		return itr == m_entities.end() ? nullptr : itr->second;
	}
	void Clear() {
		m_entities.clear();
	}

private:
	std::unordered_map<ID_TYPE, void*> m_entities;

}; // CompStorage

}