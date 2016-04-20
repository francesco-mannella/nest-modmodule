#ifndef MODULATORY_CONNECTION
#define MODULATORY_CONNECTION

#include "connection.h"
#include "static_connection.h"
#include <vector>

namespace mynest
{

    /**
     * Class containing the common properties for all synapses of type dopamine connection.
     */
    class ModulatoryCommonProperties : public nest::CommonSynapseProperties
    {
        public:
            /**
             * Default constructor.
             * Sets all property values to defaults.
             */
            ModulatoryCommonProperties();

            /**
             * Get all properties and put them into a dictionary.
             */
            void get_status( DictionaryDatum& d ) const;

            /**
             * Set properties from the values given in dictionary.
             */
            void set_status( const DictionaryDatum& d, nest::ConnectorModel& cm );

            nest::Node* get_node();

            nest::long_t get_vt_gid() const;

            nest::volume_transmitter* vt_;

            nest::long_t max_modulation_;
    };

    inline nest::long_t ModulatoryCommonProperties::get_vt_gid() const
    {
        if ( vt_ != 0 )
            return vt_->get_gid();
        else
            return -1;
    }

    /**
     * Modulatory connection
     * A third moduatory neuron can change the 
     * strength of the weights 
     */
    template < typename targetidentifierT >
        class ModulatoryConnection : public nest::Connection< targetidentifierT >
    {
        private:
            nest::double_t initial_weight_; //!< Initial synaptic weight
            nest::double_t weight_; //!< Synaptic weight

        public:
            //! Type to use for representing common synapse properties
            typedef ModulatoryCommonProperties CommonPropertiesType;

            //! Shortcut for base class
            typedef nest::Connection< targetidentifierT > ConnectionBase;

            /**
             * Default Constructor.
             * Sets default values for all parameters. Needed by GenericConnectorModel.
             */
            ModulatoryConnection() 
                : ConnectionBase(),initial_weight_(1.0), weight_(initial_weight_ )
            {

            }

            //! Default Destructor.
            virtual ~ModulatoryConnection()
            {
            }

            /**
             * Helper class defining which types of events can be transmitted.
             *
             * These methods are only used to test whether a certain type of connection
             * can be created.
             *
             * `handles_test_event()` should be added for all event types that the
             * synapse can transmit. The methods shall return `invalid_port_`; the
             * return value will be ignored.
             *             *
             * See Kunkel et al (2014), Sec 3.3.1, for background information.
             */
            class ConnTestDummyNode 
                : public nest::ConnTestDummyNodeBase 
            {
                public:
                    using nest::ConnTestDummyNodeBase::handles_test_event;
                    nest::port handles_test_event( nest::SpikeEvent&, nest::rport )
                    {
                        return nest::invalid_port_;
                    }

                    nest::port handles_test_event( nest::DSSpikeEvent&, nest::rport )
                    {
                        return nest::invalid_port_;
                    }
            };

            /**
             * Check that requested connection can be created.
             *
             * This function is a boilerplate function that should be included unchanged
             * in all synapse models. It is called before a connection is added to check
             * that the connection is legal. It is a wrapper that allows us to call
             * the "real" `check_connection_()` method with the `ConnTestDummyNode dummy_target;` class
             * for this connection type. This avoids a virtual function call for better
             * performance.
             *
             * @param s  Source node for connection
             * @param t  Target node for connection
             * @param receptor_type  Receptor type for connection
             * @param lastspike Time of most recent spike of presynaptic (sender) neuron, not used here
             */
            void check_connection( nest::Node& s,
                    nest::Node& t,
                    nest::rport receptor_type,
                    nest::double_t,
                    const CommonPropertiesType& )
            {
                ConnTestDummyNode dummy_target;
                ConnectionBase::check_connection_( dummy_target, s, t, receptor_type );
            }

            /**
             * Send an event to the receiver of this connection.
             * @param e The event to send
             * @param t Thread
             * @param t_lastspike Point in time of last spike sent.
             * @param cp Common properties to all synapses.
             */
            void send( nest::Event& e,
                    nest::thread t,
                    nest::double_t t_lastspike,
                    const CommonPropertiesType& cp );

            /**
             * triggers an update of a synaptic weight
             * this function is needed for neuromodulated synaptic plasticity
             * @param t Thread
             * @param modulatory_spikes counter of modulatory spikes
             * @param t_trig update triggering time 
             * @param cp Common properties to all synapses.
             */
            void trigger_update_weight( nest::thread t,
                    const std::vector< nest::spikecounter >& modulatory_spikes,
                    double_t t_trig,
                    const CommonPropertiesType& cp );

            // The following methods contain mostly fixed code to forward the corresponding
            // tasks to corresponding methods in the base class and the w_ data member holding
            // the weight.

            //! Store connection status information in dictionary
            void get_status( DictionaryDatum& d ) const;

            /**
             * Set connection status.
             *
             * @param d Dictionary with new parameter values
             * @param cm ConnectorModel is passed along to validate new delay values
             */
            void set_status( const DictionaryDatum& d, nest::ConnectorModel& cm );

            //! Allows efficient initialization on contstruction
            void  set_weight( nest::double_t w )
            {
                initial_weight_ = w;
            }

            virtual nest::double_t compute_modulation(nest::double_t modulation)
            {
                return modulation;
            }
    };


    template < typename targetidentifierT >
        inline void ModulatoryConnection< targetidentifierT >::send( nest::Event& e,
                nest::thread t,
                nest::double_t last,
                const CommonPropertiesType& props )
        {
            if ( e.get_stamp().get_steps() % 2 ) // stamp is odd, drop it
                return;

            // Even time stamp, we send the spike using the normal sending mechanism
            // send the spike to the target
            e.set_weight( weight_ );
            e.set_delay( ConnectionBase::get_delay_steps() );
            e.set_receiver( *ConnectionBase::get_target( t ) );
            e.set_rport( ConnectionBase::get_rport() );
            e(); // this sends the event
        }

    template < typename targetidentifierT >
        void ModulatoryConnection< targetidentifierT >::get_status( DictionaryDatum& d ) const
        {
            ConnectionBase::get_status( d );
            def< nest::double_t >( d, nest::names::weight, weight_ );
            def< nest::double_t >( d, "initial_weight", initial_weight_ );
            def< nest::long_t >( d, nest::names::size_of, sizeof( *this ) );
        }

    template < typename targetidentifierT >
        void ModulatoryConnection< targetidentifierT >::set_status( const DictionaryDatum& d,
                nest::ConnectorModel& cm )
        {
            ConnectionBase::set_status( d, cm );
            updateValue< nest::double_t >( d, nest::names::weight, weight_ );
            updateValue< nest::double_t >( d, "initial_weight", initial_weight_ );
        }
    
    template < typename targetidentifierT >
        inline void ModulatoryConnection< targetidentifierT >::trigger_update_weight( 
                nest::thread t,
                const std::vector< nest::spikecounter >& modulatory_spikes,
                const nest::double_t t_trig,
                const CommonPropertiesType& cp )
        {
            nest::double_t num_spikes = 0;
            for(const auto & sc: modulatory_spikes)
                num_spikes += sc.multiplicity_;


            DictionaryDatum d( new Dictionary );

            cp.vt_->get_status(d);

            nest::long_t di =  getValue<nest::long_t>(d, "deliver_interval");

            nest::double_t modulation =  2*num_spikes/(di*cp.max_modulation_);
            weight_ = initial_weight_*compute_modulation(modulation);
             
        }


} // namespace nest

#endif // MODULATORY_CONNECTION
