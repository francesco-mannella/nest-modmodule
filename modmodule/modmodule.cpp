/*
 *  modmodule.cpp
 *
 *  This file is part of NEST.
 *
 *  Copyright (C) 2004 The NEST Initiative
 *
 *  NEST is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  NEST is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with NEST.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


// include necessary NEST headers
#include "config.h"
#include "network.h"
#include "model.h"
#include "dynamicloader.h"
#include "genericmodel.h"
#include "booldatum.h"
#include "integerdatum.h"
#include "tokenarray.h"
#include "exceptions.h"
#include "sliexceptions.h"
#include "nestmodule.h"
#include "connector_model_impl.h"
#include "target_identifier.h"

// include headers with your own stuff
#include "modmodule.h"
#include "modulatory_connection.h"
#include "da_connection.h"

// -- Interface to dynamic module loader ---------------------------------------

/*
 * The dynamic module loader must be able to find your module.
 * You make the module known to the loader by defining an instance of your
 * module class in global scope. This instance must have the name
 *
 * <modulename>_LTX_mod
 *
 * The dynamicloader can then load modulename and search for symbol "mod" in it.
 */

mynest::ModModule modmodule_LTX_mod;

// -- DynModule functions ------------------------------------------------------

mynest::ModModule::ModModule()
{
#ifdef LINKED_MODULE
  // register this module at the dynamic loader
  // this is needed to allow for linking in this module at compile time
  // all registered modules will be initialized by the main app's dynamic loader
  nest::DynamicLoaderModule::registerLinkedModule( this );
#endif
}

mynest::ModModule::~ModModule()
{
}

const std::string
mynest::ModModule::name( void ) const
{
  return std::string( "My NEST Module" ); // Return name of the module
}

const std::string
mynest::ModModule::commandstring( void ) const
{
  // Instruct the interpreter to load modmodule-init.sli
  return std::string( "(modmodule-init) run" );
}


//-------------------------------------------------------------------------------------

void
mynest::ModModule::init( SLIInterpreter* i )
{

  /* Register a synapse type.
     Give synapse type as template argument and the name as second argument.

     There are two choices for the template argument:
         - nest::TargetIdentifierPtrRport
         - nest::TargetIdentifierIndex
     The first is the standard and you should usually stick to it.
     nest::TargetIdentifierIndex reduces the memory requirement of synapses
     even further, but limits the number of available rports. Please see
     Kunkel et al, Front Neurofinfom 8:78 (2014), Sec 3.3.2, for details.
  */
  nest::register_connection_model< ModulatoryConnection< nest::TargetIdentifierPtrRport > >(
    nest::NestModule::get_network(), "modulatory_synapse" );
  nest::register_connection_model< D1Connection< nest::TargetIdentifierPtrRport > >(
    nest::NestModule::get_network(), "d1_synapse" );
  nest::register_connection_model< D2Connection< nest::TargetIdentifierPtrRport > >(
    nest::NestModule::get_network(), "d2_synapse" );

} // ModModule::init()
