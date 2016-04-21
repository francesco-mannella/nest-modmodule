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

#include "network.h"
#include "dictdatum.h"
#include "connector_model.h"
#include "common_synapse_properties.h"
#include "modulatory_connection.h"
#include "event.h"
#include "nestmodule.h"

namespace mynest
{
    //
    // Implementation of class ModulatoryCommonProperties.
    //

    ModulatoryCommonProperties::ModulatoryCommonProperties()
        : nest::CommonSynapseProperties(),
        vt_( 0 ),
        max_modulation_(1.0)
    {
    }

    void ModulatoryCommonProperties::get_status( DictionaryDatum& d ) const
    {
        nest::CommonSynapseProperties::get_status( d );

        if ( vt_ != 0 )
            def< nest::long_t >( d, "vt", vt_->get_gid() );
        else
            def< nest::long_t >( d, "vt", -1 );
        
        def< nest::long_t >( d, "max_modulation", max_modulation_ );

    }

    void ModulatoryCommonProperties::set_status( const DictionaryDatum& d, 
            nest::ConnectorModel& cm )
    {
        nest::CommonSynapseProperties::set_status( d, cm );
       
        updateValue< nest::long_t >( d, "max_modulation", max_modulation_ );

        nest::long_t vtgid;
        if ( updateValue< nest::long_t >( d, "vt", vtgid ) )
        {
            vt_ = dynamic_cast< nest::volume_transmitter* >( 
                    nest::NestModule::get_network().get_node( vtgid ) );

            if ( vt_ == 0 )
                throw nest::BadProperty( "Modulatory source must be "
                        "volume transmitter" );
        }

    }

    nest::Node* ModulatoryCommonProperties::get_node()
    {
        if ( vt_ == 0 )
            throw nest::BadProperty( "No volume transmitter has "
                    "been assigned to the dopamine synapse." );
        else
            return vt_;
    }

} // of namespace nest
