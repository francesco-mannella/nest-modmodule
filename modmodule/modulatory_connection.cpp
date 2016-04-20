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
