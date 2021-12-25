/*!
 * \file WTSExpressData.hpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief Wt指标数据定义文件
 */
#pragma once
#include <stdint.h>
#include "WTSDataDef.hpp"
#include "WTSMarcos.h"

#ifdef _MSC_VER
#include <WTypes.h>
#else
typedef unsigned long	COLORREF;
typedef unsigned char	BYTE;
typedef unsigned short	WORD;
typedef unsigned long	DWORD;
#define RGB(r,g,b)	((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
#endif

NS_WTP_BEGIN

//////////////////////////////////////////////////////////////////////////
//线型类,指标用到
class WTSLineInfo : public WTSObject
{
public:
	static WTSLineInfo* create(COLORREF clr, int width = 1, int style = 0)
	{
		WTSLineInfo* pRet = new WTSLineInfo();
		pRet->_line_color = clr;
		pRet->_line_width = width;
		pRet->_line_style = style;
		return pRet;
	}

	COLORREF color() const{return _line_color;}
	int width() const{return _line_width;}
	int style() const{return _line_style;}

protected:
	WTSLineInfo()
	:_line_color(RGB(255,255,255))
	,_line_width(1)
	,_line_style(0){}

protected:
	COLORREF	_line_color;
	int			_line_width;
	int			_line_style;
};


//////////////////////////////////////////////////////////////////////////
//指标参数类
class WTSExpressParams : public WTSObject
{
public:
	static	WTSExpressParams* create()
	{
		WTSExpressParams* pRet = new WTSExpressParams;

		return pRet;
	}

	void	addParam(int param)
	{
		m_vecParams.emplace_back(param);
	}

	void	setParam(uint32_t idx, int param)
	{
		if(idx >= m_vecParams.size())
			return;

		m_vecParams[idx] = param;
	}

	int		getParam(uint32_t idx) const
	{
		if(idx >= m_vecParams.size())
			return INVALID_INT32;

		return m_vecParams[idx];
	}

	uint32_t	getParamCount() const{return m_vecParams.size();}

	int&	operator[](uint32_t idx){return m_vecParams[idx];}

protected:
	vector<int>		m_vecParams;
};

//////////////////////////////////////////////////////////////////////////
//指标线类
class WTSExpressLine : public WTSValueArray
{
public:
	static WTSExpressLine*	create(uint32_t size, WTSExpressLineType lineType = WELT_Polyline, uint32_t decimal = 0, uint32_t uStyle = ELS_LINE_VISIBLE|ELS_TITLE_VISIBLE)
	{
		WTSExpressLine* pRet = new WTSExpressLine;
		pRet->m_uDecimal = decimal;
		pRet->m_uStyle = uStyle;
		pRet->m_lineType = lineType;
		pRet->resize(size);

		return pRet;
	}

	WTSExpressLine(): m_ayLineInfo(NULL){}

	virtual void release()
	{
		if(isSingleRefs() && m_ayLineInfo)
		{
			m_ayLineInfo->release();
		}

		WTSObject::release();
	}

	virtual void retain()
	{
		WTSObject::retain();
	}

	/*
	 *	读取数值数组的格式化字符串
	 */
	const char* getFormat()
	{
		if(m_strFormat.empty())
		{
			char format[12] = {0};
			sprintf(format, "%%.%df", m_uDecimal);
			m_strFormat = format;
		}

		return m_strFormat.c_str();
	}

	/*
	 *	读取数据的小数点位数
	 */
	uint32_t	getDecimal(){return m_uDecimal;}

	void		addLineInfo(WTSLineInfo* lineInfo)
	{
		if(m_ayLineInfo == NULL)
		{
			m_ayLineInfo = WTSArray::create();
		}
		m_ayLineInfo->append(lineInfo, true);
	}

	WTSLineInfo*	getLineInfo(uint32_t idx = 0)
	{
		if(m_ayLineInfo == NULL || m_ayLineInfo->size()==0 || idx >= m_ayLineInfo->size())
			return NULL;

		return STATIC_CONVERT(m_ayLineInfo->at(idx), WTSLineInfo*);
	}

	void		clearLineInfo()
	{
		if(m_ayLineInfo)
			m_ayLineInfo->clear();
	}

	void		setLineTag(const char* tag){m_strLineTag = tag;}
	const char*	getLineTag(){return m_strLineTag.c_str();}

	bool		isStyle(uint32_t uStyle) const{ return (m_uStyle & uStyle) == uStyle; }

	void		setLineType(WTSExpressLineType lineType){m_lineType = lineType;}
	WTSExpressLineType getLineType() const{return m_lineType;}

protected:
	WTSArray*		m_ayLineInfo;
	std::string		m_strLineTag;
	uint32_t		m_uStyle;

	uint32_t		m_uDecimal;
	std::string		m_strFormat;
	WTSExpressLineType	m_lineType;
};
typedef vector<WTSExpressLine*>	WTSVecExpLines;


//////////////////////////////////////////////////////////////////////////
//指标类
class WTSExpressData : public WTSObject
{
public:
	static WTSExpressData*	create(const char* title, WTSExpressType eType = WET_Unique)
	{
		WTSExpressData* pRet = new WTSExpressData;
		pRet->setTitle(title);
		pRet->setType(eType);
		return pRet;
	}

protected:
	WTSExpressData() :m_ayExpLines(NULL), m_dDevide(1.0), m_dBaseLine(0.0), m_bBaseLine(false){}

	virtual ~WTSExpressData()
	{
		
	}

public:
	virtual void release()
	{
		if (isSingleRefs() && m_ayExpLines)
			m_ayExpLines->release();

		WTSObject::release();
	}

	void		setTitle(const char* title){m_strExpTitle = title;}
	const char* getTitle() const{return m_strExpTitle.c_str();}

	void		setType(WTSExpressType eType){m_expType = eType;}
	WTSExpressType	getType() const{return m_expType;}

	void		setBaseLine(bool bEnable = true, double dBaseLine = 0.0)
	{
		m_bBaseLine = bEnable;
		m_dBaseLine = dBaseLine;
	}

	bool		hasBaseLine() const{ return m_bBaseLine; }
	double		getBaseLine() const{ return m_dBaseLine; }

	uint32_t	addExpLine(WTSExpressLine* line)
	{
		if(NULL == line)
			return INVALID_UINT32;

		if(NULL == m_ayExpLines)
			m_ayExpLines = WTSArray::create();

		m_ayExpLines->append(line, false);

		return m_ayExpLines->size() - 1;
	}


	WTSExpressLine*	getExpLine(uint32_t idx)
	{
		if(NULL == m_ayExpLines || idx >= m_ayExpLines->size())
			return NULL;

		return STATIC_CONVERT(m_ayExpLines->at(idx), WTSExpressLine*);
	}

	uint32_t	getLineCount(){return (NULL == m_ayExpLines)?0:m_ayExpLines->size();}

	bool		crossOver(uint32_t idx0, uint32_t idx1)
	{
		if(NULL == m_ayExpLines)
			return false;

		if(idx0 >= m_ayExpLines->size() || idx1 >= m_ayExpLines->size())
			return false;

		WTSExpressLine* line0 = STATIC_CONVERT(m_ayExpLines->at(idx0),WTSExpressLine*);
		WTSExpressLine* line1 = STATIC_CONVERT(m_ayExpLines->at(idx1),WTSExpressLine*);

		if(line0->size() < 2 || line1->size() < 2)
			return false;

		double preValue0 = line0->at(line0->size()-2);
		double curValue0 = line0->at(line0->size()-1);

		double preValue1 = line1->at(line1->size()-2);
		double curValue1 = line1->at(line1->size()-1);

		if(preValue0 <= preValue1 && curValue0 > curValue1)
			return true;

		return false;
	}

	bool		crossUnder(uint32_t idx0, uint32_t idx1)
	{
		if(NULL == m_ayExpLines)
			return false;

		if(idx0 >= m_ayExpLines->size() || idx1 >= m_ayExpLines->size())
			return false;

		WTSExpressLine* line0 = STATIC_CONVERT(m_ayExpLines->at(idx0),WTSExpressLine*);
		WTSExpressLine* line1 = STATIC_CONVERT(m_ayExpLines->at(idx1),WTSExpressLine*);

		if(line0->size() < 2 || line1->size() < 2)
			return false;

		double preValue0 = line0->at(line0->size()-2);
		double curValue0 = line0->at(line0->size()-1);

		double preValue1 = line1->at(line1->size()-2);
		double curValue1 = line1->at(line1->size()-1);

		if(preValue0 >= preValue1 && curValue0 < curValue1)
			return true;

		return false;
	}

	double		maxvalue(int32_t head, int32_t tail) const
	{
		double ret = INVALID_DOUBLE;
		WTSArray::ConstIterator it = m_ayExpLines->begin();
		for(; it != m_ayExpLines->end(); it++)
		{
			WTSExpressLine* line = STATIC_CONVERT(*it, WTSExpressLine*);
			bool bAbs = (line->getLineType() == WELT_VolStick || line->getLineType() == WELT_AStickLine);
			double v = line->maxvalue(head, tail, bAbs);
			if(v == INVALID_DOUBLE)
				continue;
			if(ret == INVALID_DOUBLE)
				ret = v;
			else
				ret = max(ret, v);
		}

		return ret;
	}

	double		minvalue(int32_t head, int32_t tail) const
	{
		double ret = INVALID_DOUBLE;
		WTSArray::ConstIterator it = m_ayExpLines->begin();
		for(; it != m_ayExpLines->end(); it++)
		{
			WTSExpressLine* line = STATIC_CONVERT(*it, WTSExpressLine*);
			bool bAbs = (line->getLineType() == WELT_VolStick || line->getLineType() == WELT_AStickLine);
			if(bAbs)//原因是成交量柱,是以0开始绘制的
				return 0;
			double v = line->minvalue(head, tail, bAbs);
			if (v == INVALID_DOUBLE)
				continue;
			if(ret == INVALID_DOUBLE)
				ret = v;
			else
				ret = min(ret, v);
		}

		return ret;
	}

	uint32_t	size() const
	{
		if(NULL == m_ayExpLines || m_ayExpLines->size()==0)
			return 0;

		return STATIC_CONVERT(m_ayExpLines->at(0), WTSExpressLine*)->size();
	}

	uint32_t	getPrecision() const { return m_uPrec; }
	void setPrecision(uint32_t prec){ m_uPrec = prec; }

	void		setDevide(double dvd){m_dDevide = dvd;}
	double		getDevide() const{return m_dDevide;}

protected:
	WTSArray*		m_ayExpLines;
	std::string		m_strExpTitle;
	WTSExpressType	m_expType;
	uint32_t		m_uPrec;
	double			m_dDevide;

	bool			m_bHasTitle;

	bool			m_bBaseLine;
	double			m_dBaseLine;
};

NS_WTP_END