#pragma once
#include <filesystem>

namespace HG {
class HGDirectory {
public:
	/// \brief 路径不存在时进行创建
	static void CreateDirectoryIfDoesNotExsit( const char* path ) {
		namespace fs = std::filesystem;
		const fs::path& p( path ); fs::file_status s = fs::file_status {};
		if( !( fs::status_known( s ) ? fs::exists( s ) : fs::exists( p ) ) ) {
			fs::create_directory( path );
		}
	}
	static std::string GetApplicationDirectory() {
		char buffer[MAX_PATH];
		GetModuleFileNameA( NULL, buffer, MAX_PATH );
		std::string::size_type pos = std::string( buffer ).find_last_of( "\\/" );

		return std::string( buffer ).substr( 0, pos );
	}
};
}