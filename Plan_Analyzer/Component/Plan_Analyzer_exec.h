// $Id$
//
// ****              Code generated by the                 ****
// ****  Component Integrated ACE ORB (CIAO) CIDL Compiler ****
// CIAO has been developed by:
//       Center for Distributed Object Computing
//       Washington University
//       St. Louis, MO
//       USA
//       http://www.cs.wustl.edu/~schmidt/doc-center.html
// CIDL Compiler has been developed by:
//       Institute for Software Integrated Systems
//       Vanderbilt University
//       Nashville, TN
//       USA
//       http://www.isis.vanderbilt.edu/
//
// Information about CIAO is available at:
//    http://www.dre.vanderbilt.edu/CIAO

#ifndef CIAO_PLAN_ANALYZER_EXEC_H
#define CIAO_PLAN_ANALYZER_EXEC_H

#include /**/ "ace/pre.h"

#include "Plan_Analyzer_svnt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Plan_Analyzer_exec_export.h"
#include "tao/LocalObject.h"

namespace CIAO
{
  namespace RACE
  {
    namespace CIDL_Plan_Analyzer_Impl
    {
      class Plan_Analyzer_exec_i;

      class PLAN_ANALYZER_EXEC_Export PlanIngress_exec_i
      : public virtual ::CIAO::RACE::CCM_PlanIngress,
      public virtual TAO_Local_RefCounted_Object
      {
        public:
        PlanIngress_exec_i (Plan_Analyzer_exec_i &comp);
        virtual ~PlanIngress_exec_i (void);

        // Operations from ::CIAO::RACE::PlanIngress

        virtual void
        analyze_plan (
          const ::CIAO::RACE::Plan_Actions & plan_seq
          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

        Plan_Analyzer_exec_i &comp_;
      };

      class PLAN_ANALYZER_EXEC_Export Plan_Analyzer_exec_i
      : public virtual Plan_Analyzer_Exec,
      public virtual TAO_Local_RefCounted_Object
      {
        public:
        Plan_Analyzer_exec_i (void);
        virtual ~Plan_Analyzer_exec_i (void);

        // Supported or inherited operations.

        // Attribute operations.

        virtual ::CIAO::RACE::RACE_Classifications *
        classes (
          ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

        // Port operations.

        virtual ::CIAO::RACE::CCM_PlanIngress_ptr
        get_plan_ingress (
          ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

        // Operations from Components::SessionComponent

        virtual void
        set_session_context (
          ::Components::SessionContext_ptr ctx
          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((
                          ::CORBA::SystemException,
                          ::Components::CCMException));

        virtual void
        ciao_preactivate (
          ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((
                          ::CORBA::SystemException,
                          ::Components::CCMException));

        virtual void
        configuration_complete (
          ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((
                          ::CORBA::SystemException,
                          ::Components::CCMException));

        virtual void
        ccm_activate (
          ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((
                          ::CORBA::SystemException,
                          ::Components::CCMException));

        virtual void
        ccm_passivate (
          ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((
                          ::CORBA::SystemException,
                          ::Components::CCMException));

        virtual void
        ccm_remove (
          ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((
                          ::CORBA::SystemException,
                          ::Components::CCMException));

        protected:
        Plan_Analyzer_Context *context_;

        friend class PlanIngress_exec_i;

      };

      class PLAN_ANALYZER_EXEC_Export Plan_Analyzer_Home_exec_i
      : public virtual Plan_Analyzer_Home_Exec,
      public virtual TAO_Local_RefCounted_Object
      {
        public:
        Plan_Analyzer_Home_exec_i (void);
        virtual ~Plan_Analyzer_Home_exec_i (void);

        // Supported or inherited operations.

        // Home operations.

        // Factory and finder operations.

        // Attribute operations.

        // Implicit operations.

        virtual ::Components::EnterpriseComponent_ptr
        create (
          ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((
                          ::CORBA::SystemException,
                          ::Components::CCMException));
      };

      extern "C" PLAN_ANALYZER_EXEC_Export ::Components::HomeExecutorBase_ptr
      create_CIAO_RACE_Plan_Analyzer_Home_Impl (void);
    }
  }
}

#include /**/ "ace/post.h"

#endif /* CIAO_PLAN_ANALYZER_EXEC_H */
