#pragma once

//----------------------------------------------------------------------------------------
//	Copyright © 2004 - 2017 Tangible Software Solutions Inc.
//	This class can be used by anyone provided that the copyright notice remains intact.
//
//	This class is used to replace some static .NET file and folder method calls
//	with boost::filesystem method calls.
//----------------------------------------------------------------------------------------
#include <string>
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>

class FileSystem
{
public:
	static void createDirectory(const std::wstring &path)
	{
		boost::filesystem::create_directory(pathFromString(path));
	}

	static bool fileExists(const std::wstring &path)
	{
		return boost::filesystem::is_regular_file(pathFromString(path));
	}

	static bool directoryExists(const std::wstring &path)
	{
		return boost::filesystem::is_directory(pathFromString(path));
	}

	static std::wstring combine(const std::wstring &path1, const std::wstring &path2)
	{
		return (pathFromString(path1) / pathFromString(path2)).generic_wstring();
	}

	static bool isPathRooted(const std::wstring &path)
	{
		return pathFromString(path).has_root_path();
	}

	static std::wstring getFullPath(const std::wstring &path)
	{
		return boost::filesystem::absolute(pathFromString(path)).generic_wstring();
	}

	static std::wstring getFileName(const std::wstring &path)
	{
		return boost::filesystem::path(pathFromString(path)).filename().generic_wstring();
	}

	static std::wstring getDirectoryName(const std::wstring &path)
	{
		return boost::filesystem::path(pathFromString(path)).parent_path().generic_wstring();
	}

	static std::wstring getCurrentDirectory()
	{
		return boost::filesystem::current_path().generic_wstring();
	}

	static void copyFile(const std::wstring &path1, const std::wstring &path2)
	{
		boost::filesystem::copy_file(pathFromString(path1), pathFromString(path2));
	}

	static void renamePath(const std::wstring &path1, const std::wstring &path2)
	{
		boost::filesystem::rename(pathFromString(path1), pathFromString(path2));
	}

	static wchar_t preferredSeparator()
	{
		return boost::filesystem::path::preferred_separator;
	}

private:
	static boost::filesystem::path pathFromString(const std::wstring &path)
	{
		return boost::filesystem::path(&path[0]);
	}
};
