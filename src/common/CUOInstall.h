/**
* @file CUOInstall.h
* @brief Handler for UO files.
*/

#ifndef _INC_CUOINSTALL_H
#define _INC_CUOINSTALL_H

#include "sphere_library/CSTypedArray.h"
#include "sphere_library/CSFile.h"
#include "CsvFile.h"
#include "../game/uo_files/CUOIndexRec.h"
#include "../game/uo_files/CUOVersionBlock.h"
#include "../game/uo_files/uofiles_enums.h"


////////////////////////////////////////////////////////

class MapAddress
{
public:
	uint dwFirstBlock;
	uint dwLastBlock;
	int64 qwAdress;
};

ullong HashFileName(CSString csFile);

extern struct CUOInstall
{
	// "Software\\Origin Worlds Online\\Ultima Online\\1.0"
	// bool m_fFullInstall;	// Are all files avail ?
private:
	CSString m_sPreferPath;	// Prefer path in which to choose the files. (look here FIRST)
	CSString m_sExePath;		// Files that are installed. "ExePath"
	CSString m_sCDPath;		// For files that may still be on the CD. "InstCDPath"
public:
	CSFile m_File[ VERFILE_QTY ];	// Get our list of files we need to access.
	VERFILE_FORMAT m_FileFormat[ VERFILE_QTY ]; // File format versions

	CSFile	m_Maps[256];		// mapX.mul
	CSFile	m_Mapdif[256];		// mapdifX.mul
	CSFile	m_Mapdifl[256];		// mapdiflX.mul
	CSFile	m_Statics[256];		// staticsX.mul
	CSFile	m_Staidx[256];		// staidxX.mul
	CSFile	m_Stadif[256];		// stadifX.mul
	CSFile	m_Stadifi[256];		// stadifiX.mul
	CSFile	m_Stadifl[256];		// stadiflX.mul
	bool m_IsMapUopFormat[256]; // true for maps that are uop format
	MapAddress m_UopMapAddress[256][256]; //For uop parsing. Note: might need to be ajusted later if format changes.

	CSVFile m_CsvFiles[8];		// doors.txt, stairs.txt (x2), roof.txt, misc.txt, teleprts.txt, floors.txt, walls.txt

public:
	CSString GetFullExePath( lpctstr pszName = NULL ) const;
	CSString GetFullCDPath( lpctstr pszName = NULL ) const;

public:
	bool FindInstall();
	void DetectMulVersions();
	VERFILE_TYPE OpenFiles( ullong ullMask );
	bool OpenFile( CSFile & file, lpctstr pszName, word wFlags );
	bool OpenFile( VERFILE_TYPE i );
	void CloseFiles();

	static lpctstr GetBaseFileName( VERFILE_TYPE i );
	CSFile * GetMulFile( VERFILE_TYPE i );
	VERFILE_FORMAT GetMulFormat( VERFILE_TYPE i );

	void SetPreferPath( lpctstr pszName );
	CSString GetPreferPath( lpctstr pszName = NULL ) const;

	bool ReadMulIndex( VERFILE_TYPE fileindex, VERFILE_TYPE filedata, dword id, CUOIndexRec & Index );
	bool ReadMulData( VERFILE_TYPE filedata, const CUOIndexRec & Index, void * pData );

	bool ReadMulIndex(CSFile &file, dword id, CUOIndexRec &Index);
	bool ReadMulData(CSFile &file, const CUOIndexRec &Index, void * pData);
	
public:
	CUOInstall();

private:
	CUOInstall(const CUOInstall& copy);
	CUOInstall& operator=(const CUOInstall& other);
} g_Install;

///////////////////////////////////////////////////////////////////////////////

extern class CVerDataMul
{
	// Find verison diffs to the files listed.
	// Assume this is a sorted array of some sort.
public:
	static const char *m_sClassName;
	CSTypedArray < CUOVersionBlock, CUOVersionBlock& > m_Data;
private:
	int QCompare( size_t left, dword dwRefIndex ) const;
	void QSort( size_t left, size_t right );
public:
	size_t GetCount() const;
	const CUOVersionBlock * GetEntry( size_t i ) const;
	void Unload();
	void Load( CSFile & file );
	bool FindVerDataBlock( VERFILE_TYPE type, dword id, CUOIndexRec & Index ) const;
	
public:
	CVerDataMul();
	~CVerDataMul();
private:
	CVerDataMul(const CVerDataMul& copy);
	CVerDataMul& operator=(const CVerDataMul& other);
} g_VerData;

#endif	// _INC_CUOINSTALL_H
