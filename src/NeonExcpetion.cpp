#include "NeonExcpetion.h"
#include <sstream>

NeonException::NeonException( int line, const char* file ) noexcept
    :
    line( line ),
    file( file )

{

}

const char* NeonException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << "\n" << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();  // oss dead on this scrope ** this func returns Pointer **
}

const char* NeonException::GetType() const noexcept
{
    return "Neon Exception";
}

int NeonException::GetLine() const noexcept
{
    return this->line;
}

const std::string& NeonException::GetFile() const noexcept
{
    return this->file;
}

std::string NeonException::GetOriginString() const noexcept
{
    std::ostringstream oss;
    oss << "[File] : " << this->file << "\n"
        << "[Line] : " << this->line;
    return oss.str();
}
