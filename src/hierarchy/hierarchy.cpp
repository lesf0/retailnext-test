#include "hierarchy.h"

bool hierarchy::Hierarchy::addNode(const std::string& id, const std::string& name, const std::string& parent_id)
{
	if (id.empty() || name.empty())
	{
		return false;
	}

	if (this->nodes.count(id))
	{
		return false;
	}

	if ((!this->root) ^ parent_id.empty())
	{
		return false;
	}

	if (!parent_id.empty() && (!this->nodes.count(parent_id) || this->nodes.at(parent_id).hasChild(name)))
	{
		return false;
	}

	auto res = this->nodes.emplace(id, hierarchy::Node{id, name});
	Node* tmp = &res.first->second;

	if (parent_id.empty())
	{
		this->root = tmp;
	}
	else
	{
		tmp->setParent(&this->nodes.at(parent_id));
	}

	return true;
}


bool hierarchy::Hierarchy::delNode(const std::string& id)
{
	if (id.empty())
	{
		return false;
	}

	if (!this->nodes.count(id) || this->nodes.at(id).hasChildren())
	{
		return false;
	}

	this->nodes.at(id).setParent(nullptr);
	this->nodes.erase(id);

	if (this->nodes.empty())
	{
		this->root = nullptr;
	}

	return true;
}


bool hierarchy::Hierarchy::moveNode(const std::string& id, const std::string& new_parent_id)
{
	if (id.empty() || new_parent_id.empty())
	{
		return false;
	}

	if (!this->nodes.count(id) || !this->nodes.count(new_parent_id))
	{
		return false;
	}

	auto& new_parent = this->nodes.at(new_parent_id);
	auto& tmp = this->nodes.at(id);

	if (new_parent.belongsToSubtree(&tmp))
	{
		return false;
	}

	tmp.setParent(&new_parent);

	return true;
}


std::vector<hierarchy::Node const*> hierarchy::Hierarchy::query(
	int min_depth,
	int max_depth,
	const std::unordered_set<std::string>& names,
	const std::unordered_set<std::string>& ids,
	const std::vector<std::string>& root_ids
) const
{
	std::vector<hierarchy::Node const*> domain;

	if(root_ids.empty())
	{
		if (this->root)
		{
			domain.push_back(this->root);
		}
	}
	else
	{
		for (auto& root_id:root_ids)
		{
			if (this->nodes.count(root_id))
			{
				domain.push_back(&this->nodes.at(root_id));
			}
		}
	}

	std::vector<hierarchy::Node const*> result;

	for (auto root:domain)
	{
		this->query_impl(result, root, min_depth, max_depth, names, ids);
	}

	return result;
}

void hierarchy::Hierarchy::query_impl(
	std::vector<hierarchy::Node const*>& result,
	const hierarchy::Node* root,
	int min_depth,
	int max_depth,
	const std::unordered_set<std::string>& names,
	const std::unordered_set<std::string>& ids
) const
{
	if (max_depth < 0)
	{
		return;
	}

	if (min_depth <= 0)
	{
		if ((names.empty()	|| names.count(root->getName())) &&
			(ids.empty()	|| ids.count(root->getId())))
		{
			result.push_back(root);
		}
	}

	for (auto& child:*root)
	{
		this->query_impl(result, &child, min_depth-1, max_depth-1, names, ids);
	}
}