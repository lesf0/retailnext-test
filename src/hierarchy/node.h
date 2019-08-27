#ifndef RETAILNEXT_TEST_HIERARCHY_NODE
#define RETAILNEXT_TEST_HIERARCHY_NODE

#include "../json.hpp"

#include <map>
#include <string>

namespace hierarchy {
	class Node {
		std::string id;
		std::string name;

		Node* parent;
		std::map<std::string, Node*> children;

		class Iterator {
			typedef std::map<std::string, Node*>::const_iterator childrenIterator;
			childrenIterator iter;

		public:
			Iterator(const childrenIterator& iter) : iter(iter)
			{}

			bool operator!=(const Iterator& other) const;
			Iterator& operator++();
			const Node& operator*() const;
		};

	public:
		Node(const std::string& id, const std::string& name) : id(id), name(name), parent(nullptr)
		{}

		void setParent(Node* new_parent);

		bool belongsToSubtree(const Node* root) const;
		bool hasChild(const std::string& name) const;
		bool hasChildren() const;

		Iterator begin() const;
		Iterator end() const;

		const std::string& getId() const;
		const std::string& getName() const;

		friend void to_json(nlohmann::json& j, const Node* p);
	};

	void to_json(nlohmann::json& j, const Node* p);
}

#endif