#pragma once
#include "Tomos/core/Node.hh"

namespace Tomos
{
    class Script
    {
    public:
        virtual ~Script() = default;

        virtual void earlyUpdate()
        {
        }

        virtual void update()
        {
        }

        virtual void lateUpdate()
        {
        }

        virtual void onAttach()
        {
        }

        virtual void onDetach()
        {
        }

        std::shared_ptr<Node> m_node;
    };

    class ScriptComponent : public Component
    {
    public:
        ScriptComponent( const std::shared_ptr<Script>& p_script, const std::string& p_name = "UnnamedScriptComponent" )
        {
            m_name   = p_name;
            m_script = p_script;
        }

        const std::shared_ptr<Script>& getScript() { return m_script; }

    private:
        std::shared_ptr<Script> m_script;
    };
} // Tomos
