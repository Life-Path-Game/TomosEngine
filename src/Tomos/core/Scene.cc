//
// Created by dstuden on 2/7/25.
//

#include "Scene.hh"

namespace Tomos
{
    void SceneManager::pushScene( const std::shared_ptr<Scene>& p_scene )
    {
        scenes.push_back( p_scene );
        p_scene->getRoot().setActive( true );
    }

    std::shared_ptr<Scene> SceneManager::popScene()
    {
        auto s = scenes.back();
        s->getRoot().setActive( false );
        scenes.pop_back();
        return s;
    }

    void SceneManager::operator<<( const std::shared_ptr<Scene>& p_scene )
    {
        pushScene( p_scene );
    }

    const std::shared_ptr<Scene>& SceneManager::activeScene()
    {
        return scenes.back();
    }
} // namespace Tomos
