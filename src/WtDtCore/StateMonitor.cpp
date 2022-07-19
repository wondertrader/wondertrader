/*!
 * \file StateMonitor.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "StateMonitor.h"
#include "DataManager.h"

#include "../Share/TimeUtils.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSSessionInfo.hpp"
#include "../Includes/WTSVariant.hpp"

#include "../WTSTools/WTSBaseDataMgr.h"
#include "../WTSTools/WTSLogger.h"
#include "../WTSUtils/WTSCfgLoader.h"


StateMonitor::StateMonitor()
	: _stopped(false)
	, _bd_mgr(NULL)
	, _dt_mgr(NULL)
{
}


StateMonitor::~StateMonitor()
{
}

bool StateMonitor::initialize(const char* filename, WTSBaseDataMgr* bdMgr, DataManager* dtMgr)
{
	_bd_mgr = bdMgr;
	_dt_mgr = dtMgr;

	if (!StdFile::exists(filename))
	{
		WTSLogger::error("State config file {} not exists", filename);
		return false;
	}

	WTSVariant* config = WTSCfgLoader::load_from_file(filename, true);
	if (config == NULL)
	{
		WTSLogger::error("Loading state config failed");
		return false;
	}

	auto keys = config->memberNames();
	for (const std::string& sid : keys)
	{
		WTSVariant* jItem = config->get(sid.c_str());

		WTSSessionInfo* ssInfo = _bd_mgr->getSession(sid.c_str());
		if (ssInfo == NULL)
		{
			WTSLogger::error("Trading session template [{}] not exists,state control rule skipped", sid);
			continue;
		}

		StatePtr sInfo(new StateInfo);
		sInfo->_init_time = jItem->getUInt32("inittime");	//��ʼ��ʱ��,��ʼ���Ժ����ݲſ�ʼ����
		sInfo->_close_time = jItem->getUInt32("closetime");	//����ʱ��,���̺����ݲ��ٽ�����
		sInfo->_proc_time = jItem->getUInt32("proctime");	//�̺���ʱ��,��Ҫ��ʵʱ����ת����ʷȥ

		strcpy(sInfo->_session, sid.c_str());

		auto secInfo = ssInfo->getAuctionSection();//��������ƫ�ƹ���ʱ��,Ҫע����!!!
		if (secInfo.first != 0 || secInfo.second != 0)
		{
			uint32_t stime = secInfo.first;
			uint32_t etime = secInfo.second;

			stime = stime / 100 * 60 + stime % 100;//�Ƚ�ʱ��ת�ɷ�����
			etime = etime / 100 * 60 + etime % 100;

			stime = stime / 60 * 100 + stime % 60;//�ٽ�������ת��ʱ��
			etime = etime / 60 * 100 + etime % 60;//�Ȳ����ǰ�ҹ12������,Ŀǰ����,����û��
			sInfo->_sections.emplace_back(StateInfo::Section({ stime, etime }));
		}

		auto sections = ssInfo->getTradingSections();//��������ƫ�ƹ���ʱ��,Ҫע����!!!
		for (auto it = sections.begin(); it != sections.end(); it++)
		{
			auto secInfo = *it;
			uint32_t stime = secInfo.first;
			uint32_t etime = secInfo.second;

			stime = stime / 100 * 60 + stime % 100;//�Ƚ�ʱ��ת�ɷ�����
			etime = etime / 100 * 60 + etime % 100;

			stime--;//��ʼ������-1
			etime++;//����������+1

			stime = stime / 60 * 100 + stime % 60;//�ٽ�������ת��ʱ��
			etime = etime / 60 * 100 + etime % 60;//�Ȳ����ǰ�ҹ12������,Ŀǰ����,����û��
			sInfo->_sections.emplace_back(StateInfo::Section({ stime, etime }));
		}

		_map[sInfo->_session] = sInfo;

		CodeSet* pCommSet =  _bd_mgr->getSessionComms(sInfo->_session);
		if (pCommSet)
		{
			uint32_t curDate = TimeUtils::getCurDate();
			uint32_t curMin = TimeUtils::getCurMin() / 100;
			uint32_t offDate = ssInfo->getOffsetDate(curDate, curMin);
			uint32_t offMin = ssInfo->offsetTime(curMin, true);

			//�Ȼ�ȡ��׼�Ľ�����

			for (auto it = pCommSet->begin(); it != pCommSet->end(); it++)
			{
				const char* pid = (*it).c_str();

				 _bd_mgr->setTradingDate(pid,  _bd_mgr->getTradingDate(pid, offDate, offMin, false), false);
				uint32_t prevDate = TimeUtils::getNextDate(curDate, -1);
				if ((ssInfo->getOffsetMins() > 0 &&
					(! _bd_mgr->isTradingDate(pid, curDate) &&
					!(ssInfo->isInTradingTime(curMin) &&  _bd_mgr->isTradingDate(pid, prevDate)))) ||
					(ssInfo->getOffsetMins() <= 0 && ! _bd_mgr->isTradingDate(pid, offDate))
					)
				{
					WTSLogger::info("Instrument {} is in holiday", pid);
				}
			}
		}
	}
	return true;
}

void StateMonitor::run()
{
	if(_thrd == NULL)
	{
		_thrd.reset(new StdThread([this](){

			while (!_stopped)
			{
				static uint64_t lastTime = 0;

				while(true)
				{
					uint64_t now = TimeUtils::getLocalTimeNow();
					if(now - lastTime >= 1000)
						break;

					if(_stopped)
						break;

					std::this_thread::sleep_for(std::chrono::milliseconds(2));
				}

				if(_stopped)
					break;

				uint32_t curDate = TimeUtils::getCurDate();
				uint32_t curMin = TimeUtils::getCurMin() / 100;

				auto it = _map.begin();
				for (; it != _map.end(); it++)
				{
					StatePtr& sInfo = (StatePtr&)it->second;
					WTSSessionInfo* mInfo =  _bd_mgr->getSession(sInfo->_session);

					uint32_t offDate = mInfo->getOffsetDate(curDate, curMin);
					uint32_t prevDate = TimeUtils::getNextDate(curDate, -1);

					switch(sInfo->_state)
					{
					case SS_ORIGINAL:
						{
							uint32_t offTime = mInfo->offsetTime(curMin, true);
							uint32_t offInitTime = mInfo->offsetTime(sInfo->_init_time, true);
							uint32_t offCloseTime = mInfo->offsetTime(sInfo->_close_time, false);
							uint32_t aucStartTime = mInfo->getAuctionStartTime(true);

							bool isAllHoliday = true;
							std::stringstream ss_a, ss_b;
							CodeSet* pCommSet =  _bd_mgr->getSessionComms(sInfo->_session);
							if (pCommSet)
							{
								for (auto it = pCommSet->begin(); it != pCommSet->end(); it++)
								{
									const char* pid = (*it).c_str();
									/*
									 *	���ʱ������ƫ��
									 *	�����ǰ���ڲ��ǽ�����,�Ҳ�����ҹ�̺��ҹ������ʱ���������ǽ����գ�
									 *	����ʱ������ƫ�ƵĻ�,�Ϳ�ƫ�������Ƿ��ǽڼ���
									 */
									if ((mInfo->getOffsetMins() > 0 &&
										(! _bd_mgr->isTradingDate(pid, curDate) &&	//��ǰ��־���ǽ�����
										!(mInfo->isInTradingTime(curMin) &&  _bd_mgr->isTradingDate(pid, prevDate)))) ||	//��ǰ���ڽ���ʱ��,�������ǽ�����
										(mInfo->getOffsetMins() <= 0 && ! _bd_mgr->isTradingDate(pid, offDate))
										)
									{
										ss_a << pid << ",";
										WTSLogger::info("Instrument {} is in holiday", pid);
									}
									else
									{
										ss_b << pid << ",";
										isAllHoliday = false;
									}
								}

							}
							else
							{
								WTSLogger::info("No corresponding instrument of trading session {}[{}], changed into holiday state", mInfo->name(), sInfo->_session);
								sInfo->_state = SS_Holiday;
							}

							if(isAllHoliday)
							{
								WTSLogger::info("All instruments of trading session {}[{}] are in holiday, changed into holiday state", mInfo->name(), sInfo->_session);
								sInfo->_state = SS_Holiday;
							}
							else if (offTime >= offCloseTime)
							{
								sInfo->_state = SS_CLOSED;
								WTSLogger::info("Trading session {}[{}] stopped receiving data", mInfo->name(), sInfo->_session);
							}
							else if (aucStartTime != -1 && offTime >= aucStartTime)
							{
								if (sInfo->isInSections(offTime))
								{
									//if(sInfo->_schedule)
									//{
									//	_dt_mgr->preloadRtCaches();
									//}
									sInfo->_state = SS_RECEIVING;
									WTSLogger::info("Trading session {}[{}] started receiving data", mInfo->name(), sInfo->_session);
								}
								else
								{
									//С���г�����ʱ��,�Ҳ��ڽ���ʱ��,��Ϊ��;����ʱ��
									if(offTime < mInfo->getCloseTime(true))
									{
										sInfo->_state = SS_PAUSED;
										WTSLogger::info("Trading session {}[{}] paused receiving data", mInfo->name(), sInfo->_session);
									}
									else
									{//�����г�����ʱ��,����û�д��ڽ�������ʱ��,��Ҫ��������,��Ҫ��Ҫ�ս����
										sInfo->_state = SS_RECEIVING;
										WTSLogger::info("Trading session {}[{}] started receiving data", mInfo->name(), sInfo->_session);
									}
									
								}
							}								
							else if (offTime >= offInitTime)
							{
								sInfo->_state = SS_INITIALIZED;
								WTSLogger::info("Trading session {}[{}] initialized", mInfo->name(), sInfo->_session);
							}

							
						}
						break;
					case SS_INITIALIZED:
						{
							uint32_t offTime = mInfo->offsetTime(curMin, true);
							uint32_t offAucSTime = mInfo->getAuctionStartTime(true);
							if (offAucSTime == -1 || offTime >= mInfo->getAuctionStartTime(true))
							{
								if (!sInfo->isInSections(offTime) && offTime < mInfo->getCloseTime(true))
								{
									//if (sInfo->_schedule)
									//{
									//	_dt_mgr->preloadRtCaches();
									//}
									sInfo->_state = SS_PAUSED;

									WTSLogger::info("Trading session {}[{}] paused receiving data", mInfo->name(), sInfo->_session);
								}
								else
								{
									//if (sInfo->_schedule)
									//{
									//	_dt_mgr->preloadRtCaches();
									//}
									sInfo->_state = SS_RECEIVING;
									WTSLogger::info("Trading session {}[{}] started receiving data", mInfo->name(), sInfo->_session);
								}
								
							}
						}
						break;
					case SS_RECEIVING:
						{
							uint32_t offTime = mInfo->offsetTime(curMin, true);
							uint32_t offCloseTime = mInfo->offsetTime(sInfo->_close_time, false);
							if (offTime >= offCloseTime)
							{
								sInfo->_state = SS_CLOSED;

								WTSLogger::info("Trading session {}[{}] stopped receiving data", mInfo->name(), sInfo->_session);
							}
							else if (offTime >= mInfo->getAuctionStartTime(true))
							{
								if (offTime < mInfo->getCloseTime(true))
								{
									if (!sInfo->isInSections(offTime))
									{
										//if (sInfo->_schedule)
										//{
										//	_dt_mgr->preloadRtCaches();
										//}
										sInfo->_state = SS_PAUSED;

										WTSLogger::info("Trading session {}[{}] paused receiving data", mInfo->name(), sInfo->_session);
									}
								}
								else
								{
									//��������������Ժ��ʱ��
									//���ﲻ�ܸ�״̬,��ΪҪ�ս����
								}
							}
						}
						break;
					case SS_PAUSED:
						{
							//��Ϣ״ֻ̬��ת��Ϊ����״̬
							//����Ҫ��ƫ�ƹ�������,��Ȼ���������������;��Ϣ,�ͻ����
							uint32_t weekDay = TimeUtils::getWeekDay();

							bool isAllHoliday = true;
							CodeSet* pCommSet =  _bd_mgr->getSessionComms(sInfo->_session);
							if (pCommSet)
							{
								for (auto it = pCommSet->begin(); it != pCommSet->end(); it++)
								{
									const char* pid = (*it).c_str();
									if ((mInfo->getOffsetMins() > 0 &&
										(! _bd_mgr->isTradingDate(pid, curDate) &&
										!(mInfo->isInTradingTime(curMin) &&  _bd_mgr->isTradingDate(pid, prevDate)))) ||
										(mInfo->getOffsetMins() <= 0 && ! _bd_mgr->isTradingDate(pid, offDate))
										)
									{
										WTSLogger::info("Instrument {} is in holiday", pid);
									}
									else
									{
										isAllHoliday = false;
									}
								}
							}
							
							if (!isAllHoliday)
							{
								uint32_t offTime = mInfo->offsetTime(curMin, true);
								if (sInfo->isInSections(offTime))
								{
									sInfo->_state = SS_RECEIVING;
									WTSLogger::info("Trading session {}[{}] continued to receive data", mInfo->name(), sInfo->_session);
								}
							}
							else
							{
								WTSLogger::info("All instruments of trading session {}[{}] are in holiday, changed into holiday state", mInfo->name(), sInfo->_session);
								sInfo->_state = SS_Holiday;
							}
						}
						break;
					case SS_CLOSED:
						{
							uint32_t offTime = mInfo->offsetTime(curMin, true);
							uint32_t offProcTime = mInfo->offsetTime(sInfo->_proc_time, true);
							if (offTime >= offProcTime)
							{
								if(!_dt_mgr->isSessionProceeded(sInfo->_session))
								{
									sInfo->_state = SS_PROCING;

									WTSLogger::info("Trading session {}[{}] started processing closing task", mInfo->name(), sInfo->_session);
									_dt_mgr->transHisData(sInfo->_session);
								}
								else
								{
									sInfo->_state = SS_PROCED;
								}
							}
							else if (offTime >= mInfo->getAuctionStartTime(true) && offTime <= mInfo->getCloseTime(true))
							{
								if (!sInfo->isInSections(offTime))
								{
									sInfo->_state = SS_PAUSED;

									WTSLogger::info("Trading session {}[{}] paused receiving data", mInfo->name(), sInfo->_session);
								}
							}
						}
						break;
					case SS_PROCING:
						sInfo->_state = SS_PROCED;
						break;
					case SS_PROCED:
					case SS_Holiday:
						{
							uint32_t offTime = mInfo->offsetTime(curMin, true);
							uint32_t offInitTime = mInfo->offsetTime(sInfo->_init_time, true);
							if (offTime >= 0 && offTime < offInitTime)
							{
								bool isAllHoliday = true;
								CodeSet* pCommSet =  _bd_mgr->getSessionComms(sInfo->_session);
								if (pCommSet)
								{
									for (auto it = pCommSet->begin(); it != pCommSet->end(); it++)
									{
										const char* pid = (*it).c_str();
										if ((mInfo->getOffsetMins() > 0 &&
											(! _bd_mgr->isTradingDate(pid, curDate) &&
											!(mInfo->isInTradingTime(curMin) &&  _bd_mgr->isTradingDate(pid, prevDate)))) ||
											(mInfo->getOffsetMins() <= 0 && ! _bd_mgr->isTradingDate(pid, offDate))
											)
										{
											
										}
										else
										{
											isAllHoliday = false;
										}
									}
								}

								if(!isAllHoliday)
								{
									sInfo->_state = SS_ORIGINAL;
									WTSLogger::info("Trading session {}[{}] state reset", mInfo->name(), sInfo->_session);
								}
							}
						}
						break;
					}
					
				}

				lastTime = TimeUtils::getLocalTimeNow();

				if (isAllInState(SS_PROCING) && !isAllInState(SS_Holiday))
				{
					//��������
					_dt_mgr->transHisData("CMD_CLEAR_CACHE");
				}
			}
		}));
	}
}

void StateMonitor::stop()
{
	_stopped = true;

	if (_thrd)
		_thrd->join();
}

bool StateMonitor::isAllInState(SimpleState ss) const
{
	auto it = _map.begin();
	for (; it != _map.end(); it++)
	{
		const StatePtr& sInfo = it->second;
		if (sInfo->_state != SS_Holiday && sInfo->_state != ss)
			return false;
	}

	return true;
}


bool StateMonitor::isAnyInState(SimpleState ss) const
{
	auto it = _map.begin();
	for (; it != _map.end(); it++)
	{
		const StatePtr& sInfo = it->second;
		if (sInfo->_state == ss)
			return true;
	}

	return false;
}

bool StateMonitor::isInState(const char* sid, SimpleState ss) const
{
	auto it = _map.find(sid);
	if (it == _map.end())
		return false;

	const StatePtr& sInfo = it->second;
	return sInfo->_state == ss;
}