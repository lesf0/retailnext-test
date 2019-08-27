#ifndef RETAILNEXT_TEST_HIERARCHY_HIERARCHY
#define RETAILNEXT_TEST_HIERARCHY_HIERARCHY

#include "node.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>

namespace hierarchy {
	class Hierarchy {
		std::unordered_map<std::string, Node> nodes;
		Node* root;

		void query_impl(
			std::vector<hierarchy::Node const*>& result,
			const hierarchy::Node* root,
			int min_depth,
			int max_depth,
			const std::unordered_set<std::string>& names,
			const std::unordered_set<std::string>& ids
		) const;
	public:
		Hierarchy() : root(nullptr)
		{}

		bool addNode(const std::string& id, const std::string& name, const std::string& parent_id);
		bool delNode(const std::string& id);
		bool moveNode(const std::string& id, const std::string& new_parent_id);
		std::vector<Node const*> query(
			int min_depth,
			int max_depth,
			const std::unordered_set<std::string>& names,
			const std::unordered_set<std::string>& ids,
			const std::vector<std::string>& root_ids
		) const;
	};
}

#endif