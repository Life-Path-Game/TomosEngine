//
// Created by dstuden on 1/19/25.
//

#include "Shader.hh"

#include <GL/glew.h>
#include <fstream>
#include <sstream>

#include "../Logger/Logger.hh"

namespace Tomos
{
    Shader::Shader( const std::string& vertexPath, const std::string& fragmentPath )
    {
        std::string   vertexCode;
        std::string   fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        vShaderFile.exceptions( std::ifstream::failbit | std::ifstream::badbit );
        fShaderFile.exceptions( std::ifstream::failbit | std::ifstream::badbit );
        try
        {
            vShaderFile.open( vertexPath );
            fShaderFile.open( fragmentPath );
            std::stringstream vShaderStream, fShaderStream;

            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            vShaderFile.close();
            fShaderFile.close();

            vertexCode   = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch ( std::ifstream::failure& e )
        {
            Logger::log( LogLevel::ERROR ) << "Shader file not successfully read";
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        unsigned int vertex, fragment;
        int          success;
        char         infoLog[512];

        vertex = glCreateShader( GL_VERTEX_SHADER );
        glShaderSource( vertex, 1, &vShaderCode, NULL );
        glCompileShader( vertex );

        glGetShaderiv( vertex, GL_COMPILE_STATUS, &success );
        if ( !success )
        {
            glGetShaderInfoLog( vertex, 512, NULL, infoLog );
            Logger::log( LogLevel::ERROR ) << "Vertex shader compilation failed: " << infoLog;
        };

        fragment = glCreateShader( GL_FRAGMENT_SHADER );
        glShaderSource( fragment, 1, &fShaderCode, NULL );
        glCompileShader( fragment );

        glGetShaderiv( fragment, GL_COMPILE_STATUS, &success );
        if ( !success )
        {
            glGetShaderInfoLog( fragment, 512, NULL, infoLog );
            Logger::log( LogLevel::ERROR ) << "Fragment shader compilation failed: " << infoLog;
        };

        id = glCreateProgram();
        glAttachShader( id, vertex );
        glAttachShader( id, fragment );
        glLinkProgram( id );

        glGetProgramiv( id, GL_LINK_STATUS, &success );
        if ( !success )
        {
            glGetProgramInfoLog( id, 512, NULL, infoLog );
            Logger::log( LogLevel::ERROR ) << "Shader program linking failed: " << infoLog;
        }

        glDeleteShader( vertex );
        glDeleteShader( fragment );
    }

    void Shader::use() const { glUseProgram( id ); }

    void Shader::setBool( const std::string& name, bool value ) const
    {
        glUniform1i( glGetUniformLocation( id, name.c_str() ), ( int ) value );
    }
    void Shader::setInt( const std::string& name, int value ) const
    {
        glUniform1i( glGetUniformLocation( id, name.c_str() ), value );
    }
    void Shader::setFloat( const std::string& name, float value ) const
    {
        glUniform1f( glGetUniformLocation( id, name.c_str() ), value );
    }

}  // namespace Tomos
