// -*- C++ -*-
//
// $Id$

#include "RT_ORBInitializer.h"

ACE_RCSID (TAO, RT_ORBInitializer, "$Id$")

#define TAO_RTCORBA_SAFE_INCLUDE
#include "tao/RTCORBA/RTCORBAC.h"
#undef TAO_RTCORBA_SAFE_INCLUDE

#include "RT_Policy_i.h"
#include "RT_Protocols_Hooks.h"
#include "Priority_Mapping_Manager.h"
#include "Network_Priority_Mapping_Manager.h"
#include "tao/Exception.h"
#include "tao/ORB_Core.h"
#include "tao/ORBInitInfo.h"
#include "RT_ORB_Loader.h"
#include "RT_Stub_Factory.h"
#include "RT_Endpoint_Selector_Factory.h"
#include "Continuous_Priority_Mapping.h"
#include "Linear_Priority_Mapping.h"
#include "Direct_Priority_Mapping.h"
#include "Linear_Network_Priority_Mapping.h"
#include "RT_ORB.h"
#include "RT_Current.h"
#include "RT_Thread_Lane_Resources_Manager.h"

#include "ace/Service_Repository.h"
#include "ace/Svc_Conf.h"
#include "ace/Sched_Params.h"

static const char *rt_poa_factory_name = "TAO_RT_POA";
static const char *rt_poa_factory_directive = "dynamic TAO_RT_POA Service_Object * TAO_RTPortableServer:_make_TAO_RT_Object_Adapter_Factory()";

TAO_RT_ORBInitializer::TAO_RT_ORBInitializer (int priority_mapping_type,
                                              int network_priority_mapping_type,
                                              long sched_policy,
                                              long scope_policy)
  : priority_mapping_type_ (priority_mapping_type),
    network_priority_mapping_type_ (network_priority_mapping_type),
    sched_policy_ (sched_policy),
    sched_policy_flags_ (0),
    scope_policy_ (scope_policy)
{
  switch (sched_policy_)
    {
    case ACE_SCHED_RR:
      sched_policy_flags_ = THR_SCHED_RR;
      break;
    case ACE_SCHED_FIFO:
      sched_policy_flags_ = THR_SCHED_FIFO;
      break;
    case ACE_SCHED_OTHER:
      sched_policy_flags_ = THR_SCHED_DEFAULT;
      break;
    default:
      ACE_DEBUG((LM_DEBUG, ACE_LIB_TEXT("(%N,%l) Unknown sched_policy value.\nDefaulting to THR_SCHED_DEFAULT for sched_policy_flags_.\n") ));
      sched_policy_flags_ = THR_SCHED_DEFAULT;
      break;
    }
}

void
TAO_RT_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  //
  // Register all of the RT related services.
  //

  // Set the name of the Protocol_Hooks to be RT_Protocols_Hooks.
  TAO_ORB_Core::set_protocols_hooks ("RT_Protocols_Hooks");
  ACE_Service_Config::process_directive (ace_svc_desc_TAO_RT_Protocols_Hooks);

  // Set the name of the stub factory to be RT_Stub_Factory.
  TAO_ORB_Core::set_stub_factory ("RT_Stub_Factory");
  ACE_Service_Config::process_directive (ace_svc_desc_TAO_RT_Stub_Factory);

  // Set the name of the stub factory to be RT_Stub_Factory.
  TAO_ORB_Core::set_endpoint_selector_factory ("RT_Endpoint_Selector_Factory");
  ACE_Service_Config::process_directive (ace_svc_desc_RT_Endpoint_Selector_Factory);

  // Set the name of the thread lane resources manager to be RT_Thread_Lane_Resources_Manager.
  TAO_ORB_Core::set_thread_lane_resources_manager_factory ("RT_Thread_Lane_Resources_Manager_Factory");
  ACE_Service_Config::process_directive (ace_svc_desc_TAO_RT_Thread_Lane_Resources_Manager_Factory);

  // If the application resolves the root POA, make sure we load the RT POA.
  TAO_ORB_Core::set_poa_factory (rt_poa_factory_name,
                                 rt_poa_factory_directive);

  // Sets the client_protocol policy.
  TAO_RT_Protocols_Hooks::set_client_protocols_hook
    (TAO_ClientProtocolPolicy::hook);

  // Sets the server_protocol policy.
  TAO_RT_Protocols_Hooks::set_server_protocols_hook
    (TAO_ServerProtocolPolicy::hook);

  // Create the initial priority mapping instance.
  TAO_Priority_Mapping *pm;
  switch (this->priority_mapping_type_)
    {
    case TAO_PRIORITY_MAPPING_CONTINUOUS:
      ACE_NEW (pm,
               TAO_Continuous_Priority_Mapping (this->sched_policy_));
      break;
    case TAO_PRIORITY_MAPPING_LINEAR:
      ACE_NEW (pm,
               TAO_Linear_Priority_Mapping (this->sched_policy_));
      break;
    default:
    case TAO_PRIORITY_MAPPING_DIRECT:
      ACE_NEW (pm,
               TAO_Direct_Priority_Mapping (this->sched_policy_));
      break;
    }

  // Set the Priority_Mapping_Manager
  TAO_Priority_Mapping_Manager *manager = 0;

  ACE_NEW_THROW_EX (manager,
                    TAO_Priority_Mapping_Manager (pm),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;


  TAO_Priority_Mapping_Manager_var safe_manager = manager;

  info->register_initial_reference ("PriorityMappingManager",
                                    manager
                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Create the initial priority mapping instance.
  TAO_Network_Priority_Mapping *npm;
  switch (this->network_priority_mapping_type_)
    {
    default:
    case TAO_NETWORK_PRIORITY_MAPPING_LINEAR:
      ACE_NEW (npm,
               TAO_Linear_Network_Priority_Mapping (this->sched_policy_));
      break;
    }

  // Set the Priority_Mapping_Manager
  TAO_Network_Priority_Mapping_Manager *network_manager = 0;

  ACE_NEW_THROW_EX (network_manager,
                    TAO_Network_Priority_Mapping_Manager (npm),
                    CORBA::NO_MEMORY (
                                      CORBA::SystemException::_tao_minor_code (
                    TAO_DEFAULT_MINOR_CODE,
                            ENOMEM),
                                      CORBA::COMPLETED_NO));
  ACE_CHECK;


  TAO_Network_Priority_Mapping_Manager_var safe_network_manager = network_manager;

  info->register_initial_reference ("NetworkPriorityMappingManager",
                                    network_manager
                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // @@ This is busted.  TAO_ORBInitInfo should do proper reference
  //    counting.
  // Narrow to a TAO_ORBInitInfo object to get access to the
  // orb_core() TAO extension.
  TAO_ORBInitInfo_var tao_info = TAO_ORBInitInfo::_narrow (info
                                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (tao_info.in ()))
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) Security_ORBInitializer::pre_init:\n"
                    "(%P|%t)    Unable to narrow "
                    "\"PortableInterceptor::ORBInitInfo_ptr\" to\n"
                    "(%P|%t)   \"TAO_ORBInitInfo *.\"\n"));

      ACE_THROW (CORBA::INTERNAL ());
    }

  // Create the RT_ORB.
  CORBA::Object_ptr rt_orb = CORBA::Object::_nil ();
  ACE_NEW_THROW_EX (rt_orb,
                    TAO_RT_ORB (tao_info->orb_core ()),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;
  CORBA::Object_var safe_rt_orb = rt_orb;

  info->register_initial_reference (TAO_OBJID_RTORB,
                                    rt_orb
                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Create the RT_Current.
  CORBA::Object_ptr current;
  ACE_NEW_THROW_EX (current,
                    TAO_RT_Current (tao_info->orb_core ()),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;
  CORBA::Object_var safe_rt_current = current;

  info->register_initial_reference (TAO_OBJID_RTCURRENT,
                                    current
                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  tao_info->orb_core ()->orb_params ()->scope_policy (this->scope_policy_);

  /* We need to store sched_policy_flags_ and not sched_policy_ in the
   * orb_params(), because in TAO_Thread_Lane::create_dynamic_threads(),
   * the flags are passed to ACE_Task_Base::activate() in order to set
   * the priority.
   */
  tao_info->orb_core ()->orb_params ()->sched_policy (this->sched_policy_flags_);

  /* Based on what the scheduling policy is, set the priority to the lowest
   * priority for that scheduling policy.  We need to do this in order to
   * set the pthread policy for pthread_setschedparam().
   * Also, we want the pthread policy and priority to be set to a sensible
   * value, since the post_invoke() operation in the RT-POA will reset the
   * CORBA priority to what it was before an incoming request.
   */
  int priority;
  ACE_hthread_t thr_id;
  ACE_Thread::self(thr_id);

  int result = ACE_OS::thr_getprio(thr_id, priority);
  if ( result != 0 ) {
     ACE_ERROR ((LM_ERROR, "(%N,%l) ACE_OS::thr_getprio failed, priority %d errno %p\n", priority));
     return;
  }

  int priority_min = ACE_Sched_Params::priority_min(this->sched_policy_);
  int priority_max = ACE_Sched_Params::priority_max(this->sched_policy_);
  if(priority < priority_min || priority > priority_max) {  // Check this
     priority = priority_min;
  }

  result = ACE_OS::thr_setprio(thr_id, priority, this->sched_policy_);
  if ( result != 0 ) {
     ACE_ERROR ((LM_ERROR, "(%N,%l) ACE_OS::thr_setprio failed, priority %d errno %p\n", priority));
  }

}

void
TAO_RT_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->register_policy_factories (info
                                   ACE_ENV_ARG_PARAMETER);

  ACE_CHECK;
}

void
TAO_RT_ORBInitializer::register_policy_factories (
  PortableInterceptor::ORBInitInfo_ptr info
  ACE_ENV_ARG_DECL)
{
  // Register the RTCORBA policy factories.

  // The RTCORBA policy factory is stateless and reentrant, so share a
  // single instance between all ORBs.
  PortableInterceptor::PolicyFactory_ptr policy_factory =
    &(this->policy_factory_);

  // Bind the same policy factory to all RTCORBA related policy
  // types since a single policy factory is used to create each of
  // the different types of RTCORBA policies.
  CORBA::PolicyType type[] = {
    RTCORBA::PRIORITY_MODEL_POLICY_TYPE,
    RTCORBA::THREADPOOL_POLICY_TYPE,
    RTCORBA::SERVER_PROTOCOL_POLICY_TYPE,
    RTCORBA::CLIENT_PROTOCOL_POLICY_TYPE,
    RTCORBA::PRIVATE_CONNECTION_POLICY_TYPE,
    RTCORBA::PRIORITY_BANDED_CONNECTION_POLICY_TYPE
  };

  const CORBA::PolicyType *end =
    type + sizeof (type) / sizeof (type[0]);

  for (CORBA::PolicyType *i = type;
       i != end;
       ++i)
    {
      ACE_TRY
        {
          info->register_policy_factory (*i,
                                         policy_factory
                                         ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCH (CORBA::BAD_INV_ORDER, ex)
        {
          if (ex.minor () == (CORBA::OMGVMCID | 16))
            {
              // The factory is already there, it happens because the
              // magic initializer in PortableServer.cpp registers
              // with the ORB multiple times.  This is an indication
              // that we should do no more work in this
              // ORBInitializer.
              return;
            }
          ACE_RE_THROW;
        }
      ACE_CATCHANY
        {
          // Rethrow any other exceptions...
          ACE_RE_THROW;
        }
      ACE_ENDTRY;
      ACE_CHECK;
    }
}
