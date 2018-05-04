
#include <fstream>
#include <map>
#include <list>
#include <algorithm>
#include <cstdio>

#include "ConfigFile.h"

namespace NetCom
{
	const string g_strSpace = " ";// /f/n/t/r/v";

	ConfigFile::ConfigFile(const string& strFile):m_strConfigFile(strFile),m_pFile(NULL)
	{
	}

	ConfigFile::ConfigFile(void):m_strConfigFile(""),m_pFile(NULL)
	{

	}

	ConfigFile::~ConfigFile(void)
	{
		CloseFile();
	}

	// 打开配置文件
	bool ConfigFile::OpenFile(const string& strFile)
	{
		m_strConfigFile = strFile;
		return (OpenFile());
	}

	// 打开配置文件
	bool ConfigFile::OpenFile(void)
	{
		// 判断是否文件
		if (m_strConfigFile.empty())
		{
			return (false);
		}

		if (NULL != m_pFile)
		{
			return (true);
		}

		// 判断文件是被打开
		m_pFile = fopen(m_strConfigFile.c_str(), "r");
		if (NULL != m_pFile)
		{
			std::cout<<"打開配置文件成功: "<< m_strConfigFile<<std::endl;
			return (true);
		}
		else
		{
			std::cout<<"打开配置文件失败: "<<m_strConfigFile<<std::endl;
			return (false);
		}
		
		/*
		// 判断是否已经打开
		if (!m_fileOperator.is_open())
		{
			m_fileOperator.open(m_strConfigFile);

			// 写入日志
			ILogManager* pLog = IComManager::GetInstance().GetLogManager();
			if (NULL != pLog)
			{
				pLog->SetLogMessage("打开配置文件：" + m_strConfigFile);
			}
		}

		return m_fileOperator.is_open();*/
		return (0);
	}

	// 关闭文件
	void ConfigFile::CloseFile(void)
	{
		// 判断是否打开
		if (NULL == m_pFile)
		{
			return;
		}

		fclose(m_pFile);
	/*	if (m_fileOperator.is_open())
		{
			// 写入日志
			IComManager::GetInstance().WriteLogMsg("关闭配置文件：" + m_strConfigFile);
			m_fileOperator.close();
		}
		*/
	}

	// 获取文件名称对应项的值
	string ConfigFile::GetItemValue(const string& strName, const string& strItem) const
	{
		string strItemName = strName;
		Trim(strItemName);

		string strItemValue = strItem;
		Trim(strItemValue);
		// 通过名称查询项
		MapStrItem::const_iterator cstItor = m_mapStrItem.find(strItemName);
		if (m_mapStrItem.end() == cstItor)
		{
			return ("");
		}

		// 转换项目
		const LstItemValue& itemValue = cstItor->second;
		// 查询值
		LstItemValue::const_iterator cstItorValue = std::find(itemValue.begin(), itemValue.end(), strItemValue);
		if (itemValue.end() == cstItorValue)
		{
			return ("");
		}

		// 返回值
		return (cstItorValue->strValue);
		//return "";
	}

	// 获得一个项下的所有项目
	const ConfigFile::LstItemValue& ConfigFile::GetItemValus(const string& strName) const
	{
		// 查询是否存在当前的项目名称
		MapStrItem::const_iterator itor = m_mapStrItem.find(strName);
		return itor->second;
	}

	// 读取文件
	void ConfigFile::ReadConfig(void)
	{
		// 打开文件
		if(!OpenFile())
		{
			return;
		}

		// 写入日志
		//IComManager::GetInstance().WriteLogMsg("读取配置文件：" + m_strConfigFile);
		std::cout<<"读取配置文件: "<<m_strConfigFile<<std::endl;
		string strItemName;
		// 读取文件
		//while (!m_fileOperator.eof())
		while (!feof(m_pFile))
		{
			char szLine[256] = {0};
			//fread(szLine, sizeof(char), sizeof(char) * 256, m_pFile);
			::fgets(szLine, sizeof(szLine) - 1, m_pFile);
			if (1 >= strlen(szLine))
			{
				continue;
			}
			Replace(szLine); // 去除\n
			//m_fileOperator.getline(szLine, sizeof(char) * 256);

			string tmp(szLine);
			string strLeftNoSpace = Trim(tmp);
			// 判断是否为空行
			if (strLeftNoSpace.empty())
			{
				continue;
			}

			// 判断是否为注释行 #
			if (strLeftNoSpace[0] == '#')
			{
				continue;
			}

			// 读取名称
			if (ReadItemName(strLeftNoSpace, strItemName))
			{
				continue;
			}

			string strName, strValues;
			if (!ReadItem(strLeftNoSpace, strName, strValues))
			{
				continue;
			}

			// 添加到表中
			AddItem(strItemName, strName, strValues);
		}

		// 写入日志
		//IComManager::GetInstance().WriteLogMsg("读取配置文件：" + m_strConfigFile + " 完毕");
		std::cout<<"读取配置文件: "<<m_strConfigFile<<"完毕"<<std::endl;
	}

	// 读取项目名称
	bool ConfigFile::ReadItemName(const string& strText, string& strName)
	{
		// 判断是否含有'[ ]'
		int nBeginPos = strText.find_first_of('[');
		int nEndPos = strText.find_last_of(']');

		if ((-1 == nBeginPos) && (-1 == nEndPos) && (nBeginPos <= nEndPos))
		{
			return (false);
		}

		// 获取名称
		strName = strText.substr(nBeginPos + 1, nEndPos - 1);

		return (true);
	}

	// 读取项
	bool ConfigFile::ReadItem(const string& strText, string& strName, string& strValue)
	{
		// 查询是否含有‘=’
		int nPos = strText.find('=');
		if (-1 == nPos)
		{
			return (false);
		}

		// 获取字符串长度
		int nLength = strText.length();
		// 获取名称
		strName = strText.substr(0, nPos);
		strValue = strText.substr(nPos + 1,nLength - nPos);

		// 清除两遍空格
		strName = Trim(strName);
		strValue = Trim(strValue);
		return (true);
	}

	// 添加项
	void ConfigFile::AddItem(const string& strName, const string& strValueName, const string& strValue)
	{
		ITEMVALUE value(strValueName,strValue);
		// 查询是否存在当前的项目名称
		MapStrItem::iterator itor = m_mapStrItem.find(strName);
		if (m_mapStrItem.end() != itor)
		{
			LstItemValue& itemValue = itor->second;
			itemValue.push_back(value);
		}
		else
		{
			// 创建项值
			LstItemValue itemValue;
			itemValue.push_back(value);
			m_mapStrItem[strName] = itemValue;
		}
	}

	// 去掉所有的空格
	string ConfigFile::Trim(string& strLine) const
	{
		int nPos = strLine.find_last_not_of(g_strSpace);
		string str = strLine.erase(strLine.find_last_not_of(g_strSpace) + 1);
		return (str.erase(0, str.find_first_not_of(g_strSpace)));
	}

	// 去掉左边的空格
	string ConfigFile::LeftTrim(string& strLine) const
	{
		return (strLine.erase(strLine.find_first_not_of(g_strSpace)));
	}

	// 去掉右边的空格
	string ConfigFile::RightTrim(string& strLine) const
	{
		return (strLine.erase(strLine.find_last_not_of(g_strSpace) + 1));
	}

	void ConfigFile::Replace(char* szLine)
	{
		// 进行替换'\n'
		// 查找是否存在\n
		int nCount = (int)strlen(szLine) - 1;
		char* pLine = szLine + nCount;
		if ('\n' == *pLine)
		{
			*pLine = '\0';
		}	
	}

	// 创建视图对象
	ConfigFile* CreateConfigFileInstance(const string& strConfigFile)
	{
		return new ConfigFile(strConfigFile);
	}
}

