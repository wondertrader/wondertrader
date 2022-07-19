/*!
 * \file WtHelper.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include <string>
#include <stdint.h>

class WtHelper
{
public:
	static std::string getCWD();

	static std::string getModulePath(const char* moduleName, const char* subDir, bool isCWD = true);

	static const char* getBaseDir();
	static const char* getOutputDir();
	static const char* getStraDataDir();
	static const char* getStraUsrDatDir();

	static const char* getPortifolioDir();

	static const char* getExecDataDir();

	static inline void setTime(uint32_t date, uint32_t time, uint32_t secs = 0)
	{
		_cur_date = date;
		_cur_time = time;
		_cur_secs = secs;
	}

	static inline void setTDate(uint32_t tDate){ _cur_tdate = tDate; }

	static inline uint32_t getDate(){ return _cur_date; }
	static inline uint32_t getTime(){ return _cur_time; }
	static inline uint32_t getSecs(){ return _cur_secs; }
	static inline uint32_t getTradingDate(){ return _cur_tdate; }

	static const std::string& getInstDir() { return _inst_dir; }
	static void setInstDir(const char* inst_dir){ _inst_dir = inst_dir; }

	static void setGenerateDir(const char* gen_dir) { _gen_dir = gen_dir; }

private:
	static uint32_t		_cur_date;	//��ǰ����
	static uint32_t		_cur_time;	//��ǰʱ��, �Է���Ϊ׼
	static uint32_t		_cur_secs;	//��ǰ����, ��������
	static uint32_t		_cur_tdate;	//��ǰ������private:
	static std::string	_inst_dir;	//ʵ������Ŀ¼
	static std::string	_gen_dir;	//�����ļ����Ŀ¼
};

