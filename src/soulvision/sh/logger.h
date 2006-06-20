// ---------------------------------------------------------------------------------------------------------------------------------
//  _                                 _     
// | |                               | |    
// | | ___   __ _  __ _  ___ _ __    | |__  
// | |/ _ \ / _` |/ _` |/ _ \ '__|   | '_ \ 
// | | (_) | (_| | (_| |  __/ |    _ | | | |
// |_|\___/ \__, |\__, |\___|_|   (_)|_| |_|
//           __/ | __/ |                    
//          |___/ |___/                     
//
// Generic informational logging class
//
// ---------------------------------------------------------------------------------------------------------------------------------
//
// Restrictions & freedoms pertaining to usage and redistribution of this software:
//
//  * This software is 100% free
//  * If you use this software (in part or in whole) you must credit the author.
//  * This software may not be re-distributed (in part or in whole) in a modified
//    form without clear documentation on how to obtain a copy of the original work.
//  * You may not use this software to directly or indirectly cause harm to others.
//  * This software is provided as-is and without warrantee. Use at your own risk.
//
// For more information, visit HTTP://www.FluidStudios.com
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 07/06/2000 by Paul Nettle
//
// Copyright 2000, Fluid Studios, Inc., all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#ifndef	_H_LOGGER
#define _H_LOGGER

#include <string>
#include <list>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

// ---------------------------------------------------------------------------------------------------------------------------------
// The global logger
// ---------------------------------------------------------------------------------------------------------------------------------

class	Logger;
extern	Logger	logger;

// ---------------------------------------------------------------------------------------------------------------------------------
// Macros (necessary evil to take advantage of __LINE__ and __FILE__)
// ---------------------------------------------------------------------------------------------------------------------------------

#define	INDENT(m)	logger.indent(__FILE__, __LINE__, m)
#define	UNDENT(m)	logger.undent(__FILE__, __LINE__, m)
#define	LOGBLOCK(m)	LogBlock __lb__(__FILE__, __LINE__, m)

#define	LOGINFO(string)		logger.logTex(__FILE__, __LINE__, string, Logger::LOG_INFO)
#define	LOGERROR(string)	logger.logTex(__FILE__, __LINE__, string, Logger::LOG_ERR)
#define	LOGCRITICAL(string)	logger.logTex(__FILE__, __LINE__, string, Logger::LOG_CRIT)

#if defined(__FUNCTION__)
#define	LOGFUNC(f)	LogFlow __lf__(__FILE__, __LINE__, __FUNCTION__)
#else
#define	LOGFUNC(f)	LogFlow __lf__(__FILE__, __LINE__, f)
#endif

// ---------------------------------------------------------------------------------------------------------------------------------
// The logger class: does the actual logging
// ---------------------------------------------------------------------------------------------------------------------------------

class	Logger
{
public:
	// Enumerations

	enum	LogFlags
	{
		LOG_INDENT = 0x00000001,
		LOG_UNDENT = 0x00000002,
		LOG_FLOW   = 0x00000004,
		LOG_BLOK   = 0x00000008,
		LOG_DATA   = 0x00000010,
		LOG_INFO   = 0x00000012,
		LOG_WARN   = 0x00000014,
		LOG_ERR    = 0x00000018,
		LOG_CRIT   = 0x00000020,
		LOG_ALL    = 0xFFFFFFFF
	};

	// Construction/Destruction

inline							Logger()
								: _sourceLine(0), _indentCount(0), _indentChars(4), _logMask(LOG_ALL),
								  _logStarted(false), _lineCharsFlag(false), _logFile("logger.log")
								{
								}

virtual							~Logger()
								{
									stop();
								}

	// Accessors

inline	const	bool			&lineCharsFlag() const	{return _lineCharsFlag;}
inline			bool			&lineCharsFlag() 		{return _lineCharsFlag;}

inline	const	unsigned int	&logMask() const		{return _logMask;}
inline			unsigned int	&logMask()				{return _logMask;}

inline	const	std::string		&logFile() const		{return _logFile;}
inline			std::string		&logFile()				{return _logFile;}

inline	const	unsigned int	&sourceLine() const		{return _sourceLine;}
inline			unsigned int	&sourceLine()			{return _sourceLine;}

inline	const	std::string		&sourceFile() const		{return _sourceFile;}
inline			std::string		&sourceFile()			{return _sourceFile;}

inline			bool			logStarted() const		{return _logStarted;}

	// Utilitarian (public)

virtual			void			start(const char* file, const bool reset = false);
virtual			void			stop();
virtual			void			logTex(const char* file, unsigned int line, const std::string &s, const LogFlags logBits = LOG_INFO);
virtual			void			logRaw(const std::string &s);
virtual			void			logHex(const char* file, unsigned int line, const char *buffer, const unsigned int count, const LogFlags logBits = LOG_INFO);
virtual			void			indent(const std::string &s, const LogFlags logBits = LOG_INDENT);
virtual			void			undent(const std::string &s, const LogFlags logBits = LOG_UNDENT);

private:
	// Utilitarian (private)

virtual	const	std::string		&headerString(const LogFlags logBits) const;

	// Data

				std::string		_logFile;
				std::string		_sourceFile;
				unsigned int	_sourceLine;
				int				_indentCount;
				int				_indentChars;
				unsigned int	_logMask;
				bool			_logStarted;
				bool			_lineCharsFlag;
};

// ---------------------------------------------------------------------------------------------------------------------------------
// The LogBlock class: used for automatic indentation
// ---------------------------------------------------------------------------------------------------------------------------------

class	LogBlock
{
public:
inline							LogBlock(const char* file, unsigned int line, const std::string &s)
								{
									logger.sourceFile() = file;
									logger.sourceLine() = line;
									str = s;
									logger.indent("Begin block: " + str, Logger::LOG_INDENT);
								}
inline							~LogBlock()
								{
									logger.undent("", Logger::LOG_UNDENT);
								}
private:
				std::string		str;
};

// ---------------------------------------------------------------------------------------------------------------------------------
// The LogFlow class: used for logging code flow
// ---------------------------------------------------------------------------------------------------------------------------------

class	LogFlow
{
public:
inline							LogFlow(const char* file, unsigned int line, const std::string &function)
								{
									logger.sourceFile() = file;
									logger.sourceLine() = line;
									str = function;
									logger.indent(str, Logger::LOG_FLOW);
								}
inline							~LogFlow()
								{
									logger.undent("", Logger::LOG_FLOW);
								}
private:
				std::string		str;
};

#endif // _H_LOGGER
// ---------------------------------------------------------------------------------------------------------------------------------
// logger.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

