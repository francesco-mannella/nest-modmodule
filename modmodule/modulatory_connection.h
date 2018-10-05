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
 *   Modified from https://github.com/nest/nest-simulator/blob/v2.10.0/examples/MyModule/drop_odd_spike_connection.h
 */


/*
 *  The class ModulatoryConnection implements a generic synapse in which   
 *  the information from the volume transmitter modulates the amplitude of the weight.
 *  In particular the *modulation* ( ratio of spikes per delivery interval of the 
 *  volume transmitter) directly multiplies the baseline weight.
 *
 *  Parameters:
 *      initial_weight =>  the baseline value which has to be multiplied times the *modulation* 
 */


#ifndef MODULATORY_CONNECTION
#define MODULATORY_CONNECTION

#include "connection.h"
#include "static_connection.h"
#include "volume_transmitter.h"
#include <vector>

typedef long int long_t;

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

            long_t get_vt_gid() const;

            nest::volume_transmitter* vt_;
            
            /**
             * deliver interval of the  volume transmitter
             */ 
            long_t deliver_interval_;

            /**
             * The max amount of spikes that this transmitter receives
             * (usually the number of neurons in the source population)
             */ 
            long_t max_modulation_;
    };

    inline long_t ModulatoryCommonProperties::get_vt_gid() const
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
            double_t weight_baseline; //!< Initial synaptic weight
            double_t weight_; //!< Synaptic weight


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
                : ConnectionBase()
                  ,weight_baseline(1.0)
                  ,weight_(1.0)
            {
                weight_ = weight_baseline;
            }

            ModulatoryConnection( const ModulatoryConnection& rhs) 
                : ConnectionBase(rhs)
                  ,weight_baseline(rhs.weight_baseline)
                  ,weight_(rhs.weight_ )
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
            };

            /**
             * Check that requested connection can be created.
             *
             * This function is a boilerplate function that should be included unchanged
             * in all synapse models. It is called before a connection is added to check
             * that the connection is legal. It is a wrapper that allows us to call
             * the "real" `check_connection_()` method with the `ConnTestDummyNode
             * dummy_target;` class for this connection type. This avoids a virtual
             * function call for better performance.
             *
             * @param s  Source node for connection
             * @param t  Target node for connection
             * @param receptor_type  Receptor type for connection
             */
            void
                check_connection( nest::Node& s,
                        nest::Node& t,
                        nest::rport receptor_type,
                        const CommonPropertiesType& )
                {
                    ConnTestDummyNode dummy_target;
                    ConnectionBase::check_connection_( dummy_target, s, t, receptor_type );
                }

            /**
             * Send an event to the receiver of this connection.
             * @param e The event to send
             * @param t Thread
* @param cp Common properties to all synapses.
             */
            void send( nest::Event& e, nest::thread t, const CommonPropertiesType& cp );

            // The following methods contain mostly fixed code to forward the
            // corresponding tasks to corresponding methods in the base class and the w_
            // data member holding the weight.

            //! Store connection status information in dictionary
            void get_status( DictionaryDatum& d ) const;

            /**
             * Set connection status.
             *
             * @param d Dictionary with new parameter values
             * @param cm ConnectorModel is passed along_t to validate new delay values
             */
            void set_status( const DictionaryDatum& d, nest::ConnectorModel& cm );

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


            //! Allows efficient initialization on contstruction
            void  set_weight( double_t w )
            {
                weight_ = w;
            }

            virtual double_t compute_modulation(double_t modulation)
            {
                return modulation;
            }
    };


    template < typename targetidentifierT >
        inline void ModulatoryConnection< targetidentifierT >::send( 
                nest::Event& e, 
                nest::thread t, 
                const CommonPropertiesType& cp )
        {

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
            def< double_t >( d, nest::names::weight, weight_ );
            def< double_t >( d, "weight_baseline", weight_baseline );
            def< long_t >( d, nest::names::size_of, sizeof( *this ) );
        }

    template < typename targetidentifierT >
        void ModulatoryConnection< targetidentifierT >::set_status( const DictionaryDatum& d,
                nest::ConnectorModel& cm )
        {
            ConnectionBase::set_status( d, cm );
            updateValue< double_t >( d, nest::names::weight, weight_ );
            updateValue< double_t >( d, "weight_baseline", weight_baseline );
        }
    
    template < typename targetidentifierT >
        inline void ModulatoryConnection< targetidentifierT >::trigger_update_weight( 
                nest::thread t,
                const std::vector< nest::spikecounter >& modulatory_spikes,
                const double_t t_trig,
                const CommonPropertiesType& cp )
        {     
            
            double_t num_spikes = 0;
            for(const auto & sc: modulatory_spikes)
                num_spikes += sc.multiplicity_;

            // compute the ratio of spikes per deliver_interval between [0,1]
            double_t modulation =  num_spikes/(cp.deliver_interval_*cp.max_modulation_);
            
            // update the weight based on a function of the ratio 
            // given by the compute_modulation() method
            weight_ = weight_baseline*compute_modulation(modulation);
          
        }

} // namespace nest

#endif // MODULATORY_CONNECTION_/
