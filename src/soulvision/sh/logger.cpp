// ---------------------------------------------------------------------------------------------------------------------------------
//  _                                                  
// | |                                                 
// | | ___   __ _  __ _  ___ _ __      ___ _ __  _ __  
// | |/ _ \ / _` |/ _` |/ _ \ '__|    / __| '_ \| '_ \ 
// | | (_) | (_| | (_| |  __/ |    _ | (__| |_) | |_) |
// |_|\___/ \__, |\__, |\___|_|   (_) \___| .__/| .__/ 
//           __/ | __/ |                  | |   | |    
//          |___/ |___/                   |_|   |_|    
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

#include "logger.h"

#include "mmgr.h"	// Must be LAST include

using namespace std;

// ---------------------------------------------------------------------------------------------------------------------------------
// The global logger object
// ---------------------------------------------------------------------------------------------------------------------------------

Logger	logger;

// ---------------------------------------------------------------------------------------------------------------------------------

void	Logger::start(const char* file, const bool reset)
{
	_logFile = file;

	if (logStarted()) return;
	_logStarted = true;

	// Get the time

	time_t	t = time(NULL);
	string	ts = asctime(localtime(&t));
	ts[ts.length() - 1] = 0;

	// Start the log

	ofstream of(logFile().c_str(), ios::out | (reset ? ios::trunc : ios::app));
	if (!of) return;

	of << "---------------------------------------------- Log begins on " << ts << " ----------------------------------------------" << endl;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	Logger::stop()
{
	// Automatic One time only startup

	if (!logStarted()) return;
	_logStarted = false;

	// Get the time

	time_t	t = time(NULL);
	string	ts = asctime(localtime(&t));
	ts.erase(ts.length() - 1);

	// Stop the log

	ofstream of(logFile().c_str(), ios::out|ios::app);
	if (!of) return;

	of << "----------------------------------------------- Log ends on " << ts << " -----------------------------------------------" << endl << endl;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	Logger::logTex(const char* file, unsigned int line, const string &s, const LogFlags logBits)
{
	if (!logStarted()) return;

	sourceFile() = file;
	sourceLine() = line;

	// If the bits don't match the mask, then bail

	if (!(logBits & logMask())) return;

	// Open the file

	ofstream of(logFile().c_str(), ios::out|ios::app);
	if (!of) return;

	// Output to the log file

	of << headerString(logBits) << s << endl;

#ifdef _DEBUG
	cout << headerString(logBits) << s << endl;
#endif
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	Logger::logRaw(const string &s)
{
	if (!logStarted()) return;

	// Open the file

	ofstream of(logFile().c_str(), ios::out|ios::app);
	if (!of) return;

	// Log the output

	of << s << endl;

#ifdef _DEBUG
	cout << s << endl;
#endif
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	Logger::logHex(const char* file, unsigned int line, const char *buffer, const unsigned int count, const LogFlags logBits)
{
	if (!logStarted()) return;

	sourceFile() = file;
	sourceLine() = line;

	// No input? No output

	if (!buffer) return;

	// If the bits don't match the mask, then bail

	if (!(logBits & logMask())) return;

	// Open the file

	ofstream of(logFile().c_str(), ios::out|ios::app);
	if (!of) return;

	// Log the output

	unsigned int	logged = 0;
	while(logged < count)
	{
		// One line at a time...

		string		line;

		// The number of characters per line

		unsigned int	hexLength = 20;

		// Default the buffer

		unsigned int	i;
		for (i = 0; i < hexLength; i++)
		{
			line += "-- ";
		}

		for (i = 0; i < hexLength; i++)
		{
			line += ".";
		}

		// Fill it in with real data

		for (i = 0; i < hexLength && logged < count; i++, logged++)
		{
			unsigned char	byte = buffer[logged];
			unsigned int	index = i * 3;

			// The hex characters

			const string	hexlist("0123456789ABCDEF");
			line[index+0] = hexlist[byte >> 4];
			line[index+1] = hexlist[byte & 0xf];

			// The ascii characters

			if (byte < 0x20 || byte > 0x7f) byte = '.';
			line[(hexLength*3)+i+0] = byte;
		}

		// Write it to the log file

		of << headerString(logBits) << line << endl;
#ifdef _DEBUG
		cout << headerString(logBits) << line << endl;
#endif
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	Logger::indent(const string &s, const LogFlags logBits)
{
	if (!logStarted()) return;

	// If the bits don't match the mask, then bail

	if (!(logBits & logMask())) return;

	// Open the file

	ofstream of(logFile().c_str(), ios::out|ios::app);
	if (!of) return;

	// Log the output

	ostringstream output;
	if (lineCharsFlag())	output << headerString(logBits) << "\xDA\xC4\xC4" << s << endl;
	else			output << headerString(logBits) << "+-  " << s << endl;

	of << output;
#ifdef _DEBUG
	cout << output;
#endif

	// Indent...

	_indentCount += _indentChars;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	Logger::undent(const string &s, const LogFlags logBits)
{
	if (!logStarted()) return;

	// If the bits don't match the mask, then bail

	if (!(logBits & logMask())) return;

	// Undo the indentation

	_indentCount -= _indentChars;
	if (_indentCount < 0) _indentCount = 0;

	// Open the file

	ofstream of(logFile().c_str(), ios::out|ios::app);
	if (!of) return;

	// Log the output

	ostringstream output;
	if (lineCharsFlag())	output << headerString(logBits) << "\xC0\xC4\xC4" << s << endl;
	else			output << headerString(logBits) << "+-  " << s << endl;

	of << output;
#ifdef _DEBUG
	cout << output;
#endif
}

// ---------------------------------------------------------------------------------------------------------------------------------

const	string	&Logger::headerString(const LogFlags logBits) const
{
	static	string	headerString;
	headerString.erase();

	// Get the string that represents the bits

	switch(logBits)
	{
		case LOG_INDENT : headerString += "> "; break;
		case LOG_UNDENT : headerString += "< "; break;
		case LOG_ALL    : headerString += "A "; break;
		case LOG_CRIT   : headerString += "! "; break;
		case LOG_DATA   : headerString += "D "; break;
		case LOG_ERR    : headerString += "E "; break;
		case LOG_FLOW   : headerString += "F "; break;
		case LOG_INFO   : headerString += "I "; break;
		case LOG_WARN   : headerString += "W "; break;
		default:          headerString += "  "; break;
	}

	// File string (strip out the path)

	char	temp[1024];
	int	ix = sourceFile().rfind('\\');
	ix = (ix == (int) string::npos ? 0: ix+1);
	sprintf(temp, "%15s[%04d]", sourceFile().substr(ix).c_str(), sourceLine());
	headerString += temp;

	// Time string (specially formatted to save room)

	time_t	t = time(NULL);
	struct	tm *tme = localtime(&t);
	sprintf(temp, "%02d/%02d %02d:%02d ", tme->tm_mon + 1, tme->tm_mday, tme->tm_hour, tme->tm_min);
	headerString += temp;

	// Spaces for indentation

	memset(temp, ' ', sizeof(temp));
	temp[_indentCount] = '\0';

	// Add the indentation markers

	int	count = 0;
	while(count < _indentCount)
	{
		if (lineCharsFlag())	temp[count] = '\xB3';
		else			temp[count] = '|';
		count += _indentChars;
	}
	headerString += temp;

	return headerString;
}

// ---------------------------------------------------------------------------------------------------------------------------------
// logger.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

