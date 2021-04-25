#pragma once
#include <ostream>
#include <unordered_map>
#include <string>
namespace lakvik
{
	template<typename T>
	void print(std::ostream& stream, const T& value)
	{
		stream << value;
	}

	void print(std::ostream& stream, const std::string& value);


	template<typename T, typename Hash, typename KeyEqual, typename Allocator>
	void print(std::ostream& stream, const std::unordered_map<std::string, T, Hash, KeyEqual, Allocator>& values)
	{
		stream << '{';
		auto first = true;
		for (const auto& value : values)
		{
			if (first)
			{
				first = false;
				print(stream, value.first);
				stream << ':';
				print(stream, value.second);
				continue;
			}
			stream << ',';
			print(stream, value.first);
			stream << ':';
			print(stream, value.second);
		}
		stream << '}';
	}
}
