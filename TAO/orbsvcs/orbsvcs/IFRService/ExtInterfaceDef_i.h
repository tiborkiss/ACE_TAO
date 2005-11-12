// -*- C++ -*-

// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/orbsvcs/IFRService
//
// = FILENAME
//    ExtInterfaceDef_i.h
//
// = DESCRIPTION
//    ExtInterfaceDef_i servant class.
//
// = AUTHOR
//    Jeff Parsons <j.parsons@vanderbilt.edu>
//
// ============================================================================

#ifndef TAO_EXTINTERFACEDEF_I_H
#define TAO_EXTINTERFACEDEF_I_H

#include "InterfaceDef_i.h"
#include "ifr_service_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "InterfaceAttrExtension_i.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_IFRService_Export TAO_ExtInterfaceDef_i
  : public virtual TAO_InterfaceDef_i,
    public virtual TAO_InterfaceAttrExtension_i
{
  // = TITLE
  //    TAO_ExtInterfaceDef_i
  //
  // = DESCRIPTION
  //    Mixes in TAO_InterfaceAttrExtension_i to get attribute exceptions.
  //
public:
  TAO_ExtInterfaceDef_i (TAO_Repository_i *repo);
  // Constructor

  virtual ~TAO_ExtInterfaceDef_i (void);
  // Destructor
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_EXTINTERFACEDEF_I_H */
