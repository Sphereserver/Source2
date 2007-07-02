//
//	CDataBase
//		mySQL wrapper for easier data operations witheen in-game server
//
#ifndef CDATABASE_H
#define	CDATABASE_H

#include "../common/graycom.h"
#ifndef _EXTERNAL_DLL
	#include <mysql.h>
//	#include "../common/mysql/include/mysql.h"
	#include <errmsg.h>	// mysql standart include
//	#include "../common/mysql/include/errmsg.h"
#else
	#include "../common/CDatabaseLoader.h"
#endif
#include "../common/CScriptObj.h"

#ifndef _EXTERNAL_DLL
	#ifdef _WIN32
		#pragma comment(lib, "libmySQL")
	#else
		#pragma comment(lib, "libmysqlclient")
	#endif

	#define	MIN_MYSQL_VERSION_ALLOW	40115
#else
	#define DEFAULT_RESULT_SIZE 30
#endif

class CDataBase : public CScriptObj
{
public:
	static const char *m_sClassName;
	//	construction
	CDataBase();
	~CDataBase();
	bool Connect(const char *user, const char *password, const char *base = "", const char *host = "localhost");
	bool Connect();
	void Close();							//	close link with db

	//	select
	bool	query(const char *query);			//	proceeds the query for SELECT
	bool	__cdecl queryf(char *fmt, ...);
	void	exec(const char *query);			//	executes query (pretty faster) for ALTER, UPDATE, INSERT, DELETE, ...
	void	__cdecl execf(char *fmt, ...);

	//	set / get / info methods
	bool	isConnected();
#ifndef _EXTERNAL_DLL
	UINT	getLastId();					//	get last id generated by auto-increment field
#else
private:
	fieldarray_t * GetFieldArrayBuffer();
	int GetFieldArraySize();
	void ResizeFieldArraySize(int, bool = false);

	resultarray_t * GetResultArrayBuffer();
	int GetResultArraySize();
	void ResizeResultArraySize(int, bool = false);

public:

#endif

	bool OnTick();
	int FixWeirdness();

	virtual bool r_GetRef( LPCTSTR & pszKey, CScriptObj * & pRef );
	virtual bool r_LoadVal( CScript & s );
	virtual bool r_WriteVal( LPCTSTR pszKey, CGString &sVal, CTextConsole * pSrc );
	virtual bool r_Verb( CScript & s, CTextConsole * pSrc );

	LPCTSTR GetName() const
	{
		return "SQL_OBJ";
	}

public:
	CVarDefMap	m_QueryResult;
	static LPCTSTR const sm_szLoadKeys[];
	static LPCTSTR const sm_szVerbKeys[];

protected:
#ifndef _EXTERNAL_DLL
	bool	_bConnected;					//	are we online?
	MYSQL	*_myData;						//	mySQL link
#else
	struct __fieldarray_container { fieldarray_t * faData; int faDataSize; int faDataActualSize; } faContainer;
	struct __resultarray_container { resultarray_t * raData; int raDataSize; int raDataActualSize; } raContainer;
#endif
};

#endif