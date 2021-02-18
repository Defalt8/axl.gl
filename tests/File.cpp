#include <cstdio>
#include "File.hpp"

bool File::exists(const axl::util::String& filename)
{
	FILE* file = (FILE*)0;
#	ifdef __STDC_WANT_SECURE_LIB__
		fopen_s(&file, filename.cstr(), "r");
#	else
		file = fopen(filename.cstr(), "r");
#	endif
	if(file)
	{
		fclose(file);
		return true;
	}
	return false;
}

axl::util::String File::getStringContent(const axl::util::String& filename)
{
	axl::util::String content;
	FILE* file = (FILE*)0;
#	ifdef __STDC_WANT_SECURE_LIB__
		fopen_s(&file, filename.cstr(), "r");
#	else
		file = fopen(filename.cstr(), "r");
#	endif
	if(file)
	{
		fseek(file, 0, SEEK_END);
		long size = ftell(file);
		fseek(file, 0, SEEK_SET);
		if(content.resize(size))
		{
			size_t read_size = fread(content.str(), sizeof(axl::util::String::char_t), size, file);
			content[read_size] = '\0';
			content.length(true);
		}
		fclose(file);
	}
	return content;
}


axl::util::WString File::getWStringContent(const axl::util::String& filename)
{
	axl::util::WString content;
	FILE* file = (FILE*)0;
#	ifdef __STDC_WANT_SECURE_LIB__
		fopen_s(&file, filename.cstr(), "r");
#	else
		file = fopen(filename.cstr(), "r");
#	endif
	if(file)
	{
		fseek(file, 0, SEEK_END);
		long size = ftell(file);
		fseek(file, 0, SEEK_SET);
		if(content.resize(size))
		{
			size_t read_size = 0;
			int c;
			while((c = fgetc(file)) != EOF)
			{
				content[read_size++] = (axl::util::WString::char_t)c;
			}
			content[read_size] = L'\0';
			content.length(true);
		}
		fclose(file);
	}
	return content;
}
