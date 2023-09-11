#pragma once
#include <string.h>
#include <string>
#include <unordered_map>
#include <stdint.h>
#include <fstream>
#include <vector>
#include <sstream>

class CsvReader
{
public:
	CsvReader(const char* item_splitter = ",");

public:
	bool	load_from_file(const char* filename);

public:
	inline uint32_t	col_count() { return (uint32_t)_fields_map.size(); }

	int32_t		get_int32(int32_t col);
	uint32_t	get_uint32(int32_t col);

	int64_t		get_int64(int32_t col);
	uint64_t	get_uint64(int32_t col);

	double		get_double(int32_t col);

	const char*	get_string(int32_t col);

	int32_t		get_int32(const char* field);
	uint32_t	get_uint32(const char* field);

	int64_t		get_int64(const char* field);
	uint64_t	get_uint64(const char* field);

	double		get_double(const char* field);

	const char*	get_string(const char* field);

	bool		next_row();

	const char* fields() const 
	{ 
		static std::string s;
		if(s.empty())
		{
			std::stringstream ss;
			for (auto item : _fields_map)
				ss << item.first << ",";

			s = ss.str();
			s = s.substr(0, s.size() - 1);
		}

		return s.c_str();
	}

private:
	bool		check_cell(int32_t col);
	int32_t		get_col_by_filed(const char* field);

private:
	std::ifstream	_ifs;
	char			_buffer[1024];
	std::string		_item_splitter;

	std::unordered_map<std::string, int32_t> _fields_map;
	std::vector<std::string> _current_cells;
};
