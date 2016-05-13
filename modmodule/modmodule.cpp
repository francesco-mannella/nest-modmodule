/*  
 *  
 *   MIT License
 *   
 *   Copyright (c) 2016 Francesco Mannella and Daniele Caligiore
 *   
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *   
 *   The above copyright notice and this permission notice shall be included in all
 *   copies or substantial portions of the Software.
 *   
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *   SOFTWARE.
 *  
 */

/*
 *   Modified from https://github.com/nest/nest-simulator/blob/v2.10.0/examples/MyModule/mymodule.cpp 
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
  return std::string( "ModModule" ); // Return name of the module
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
  nest::register_connection_model< D2DivConnection< nest::TargetIdentifierPtrRport > >(
    nest::NestModule::get_network(), "d2_div_synapse" );
} // ModModule::init()
