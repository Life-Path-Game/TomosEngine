//
// Created by dstuden on 1/19/25.
//

#include "Application.hh"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../events/application/ApplicationEvent.hh"
#include "../events/key/KeyEvent.hh"
#include "../lib/stb_image/stb_image.h"
#include "../systems/camera/CameraComponent.hh"
#include "../systems/camera/CameraSystem.hh"
#include "../util/logger/Logger.hh"
#include "../util/shader/Shader.hh"

namespace Tomos
{
    Application* Application::instance = nullptr;

    Application::Application()
    {
        LOG_INFO() << "Tomos Engine v0.1";
        LOG_INFO() << "By dstuden";
        LOG_INFO() << "Initializing Application";

        window = std::make_unique<Window>( WindowProps() );

        window->setEventCallback( [this]( Event& e ) { onEvent( e ); } );
    }

    Application* Application::get()
    {
        if ( instance == nullptr )
        {
            instance = new Application();
        }

        return instance;
    }

    void Application::run()
    {
        const Shader shader( "/home/dstuden/Documents/rand/personal/Tomos/src/Tomos/shaders/generic.vertex.glsl",
                             "/home/dstuden/Documents/rand/personal/Tomos/src/Tomos/shaders/generic.fragment.glsl" );
        float        vertices[] = {
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
                0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

                -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 1.0f, -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

                -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

                0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
                0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
                0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

                -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f };
        unsigned int indices[] = {
                0, 1, 3,  // first triangle
                1, 2, 3  // second triangle
        };

        glEnable( GL_DEPTH_TEST );

        unsigned int VBO, VAO, EBO;
        glGenVertexArrays( 1, &VAO );
        glGenBuffers( 1, &VBO );
        glGenBuffers( 1, &EBO );

        glBindVertexArray( VAO );

        glBindBuffer( GL_ARRAY_BUFFER, VBO );
        glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );

        // position attribute
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), ( void* ) 0 );
        glEnableVertexAttribArray( 0 );
        // texture coord attribute
        glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), ( void* ) ( 3 * sizeof( float ) ) );
        glEnableVertexAttribArray( 1 );

        // load and create a texture
        // -------------------------
        unsigned int texture1, texture2;
        // texture 1
        // ---------
        glGenTextures( 1, &texture1 );
        glBindTexture( GL_TEXTURE_2D, texture1 );
        // set the texture wrapping parameters
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        // set texture filtering parameters
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load( true );  // tell stb_image.h to flip loaded texture's on the y-axis.
        unsigned char* data = stbi_load( "/home/dstuden/Downloads/container.jpg", &width, &height, &nrChannels, 0 );
        if ( data )
        {
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
            glGenerateMipmap( GL_TEXTURE_2D );
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free( data );

        shader.use();
        shader.setInt( "texture1", 0 );

        glm::vec3 cubePositions[] = { glm::vec3( 0.0f, 0.0f, 0.0f ),    glm::vec3( 2.0f, 5.0f, -15.0f ),
                                      glm::vec3( -1.5f, -2.2f, -2.5f ), glm::vec3( -3.8f, -2.0f, -12.3f ),
                                      glm::vec3( 2.4f, -0.4f, -3.5f ),  glm::vec3( -1.7f, 3.0f, -7.5f ),
                                      glm::vec3( 1.3f, -2.0f, -2.5f ),  glm::vec3( 1.5f, 2.0f, -2.5f ),
                                      glm::vec3( 1.5f, 0.2f, -1.5f ),   glm::vec3( -1.3f, 1.0f, -1.5f ) };

        get()->getState().ecs.registerSystem<CameraSystem>();

        auto node = std::make_shared<Node>();
        node->addComponent( std::make_shared<CameraComponent>() );

        get()->state.scene.addChild( node );

        while ( running )
        {
            glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

            // bind textures on corresponding texture units
            glActiveTexture( GL_TEXTURE0 );
            glBindTexture( GL_TEXTURE_2D, texture1 );

            // activate shader
            shader.use();

            // create transformations
            glm::mat4 model      = glm::mat4( 1.0f );  // make sure to initialize matrix to identity matrix first
            glm::mat4 view       = glm::mat4( 1.0f );
            glm::mat4 projection = glm::mat4( 1.0f );
            model                = glm::rotate( model, glm::radians( -55.0f ), glm::vec3( 1.0f, 0.0f, 0.0f ) );
            view                 = glm::translate( view, glm::vec3( 0.0f, 0.0f, -3.0f ) );
            projection           = glm::perspective( glm::radians( 45.0f ),
                                                     ( float ) window->getWidth() / ( float ) window->getHeight(), 0.1f, 100.0f );


            // retrieve the matrix uniform locations
            unsigned int modelLoc = glGetUniformLocation( shader.id, "model" );
            unsigned int viewLoc  = glGetUniformLocation( shader.id, "view" );
            // pass them to the shaders (3 different ways)
            glUniformMatrix4fv( modelLoc, 1, GL_FALSE, glm::value_ptr( model ) );
            glUniformMatrix4fv( viewLoc, 1, GL_FALSE, &view[0][0] );
            // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes
            // it's often best practice to set it outside the main loop only once.
            shader.setMat4( "projection", projection );

            glBindVertexArray( VAO );
            for ( unsigned int i = 0; i < 10; i++ )
            {
                glm::mat4 model = glm::mat4( 1.0f );
                model           = glm::translate( model, cubePositions[i] );
                float angle     = 20.0f * i;
                model           = glm::rotate( model, glm::radians( angle ), glm::vec3( 1.0f, 0.3f, 0.5f ) );
                shader.setMat4( "model", model );

                glDrawArrays( GL_TRIANGLES, 0, 36 );
            }

            for ( auto& layer : layerStack )
            {
                layer->onUpdate();
            }

            window->onUpdate();
        }
    }

    void Application::onEvent( Event& e )
    {
        EventDispatcher dispatcher( e );
        dispatcher.dispatch<WindowCloseEvent>( [this]( Event& event )
                                               { return onWindowClose( dynamic_cast<WindowCloseEvent&>( event ) ); } );

        LOG_DEBUG() << e.toString();

        for ( auto it = layerStack.end(); it != layerStack.begin(); )
        {
            ( *--it )->onEvent( e );
            if ( e.isHandled() )
            {
                break;
            }
        }
    }

    bool Application::onWindowClose( WindowCloseEvent& e )
    {
        running = false;
        return true;
    }

    Window& Application::getWindow() const { return *window; }

    void Application::PushLayer( std::unique_ptr<Layer> layer ) { layerStack.pushLayer( std::move( layer ) ); }

    void Application::PushOverlay( std::unique_ptr<Layer> overlay ) { layerStack.pushOverlay( std::move( overlay ) ); }

}  // namespace Tomos
