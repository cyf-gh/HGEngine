#include <filesystem>

namespace HGCore {
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
};
}