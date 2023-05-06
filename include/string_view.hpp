/*
 * stringview.hpp
 *
 *  Created on: Jan 6, 2021
 *      Author: andrewrubinstein
 */

#ifndef STRINGVIEW_HPP_
#define STRINGVIEW_HPP_
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#if defined(WIN32) || defined(_WIN32)
#include <cstring>
#else
#include <string.h>
#endif
//class ObjectIDView;
class string_view {
protected:
    char *ptr = nullptr;
    uint32_t len = 0;
public:
    inline string_view() noexcept;
    inline string_view(const std::string &s) noexcept;
//    inline string_view(const ObjectIDView &s);
    inline string_view(const std::string &s, const uint32_t start, const uint32_t end) noexcept;
    inline string_view(const string_view s, const uint32_t start, const uint32_t end) noexcept;
    inline string_view(const char *s, const uint32_t len) noexcept;
    const char* c_str() const noexcept { return ptr; }
    void resize(uint32_t newLen) noexcept
    {
        len = newLen;
    }
    uint32_t size() const noexcept
    {
        return this->len;
    }
    uint32_t length() const noexcept
    {
        return this->size();
    }
    char& operator[](uint32_t index) const noexcept
    {
        return ptr[index];
    }
    inline std::string operator+(const string_view&& s) const noexcept;
    inline std::string operator+(const string_view& s) const noexcept;
    inline std::string operator+(const std::string&& s) const noexcept;
    inline std::string operator+(const std::string& s) const noexcept;
    inline string_view& operator=(const std::string& s) noexcept;
    inline bool operator==(const string_view &s) const noexcept;
    inline bool operator!=(const string_view &s) const noexcept;
    inline bool operator<(const string_view &s) const noexcept;
    inline bool operator>(const string_view &s) const noexcept;
    inline uint32_t find(const std::string &s) const noexcept;
    inline uint32_t find(const char *s, uint32_t start = 0, uint32_t size = -1) const noexcept;
    inline uint32_t find(const string_view, const uint32_t start = 0, uint32_t size = -1) const noexcept;
    inline uint32_t find(const char, const uint32_t start = 0) const noexcept;
    inline string_view substr(const uint32_t start, uint32_t len = -1) const noexcept;
    inline std::string str() const;
    inline friend std::ostream& operator<<(std::ostream &o, const string_view &l);
    char& back() const noexcept
    {
        return (*this)[this->length() - 1];
    }
    char& front() const noexcept
    {
        return (*this)[0];
    }
};
using sv = string_view;
template <> struct std::hash<sv>
{
    //fnv-1a
  size_t operator()(const sv &x) const
  {
      static const uint64_t prime = 0x100000001b3, offset = 0xcbf29ce484222325;
      size_t hash = offset;
      for(uint32_t i = 0; i < x.length(); i++)
      {
          hash ^= (x[i]);
          hash *= prime;
      }
      return hash;
  }
};
template <> struct std::less<sv>
{
  size_t operator()(const sv &x, const sv &y) const
  {
      return x<y;
  }
};

template <> struct std::equal_to<sv>
{
  size_t operator()(const sv &x, const sv &y) const
  {
      return x == y;
  }
};

std::ostream& operator<<(std::ostream &o,const string_view &l)
{
	for(uint32_t i = 0; i < l.len; i++)
		o<<l[i];
	return o;
}


string_view::string_view() noexcept: ptr(nullptr), len(0)  {}  


string_view& string_view::operator=(const std::string &s) noexcept
{
	this->ptr = (char*) s.c_str();
	this->len = s.size();
	return *this;
}
string_view::string_view(const std::string &s) noexcept
{
    this->ptr = (char*) s.c_str();
    this->len = s.size();
}
string_view::string_view(const std::string &s, const uint32_t start, const uint32_t end) noexcept
{
	this->ptr = (char*) s.c_str()+start;
	this->len = (end - start)<s.size()?(end-start):s.size()-start;
}
string_view::string_view(string_view s, const uint32_t start, const uint32_t end) noexcept
{
	this->ptr = (char*) s.ptr+start;
	this->len = (end - start)<s.size()?(end-start):s.size()-start;
}
string_view::string_view(const char *s, const uint32_t len) noexcept
{
	ptr = (char*) s;
	this->len = len;
}
//given an original string, and a string to search for return index of start of match, and -1 if none present
inline size_t findText(const char *original, const uint32_t originalLen, const char *lookup, const uint32_t lookupLen) noexcept
 {
    for (uint32_t i = 0; i + lookupLen <= originalLen; i++)
		if(memcmp(original + i, lookup, lookupLen) == 0)
			return i;
    
    return -1;
}

uint32_t string_view::find(const std::string &s) const noexcept
{
	uint32_t result = findText(ptr, len, s.c_str(), (uint32_t) s.size());
	return result;
}
uint32_t string_view::find(const char *s, const uint32_t start, uint32_t size) const noexcept
{
	if(size == -1)
		size = strlen(s);
	uint32_t result = findText(ptr+start, len-start, s, size);
	result += (result != -1)*start;
	return result;
}
uint32_t string_view::find(const string_view s, const uint32_t start, uint32_t size) const noexcept
{
	if(size == -1)
		size = s.size();
	uint32_t result = findText(ptr+start, len-start, s.ptr, size);
	result += (result != -1)*start;
	return result;
}
uint32_t string_view::find(const char s, const uint32_t start) const noexcept
{
	uint32_t result = findText(ptr+start, len-start, &s, 1);
	result += (result != -1)*start;
	return result;
}
string_view string_view::substr(const uint32_t start, uint32_t len) const noexcept
{
	if(len==-1)
		len = this->len;
	return string_view(*this, start, len+start);
}
std::string string_view::str() const 
{
	return std::string(ptr, len);
}

bool string_view::operator!=(const string_view &s) const noexcept
{
	return !(*this == s);
}

bool string_view::operator==(const string_view &s) const noexcept
{
	return this->length() == s.length() && memcmp(this->c_str(), s.c_str(), this->length()) == 0;
}
bool string_view::operator>(const string_view &s) const noexcept
{
	uint32_t result = memcmp(this->c_str(), s.c_str(), std::min(this->length(), s.length()));
    return result + (this->length() < s.length()) > 0;
}
bool string_view::operator<(const string_view &s) const noexcept
{
    
	uint32_t result = memcmp(this->c_str(), s.c_str(), std::min(this->length(), s.length()));
    return result - (this->length() > s.length()) < 0;
}

std::string string_view::operator+(const string_view&& s) const noexcept
{
	return (*this) + s;
}
std::string string_view::operator+(const string_view& s) const noexcept
{
    std::string result;
    result.reserve(this->size() + s.size());
    uint32_t i = 0;
    for(; i < this->size(); i++)
    {
        result += (*this)[i];
    }
    for(uint32_t j = 0; j < s.size(); j++, i++)
    {
        result += s[j];
    }
	return result;
}
std::string string_view::operator+(const std::string&& s) const noexcept
{
    return  (*this) + s;
}
std::string string_view::operator+(const std::string& s) const noexcept
{
	return (*this) + string_view(s);
}
#endif