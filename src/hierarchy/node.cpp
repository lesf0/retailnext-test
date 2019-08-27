#include "node.h"

#include <cassert>


// Node iterator
bool hierarchy::Node::Iterator::operator!=(const hierarchy::Node::Iterator& other) const
{
	return this->iter != other.iter;
}


hierarchy::Node::Iterator& hierarchy::Node::Iterator::operator++()
{
	this->iter++;

	return *this;
}


const hierarchy::Node& hierarchy::Node::Iterator::operator*() const
{
	return *this->iter->second;
}


//Node
void hierarchy::Node::setParent(hierarchy::Node* new_parent)
{
	if (this->parent)
	{
		this->parent->children.erase(this->name);
	}

	if (new_parent) {
		assert(!new_parent->children.count(this->name));

		new_parent->children.emplace(this->name, this);
	}

	this->parent = new_parent;
}


bool hierarchy::Node::belongsToSubtree(const Node* root) const
{
	for(const Node* it = this; it; it = it->parent)
	{
		if (it == root)
		{
			return true;
		}
	}

	return false;
}


bool hierarchy::Node::hasChild(const std::string& name) const
{
	return this->children.count(name);
}


bool hierarchy::Node::hasChildren() const
{
	return !this->children.empty();
}


hierarchy::Node::Iterator hierarchy::Node::begin() const
{
	return this->children.begin();
}


hierarchy::Node::Iterator hierarchy::Node::end() const
{
	return this->children.end();
}


const std::string& hierarchy::Node::getId() const
{
	return this->id;
}


const std::string& hierarchy::Node::getName() const
{
	return this->name;
}


// Node -> json conversion
void hierarchy::to_json(nlohmann::json& j, const Node* p) {
	j = {
		{"id", p->id},
		{"name", p->name},
		{"parent_id", ""}
	};

	if (p->parent)
	{
		j["parent_id"] = p->parent->id;
	}
}