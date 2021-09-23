#include "CsvHelper.h"

#include <limits.h>

#include "../Share/StdUtils.hpp"
#include "../Share/StrUtil.hpp"

CsvReader::CsvReader(const char* item_splitter /* = "," */)
	: _item_splitter(item_splitter)
{

}

bool CsvReader::load_from_file(const char* filename)
{
	if (!StdFile::exists(filename))
		return false;

	_ifs.open(filename);

	_ifs.getline(_buffer, 1024);
	//判断是不是UTF-8BOM 编码
	static char flag[] = { (char)0xEF, (char)0xBB, (char)0xBF };
	char* buf = _buffer;
	if (memcmp(_buffer, flag, sizeof(char) * 3) == 0)
		buf += 3;

	std::string row = buf;

	//替换掉一些字段的特殊符号
	StrUtil::replace(row, "<", "");
	StrUtil::replace(row, ">", "");
	StrUtil::replace(row, "\"", "");
	StrUtil::replace(row, "'", "");

	//将字段名转成小写
	StrUtil::toLowerCase(row);

	StringVector fields = StrUtil::split(row, _item_splitter.c_str());
	for (uint32_t i = 0; i < fields.size(); i++)
	{
		std::string field = StrUtil::trim(fields[i].c_str(), " ");
		if (field.empty())
			break;

		_fields_map[field] = i;
	}

	return true;
}

bool CsvReader::next_row()
{
	if (_ifs.eof())
		return false;

	while (!_ifs.eof())
	{
		_ifs.getline(_buffer, 1024);
		if(strlen(_buffer) == 0)
			continue;
		else
			break;
	} 
	
	if (strlen(_buffer) == 0)
		return false;
	_current_cells.clear();
	StrUtil::split(_buffer, _current_cells, _item_splitter.c_str());
	return true;
}

int32_t CsvReader::get_int32(int32_t col)
{
	if (!check_cell(col))
		return 0;

	return strtol(_current_cells[col].c_str(), NULL, 10);
}

uint32_t CsvReader::get_uint32(int32_t col)
{
	if (!check_cell(col))
		return 0;

	return strtoul(_current_cells[col].c_str(), NULL, 10);
}

int64_t CsvReader::get_int64(int32_t col)
{
	if (!check_cell(col))
		return 0;

	return strtoll(_current_cells[col].c_str(), NULL, 10);
}

uint64_t CsvReader::get_uint64(int32_t col)
{
	if (!check_cell(col))
		return 0;

	return strtoull(_current_cells[col].c_str(), NULL, 10);
}

double CsvReader::get_double(int32_t col)
{
	if (!check_cell(col))
		return 0;

	return strtod(_current_cells[col].c_str(), NULL);
}

const char* CsvReader::get_string(int32_t col)
{
	if (!check_cell(col))
		return "";

	return _current_cells[col].c_str();
}

int32_t CsvReader::get_int32(const char* field)
{
	int32_t col = get_col_by_filed(field);
	return get_int32(col);
}

uint32_t CsvReader::get_uint32(const char* field)
{
	int32_t col = get_col_by_filed(field);
	return get_uint32(col);
}

int64_t CsvReader::get_int64(const char* field)
{
	int32_t col = get_col_by_filed(field);
	return get_int64(col);
}

uint64_t CsvReader::get_uint64(const char* field)
{
	int32_t col = get_col_by_filed(field);
	return get_uint64(col);
}

double CsvReader::get_double(const char* field)
{
	int32_t col = get_col_by_filed(field);
	return get_double(col);
}

const char* CsvReader::get_string(const char* field)
{
	int32_t col = get_col_by_filed(field);
	return get_string(col);
}

bool CsvReader::check_cell(int32_t col)
{
	if (col == INT_MAX )
		return false;

	if (col < 0 || col >= (int32_t)_fields_map.size())
		return false;

	return true;
}

int32_t CsvReader::get_col_by_filed(const char* field)
{
	auto it = _fields_map.find(field);
	if (it == _fields_map.end())
		return INT_MAX;

	return it->second;
}