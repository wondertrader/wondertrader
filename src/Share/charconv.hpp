#pragma once
#include <stdlib.h>
#include <string>
#ifdef _MSC_VER
#include <windows.h>
#else
#include <iconv.h>
#endif

class UTF8toChar
{
public :
	UTF8toChar(const char *utf8_string)
	{
		init(utf8_string);
	}

	UTF8toChar(const std::string& utf8_string)
	{
		init(utf8_string.c_str());
	}

	void init(const char *utf8_string)
	{
		if (0 == utf8_string)
			t_string = 0;
		else if (0 == *utf8_string)
		{
			needFree = false;
			t_string = ("");
		}
		else if ( isPureAscii(utf8_string))
		{
			needFree = false;
			t_string = (char *)utf8_string;
		}
		else
		{
			// Either TCHAR = Unicode (2 bytes), or utf8_string contains non-ASCII characters.
			// Needs conversion
			needFree = true;

			// Convert to Unicode (2 bytes)
			std::size_t string_len = strlen(utf8_string);
			std::size_t dst_len = string_len * 2 + 2;
#ifdef _MSC_VER
			wchar_t *buffer = new wchar_t[string_len + 1];
			MultiByteToWideChar(CP_UTF8, 0, utf8_string, -1, buffer, string_len + 1);
			buffer[string_len] = 0;

			t_string = new char[string_len * 2 + 2];
			WideCharToMultiByte(CP_ACP, 0, buffer, -1, t_string, dst_len, 0, 0);
			t_string[string_len * 2 + 1] = 0;
			delete[] buffer;
#else
			iconv_t cd;
			t_string = new char[dst_len];

			cd = iconv_open("gbk", "utf8");
			if (cd != 0)
			{
				memset(t_string, 0, dst_len);
				iconv(cd, (char**)&utf8_string, &string_len, &t_string, &dst_len);
				iconv_close(cd);
				t_string[dst_len] = '\0';
			}
#endif
		}
	}

	operator const char*()
	{
		return t_string;
	}

	const char* c_str()
	{
		return t_string;
	}

	~UTF8toChar()
	{
		if (needFree)
			delete[] t_string;
	}

private :
	char *t_string;
	bool needFree;

	//
	// helper utility to test if a string contains only ASCII characters
	//
	bool isPureAscii(const char *s)
	{
		while (*s != 0) { if (*(s++) & 0x80) return false; }
		return true;
	}

	//disable assignment
	UTF8toChar(const UTF8toChar &rhs);
	UTF8toChar &operator=(const UTF8toChar &rhs);
};

class ChartoUTF8
{
public :
	ChartoUTF8(const std::string& str)
	{
		init(str.c_str());
	}

	ChartoUTF8(const char *t_string)
	{
		init(t_string);
	}

	void init(const char *t_string)
	{
		if (0 == t_string)
			utf8_string = 0;
		else if (0 == *t_string)
		{
			utf8_string = "";
			needFree = false;
		}
		else if (isPureAscii((char *)t_string))
		{
			utf8_string = (char *)t_string;
			needFree = false;
		}
		else
		{

			needFree = true;

			std::size_t string_len = strlen(t_string);
			std::size_t dst_len = string_len * 3 + 1;
#ifdef _MSC_VER		

			// Convert to Unicode if not already in unicode.
			wchar_t *w_string = new wchar_t[string_len + 1];
			MultiByteToWideChar(CP_ACP, 0, t_string, -1, w_string, string_len + 1);
			w_string[string_len] = 0;

			// Convert from Unicode (2 bytes) to UTF8
			utf8_string = new char[dst_len];
			WideCharToMultiByte(CP_UTF8, 0, w_string, -1, utf8_string, dst_len, 0, 0);
			utf8_string[string_len * 3] = 0;

			if (w_string != (wchar_t *)t_string)
				delete[] w_string;
#else
			iconv_t cd;
			utf8_string = new char[dst_len];

			cd = iconv_open("utf8", "gbk");
			if (cd != 0)
			{
				memset(utf8_string, 0, dst_len);
				iconv(cd, (char**)&t_string, &string_len, &utf8_string, &dst_len);
				iconv_close(cd);
				utf8_string[dst_len] = '\0';
			}
#endif
		}
	}

	operator const char*()
	{
		return utf8_string;
	}

	const char* c_str() const
	{
		return utf8_string;
	}

	~ChartoUTF8()
	{
		if (needFree)
			delete[] utf8_string;
	}

private :
	char *utf8_string;
	bool needFree;

	//
	// helper utility to test if a string contains only ASCII characters
	//
	bool isPureAscii(const char *s)
	{
		while (*s != 0) { if (*(s++) & 0x80) return false; }
		return true;
	}

	//disable assignment
	ChartoUTF8(const ChartoUTF8 &rhs);
	ChartoUTF8 &operator=(const ChartoUTF8 &rhs);
};


class URLEncode
{
public:
	URLEncode(const char* src)
	{ 
		char hex[] = "0123456789ABCDEF";  

		for (unsigned int i = 0; i < strlen(src); ++i)
		{  
			const char cc = src[i];  
			if (isPureAscii(&cc))  
			{  
				if (cc == ' ')  
				{  
					encoded_string += "%20";  
				}  
				else 
					encoded_string += cc;  
			}  
			else 
			{  
				unsigned char c = static_cast<unsigned char>(src[i]);
				encoded_string += '%';  
				encoded_string += hex[c / 16];  
				encoded_string += hex[c % 16];
			}  
		}  
	}
	
	operator const char*(){return encoded_string.c_str();}

private:
	bool isPureAscii(const char *s)
	{
		while (*s != 0) { if (*(s++) & 0x80) return false; }
		return true;
	}

private:
	std::string encoded_string;
};

class URLDecode
{
public:
	URLDecode(const char* src)
	{ 
		int hex = 0;  
		for (unsigned int i = 0; i < strlen(src); ++i)
		{  
			switch (src[i])  
			{  
			case '+':  
				decoded_string += ' ';  
				break;  
			case '%':  
				if (isxdigit(src[i + 1]) && isxdigit(src[i + 2]))  
				{
					std::string hexStr;
					hexStr += src[i+1];
					hexStr += src[i+2];
					hex = strtol(hexStr.c_str(), 0, 16);
					//字母和数字[0-9a-zA-Z]、一些特殊符号[$-_.+!*'(),] 、以及某些保留字[$&+,/:;=?@]  
					//可以不经过编码直接用于URL  
					if (!((hex >= 48 && hex <= 57) || //0-9  
						(hex >=97 && hex <= 122) ||   //a-z  
						(hex >=65 && hex <= 90) ||    //A-Z  
						//一些特殊符号及保留字[$-_.+!*'(),]  [$&+,/:;=?@]  
						hex == 0x21 || hex == 0x24 || hex == 0x26 || hex == 0x27 || hex == 0x28 || hex == 0x29 
						|| hex == 0x2a || hex == 0x2b|| hex == 0x2c || hex == 0x2d || hex == 0x2e || hex == 0x2f 
						|| hex == 0x3A || hex == 0x3B|| hex == 0x3D || hex == 0x3f || hex == 0x40 || hex == 0x5f 
						))  
					{  
						decoded_string += char(hex);
						i += 2;  
					}  
					else decoded_string += '%';  
				}else {  
					decoded_string += '%';  
				}  
				break;  
			default:
				decoded_string += src[i];  
				break;  
			}  
		}  
	}

	operator const char*(){return decoded_string.c_str();}

private:
	std::string decoded_string;
};