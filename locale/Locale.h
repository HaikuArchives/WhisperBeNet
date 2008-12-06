// -----------------------------------------------------------------------
// locale/Locale.h
//
// R5/OpenBeOS compatibility header for the Zeta Locale/Translation
//  API.
//
// (c) 2003 YellowTAB Gmbh
//
//  Please let us know if you find missing peices you need to maintian
//  compatibility between your Zeta and R5 versions of your apps.
//
//  alan@yellowtab.com
//
// -----------------------------------------------------------------------

#ifndef LOCALE_H
#define LOCALE_H

#include <be/support/String.h>

#include <stdio.h>

// -----------------------------------------------------------------------
// class BReplacer
//
// Simple utility class used for replacing values in BParamable.
//
//  May be removed, as I always find myself just using the 
//  BParamable::Replace() method.
//2

class BReplacer
{
	public:
		const	char*		key;
		const	char*		value;

							BReplacer(const char* Key, const char* Value)
								:key(NULL)
								,value(NULL)
								{ SetTo(Key, Value); }
				
				void		SetTo(const char* Key, const char* Value)
								{ key = Key; 
								  value = Value; }
	private:
							BReplacer() {}
	
};


// ----------------------------------------------------------------------
// class BParamable
//
// Very useful, yet simple class for doing variable replacement in the
// Locale kit.
//

class BParamable
{
	public:
							BParamable(const char* StringWithParams)
								:fString(StringWithParams)
								{}
		
		// What to use for replacing params.
		// This for is in danger of being removed if I cannot find any reason
		//  to keep it around. Please let us know if you find it valuable.
		BParamable&			operator <<(const BReplacer& Replacer)
								{ return Replace(Replacer.key, Replacer.value); }

		// This form seems to be the most conveniant.
		BParamable&			Replace(const char* This, const char* WithThis)
								{ fString.ReplaceAll(This, WithThis);
	                              return *this; }
	                              
		// Just add a simple string, more useful than you might think...
		BParamable&			operator <<(const char* String)
								{ fString << String;
								  return *this; }			
		
		// How to get the string out.
							operator const char*() const 
								{ return String(); }
								
		const char*			String() const 
								{ return fString.String(); }

	private:
		BString		fString;
};

class BHandler;

class BLanguageNotifier
{
	public:
				BLanguageNotifier() {;}
				~BLanguageNotifier() {;}
				
		void	SetTo(BHandler* Handler) {;}
	
	private:
		BHandler* fHandler;	
};

class BFormatter
{
	public:
						BFormatter(const char* FormatStr, char Value);
						BFormatter(const char* FormatStr, uint8 Value);
						BFormatter(const char* FormatStr, int8 Value);
						BFormatter(const char* FormatStr, uint16 Value);
						BFormatter(const char* FormatStr, int16 Value);
						BFormatter(const char* FormatStr, uint32 Value);
						BFormatter(const char* FormatStr, int32 Value);
						BFormatter(const char* FormatStr, uint64 Value);
						BFormatter(const char* FormatStr, int64 Value);
						BFormatter(const char* FormatStr, float Value);
						BFormatter(const char* FormatStr, double Value);
						
						
						operator const char*() const;
		const char*		String() const;

	protected:

	private:
		BString			fValue;
};

inline
BFormatter::BFormatter(const char *FormatStr, char Value)
{
	sprintf(fValue.LockBuffer(strlen(FormatStr) + 4), FormatStr, Value);
	fValue.UnlockBuffer();
}

inline
BFormatter::BFormatter(const char *FormatStr, uint8 Value)
{
	sprintf(fValue.LockBuffer(strlen(FormatStr) + 4), FormatStr, Value);
	fValue.UnlockBuffer();
}

inline
BFormatter::BFormatter(const char *FormatStr, int8 Value)
{
	sprintf(fValue.LockBuffer(strlen(FormatStr) + 4), FormatStr, Value);
	fValue.UnlockBuffer();
}


inline
BFormatter::BFormatter(const char *FormatStr, uint16 Value)
{
	sprintf(fValue.LockBuffer(strlen(FormatStr) + 6), FormatStr, Value);
	fValue.UnlockBuffer();
}


inline
BFormatter::BFormatter(const char *FormatStr, int16 Value)
{
	sprintf(fValue.LockBuffer(strlen(FormatStr) + 4), FormatStr, Value);
	fValue.UnlockBuffer();
}


inline
BFormatter::BFormatter(const char *FormatStr, uint32 Value)
{
	sprintf(fValue.LockBuffer(strlen(FormatStr) + 32), FormatStr, Value);
	fValue.UnlockBuffer();
}


inline
BFormatter::BFormatter(const char *FormatStr, int32 Value)
{
	sprintf(fValue.LockBuffer(strlen(FormatStr) + 32), FormatStr, Value);
	fValue.UnlockBuffer();
}


inline
BFormatter::BFormatter(const char *FormatStr, uint64 Value)
{
	sprintf(fValue.LockBuffer(strlen(FormatStr) + 64), FormatStr, Value);
	fValue.UnlockBuffer();
}


inline
BFormatter::BFormatter(const char *FormatStr, int64 Value)
{
	sprintf(fValue.LockBuffer(strlen(FormatStr) + 64), FormatStr, Value);
	fValue.UnlockBuffer();
}


inline
BFormatter::BFormatter(const char *FormatStr, float Value)
{
	sprintf(fValue.LockBuffer(strlen(FormatStr) + 64), FormatStr, Value);
	fValue.UnlockBuffer();
}


inline
BFormatter::BFormatter(const char *FormatStr, double Value)
{
	sprintf(fValue.LockBuffer(strlen(FormatStr) + 128), FormatStr, Value);
	fValue.UnlockBuffer();
}




inline 
BFormatter::operator const char*() const
{
	return String();
}

inline const char* 
BFormatter::String() const
{
	return fValue.String();
}


// compatibility area:

class BLocale
{
	public:
							BLocale() {;}

				void		LoadLanguageFile(const char*) { ; }	
		const	char*		GetString(const char* str) { return str; }
		const	BString		GetBString(const char* str) { return BString(str); }
				BParamable	GetParamStr(const char* str) { return BParamable(str); }
};

static BLocale be_locale;

// Conveniant Macros
#define _T(str)				be_locale.GetString(str)
#define _TBS(str)			BString(_T(str))
#define _TCBS(str)			be_locale.GetBString(str)
#define _TPS(str)			be_locale.GetParamStr(str)




#endif
