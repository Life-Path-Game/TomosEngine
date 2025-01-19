#pragma once

#include <string>

namespace Tomos
{

    class Shader
    {
    public:
        unsigned int id;

        Shader( const std::string &vertexPath, const std::string &fragmentPath );

        void use() const;

        void setBool( const std::string &name, bool value ) const;
        void setInt( const std::string &name, int value ) const;
        void setFloat( const std::string &name, float value ) const;
    };

}  // namespace Tomos
