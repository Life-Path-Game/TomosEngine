//
// Created by dstuden on 1/19/25.
//

#include "Shader.hh"

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>

#include "../logger/Logger.hh"

namespace Tomos
{
    Shader::Shader( const std::string& p_vertexPath, const std::string& p_fragmentPath )
    {
        LOG_DEBUG() << "Start";

        std::string   vertexCode;
        std::string   fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        vShaderFile.exceptions( std::ifstream::failbit | std::ifstream::badbit );
        fShaderFile.exceptions( std::ifstream::failbit | std::ifstream::badbit );
        try
        {
            vShaderFile.open( p_vertexPath );
            fShaderFile.open( p_fragmentPath );
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
            LOG_ERROR() << "Shader file not successfully read";
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        unsigned int vertex, fragment;
        int          success;
        char         infoLog[512];

        vertex = glCreateShader( GL_VERTEX_SHADER );
        glShaderSource( vertex, 1, &vShaderCode, nullptr );
        glCompileShader( vertex );

        glGetShaderiv( vertex, GL_COMPILE_STATUS, &success );
        if ( !success )
        {
            glGetShaderInfoLog( vertex, 512, nullptr, infoLog );
            LOG_ERROR() << "Vertex shader compilation failed: " << infoLog;
        };

        fragment = glCreateShader( GL_FRAGMENT_SHADER );
        glShaderSource( fragment, 1, &fShaderCode, nullptr );
        glCompileShader( fragment );

        glGetShaderiv( fragment, GL_COMPILE_STATUS, &success );
        if ( !success )
        {
            glGetShaderInfoLog( fragment, 512, nullptr, infoLog );
            Logger::log( LogLevel::ERROR ) << "Fragment shader compilation failed: " << infoLog;
        };

        m_rendererId = glCreateProgram();
        glAttachShader( m_rendererId, vertex );
        glAttachShader( m_rendererId, fragment );
        glLinkProgram( m_rendererId );

        glGetProgramiv( m_rendererId, GL_LINK_STATUS, &success );
        if ( !success )
        {
            glGetProgramInfoLog( m_rendererId, 512, nullptr, infoLog );
            Logger::log( LogLevel::ERROR ) << "Shader program linking failed: " << infoLog;
            glDeleteProgram( m_rendererId );

            glDeleteShader( vertex );
            glDeleteShader( fragment );
        }
        else
        {
            glDeleteShader( vertex );
            glDeleteShader( fragment );

            glDetachShader( m_rendererId, vertex );
            glDetachShader( m_rendererId, fragment );
        }

        LOG_DEBUG() << "End";
    }

    Shader::~Shader()
    {
        glDeleteProgram( m_rendererId );
    }

    void Shader::bind() const
    {
        glUseProgram( m_rendererId );
    }

    void Shader::setBool( const std::string& p_name, bool p_value ) const
    {
        glUniform1i( glGetUniformLocation( m_rendererId, p_name.c_str() ), ( int ) p_value );
    }

    void Shader::setInt( const std::string& p_name, int p_value ) const
    {
        glUniform1i( glGetUniformLocation( m_rendererId, p_name.c_str() ), p_value );
    }

    void Shader::setFloat( const std::string& p_name, float p_value ) const
    {
        glUniform1f( glGetUniformLocation( m_rendererId, p_name.c_str() ), p_value );
    }

    void Shader::setVec2( const std::string& p_name, const glm::vec2& p_value ) const
    {
        glUniform2fv( glGetUniformLocation( m_rendererId, p_name.c_str() ), 1, &p_value[0] );
    }

    void Shader::setVec2( const std::string& p_name, float p_x, float p_y ) const
    {
        glUniform2f( glGetUniformLocation( m_rendererId, p_name.c_str() ), p_x, p_y );
    }

    void Shader::setVec3( const std::string& p_name, const glm::vec3& p_value ) const
    {
        glUniform3fv( glGetUniformLocation( m_rendererId, p_name.c_str() ), 1, &p_value[0] );
    }

    void Shader::setVec3( const std::string& p_name, float p_x, float p_y, float p_z ) const
    {
        glUniform3f( glGetUniformLocation( m_rendererId, p_name.c_str() ), p_x, p_y, p_z );
    }

    void Shader::setVec4( const std::string& p_name, const glm::vec4& p_value ) const
    {
        glUniform4fv( glGetUniformLocation( m_rendererId, p_name.c_str() ), 1, &p_value[0] );
    }

    void Shader::setVec4( const std::string& p_name, float p_x, float p_y, float p_z, float p_w ) const
    {
        glUniform4f( glGetUniformLocation( m_rendererId, p_name.c_str() ), p_x, p_y, p_z, p_w );
    }

    void Shader::setMat2( const std::string& p_name, const glm::mat2& p_mat ) const
    {
        glUniformMatrix2fv( glGetUniformLocation( m_rendererId, p_name.c_str() ), 1, GL_FALSE, glm::value_ptr( p_mat ) );
    }

    void Shader::setMat3( const std::string& p_name, const glm::mat3& p_mat ) const
    {
        glUniformMatrix3fv( glGetUniformLocation( m_rendererId, p_name.c_str() ), 1, GL_FALSE, glm::value_ptr( p_mat ) );
    }

    void Shader::setMat4( const std::string& p_name, const glm::mat4& p_mat ) const
    {
        glUniformMatrix4fv( glGetUniformLocation( m_rendererId, p_name.c_str() ), 1, GL_FALSE, glm::value_ptr( p_mat ) );
    }
} // namespace Tomos
