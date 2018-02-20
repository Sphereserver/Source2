/**
* @file CLog.h
*
*/

#pragma once
#ifndef _INC_CLOG_H
#define _INC_CLOG_H

#include "sphere_library/CSFile.h"
#include "sphere_library/CSTime.h"
#include "sphere_library/mutex.h"
#include "common.h"
#include "../sphere/UnixTerminal.h"


// -----------------------------
//	CEventLog
// -----------------------------

enum LOG_TYPE
{
	// --severity Level
	//	it can be only one of them, so we don't need to use a bitmask
	LOGL_FATAL	= 1, 	// fatal error ! cannot continue.
	LOGL_CRIT	= 2, 	// critical. might not continue.
	LOGL_ERROR	= 3, 	// non-fatal errors. can continue.
	LOGL_WARN	= 4,	// strange.
	LOGL_EVENT	= 5,	// Misc major events.
	LOGL_QTY	= 0xF,	// first 4 bits = binary 1111 = dec 15 = hex 0xF

	// --log Flags
	LOGF_CONSOLE_ONLY	= 0x10,	// do not write this message to the log file
	LOGF_LOGFILE_ONLY	= 0x20,	// write this message only to the log file
	LOGF_QTY			= 0xF0,

	// --subject Matter (severity level is first 4 bits, LOGL_EVENT)
	LOGM_ACCOUNTS		= 0x000100,
	LOGM_INIT			= 0x000200,	// start up messages.
	LOGM_SAVE			= 0x000400,	// world save status.
	LOGM_CLIENTS_LOG	= 0x000800,	// all clients as they log in and out.
	LOGM_GM_PAGE		= 0x001000,	// player gm pages.
	LOGM_PLAYER_SPEAK	= 0x002000,	// All that the players say.
	LOGM_GM_CMDS		= 0x004000,	// Log all GM commands.
	LOGM_CHEAT			= 0x008000,	// Probably an exploit !
	LOGM_KILLS			= 0x010000,	// Log player combat results.
	LOGM_HTTP			= 0x020000,
	LOGM_NOCONTEXT		= 0x040000,	// do not include context information
	LOGM_DEBUG			= 0x080000	// debug kind of message with "DEBUG:" prefix	
};

class CScript;
class CScriptObj;


extern class CEventLog
{
	// Any text event stream. (destination is independant)
	// May include __LINE__ or __FILE__ macro as well ?

protected:
	virtual int EventStr(dword dwMask, lpctstr pszMsg)
	{
		UNREFERENCED_PARAMETER(dwMask);
		UNREFERENCED_PARAMETER(pszMsg);
		return 0;
	}
	virtual int VEvent(dword dwMask, lpctstr pszFormat, va_list args);

public:
	int _cdecl Event( dword dwMask, lpctstr pszFormat, ... ) __printfargs(3,4)
	{
		va_list vargs;
		va_start( vargs, pszFormat );
		int iret = VEvent( dwMask, pszFormat, vargs );
		va_end( vargs );
		return iret;
	}

	int _cdecl EventDebug(lpctstr pszFormat, ...) __printfargs(2,3)
	{
		va_list vargs;
		va_start(vargs, pszFormat);
		int iret = VEvent(LOGM_NOCONTEXT|LOGM_DEBUG, pszFormat, vargs);
		va_end(vargs);
		return iret;
	}

	int _cdecl EventError(lpctstr pszFormat, ...) __printfargs(2,3)
	{
		va_list vargs;
		va_start(vargs, pszFormat);
		int iret = VEvent(LOGL_ERROR, pszFormat, vargs);
		va_end(vargs);
		return iret;
	}

	int _cdecl EventWarn(lpctstr pszFormat, ...) __printfargs(2,3)
	{
		va_list vargs;
		va_start(vargs, pszFormat);
		int iret = VEvent(LOGL_WARN, pszFormat, vargs);
		va_end(vargs);
		return iret;
	}

#ifdef _DEBUG
	int _cdecl EventEvent( lpctstr pszFormat, ... ) __printfargs(2,3)
	{
		va_list vargs;
		va_start( vargs, pszFormat );
		int iret = VEvent( LOGL_EVENT, pszFormat, vargs );
		va_end( vargs );
		return iret;
	}
#endif //_DEBUG

#define DEBUG_ERR(_x_)	g_pLog->EventError _x_

#ifdef _DEBUG
	#define DEBUG_WARN(_x_)		g_pLog->EventWarn _x_
	#define DEBUG_MSG(_x_)		g_pLog->EventDebug _x_
	#define DEBUG_EVENT(_x_)	g_pLog->EventEvent _x_
	#define DEBUG_MYFLAG(_x_)	g_pLog->Event _x_
#else
	// Better use placeholders than leaving it empty, because unwanted behaviours may occur.
	//  example: an else clause without brackets will include next line instead of DEBUG_WARN, because the macro is empty.
	#define DEBUG_WARN(_x_)		(void)0
	#define DEBUG_MSG(_x_)		(void)0
	#define DEBUG_EVENT(_x_)	(void)0
	#define DEBUG_MYFLAG(_x_)	(void)0
#endif

public:
	CEventLog()
	{
	}

private:
	CEventLog(const CEventLog& copy);
	CEventLog& operator=(const CEventLog& other);
} * g_pLog;


extern struct CLog : public CSFileText, public CEventLog
{
private:
	dword m_dwMsgMask;			// Level of log detail messages. IsLogMsg()
	CSTime m_dateStamp;			// last real time stamp.
	CSString m_sBaseDir;

	const CScript * m_pScriptContext;		// The current context.
	const CScriptObj * m_pObjectContext;	// The current context.

	static CSTime sm_prevCatchTick;			// don't flood with these.

public:
	bool m_fLockOpen;
	SimpleMutex m_mutex;

public:
	const CScript * SetScriptContext( const CScript * pScriptContext );
	const CScriptObj * SetObjectContext( const CScriptObj * pObjectContext );
	bool SetFilePath( lpctstr pszName );

	lpctstr GetLogDir() const;
	bool OpenLog( lpctstr pszName = NULL );	// name set previously.
	dword GetLogMask() const;
	void SetLogMask( dword dwMask );
	bool IsLoggedMask( dword dwMask ) const;
	LOG_TYPE GetLogLevel() const;
	void SetLogLevel( LOG_TYPE level );
	bool IsLoggedLevel( LOG_TYPE level ) const;
	bool IsLogged( dword dwMask ) const;

	virtual int EventStr( dword dwMask, lpctstr pszMsg );
	void _cdecl CatchEvent( const CSError * pErr, lpctstr pszCatchContext, ...  ) __printfargs(3,4);

public:
	CLog();

private:
	CLog(const CLog& copy);
	CLog& operator=(const CLog& other);

	/**
	* @name SetColor
	* @brief Changes current console text color to the specified one. Note, that the color should be reset after being set
	*/
	void SetColor(ConsoleTextColor color);
} g_Log;		// Log file


#endif // _INC_CLOG_H
