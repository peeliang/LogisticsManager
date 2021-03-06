#include "stdafx.h"
#include "CustomerTable.h"

#include "DataBase/DBConn.h"

CCustomerListTable::CCustomerListTable()
{
	m_szTableName = "CustomerList";

	m_szTableColumnV.clear();
	m_szTableColumnV.push_back("CustomerName");	// 0
	m_szTableColumnV.push_back("PhoneNumber");	// 1
	m_szTableColumnV.push_back("Email");		// 2
	m_szTableColumnV.push_back("Address");		// 3
	m_szTableColumnV.push_back("Balance");		// 4
	m_szTableColumnV.push_back("Destination1");	// 5
	m_szTableColumnV.push_back("CustomerID1");	// 6
	m_szTableColumnV.push_back("Destination2");	// 7
	m_szTableColumnV.push_back("CustomerID2");	// 8
	m_szTableColumnV.push_back("Destination3");	// 9
	m_szTableColumnV.push_back("CustomerID3");	// 10
	m_szTableColumnV.push_back("Destination4");	// 11
	m_szTableColumnV.push_back("CustomerID4");	// 12
}


CCustomerListTable::~CCustomerListTable()
{
}


CCustomerListTable* CCustomerListTable::Instance()
{
	static CCustomerListTable instance;

	return &instance;
}


BOOL CCustomerListTable::CreateTable()
{
	BOOL bReply = CDBConn::Instance()->CreateTable(m_szTableName, m_szTableColumnV);
	CString szErrInfo = CDBConn::Instance()->GetLastErrInfo();

	return bReply;
}


BOOL CCustomerListTable::AddCustomerInfo(const std::vector<CString>& szCustomerInfoV)
{
	BOOL bReply = CDBConn::Instance()->AddTableRecord(m_szTableName, m_szTableColumnV, szCustomerInfoV);
	CString szErrInfo = CDBConn::Instance()->GetLastErrInfo();

	return bReply;
}


BOOL CCustomerListTable::QueryAllCustomerInfo(std::vector<std::vector<CString>>& szAllCustomerInfoVV)
{
	BOOL bReply = FALSE;

	szAllCustomerInfoVV.clear();
	CString szSQLCmd;
	szSQLCmd.Format("SELECT * FROM [%s] ", m_szTableName);

	_RecordsetPtr pRecordset;
	bReply = CDBConn::Instance()->SQLCommandQuery(szSQLCmd, pRecordset);

	if (!bReply)
	{
		return bReply;
	}

	while (!pRecordset->adoEOF)
	{
		std::vector<CString> szRowDaraV;
		auto iterCol = m_szTableColumnV.begin();
		for (; iterCol != m_szTableColumnV.end(); ++iterCol)
		{
			szRowDaraV.push_back(pRecordset->GetCollect(_variant_t(*iterCol)));
		}

		szAllCustomerInfoVV.push_back(szRowDaraV);

		pRecordset->MoveNext();
	}
	pRecordset->Close();

	return bReply;
}


BOOL CCustomerListTable::QueryCustomerInfoByCustomerID(const CString szCustomerID, std::vector<CString>& szCustomerInfoV)
{
	BOOL bReply = FALSE;

	szCustomerInfoV.clear();
	CString szSQLCmd;
	szSQLCmd.Format("SELECT * FROM [%s] WHERE CustomerID1 = '%s' or CustomerID2 = '%s' or CustomerID3 = '%s' or CustomerID4 = '%s'", m_szTableName, szCustomerID, szCustomerID, szCustomerID, szCustomerID);
	
	_RecordsetPtr pRecordset;
	bReply = CDBConn::Instance()->SQLCommandQuery(szSQLCmd, pRecordset);

	if (!bReply)
	{
		return bReply;
	}

	auto iterCol = m_szTableColumnV.begin();
	while (!pRecordset->adoEOF)
	{
		if (iterCol == m_szTableColumnV.end())
		{
			break;
		}

		szCustomerInfoV.push_back(pRecordset->GetCollect(_variant_t(*iterCol++)));
	}
	pRecordset->Close();

	return bReply;
}


BOOL CCustomerListTable::QueryCustomerInfoByCustomerName(const CString szCustomerName, std::vector<CString>& szCustomerInfoV)
{
	BOOL bReply = FALSE;

	szCustomerInfoV.clear();
	CString szSQLCmd;
	szSQLCmd.Format("SELECT * FROM [%s] WHERE CustomerName = '%s' ", m_szTableName, szCustomerName);

	_RecordsetPtr pRecordset;
	bReply = CDBConn::Instance()->SQLCommandQuery(szSQLCmd, pRecordset);

	if (!bReply)
	{
		return bReply;
	}

	auto iterCol = m_szTableColumnV.begin();
	while (!pRecordset->adoEOF)
	{
		if (iterCol == m_szTableColumnV.end())
		{
			break;
		}

		szCustomerInfoV.push_back(pRecordset->GetCollect(_variant_t(*iterCol++)));
	}
	pRecordset->Close();

	return bReply;
}


BOOL CCustomerListTable::QueryCustomerInfoByPhone(const CString szPhoneNum, std::vector<CString>& szCustomerInfoV)
{
	BOOL bReply = FALSE;

	szCustomerInfoV.clear();
	CString szSQLCmd;
	szSQLCmd.Format("SELECT * FROM [%s] WHERE PhoneNumber = '%s'", m_szTableName, szPhoneNum);

	_RecordsetPtr pRecordset;
	bReply = CDBConn::Instance()->SQLCommandQuery(szSQLCmd, pRecordset);

	if (!bReply)
	{
		return bReply;
	}

	auto iterCol = m_szTableColumnV.begin();
	while (!pRecordset->adoEOF)
	{
		if (iterCol == m_szTableColumnV.end())
		{
			break;
		}

		szCustomerInfoV.push_back(pRecordset->GetCollect(_variant_t(*iterCol++)));
	}
	pRecordset->Close();

	return bReply;
}


BOOL CCustomerListTable::AlterCustomerInfoByQueryPhone(const CString szPhoneNum, const std::vector<CString>& szCustomerInfoV)
{
	BOOL bReply = FALSE;

	if (szCustomerInfoV.size() != m_szTableColumnV.size())
	{
		return FALSE;
	}

	CString szSQLCmd;
	szSQLCmd.Format("UPDATE [%s] SET ", m_szTableName);
	auto iter = m_szTableColumnV.begin();
	for (int i = 0; iter != m_szTableColumnV.end(); ++iter,++i)
	{
		CString szAlterCol;
		szAlterCol.Format("%s = '%s', ", *iter, szCustomerInfoV[i]);
		szSQLCmd += szAlterCol;
	}
	szSQLCmd = szSQLCmd.Left(szSQLCmd.GetLength() - 2);
	CString szCondtion;
	szCondtion.Format(" WHERE PhoneNumber = '%s'", szPhoneNum);
	szSQLCmd += szCondtion;

	bReply = CDBConn::Instance()->SQLCommandExecute(szSQLCmd);

	return FALSE;
}


BOOL CCustomerListTable::AlterCustomerInfoItem(const CString szPhoneNum, const CString szColName, const CString szValue)
{
	BOOL bReply = FALSE;

	CString szSQLCmd;
	szSQLCmd.Format("UPDATE [%s] SET %s = '%s'", m_szTableName, szColName, szValue);
	CString szCondtion;
	szCondtion.Format(" WHERE PhoneNumber = '%s'", szPhoneNum);
	szSQLCmd += szCondtion;

	bReply = CDBConn::Instance()->SQLCommandExecute(szSQLCmd);

	return FALSE;
}


CustomerAccountTable::CustomerAccountTable()
{
	m_szTableName = "CustomerAccount";

	m_szTableColumnV.clear();
	m_szTableColumnV.push_back("PhoneNumber");
	m_szTableColumnV.push_back("DayTime");
	m_szTableColumnV.push_back("AddExp");
	m_szTableColumnV.push_back("SubtractExp");
	m_szTableColumnV.push_back("Balance");
}


CustomerAccountTable::~CustomerAccountTable()
{
}


CustomerAccountTable* CustomerAccountTable::Instance()
{
	static CustomerAccountTable instance;
	return &instance;
}


BOOL CustomerAccountTable::CreateTable()
{
	BOOL bReply = CDBConn::Instance()->CreateTable(m_szTableName, m_szTableColumnV);
	CString szErrInfo = CDBConn::Instance()->GetLastErrInfo();

	return TRUE;
}


BOOL CustomerAccountTable::AddCustomerAccountRecord(const CString szPhoneNumber, const std::vector<CString>& szAccountRecordV)
{
	std::vector<CString> szTempV;
	BOOL bReply = CCustomerListTable::Instance()->QueryCustomerInfoByPhone(szPhoneNumber, szTempV);
	if (!bReply || szTempV.empty())
	{
		return FALSE;
	}

	bReply = CDBConn::Instance()->AddTableRecord(m_szTableName, m_szTableColumnV, szAccountRecordV);
	CString szErrInfo = CDBConn::Instance()->GetLastErrInfo();

	return TRUE;
}


BOOL CustomerAccountTable::QueryCustomerAccountRecord(const CString szPhoneNumber, std::vector<std::vector<CString>>& szAccountRecordV)
{
	BOOL bReply = FALSE;

	szAccountRecordV.clear();
	CString szSQLCmd;
	szSQLCmd.Format("SELECT * FROM [%s] WHERE PhoneNumber = '%s'", m_szTableName, szPhoneNumber);

	_RecordsetPtr pRecordset;
	bReply = CDBConn::Instance()->SQLCommandQuery(szSQLCmd, pRecordset);

	if (!bReply)
	{
		return bReply;
	}

	while (!pRecordset->adoEOF)
	{
		std::vector<CString> szRowDaraV;
		auto iterCol = m_szTableColumnV.begin();
		for (; iterCol != m_szTableColumnV.end(); ++iterCol)
		{
			szRowDaraV.push_back(pRecordset->GetCollect(_variant_t(*iterCol)));
		}

		szAccountRecordV.push_back(szRowDaraV);

		pRecordset->MoveNext();
	}
	pRecordset->Close();

	return bReply;
}