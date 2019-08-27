#include <algorithm>
#include <iostream>
#include <exception>
#include <climits>

#include "json.hpp"

#include "hierarchy/hierarchy.h"

char* getCmdOption(char ** begin, char ** end, const std::string & option)
{
	char ** itr = std::find(begin, end, option);
	if (itr != end && ++itr != end)
	{
		return *itr;
	}
	return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
	return std::find(begin, end, option) != end;
}

void printJSON(const nlohmann::json& data, bool pretty = false)
{
	if (pretty)
	{
		std::cout << data.dump(4) << std::endl;
	}
	else
	{
		std::cout << data << std::endl;
	}
}

int main(int argc, char** argv)
{
	if (cmdOptionExists(argv, argv + argc, "-h") || cmdOptionExists(argv, argv + argc, "--help"))
	{
		std::cout <<
			"RetailNext test task done by Denis Sheremet\n"
			"\n"
			"options:\n"
			"-h | --help\t- this message\n"
			"-p | --pretty\t- pretty-print the output\n";

		return 0;
	}

	bool pretty = cmdOptionExists(argv, argv + argc, "-p") || cmdOptionExists(argv, argv + argc, "--pretty");

	hierarchy::Hierarchy h;

	std::string buf;
	nlohmann::json request, response;
	while(!std::cin.eof())
	{
		std::getline(std::cin, buf);

		try {
			request = nlohmann::json::parse(buf);
		}
		catch (const std::exception& e) {
			//	JSON error

			printJSON(R"({"error":"invalid input"})"_json, pretty);
			continue;
		}

		response = nullptr;

		if (request.count("add_node"))
		{
			auto& data = request.at("add_node");

			if (!data.count("id") || !data.count("name"))
			{
				printJSON(R"({"error":"missing data"})"_json, pretty);
				continue;
			}

			const std::string& id = data.at("id").get_ref<const std::string&>();
			const std::string& name = data.at("name").get_ref<const std::string&>();

			std::string parent_id = "";

			if (data.count("parent_id"))
			{
				parent_id = data.at("parent_id");
			}

			response = {{"ok", h.addNode(id, name, parent_id)}};
		}
		else
		if (request.count("delete_node"))
		{
			auto& data = request.at("delete_node");

			if (!data.count("id"))
			{
				printJSON(R"({"error":"missing data"})"_json, pretty);
				continue;
			}

			const std::string& id = data.at("id").get_ref<const std::string&>();

			response = {{"ok", h.delNode(id)}};
		}
		else
		if (request.count("move_node"))
		{
			auto& data = request.at("move_node");

			if (!data.count("id") || !data.count("new_parent_id"))
			{
				printJSON(R"({"error":"missing data"})"_json, pretty);
				continue;
			}

			const std::string& id = data.at("id").get_ref<const std::string&>();
			const std::string& new_parent_id = data.at("new_parent_id").get_ref<const std::string&>();

			response = {{"ok", h.moveNode(id, new_parent_id)}};
		}
		else
		if (request.count("query"))
		{
			auto& data = request.at("query");

			int min_depth = data.count("min_depth") ? data.at("min_depth").get<int>() : 0;
			int max_depth = data.count("max_depth") ? data.at("max_depth").get<int>() : INT_MAX;

			std::unordered_set<std::string> names;
			if (data.count("names"))
			{
				auto& raw = data.at("names");

				names.insert(raw.begin(), raw.end());
			}

			std::unordered_set<std::string> ids;
			if (data.count("ids"))
			{
				auto& raw = data.at("ids");

				ids.insert(raw.begin(), raw.end());
			}

			std::vector<std::string> root_ids;
			if (data.count("root_ids"))
			{
				auto& raw = data.at("root_ids");

				root_ids = raw.get<const std::vector<std::string>>();
			}

			response = {{"nodes", h.query(
				min_depth,
				max_depth,
				names,
				ids,
				root_ids
			)}};
		}
		else
		{
			response = R"({"error":"no valid action specified"})"_json;
		}

		printJSON(response, pretty);
	}
}